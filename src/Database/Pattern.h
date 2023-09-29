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
		txt.Clear();
		attributes.Clear();
		partscore.Clear();
		data.Clear();
		PatternMask::Clear();
	}
	/*void Jsonize(JsonIO& json) {
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
	}*/
	void SetId(int i) {id = i;}
	void ResolveId() {
		for (const SnapAttrStr& sa : attributes.GetKeys())
			sa.RealizeId(*pipe);
		PatternMask::ResolveId();
	}
	void FindAddAttr(const SnapAttrStr& sa);
	
	template <class A, class B>
	static void MergeOwnerT(const SnapArg& a, A* owner, Array<B>& sub) {
		if (sub.GetCount() >= 2) {
			for (B& o : sub)
				MergeOwnerT(a, &o, o.GetSub());
			
			VectorMap<SnapAttrStr, int> attrs;
			for (B& o : sub) {
				for (const SnapAttrStr& sa : o.Get(a).PatternSnap::attributes)
					attrs.GetAdd(sa, 0)++;
			}
			int c = sub.GetCount();
			for(int i = 0; i < attrs.GetCount(); i++) {
				if (attrs[i] != c)
					continue;
				const SnapAttrStr& common = attrs.GetKey(i);
				for (B& o : sub)
					o.Get(a).attributes.RemoveKey(common);
				owner->Get(a).PatternSnap::FindAddAttr(common);
			}
		}
	}
	template <class A, class B>
	static void DeepClearSnapT(const SnapArg& a, A* owner, Array<B>& sub) {
		owner->Get(a).PatternSnap::Clear();
		for (B& o : sub)
			DeepClearSnapT(a, &o, o.GetSub());
	}
	int GetLevel() const {
		if (PipePtrs::brk) return 0;
		if (PipePtrs::line) return 1;
		if (PipePtrs::part) return 2;
		if (PipePtrs::pipe) return 3;
		//if (Ptrs::song) return 3;
		//if (Ptrs::release) return 4;
		//if (Ptrs::artist) return 5;
		return 4;
	}
	
	
	template <class B>
	static void GetSnapsLevel(const SnapArg& a, int level, PatternSnap* owner, Array<B>& sub, Vector<PatternSnap*>& level_snaps) {
		if (owner->GetLevel() == level)
			level_snaps.Add(owner);
		else
			for (B& o : sub)
				GetSnapsLevel(a, level, &o.Get(a), o.GetSub(), level_snaps);
	}
	
	template <class A, class B>
	static void GetSnapAttributes(const SnapArg& a, A* owner, Array<B>& sub, Index<SnapAttrStr>& attrs) {
		for (const SnapAttrStr& a : owner->Get(a).attributes.GetKeys())
			attrs.FindAdd(a);
		for (B& o : sub)
			GetSnapAttributes(a, &o, o.GetSub(), attrs);
	}
	template <class A, class B>
	static void GetMaskAttributes(const SnapArg& a, A* owner, Array<B>& sub, Index<SnapAttrStr>& attrs) {
		for (const SnapAttrStr& a : owner->Get(a).PatternMask::mask.GetKeys())
			attrs.FindAdd(a);
		for (B& o : sub)
			GetMaskAttributes(a, &o, o.GetSub(), attrs);
	}
	template <class A, class B>
	static void GetLineSnapshots(const SnapArg& a, A* owner, Array<B>& sub, const String& txt_line, Vector<PatternSnap*>& snaps) {
		if (ToLower(owner->Get(a).txt) == txt_line)
			snaps.Add(&owner->Get(a));
		for (B& o : sub)
			GetLineSnapshots(a, &o, o.GetSub(), txt_line, snaps);
	}
	template <class A, class B>
	static void ResolveIdT(const SnapArg& a, A* owner, Array<B>& sub) {
		owner->Get(a).PatternSnap::ResolveId();
		for (B& o : sub) ResolveIdT(a, &o, o.GetSub());
	}
	
	template <class A, class B>
	static void ClearAttrsT(const SnapArg& a, A* owner, Array<B>& sub) {
		owner->Get(a).PatternSnap::attributes.Clear();
		owner->Get(a).PatternSnap::partscore.Clear();
		for (B& o : sub) ClearAttrsT(a, &o, o.GetSub());
	}
	
	template <class B>
	String GetStructuredText(const SnapArg& a, bool pretty, int indent, const Array<B>& sub) const;

	
	#define FOR_SNAP for(const SnapArg& a : AllArgs())
	
	#define PATTERNMASK_MACROS \
		void GetSnapsLevel(const SnapArg& a, int level, Vector<PatternSnap*>& level_snaps) {this->Get(a).PatternSnap::GetSnapsLevel(a, level, &Get(a), GetSub(), level_snaps);} \
		void GetSnapAttributes(const SnapArg& a, Index<SnapAttrStr>& attrs) {this->Get(a).PatternSnap::GetSnapAttributes(a, this, GetSub(), attrs);} \
		void GetMaskAttributes(const SnapArg& a, Index<SnapAttrStr>& attrs) {this->Get(a).PatternSnap::GetMaskAttributes(a, this, GetSub(), attrs);} \
		void GetLineSnapshots(const SnapArg& a, const String& txt_line, Vector<PatternSnap*>& snaps) {this->Get(a).PatternSnap::GetLineSnapshots(a, this, GetSub(), txt_line, snaps);} \
		String GetStructuredText(const SnapArg& a, bool pretty, int indent=0) const {return this->Get(a).PatternSnap::GetStructuredText(a, pretty, indent, GetSub());} \
		void ResolveId() {FOR_SNAP PatternSnap::ResolveIdT(a, this, GetSub());} \
		void MergeOwner() {FOR_SNAP PatternSnap::MergeOwnerT(a, this, GetSub());} \
		void ClearAttrs() {FOR_SNAP PatternSnap::ClearAttrsT(a, this, GetSub());} \
		void DeepClearSnap() {FOR_SNAP PatternSnap::DeepClearSnapT(a, this, GetSub());} \
		void GetContextLevel(int level, Vector<SnapContext*>& ctxs) {SnapContext::GetContextLevelT(this, GetSub(), level, ctxs);}
	
};


