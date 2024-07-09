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
		
		line.hash = td.GetHash(section_cmp_header_len);
	}
	
	#if 1
	
	
	uniq_lines.Clear();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		UniqueLine& u = uniq_lines.GetAdd(line.hash);
		if (u.count == 0)
			u.txt = line.txt;
		u.count++;
		u.lines << i;
	}
	SortByValue(uniq_lines, UniqueLine());
	
	DUMPM(uniq_lines);
	
	VectorMap<hash_t,int> hash_branches;
	Vector<int> rm_list;
	
	struct TempSection {
		Vector<int> lines;
		int length = 0;
		
		void Chk() {
			for(int i = 0; i < lines.GetCount(); i++) {
				for(int j = i+1; j < lines.GetCount(); j++) {
					ASSERT(lines[i] != lines[j]);
				}
			}
		}
		String ToString() const {
			String s = "weight=" + IntStr(length*lines.GetCount()) + ",length=" + IntStr(length) + ": ";
			for(int i = 0; i < lines.GetCount(); i++) {if (i) s << ", "; s << lines[i];}
			return s;
		}
		bool operator()(const TempSection& a, const TempSection& b) const {
			return (a.length*a.lines.GetCount()) > (b.length*b.lines.GetCount());
		}
	};
	Array<TempSection> v;
	
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		UniqueLine& u = uniq_lines[i];
		
		if (u.lines.GetCount() < 2)
			continue;
		
		
		Vector<int> proc_v;
		{
			proc_v << v.GetCount();
			TempSection& t = v.Add();
			t.lines <<= u.lines;
			t.length = 1;
			t.Chk();
		}
		
		while (proc_v.GetCount()) {
			for(int pv_i = 0; pv_i < proc_v.GetCount(); pv_i++) {
				TempSection& t = v[proc_v[pv_i]];
				
				hash_branches.Clear();
				rm_list.SetCount(0);
				for(int j = 0; j < t.lines.GetCount(); j++) {
					int line_i = t.lines[j];
					int pos = line_i + t.length;
					if (pos >= lines.GetCount()) {
						rm_list << j;
					}
					else {
						Line& line = lines[pos];
						hash_branches.GetAdd(line.hash,0)++; // count next hashes
					}
				}
				SortByValue(hash_branches, StdGreater<int>());
				
				// Loop all branches
				for(int j = 1; j < hash_branches.GetCount(); j++) {
					// With 2 or more lines, a new section can be made
					if (hash_branches[j] >= 2) {
						hash_t hash = hash_branches.GetKey(j);
						proc_v << v.GetCount(); // add new section to processing queue
						TempSection& t0 = v.Add();
						for(int k = 0; k < t.lines.GetCount(); k++) {
							int line_i = t.lines[k];
							int pos = line_i + t.length;
							if (pos < lines.GetCount() && lines[pos].hash == hash) {
								t0.lines << line_i;
								t0.Chk();
								rm_list << k;
							}
						}
						t0.Chk();
					}
					// With 1 line, don't start new branch and remove it from the current,
					// which triggers the forking of current branch too.
					else {
						ASSERT(hash_branches[j] == 1);
						hash_t hash = hash_branches.GetKey(j);
						for(int k = 0; k < t.lines.GetCount(); k++) {
							int line_i = t.lines[k];
							int pos = line_i + t.length;
							if (pos < lines.GetCount() && lines[pos].hash == hash) {
								rm_list << k;
							}
						}
					}
				}
				
				// Stop, if everything would be removed
				if (rm_list.GetCount() == t.lines.GetCount()) {
					proc_v.Remove(pv_i--);
				}
				// Fork and modify the current branch, if some line-range has ended
				else if (rm_list.GetCount()) {
					int remaining = t.lines.GetCount() - rm_list.GetCount();
					
					// Don't start new branch, if only one section remains
					if (remaining == 1) {
						// Just discard this section from processing queue
						proc_v.Remove(pv_i);
					}
					else {
						// discard this section in processing queue (and replace with the new one)
						// IMPORTANT: this is processed only in the next 'while' iteration,
						//            because the next will be pv_i+1
						proc_v[pv_i] = v.GetCount();
						
						TempSection& t0 = v.Add();
						t0.lines <<= t.lines;
						t0.length = t.length + 1;
						
						Sort(rm_list, StdLess<int>());
						t0.lines.Remove(rm_list);
						
						t0.Chk();
					}
				}
				// Otherwise, this branch was extended without modification
				else {
					t.length++;
				}
			}
		}
	}
	
	Sort(v, TempSection());
	DUMPC(v);
	LOG("");
	
	// Try to add actual sections
	for (TempSection& s : v) {
		
		// Check if lines are still assignable
		bool all_ok = true;
		for(int line_i : s.lines) {
			for(int j = 0; j < s.length; j++) {
				int pos = line_i+j;
				if (lines[pos].section >= 0) {
					all_ok = false;
					break;
				}
			}
			if (!all_ok) break;
		}
		if (!all_ok) continue;
		
		// Add section
		int sect_i = sections.GetCount();
		Section& sect = sections.Add();
		sect.count = s.lines.GetCount();
		sect.orig_count = sect.count;
		sect.orig_weight = s.length * s.lines.GetCount();
		
		// Write section
		for(int line_i : s.lines) {
			for(int j = 0; j < s.length; j++) {
				int pos = line_i+j;
				lines[pos].section = sect_i;
			}
		}
	}
	
	
	#else
	
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
	
	//#error TODO Fill all possible sections and pick best
	
	Sort(sections, Section());
	
	for (int section_i = 0; section_i < sections.GetCount(); section_i++) {
		Section& sect = sections[section_i];
		
		for(int i = 0; i < lines.GetCount(); i++) {
			Line& line = lines[i];
			if (line.section >= 0)
				continue;
			bool matches = sect.hashes.GetCount() > 0;
			for(int j = 0; j < sect.hashes.GetCount(); j++) {
				int pos = i+j;
				if (pos >= lines.GetCount()) {
					matches = false;
					break;
				}
				Line& l0 = lines[pos];
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

#if 0
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
#endif





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

