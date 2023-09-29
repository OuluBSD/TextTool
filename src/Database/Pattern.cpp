#include "Database.h"


/*void PatternSnap::Init(int pos, int len) {
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
}*/



void SnapContext::MergeOwner() {
	int level = Get0().GetLevel();
	if (level == 0) static_cast<Break*>(this)->MergeOwner();
	if (level == 1) static_cast<Line*>(this)->MergeOwner();
	if (level == 2) static_cast<Part*>(this)->MergeOwner();
	if (level == 3) static_cast<Pipe*>(this)->MergeOwner();
	//if (level == 4) static_cast<Release*>(this)->MergeOwner();
	//if (level == 5) static_cast<Artist*>(this)->MergeOwner();
}

void PatternSnap::FindAddAttr(const SnapAttrStr& sa) {
	attributes.FindAdd(sa);
}













void Song::ReloadStructure() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	for(const SnapArg& a : AllArgs())
		db.ctx.snap[a] = 0;
	
	TODO
	
	#if 0
	this->parts.Clear();
	this->structure = Split(this->structure_str, ",");
	
	Index<String> part_seen;
	
	for (String& p : structure) {
		p = TrimBoth(p);
		
		// Split name and beat count
		int i = p.Find(":");
		String name;
		
		if (i < 0) {
			i = this->FindPartIdx(p);
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
			i = this->FindPartIdx(name);
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
				Part& part = this->GetAddPart(name);
				part.lines.SetCount(beats);
			}
		}
		
		// Add part
		this->GetAddPart(name);
		part_seen.FindAdd(name);
	}
	
	//DUMPM(this->parts);
	//DUMPC(this->structure);
	for (Part& part : this->parts) {
		part.FixPtrs();
	}
	#endif
}
