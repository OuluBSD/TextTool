#include "SongTool.h"


void PatternSnap::Init(int pos, int len) {
	this->pos = pos;
	this->len = len;
	
	if (len > 1) {
		int len2 = len / 2;
		int len0 = len2;
		int len1 = len - len2;
		if (a.IsEmpty()) a.Create();
		if (b.IsEmpty()) b.Create();
		a->Init(pos,       len0);
		b->Init(pos + len0, len1);
	}
}

String PatternSnap::GetStructuredText(bool pretty, int indent) const {
	const Attributes& g = Database::Single().attrs;
	String s;
	if (pretty) s.Cat('\t', indent);
	s << "line(" << pos << ":" << len << ") {";
	if (pretty) s << "\n";
	int i = 0;
	Index<int> used_groups;
	for (const SnapAttr& sa : this->attributes.GetKeys()) {
		used_groups.FindAdd(sa.group);
	}
	
	for (int group : used_groups.GetKeys()) {
		const Attributes::Group& gg = g.groups[group];
		if (pretty) s.Cat('\t', indent+1);
		s << ToLower(gg.description) << " {";
		if (pretty) s << "\n";
		for (const SnapAttr& sa : this->attributes.GetKeys()) {
			if (sa.group != group)
				continue;
			if (pretty) s.Cat('\t', indent+2);
			s	<< ToLower(gg.values[sa.item])
				<< ";"
				;
			if (pretty) s << "\n";
			i++;
		}
		if (pretty) s.Cat('\t', indent+1);
		s << "}";
		if (pretty) s << "\n";
	}
	if (a) s << a->GetStructuredText(pretty, indent+1);
	if (b) s << b->GetStructuredText(pretty, indent+1);
	if (pretty) s.Cat('\t', indent);
	s << "}";
	if (pretty) s << "\n";
	return s;
}














void Pattern::ReloadStructure() {
	Database& db = Database::Single();
	db.active_snap = 0;
	
	this->parts.Clear();
	//this->unique_parts.Clear();
	
	Vector<String> part_str = Split(this->structure, ",");
	Index<String> part_seen;
	
	for (String& p : part_str) {
		p = TrimBoth(p);
		
		// Split name and beat count
		int i = p.Find(":");
		String name;
		
		if (i < 0) {
			i = this->unique_parts.Find(p);
			if (i >= 0) {
				name = p; // ok
			}
			else {
				PromptOK(DeQtf("error: no ':' character and beat length"));
				return;
			}
		}
		else {
			name = p.Left(i);
			int beats = StrInt(p.Mid(i+1));
			
			// Check for beat length error
			i = this->unique_parts.Find(name);
			if (i >= 0) {
				Part& part = this->unique_parts[i];
				if (part.len != beats) {
					if (part_seen.Find(name) < 0) {
						part.len = beats;
					}
					else {
						PromptOK(DeQtf("error: part length mismatch"));
						return;
					}
				}
			}
			else {
				Part& part = this->unique_parts.GetAdd(name);
				part.len = beats;
			}
		}
		
		// Add part
		this->parts.Add(name);
		part_seen.Add(name);
	}
	
	DUMPM(this->unique_parts);
	DUMPC(this->parts);
	
	for (Part& part : this->unique_parts) {
		part.snap.Init(0, part.len);
		part.FixPtrs();
	}
}

