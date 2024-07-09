#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


hash_t TextDescriptor::GetHash(int c) const {
	CombineHash ch;
	c = max(0,min(c, ITEM_COUNT));
	for(int i = 0; i < c; i++) {
		const TextDescriptor::Item& it = items[i];
		if (!it.count) break;
		ch.Do(it.from);
		ch.Do(it.to);
	}
	return ch;
}










BasicScriptStructureSolver::BasicScriptStructureSolver() {
	
}

void BasicScriptStructureSolver::Process(String s) {
	s.Replace("\r", "");
	Vector<String> txt_lines = Split(s, "\n");
	lines.SetCount(0);
	sections.SetCount(0);
	
	VectorMap<uint32, TextDescriptor::Item> items;
	
	for(int i = 0; i < txt_lines.GetCount(); i++) {
		String& l = txt_lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty())
			continue;
		Line& line = lines.Add();
		line.txt = l;
		TextDescriptor& td = line.descriptor;
		td.Zero();
		
		items.Clear();
		
		WString ws = l.ToWString();
		ws = ToLower(ws);
		for(int j = 1; j < ws.GetCount(); j++) {
			int chr0 = ws[j-1];
			int chr1 = ws[j];
			CombineHash ch;
			ch.Do(chr0).Do(chr1);
			hash_t hash = ch;
			int k = items.Find(hash);
			if (k < 0) {
				k = items.GetCount();
				TextDescriptor::Item& it = items.Add(hash);
				it.from = chr0;
				it.to = chr1;
				it.count = 0;
			}
			TextDescriptor::Item& it = items[k];
			it.count++;
		}
		
		SortByValue(items, TextDescriptor::Item());
		int count = min(TextDescriptor::ITEM_COUNT, items.GetCount());
		LOG("Line #" << i << ": " << l);
		for(int j = 0; j < count; j++) {
			const TextDescriptor::Item& from = items[j];
			TextDescriptor::Item& to = td.items[j];
			to.from = from.from;
			to.to = from.to;
			to.count = from.count;
			LOG(j << ": " << (int)to.from << " -> " << (int)to.to << ": " << (int)to.count);
		}
		LOG("");
	}
	
	// Count transforms from unique line to other unique line
	transfers.Clear();
	for(int i = 0; i < lines.GetCount()-1; i++) {
		TextDescriptor& td0 = lines[i].descriptor;
		TextDescriptor& td1 = lines[i+1].descriptor;
		hash_t h0 = td0.GetHash(section_cmp_header_len);
		hash_t h1 = td1.GetHash(section_cmp_header_len);
		auto& t = GetAddTransfer(h0,h1);
		t.count++;
		auto& vec = t.vectors.Add();
		vec.line = i;
	}
	Sort(transfers, Transfer());
	
	// Calculate lengths of continuous transform vectors
	// Having only 1 instance of the transform will cut the Transfer::Vec length
	// But you are not searching for the longest Transfer::Vec lenght, but also considering 'weight'
	// The weight considers both length and how "popular" that transfer is.
	for (Transfer& t : transfers) {
		for(auto& vec : t.vectors) {
			ASSERT(t.from == lines[vec.line].descriptor.GetHash(section_cmp_header_len));
			vec.weight = t.count;
			vec.len = 2;
			hash_t from = t.to;
			for(int i = vec.line+1; i < lines.GetCount(); i++) {
				Line& line = lines[i];
				hash_t to = line.descriptor.GetHash(section_cmp_header_len);
				Transfer* t1 = FindTransfer(from, to);
				if (!t1 || t1->count <= 1) break;
				vec.weight += t1->count;
				vec.len++;
				from = to;
			}
		}
		Sort(t.vectors, Transfer::Vec());
	}
	
	// Make repeating sections
	sections.Clear();
	#if 1
	for (Transfer& t : transfers) {
		for(auto& vec : t.vectors) {
			if (vec.len < 3)
				continue;
			Section& sect = sections.Add();
			sect.orig_weight = vec.weight;
			sect.orig_count = t.count;
			for(int i = 0; i < vec.len-1; i++) {
				int pos = vec.line + i;
				Line& line = lines[pos];
				hash_t h = line.descriptor.GetHash(section_cmp_header_len);
				sect.hashes << h;
			}
		}
	}
	Sort(sections, Section());
	
	for (int section_i = 0; section_i < sections.GetCount(); section_i++) {
		Section& sect = sections[section_i];
		
		for(int i = 0; i < lines.GetCount(); i++) {
			Line& line = lines[i];
			if (line.section >= 0)
				continue;
			bool matches = sect.hashes.GetCount() > 0;
			for(int j = 0; j < sect.hashes.GetCount(); j++) {
				Line& l0 = lines[i+j];
				if (l0.section >= 0 ||
					l0.descriptor.GetHash(section_cmp_header_len) != sect.hashes[j]) {
					matches = false;
					break;
				}
			}
			if (matches) {
				sect.count++;
				for(int j = 0; j < sect.hashes.GetCount(); j++)
					lines[i+j].section = section_i;
			}
		}
		
	}
	#else
	Index<int> visited;
	for(int i = 0; i < transfers.GetCount(); i++) {
		Transfer& t = transfers[i];
		hash_t from = t.to;
		visited.Clear();
		visited.Add(t.from);
		visited.Add(t.to);
		Section* s = 0;
		while (1) {
			Transfer* t0 = FindTransfer(from);
			if (!t0 || t0->count <= 1)
				break;
			// The next line is accepted
			// At this point, the section has at least 2 lines, so we can actually add it
			if (!s) {
				s = &sections.Add();
				s->orig_count = t.count;
				s->hashes.Add(t.from);
				s->hashes.Add(t.to);
			}
			
			// Add line to section
			s->hashes.Add(t0->to);
			from = t0->to;
			
			// Prevent forever loops by checking visited lines
			int i = visited.Find(from);
			if (i >= 0)
				break;
			visited.Add(from);
		}
	}
	
	// At this point, all sections have at least 2 lines and they occur at least 2 times
	// If at least 2 consecutive lines belongs to any section, they can be attached to that section.
	// Lines, which haven't been attached can be joined to once occuring sections.
	Vector<int> matching_sections;
	Vector<int> rm_list;
	int dbg_assigned_section_count = 0;
	int prev_sect = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		
		// Get sections, which has matching first hash
		matching_sections.SetCount(0);
		hash_t h0 = line.descriptor.GetHash(section_cmp_header_len);
		for(int j = 0; j < sections.GetCount(); j++) {
			auto& sect = sections[j];
			ASSERT(!sect.hashes.IsEmpty());
			if (sect.hashes[0] == h0)
				matching_sections.Add(j);
		}
		
		if (matching_sections.GetCount() >= 2) {
			// LOG("A"); DUMPC(matching_sections);
			
			for(int j = i+1, pos = 1; j < lines.GetCount(); j++, pos++) {
				hash_t h = lines[j].descriptor.GetHash(section_cmp_header_len);
				
				// Check if section is too short
				rm_list.SetCount(0);
				for(int k = 0; k < matching_sections.GetCount(); k++) {
					Section& sect = sections[matching_sections[k]];
					if (pos >= sect.hashes.GetCount())
						rm_list.Add(k);
				}
				
				// Save first section for default selection in case of all are removed
				int def_selection = matching_sections[0];
				
				// If some sections remains, remove too short sections
				if (rm_list.GetCount() > 0) {
					matching_sections.Remove(rm_list);
					// LOG("B"); DUMPC(matching_sections);
					rm_list.SetCount(0);
				}
				
				// Remove sections without matching hash
				for(int k = 0; k < matching_sections.GetCount(); k++) {
					Section& sect = sections[matching_sections[k]];
					if (sect.hashes[pos] != h)
						rm_list.Add(k);
				}
				if (rm_list.GetCount() > 0) {
					matching_sections.Remove(rm_list);
					// LOG("C"); DUMPC(matching_sections);
				}
				if (matching_sections.IsEmpty())
					matching_sections << def_selection;
				if (matching_sections.GetCount() == 1)
					break;
			}
			ASSERT(matching_sections.GetCount() >= 1);
			line.section = matching_sections[0];
			dbg_assigned_section_count++;
			
			// Count section occurances
			if (prev_sect != line.section)
				sections[line.section].count++;
		}
		prev_sect = line.section;
	}
	
	double assigned_perc = (double)dbg_assigned_section_count / (double)lines.GetCount();
	
	#endif
	
	int sect_i = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		if (line.section < 0) {
			if (sect_i < 0) {
				sect_i = sections.GetCount();
				Section& sect = sections.Add();
				sect.orig_count = 1;
				sect.count = 1;
			}
			line.section = sect_i;
			sections[sect_i].hashes << line.descriptor.GetHash(section_cmp_header_len);
		}
		else sect_i = -1;
	}
	
	
}

