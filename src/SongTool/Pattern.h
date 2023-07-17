#ifndef _SongTool_Pattern_h_
#define _SongTool_Pattern_h_


struct Part;

struct PatternSnap : PatternMask {
	String						txt;
	Index<SnapAttrStr>			attributes;
	Vector<int>					partscore;
	VectorMap<String,String>	data;
	
	int							id = -1;
	PatternSnap*				owner = 0;
	
	
	void Clear() {
		PatternMask::Clear();
		txt.Clear();
		attributes.Clear();
		partscore.Clear();
		data.Clear();
		PatternMask::Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("txt",			txt)
			("attributes",	attributes)
			("partscore",	partscore)
			("data",		data)
			;
		PatternMask::Jsonize(json);
	}
	void SetId(int i) {id = i;}
	void ResolveId() {
		for (const SnapAttrStr& sa : attributes.GetKeys())
			sa.RealizeId();
		PatternMask::ResolveId();
	}
	#if 0
	Part* part = 0;
	
	One<PatternSnap> a, b;
	int pos = -1, len = 0;
	
	void Init(int pos, int len);
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
	#endif
	template <class A, class B>
	static void MergeOwner(A* owner, Array<B>& sub) {
		if (sub.GetCount() >= 2) {
			for (B& o : sub)
				MergeOwner(&o, o.GetSub());
			
			VectorMap<SnapAttrStr, int> attrs;
			for (B& o : sub) {
				for (const SnapAttrStr& sa : o.PatternSnap::attributes)
					attrs.GetAdd(sa, 0)++;
			}
			int c = sub.GetCount();
			for(int i = 0; i < attrs.GetCount(); i++) {
				if (attrs[i] != c)
					continue;
				const SnapAttrStr& common = attrs.GetKey(i);
				for (B& o : sub)
					o.attributes.RemoveKey(common);
				owner->PatternSnap::attributes.FindAdd(common);
			}
		}
	}
	#if 0
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
	#endif
	int GetLevel() const {
		if (Ptrs::brk) return 0;
		if (Ptrs::line) return 1;
		if (Ptrs::part) return 2;
		if (Ptrs::song) return 3;
		if (Ptrs::release) return 4;
		if (Ptrs::artist) return 5;
		return 6;
	}
	
	void MergeOwner();
	
	template <class A, class B>
	static void GetSnapsLevel(int level, A* owner, Array<B>& sub, Vector<PatternSnap*>& level_snaps) {
		if (owner->GetLevel() == level)
			level_snaps.Add(owner);
		else
			for (B& o : sub)
				GetSnapsLevel(level, &o, o.GetSub(), level_snaps);
	}
	
	template <class A, class B>
	static void GetAttributes(A* owner, Array<B>& sub, Index<SnapAttrStr>& attrs) {
		for (const SnapAttrStr& a : owner->attributes.GetKeys())
			attrs.FindAdd(a);
		for (B& o : sub)
			GetAttributes(&o, o.GetSub(), attrs);
	}
	template <class A, class B>
	static void GetLineSnapshots(A* owner, Array<B>& sub, const String& txt_line, Vector<PatternSnap*>& snaps) {
		if (sub.IsEmpty()) {
			ASSERT(!owner->txt.IsEmpty());
			if (ToLower(owner->txt) == txt_line)
				snaps.Add(owner);
		}
		else {
			for (B& o : sub)
				GetLineSnapshots(&o, o.GetSub(), txt_line, snaps);
		}
	}
	template <class A, class B>
	static void ResolveIdT(A* owner, Array<B>& sub) {
		owner->PatternSnap::ResolveId();
		for (B& o : sub) ResolveIdT(&o, o.GetSub());
	}
	
	template <class A, class B>
	static void ClearAttrs(A* owner, Array<B>& sub) {
		owner->PatternSnap::attributes.Clear();
		owner->PatternSnap::partscore.Clear();
		for (B& o : sub) ClearAttrs(&o, o.GetSub());
	}
	
	template <class B>
	String GetStructuredText(bool pretty, int indent, const Array<B>& sub) const;

	
	#define PATTERNMASK_MACROS \
		void GetSnapsLevel(int level, Vector<PatternSnap*>& level_snaps) {PatternSnap::GetSnapsLevel(level, this, GetSub(), level_snaps);} \
		void GetAttributes(Index<SnapAttrStr>& attrs) {PatternSnap::GetAttributes(this, GetSub(), attrs);} \
		void GetLineSnapshots(const String& txt_line, Vector<PatternSnap*>& snaps) {PatternSnap::GetLineSnapshots(this, GetSub(), txt_line, snaps);} \
		void ResolveId() {ResolveIdT(this, GetSub());} \
		String GetStructuredText(bool pretty, int indent=0) const {return PatternSnap::GetStructuredText(pretty, indent, GetSub());} \
		void MergeOwner() {PatternSnap::MergeOwner(this, GetSub());} \
		void ClearAttrs() {PatternSnap::ClearAttrs(this, GetSub());}
	
};




#endif