struct SnapContext {
	PArr<PatternSnap> snap;
	
	
	void Serialize(Stream& s) {
		s % snap;
	}
	PatternSnap& Get(const SnapArg& a) {
		return snap[a];
	}
	
	const PatternSnap& Get(const SnapArg& a) const {
		return snap[a];
	}
	
	const PatternSnap& Get0() const {
		return snap[ZeroArg()];
	}
	
	
	template <class A, class B>
	static void GetContextLevelT(A* owner, Array<B>& sub, int level, Vector<SnapContext*>& ctxs) {
		int level0 = owner->Get0().GetLevel();
		if (level == level0)
			ctxs.Add(owner);
		for (B& o : sub)
			GetContextLevelT(&o, o.GetSub(), level, ctxs);
	}
	
	void MergeOwner();
	void Clear() {
		for(const SnapArg& a : AllArgs())
			Get(a).PatternSnap::Clear();
	}
	void ResolveId() {
		for(const SnapArg& a : AllArgs())
			Get(a).PatternSnap::ResolveId();
	}
	/*void Jsonize(JsonIO& json) {
		for(const SnapArg& a : AllArgs())
			json("snap" + a.SubscriptString(), Get(a));
	}*/
	/*void SetArtistPtr(Artist* art) {
		for(const SnapArg& a : AllArgs())
			Get(a).Ptrs::artist = art;
	}
	void SetReleasePtr(Release* r) {
		for(const SnapArg& a : AllArgs())
			Get(a).Ptrs::release = r;
	}
	void SetSongPtr(Song* s) {
		for(const SnapArg& a : AllArgs())
			Get(a).Ptrs::song = s;
	}*/
	void SetPipePtr(Pipe* p) {
		for(const SnapArg& a : AllArgs())
			Get(a).PipePtrs::pipe = p;
	}
	void SetPartPtr(Part* p) {
		for(const SnapArg& a : AllArgs())
			Get(a).PipePtrs::part = p;
	}
	void SetLinePtr(Line* l) {
		for(const SnapArg& a : AllArgs())
			Get(a).PipePtrs::line = l;
	}
	void SetBreakPtr(Break* b) {
		for(const SnapArg& a : AllArgs())
			Get(a).PipePtrs::brk = b;
	}
	
	
	void SetOwner(SnapContext& ctx) {
		for(const SnapArg& a : AllArgs())
			Get(a).owner = &ctx.Get(a);
	}
	void CopyPtrs(const SnapContext& ctx) {
		for(const SnapArg& a : AllArgs())
			Get(a).CopyPtrs(ctx.Get(a));
	}
	void SetId(int id) {
		for(const SnapArg& a : AllArgs())
			Get(a).SetId(id);
	}
};


#endif
