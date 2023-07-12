#ifndef _SongTool_Pattern_h_
#define _SongTool_Pattern_h_


struct Part;

struct PatternSnap {
	PatternSnap* owner = 0;
	Part* part = 0;
	
	One<PatternSnap> a, b;
	int part_id = -1;
	int pos = -1, len = 0;
	Index<SnapAttrStr> attributes;
	String txt;
	
	void Init(int pos, int len);
	void Clear() {
		a.Clear();
		b.Clear();
		attributes.Clear();
		txt.Clear();
	}
	String GetStructuredText(bool pretty, int indent = 0) const;
	void SetId(int i) {
		part_id = i;
		if (a) a->SetId(i);
		if (b) b->SetId(i);
	}
	void Jsonize(JsonIO& json) {
		json
			("part_id",  part_id)
			("pos",  pos)
			("len",  len)
			("attributes",  attributes)
			("txt",  txt)
		;
		if (json.IsStoring()) {
			if (a) json("a", *a);
			if (b) json("b", *b);
		}
		else {
			a.Clear();
			b.Clear();
			if (len > 1) {
				json("a", a.Create());
				json("b", b.Create());
			}
		}
	}
	void MergeOwner() {
		if (a) a->MergeOwner();
		if (b) b->MergeOwner();
		if (a && b) {
			for(int i = 0; i < a->attributes.GetCount(); i++) {
				const SnapAttrStr& sa = a->attributes[i];
				int j = b->attributes.Find(sa);
				if (j < 0)
					continue;
				attributes.FindAdd(sa);
				a->attributes.Remove(i--);
				b->attributes.Remove(j);
			}
		}
	}
	void FixPtrs() {
		if (a) {
			a->owner = this;
			a->part = part;
			a->FixPtrs();
		}
		if (b) {
			b->owner = this;
			b->part = part;
			b->FixPtrs();
		}
	}
	int GetLevel() const {
		if (!a) return 0;
		else return a->GetLevel() + 1;
	}
	void GetSnapsLevel(int level, Vector<PatternSnap*>& level_snaps) {
		if (GetLevel() == level)
			level_snaps.Add(this);
		else {
			if (a) a->GetSnapsLevel(level, level_snaps);
			if (b) b->GetSnapsLevel(level, level_snaps);
		}
	}
	void GetAttributes(Index<SnapAttrStr>& attrs) const {
		for (const SnapAttrStr& a : this->attributes.GetKeys())
			attrs.FindAdd(a);
		if (a) a->GetAttributes(attrs);
		if (b) b->GetAttributes(attrs);
	}
	void GetLineSnapshots(const String& txt_line, Vector<PatternSnap*>& snaps) {
		// if level == 0 (== a and b are empty)
		if (!a && !b) {
			ASSERT(!txt.IsEmpty());
			if (txt == txt_line)
				snaps.Add(this);
		}
	}
};




#endif
