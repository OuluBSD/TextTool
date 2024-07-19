#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE


void TryHashSectionSolverBase::MakeLines() {
	String s = input;
	ASSERT(s.GetCount());
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
		
		// Clear common special characters for better hash matching
		ws.Replace(","," ");
		ws.Replace("."," ");
		ws.Replace("!"," ");
		ws.Replace("?"," ");
		ws.Replace("-"," ");
		ws.Replace("—"," ");
		ws.Replace(":"," ");
		ws.Replace(";"," ");
		ws.Replace("\""," ");
		ws.Replace("'"," ");
		ws.Replace("("," ");
		ws.Replace(")"," ");
		ws.Replace("["," ");
		ws.Replace("]"," ");
		ws.Replace("/"," ");
		ws.Replace("&"," ");
		ws.Replace("+"," ");
		ws.Replace("    "," ");
		ws.Replace("   "," ");
		ws.Replace("  "," ");
		ws = TrimBoth(ws.ToString()).ToWString();
		//LOG(ws);
		
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
		
		bool print_dbg = false;
		
		if (print_dbg) debug_out << "Line #" << i << ": " << l;
		for(int j = 0; j < count; j++) {
			const TextDescriptor::Item& from = items[j];
			TextDescriptor::Item& to = td.items[j];
			to.from = from.from;
			to.to = from.to;
			to.count = from.count;
			if (print_dbg) debug_out << j << ": " << (int)to.from << " -> " << (int)to.to << ": " << (int)to.count << "\n";
		}
		if (print_dbg) debug_out << "\n";
		
		line.hash = td.GetHash(section_cmp_header_len);
	}
}

void TryHashSectionSolverBase::MakeUniqueLines() {
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
	
	debug_out << "uniq_lines:\n";
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		debug_out << Format("%d: [%X]: %s\n", i, (int64)uniq_lines.GetKey(i), uniq_lines[i].ToString());
	}
}

void TryHashSectionSolverBase::DefaultProcess() {
	double best_sect_weight = 0;
	double best_sect_limit = 0;
	double last_d = 0;
	if (!force_limit) {
		for (double d = 0.8; d > 0; d -= 0.1) {
			iter_r_limit = d;
			last_d = d;
			SingleIteration();
			
			double weight = 0;
			if (1) {
				for (auto& sect : sections)
					weight += max(0, sect.count - 1) * sect.hashes.GetCount();
			}
			else if (1) {
				for (auto& sect : sections)
					weight += max(0, sect.count - 1);
			}
			else if (1) {
				for (auto& sect : sections)
					weight += max(0, sect.count - 1);
				double percentage = weight / (double)lines.GetCount();
				
				// when weight is over 75% of total lyrics, assume failure
				if (percentage >= 0.75)
					weight = 0;
			}
			else {
				for (auto& sect : sections) {
					if (sect.hashes.GetCount())
						weight += pow(sect.count * sect.hashes.GetCount(), 2);
					else
						weight += pow(sect.count * sect.orig_weight, 2);
				}
			}
			if (weight > best_sect_weight) {
				best_sect_weight = weight;
				best_sect_limit = d;
			}
		}
	}
	else {
		last_d = DBL_MAX;
		best_sect_limit = forced_limit_value;
	}
	
	if (best_sect_limit == 0) best_sect_limit = 0.8;
	
	if (last_d != best_sect_limit) {
		iter_r_limit = best_sect_limit;
		last_d = best_sect_limit;
		SingleIteration();
	}
}

void TryHashSectionSolverBase::MakeSingleLineSections() {
	int sect_i = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		if (line.section < 0) {
			double repeat_sum = line.GetRepeatabilitySum();
			if (sect_i >= 0 &&
				((repeat_sum > sections[sect_i].repeat + repeatability_range) ||
				(repeat_sum < sections[sect_i].repeat - repeatability_range)))
				sect_i = -1;
			if (sect_i < 0) {
				sect_i = sections.GetCount();
				Section& sect = sections.Add();
				sect.orig_count = 1;
				sect.count = 1;
				sect.repeat = repeat_sum;
				sect.first_line = i;
			}
			line.section = sect_i;
			sections[sect_i].hashes << line.descriptor.GetHash(section_cmp_header_len);
		}
		else sect_i = -1;
	}
}

void TryHashSectionSolverBase::MakeMetaSections() {
	TextComparison tc;
	
	meta_sections.Clear();
	
	for(int i = 0; i < sections.GetCount(); i++) {
		Section& sect0 = sections[i];
		if (sect0.count < 2) continue;
		for(int j = 0; j < sections.GetCount(); j++) {
			Section& sect1 = sections[j];
			if (sect1.count > 1) continue;
			
			int len = min(sect0.hashes.GetCount(), sect1.hashes.GetCount());
			for(int k = 0; k < len; k++) {
				int li0 = sect0.first_line + k;
				int li1 = sect1.first_line + k;
				const Line& line0 = lines[li0];
				const Line& line1 = lines[li1];
				tc.Process(line0.txt, line1.txt);
			}
		}
	}
	
	// TODO THIS IS UNFINISHED!
	
}

String TryHashSectionSolverBase::GetDebugLines() const {
	String s;
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		const UniqueLine& ul = uniq_lines[i];
		s << ul.count << ":\t" << ul.txt  << "\n";
	}
	s << "\n\n";
	for(int i = 0; i < lines.GetCount(); i++) {
		const Line& l = lines[i];
		s << l.txt.Left(12) + "... r=" << l.repeatability << ", cr=" << l.circumstacial_repeatability << "\n";
	}
	return s;
}

String TryHashSectionSolverBase::GetDebugSections() const {
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

String TryHashSectionSolverBase::GetResult() const {
	String s;
	int prev_sect = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		const Line& line = lines[i];
		if (line.section != prev_sect) {
			const Section& sect = sections[line.section];
			if (s.GetCount()) s << "\n";
			s << Format("[section %d: count %d, repeat %.2!m %s]\n", line.section, sect.count, sect.repeat, sect.flag_repeating ? ", flag-repeating" : "");
		}
		s << line.txt << "\n";
		prev_sect = line.section;
	}
	return s;
}

String TryHashSectionSolverBase::GetDebugHashes() const {
	String s;
	for(int i = 0; i < lines.GetCount(); i++) {
		if (i) s << "\n";
		s << lines[i].txt.Left(6) + ": ";
		hash_t h = lines[i].descriptor.GetHash(5);
		s << IntStr64(h);
	}
	return s;
}

String TryHashSectionSolverBase::FindLine(hash_t h) const {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].descriptor.GetHash(section_cmp_header_len) == h)
			return lines[i].txt;
	}
	return "<line not found, hash " + IntStr64(h) + ">";
}




END_TEXTLIB_NAMESPACE