String BasicScriptStructureSolver::GetResult() const {
	if (0) {
		String s;
		for(int i = 0; i < lines.GetCount(); i++) {
			if (i) s << "\n";
			s << lines[i].txt.Left(6) + ": ";
			hash_t h = lines[i].descriptor.GetHash(5);
			s << IntStr64(h);
		}
		return s;
	}
	else if (0) {
		String s;
		for(int i = 0; i < sections.GetCount(); i++) {
			const Section& sect = sections[i];
			s << "[section " << i << ": " << sect.orig_count << "]\n";
			for(int j = 0; j < sect.hashes.GetCount(); j++) {
				hash_t h = sect.hashes[j];
				String line = FindLine(h);
				s << line << "\n";
			}
			s << "\n";
		}
		return s;
	}
	else if (1) {
		String s;
		int prev_sect = -1;
		for(int i = 0; i < lines.GetCount(); i++) {
			const Line& line = lines[i];
			if (line.section != prev_sect) {
				const Section& sect = sections[line.section];
				if (s.GetCount()) s << "\n";
				s << "[section " << line.section << ": count " << sect.count << "]\n";
			}
			s << line.txt << "\n";
			prev_sect = line.section;
		}
		return s;
	}
}

String BasicScriptStructureSolver::FindLine(hash_t h) const {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].descriptor.GetHash(section_cmp_header_len) == h)
			return lines[i].txt;
	}
	return "<line not found, hash " + IntStr64(h) + ">";
}

