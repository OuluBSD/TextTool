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
	for (const SnapAttrStr& sa : this->attributes.GetKeys()) {
		used_groups.FindAdd(sa.group_i);
	}
	
	for (int group_i : used_groups.GetKeys()) {
		const Attributes::Group& gg = g.groups[group_i];
		if (pretty) s.Cat('\t', indent+1);
		s << ToLower(gg.description) << " {";
		if (pretty) s << "\n";
		for (const SnapAttrStr& sa : this->attributes.GetKeys()) {
			ASSERT(sa.has_id);
			if (sa.group_i != group_i)
				continue;
			if (pretty) s.Cat('\t', indent+2);
			s	<< ToLower(gg.values[sa.item_i])
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














void Song::ReloadStructure() {
	Database& db = Database::Single();
	db.active_snap = 0;
	
	this->parts.Clear();
	this->structure = Split(this->structure_str, ",");
	
	Index<String> part_seen;
	
	for (String& p : structure) {
		p = TrimBoth(p);
		
		// Split name and beat count
		int i = p.Find(":");
		String name;
		
		if (i < 0) {
			i = this->parts.Find(p);
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
			p = name; // trim : from string in structure vector
			
			// Check for beat length error
			i = this->parts.Find(name);
			if (i >= 0) {
				Part& part = this->parts[i];
				int len = part.lines.GetCount();
				if (len != beats) {
					if (part_seen.Find(name) < 0) {
						part.lines.SetCount(beats);
					}
					else {
						PromptOK(DeQtf("error: part length mismatch"));
						return;
					}
				}
			}
			else {
				Part& part = this->parts.GetAdd(name);
				part.lines.SetCount(beats);
			}
		}
		
		// Add part
		this->parts.GetAdd(name);
		part_seen.FindAdd(name);
	}
	
	//DUMPM(this->parts);
	//DUMPC(this->structure);
	for (Part& part : this->parts) {
		//DUMP(part.lines.GetCount());
		part.snap.Init(0, part.lines.GetCount());
		part.rev_snap.Init(0, part.lines.GetCount());
		ASSERT(part.snap.GetLevel() > 0 || part.lines.GetCount() <= 1);
		part.FixPtrs();
	}
}

void PatternSnap::ResolveId() {
	for (const SnapAttrStr& sa : attributes.GetKeys())
		sa.RealizeId();
	if (a) a->ResolveId();
	if (b) b->ResolveId();
}
