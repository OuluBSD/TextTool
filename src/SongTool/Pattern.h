#ifndef _SongTool_Pattern_h_
#define _SongTool_Pattern_h_


struct Part;

struct PatternSnap : PatternMask {
	String						txt;
	Index<SnapAttrStr>			attributes;
	Vector<int>					partscore;
	VectorMap<String,String>	data;
	int							syllables = 0;
	String						impact;
	Vector<int>					impact_score;
	
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
			("txt",				txt)
			("attributes",		attributes)
			("partscore",		partscore)
			("data",			data)
			("syllables",		syllables)
			("impact",			impact)
			("impact_score",	impact_score)
			;
		PatternMask::Jsonize(json);
	}
	void SetId(int i) {id = i;}
	void ResolveId() {
		for (const SnapAttrStr& sa : attributes.GetKeys())
			sa.RealizeId();
		PatternMask::ResolveId();
	}
	void FindAddAttr(const SnapAttrStr& sa);
	
	template <class A, class B>
	static void MergeOwner(int mode, A* owner, Array<B>& sub) {
		if (sub.GetCount() >= 2) {
			for (B& o : sub)
				MergeOwner(mode, &o, o.GetSub());
			
			VectorMap<SnapAttrStr, int> attrs;
			for (B& o : sub) {
				for (const SnapAttrStr& sa : o.snap[mode].PatternSnap::attributes)
					attrs.GetAdd(sa, 0)++;
			}
			int c = sub.GetCount();
			for(int i = 0; i < attrs.GetCount(); i++) {
				if (attrs[i] != c)
					continue;
				const SnapAttrStr& common = attrs.GetKey(i);
				for (B& o : sub)
					o.snap[mode].attributes.RemoveKey(common);
				owner->snap[mode].PatternSnap::FindAddAttr(common);
			}
		}
	}
	template <class A, class B>
	static void DeepClearSnap(int mode, A* owner, Array<B>& sub) {
		owner->snap[mode].PatternSnap::Clear();
		for (B& o : sub)
			DeepClearSnap(mode, &o, o.GetSub());
	}
	int GetLevel() const {
		if (Ptrs::brk) return 0;
		if (Ptrs::line) return 1;
		if (Ptrs::part) return 2;
		if (Ptrs::song) return 3;
		if (Ptrs::release) return 4;
		if (Ptrs::artist) return 5;
		return 6;
	}
	
	
	template <class B>
	static void GetSnapsLevel(int mode, int level, PatternSnap* owner, Array<B>& sub, Vector<PatternSnap*>& level_snaps) {
		if (owner->GetLevel() == level)
			level_snaps.Add(owner);
		else
			for (B& o : sub)
				GetSnapsLevel(mode, level, &o.snap[mode], o.GetSub(), level_snaps);
	}
	
	template <class A, class B>
	static void GetAttributes(int mode, A* owner, Array<B>& sub, Index<SnapAttrStr>& attrs) {
		for (const SnapAttrStr& a : owner->snap[mode].attributes.GetKeys())
			attrs.FindAdd(a);
		for (B& o : sub)
			GetAttributes(mode, &o, o.GetSub(), attrs);
	}
	template <class A, class B>
	static void GetLineSnapshots(int mode, A* owner, Array<B>& sub, const String& txt_line, Vector<PatternSnap*>& snaps) {
		if (ToLower(owner->snap[mode].txt) == txt_line)
			snaps.Add(&owner->snap[mode]);
		for (B& o : sub)
			GetLineSnapshots(mode, &o, o.GetSub(), txt_line, snaps);
	}
	template <class A, class B>
	static void ResolveIdT(int mode, A* owner, Array<B>& sub) {
		owner->snap[mode].PatternSnap::ResolveId();
		for (B& o : sub) ResolveIdT(mode, &o, o.GetSub());
	}
	
	template <class A, class B>
	static void ClearAttrs(int mode, A* owner, Array<B>& sub) {
		owner->snap[mode].PatternSnap::attributes.Clear();
		owner->snap[mode].PatternSnap::partscore.Clear();
		for (B& o : sub) ClearAttrs(mode, &o, o.GetSub());
	}
	
	template <class B>
	String GetStructuredText(int mode, bool pretty, int indent, const Array<B>& sub) const;

	
	#define FOR_SNAP for (int i = 0; i < PTR_COUNT; i++) this->snap[i].
	
	#define PATTERNMASK_MACROS \
		void GetSnapsLevel(int mode, int level, Vector<PatternSnap*>& level_snaps) {this->snap[mode].PatternSnap::GetSnapsLevel(mode, level, &snap[mode], GetSub(), level_snaps);} \
		void GetAttributes(int mode, Index<SnapAttrStr>& attrs) {this->snap[mode].PatternSnap::GetAttributes(mode, this, GetSub(), attrs);} \
		void GetLineSnapshots(int mode, const String& txt_line, Vector<PatternSnap*>& snaps) {this->snap[mode].PatternSnap::GetLineSnapshots(mode, this, GetSub(), txt_line, snaps);} \
		String GetStructuredText(int mode, bool pretty, int indent=0) const {return this->snap[mode].PatternSnap::GetStructuredText(mode, pretty, indent, GetSub());} \
		void ResolveId() {FOR_SNAP ResolveIdT(i, this, GetSub());} \
		void MergeOwner() {FOR_SNAP PatternSnap::MergeOwner(i, this, GetSub());} \
		void ClearAttrs() {FOR_SNAP PatternSnap::ClearAttrs(i, this, GetSub());} \
		void DeepClearSnap() {FOR_SNAP PatternSnap::DeepClearSnap(i, this, GetSub());} \
		void GetContextLevel(int level, Vector<SnapContext*>& ctxs) {SnapContext::GetContextLevelT(this, GetSub(), level, ctxs);}
	
};


struct SnapContext {
	PatternSnap snap[PTR_COUNT];
	
	
	
	template <class A, class B>
	static void GetContextLevelT(A* owner, Array<B>& sub, int level, Vector<SnapContext*>& ctxs) {
		int level0 = owner->snap[0].GetLevel();
		if (level == level0)
			ctxs.Add(owner);
		for (B& o : sub)
			GetContextLevelT(&o, o.GetSub(), level, ctxs);
	}
	
	void MergeOwner();
	void Clear() {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].PatternSnap::Clear();
	}
	void ResolveId() {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].PatternSnap::ResolveId();
	}
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < PTR_COUNT; i++)
			json("snap[" + IntStr(i) + "]", snap[i]);
	}
	void SetArtistPtr(Artist* a) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::artist = a;
	}
	void SetReleasePtr(Release* r) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::release = r;
	}
	void SetSongPtr(Song* s) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::song = s;
	}
	void SetPartPtr(Part* p) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::part = p;
	}
	void SetLinePtr(Line* l) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::line = l;
	}
	void SetBreakPtr(Break* b) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].Ptrs::brk = b;
	}
	
	
	void SetOwner(SnapContext& ctx) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].owner = &ctx.snap[i];
	}
	void CopyPtrs(SnapContext& ctx) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].CopyPtrs(ctx.snap[i]);
	}
	void SetId(int id) {
		for(int i = 0; i < PTR_COUNT; i++)
			snap[i].SetId(id);
	}
};


#endif