BasicScriptStructureSolver::Transfer* BasicScriptStructureSolver::FindTransfer(hash_t h) {
	int max_count = 0, pos = -1;
	for(int i = 0; i < transfers.GetCount(); i++) {
		Transfer& t = transfers[i];
		if (t.from == h && t.count > max_count) {
			pos = i;
			max_count = t.count;
		}
	}
	if (pos >= 0)
		return &transfers[pos];
	return 0;
}

BasicScriptStructureSolver::Transfer* BasicScriptStructureSolver::FindTransfer(hash_t from, hash_t to) {
	for(Transfer& t : transfers)
		if (t.from == from && t.to == to)
			return &t;
	return 0;
}

BasicScriptStructureSolver::Transfer& BasicScriptStructureSolver::GetAddTransfer(hash_t h0, hash_t h1) {
	for (Transfer& t : transfers) {
		if (t.from == h0 && t.to == h1)
			return t;
	}
	Transfer& t = transfers.Add();
	t.from = h0;
	t.to = h1;
	t.count = 0;
	return t;
}






SourceDataImporter::SourceDataImporter() {
	
}

int SourceDataImporter::GetPhaseCount() const {
	return PHASE_COUNT;
}

void SourceDataImporter::DoPhase() {
	
}

SourceDataImporter& SourceDataImporter::Get(int appmode) {
	static ArrayMap<String, SourceDataImporter> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	SourceDataImporter& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

