#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


hash_t TextDescriptor::GetHash(int c) const {
	CombineHash ch;
	c = max(0,min(c, ITEM_COUNT));
	for(int i = 0; i < c; i++) {
		const TextDescriptor::Item& it = items[i];
		//if (i >= 3 && it.count <= 1) break;
		if (it.count < 1) break;
		ch.Do(it.from);
		ch.Do(it.to);
	}
	return ch;
}










ScriptStructureSolverBase::ScriptStructureSolverBase() {
	
}

void ScriptStructureSolverBase::Process(String s) {
	input = s;
		
	DefaultProcess();
}

void ScriptStructureSolverBase::MakeLines() {
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
}

void ScriptStructureSolverBase::MakeUniqueLines() {
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
}

void ScriptStructureSolverBase::DefaultProcess() {
	MakeLines();
	MakeUniqueLines();
	MakeSections();
	MakeRepeatingSectionLines();
	MakeSingleLineSections();
}

void ScriptStructureSolverBase::MakeSingleLineSections() {
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

String ScriptStructureSolverBase::GetDebugLines() const {
	String s;
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		const UniqueLine& ul = uniq_lines[i];
		s << ul.count << ":\t" << ul.txt  << "\n";
	}
	s << "\n\n";
	for(int i = 0; i < lines.GetCount(); i++) {
		const Line& l = lines[i];
		s << l.txt.Left(6) + "... r=" << l.repeatability << ", cr=" << l.circumstacial_repeatability << "\n";
	}
	return s;
}

String ScriptStructureSolverBase::GetDebugHashes() const {
	String s;
	for(int i = 0; i < lines.GetCount(); i++) {
		if (i) s << "\n";
		s << lines[i].txt.Left(6) + ": ";
		hash_t h = lines[i].descriptor.GetHash(5);
		s << IntStr64(h);
	}
	return s;
}

String ScriptStructureSolverBase::GetDebugSections() const {
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

String ScriptStructureSolverBase::GetResult() const {
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

String ScriptStructureSolverBase::FindLine(hash_t h) const {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].descriptor.GetHash(section_cmp_header_len) == h)
			return lines[i].txt;
	}
	return "<line not found, hash " + IntStr64(h) + ">";
}











MultiScriptStructureSolver::MultiScriptStructureSolver() {
	c = 3;
	
}

ScriptStructureSolverBase& MultiScriptStructureSolver::Get() {
	switch (c) {
		case 0: return t1;
		case 1: return t2;
		case 2: return t3;
		case 3: return t4;
		default: return t1;
	}
}


END_TEXTLIB_NAMESPACE

