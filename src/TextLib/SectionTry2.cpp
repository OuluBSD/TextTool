#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE


void TryStrDistSectionSolverBase::MakeLines() {
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
		ws.Replace(""," ");
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
		
		//line.hash = td.GetHash(section_cmp_header_len);
	}
}

void TryStrDistSectionSolverBase::MakeUniqueLines() {
	uniq_lines.Clear();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		UniqueLine& u = GetAddUniqueLine(line.txt);
		if (u.count == 0)
			u.txt = line.txt;
		u.count++;
		u.lines << i;
	}
	Sort(uniq_lines, UniqueLine());
	
	debug_out << "uniq_lines:\n";
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		debug_out << Format("%d: [%X]: %s\n", i, (int64)uniq_lines[i].txt.GetHashValue(), uniq_lines[i].ToString());
	}
}

void TryStrDistSectionSolverBase::DefaultProcess() {
	double best_sect_weight = 0;
	double best_sect_limit = 0;
	double last_d = 0;
	if (!force_limit) {
		for (double d = 0.8; d >= 0.099; d -= 0.1) {
			iter_r_limit = d;
			last_d = d;
			SingleIteration();
			
			double weight = 0;
			if (1) {
				for (auto& sect : sections)
					weight += max(0, sect.count - 1) * sect.lines.GetCount();
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
					if (sect.lines.GetCount())
						weight += pow(sect.count * sect.lines.GetCount(), 2);
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

void TryStrDistSectionSolverBase::MakeSingleLineSections() {
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
			sections[sect_i].lines << line.txt;
		}
		else sect_i = -1;
	}
}

void TryStrDistSectionSolverBase::MakeMetaSections() {
	TextComparison tc;
	
	meta_sections.Clear();
	
	int prev_type = -1;
	int prev_section = -1;
	int meta_sect = -1;
	int type_counts[TXT_COUNT];
	memset(type_counts, 0, sizeof(type_counts));
	
	for(int i = 0; i < lines.GetCount(); i++) {
		const Line& line = lines[i];
		const Line* next_line = i+1 < lines.GetCount() ? &lines[i+1] : 0;
		
		if (line.section != prev_section) {
			Section& sect = sections[line.section];
			Section* next_sect = next_line ? &sections[next_line->section] : 0;
			
			int type = -1;
			if (sect.flag_repeating || sect.repeat > 0.666) {
				type = TXT_REPEAT;
			}
			else if (prev_type == TXT_REPEAT && next_sect && (next_sect->flag_repeating || next_sect->repeat > 0.666)) {
				type = TXT_REPEAT;
			}
			else if (sect.repeat > 0.2 && prev_type == TXT_NORMAL) {
				type = TXT_PRE_REPEAT;
			}
			else {
				type = TXT_NORMAL;
			}
			
			if (sect.meta_section < 0) {
				bool skip_msect = false;
				if (sect.lines.GetCount() <= 1 && prev_type >= 0 && meta_sect >= 0) {
					if (next_sect && next_sect->lines.GetCount() > 1 && next_sect->repeat < 0.2)
						skip_msect = true;
					else if (!next_sect)
						skip_msect = true;
				}
				
				if (skip_msect) {
					type = prev_type; // reset
				}
				else if (prev_type != type) {
					meta_sect = meta_sections.GetCount();
					MetaSection& ms = meta_sections.Add();
					ms.type = type;
					ms.num = type_counts[type]++;
					ms.count = 1;
				}
			}
			else {
				meta_sections[sect.meta_section].count++;
				meta_sect = sect.meta_section;
			}
			
			VectorFindAdd(meta_sections[meta_sect].sections, line.section);
			
			sect.meta_section = meta_sect;
			
			prev_type = type;
		}
		ASSERT(meta_sect >= 0);
		
		prev_section = line.section;
	}
	
	
	// Simple check for twist (must have repeat after it)
	bool matching_len = true;
	int normal_len = 0;
	int normal_count = 0;
	int last_repeat = -1;
	Vector<int> potential_twists;
	const double same_diff_limit = 0.25;
	const double twist_diff_limit = 0.33;
	for(int i = 0; i < meta_sections.GetCount(); i++) {
		MetaSection& ms = meta_sections[i];
		if (ms.type == TXT_NORMAL) {
			normal_count++;
			if (ms.num == 0)
				normal_len = GetMetaSectionLen(i);
			else if (ms.num == 1) {
				double diff = fabs((double)normal_len / GetMetaSectionLen(i) - 1.0);
				if (diff > same_diff_limit)
					matching_len = false;
			}
			else if (ms.num > 1)
				potential_twists << i;
		}
		else if (ms.type == TXT_REPEAT)
			last_repeat = i;
	}
	
	// Assume that different lengths of "normal" parts are twists (=bridges)
	if (matching_len) {
		for (int potential_twist_i : potential_twists) {
			MetaSection& ms = meta_sections[potential_twist_i];
			double diff = fabs((double)normal_len / GetMetaSectionLen(potential_twist_i) - 1.0);
			if (diff > twist_diff_limit) {
				ms.type = TXT_TWIST;
				ms.num = type_counts[ms.type]++;
			}
		}
	}
	// Just assume that last normal is twist
	else {
		if (normal_count > 2) {
			int normal_i = 0;
			for(int i = 0; i < meta_sections.GetCount(); i++) {
				MetaSection& ms = meta_sections[i];
				if (ms.type == TXT_NORMAL) {
					if (normal_i == normal_count - 1)
						ms.type = TXT_TWIST;
					normal_i++;
				}
			}
		}
	}
}

int TryStrDistSectionSolverBase::GetMetaSectionLen(int ms_i) const {
	const MetaSection& ms = meta_sections[ms_i];
	int len = 0;
	for(int i = 0; i < ms.sections.GetCount(); i++)
		len += sections[ms.sections[i]].lines.GetCount();
	return len;
}

String TryStrDistSectionSolverBase::GetDebugLines() const {
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

String TryStrDistSectionSolverBase::GetDebugSections() const {
	String s;
	for(int i = 0; i < sections.GetCount(); i++) {
		const Section& sect = sections[i];
		s << "[section " << i << ": " << sect.orig_count << "]\n";
		for(int j = 0; j < sect.lines.GetCount(); j++) {
			const String& line = sect.lines[j];
			s << line << "\n";
		}
		s << "\n";
	}
	return s;
}

String TryStrDistSectionSolverBase::GetResult() const {
	String s;
	int prev_msect = -1;
	int prev_sect = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		const Line& line = lines[i];
		if (line.section != prev_sect) {
			const Section& sect = sections[line.section];
			if (sect.meta_section != prev_msect) {
				const MetaSection& ms = meta_sections[sect.meta_section];
				if (s.GetCount()) s << "\n";
				s << Format("[meta-section %d: %s, count %d]\n", sect.meta_section, GetTextModeString(ms.type) + " " + IntStr(ms.num+1), ms.count);
				prev_msect = sect.meta_section;
			}
			else if (s.GetCount()) s << "\n";
			s << Format("\t[section %d: count %d, repeat %.2!m %s]\n", line.section, sect.count, sect.repeat, sect.flag_repeating ? ", flag-repeating" : "");
		}
		s << "\t\t" << line.txt << "\n";
		prev_sect = line.section;
	}
	return s;
}

String TryStrDistSectionSolverBase::GetDebugHashes() const {
	String s;
	for(int i = 0; i < lines.GetCount(); i++) {
		if (i) s << "\n";
		s << lines[i].txt.Left(6) + ": ";
		hash_t h = lines[i].descriptor.GetHash(5);
		s << IntStr64(h);
	}
	return s;
}

String TryStrDistSectionSolverBase::FindLine(hash_t h) const {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].descriptor.GetHash(section_cmp_header_len) == h)
			return lines[i].txt;
	}
	return "<line not found, hash " + IntStr64(h) + ">";
}


END_TEXTLIB_NAMESPACE
