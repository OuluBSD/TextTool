#ifndef _SongTool_Common_h_
#define _SongTool_Common_h_


#define TODO Panic("TODO");

struct SnapAttr : Moveable<SnapAttr> {
	int group = 0, item = 0;
	
	bool operator==(const SnapAttr& a) const {return group == a.group && item == a.item;}
	void Clear() {
		group = 0;
		item = 0;
	}
	void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}
	void Serialize(Stream& s) {s % group % item;}
	String ToString() const {return IntStr(group) + ":" + IntStr(item);}
	hash_t GetHashValue() const {CombineHash c; c.Put(group); c.Put(item); return c;}
};

struct SnapAttrStr : Moveable<SnapAttrStr> {
	String group, item;
	int group_i = -1, item_i = -1;
	bool has_id = false;
	
	bool operator==(const SnapAttrStr& a) const {return group == a.group && item == a.item;}
	bool operator==(const SnapAttr& a) const {RealizeId(); return group_i == a.group && item_i == a.item;}
	void Clear() {
		group = "";
		item = "";
	}
	void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}
	void RealizeId() const;
	void Load(const SnapAttr& sa);
	void Serialize(Stream& s) {s % group % item % group_i % item_i % has_id;}
	String ToString() const {return group + ":" + item;}
	hash_t GetHashValue() const {CombineHash c; c << group << item; return c;}
	bool operator()(const SnapAttrStr& a, const SnapAttrStr& b) const {
		if (a.has_id && b.has_id)
			return a.group_i < b.group_i || a.item_i < b.item_i;
		else
			return a.group < b.group || a.item < b.item;
	}
};


struct DataFile {
	String file_title;
	
};

typedef enum : int {
	MODE_INVALID = -1,
	
	MALE,
	FEMALE,
	
	MODE_COUNT,
	MODE_BEGIN = MALE,
	
	GENDER_COUNT = MODE_COUNT,
	GENDER_COMMON_COUNT = GENDER_COUNT + 1,
	GENDER_COMMON_WEIGHTED_COUNT = GENDER_COUNT + 2,
	COMMON = GENDER_COUNT,
	WEIGHTED = GENDER_COMMON_COUNT,
} SnapMode;

typedef enum : int {
	REV_INVALID = -1,
	
	FORWARD,
	BACKWARD,
	
	REV_COUNT,
	REV_BEGIN = FORWARD,
} RevMode;

String GetModeString(SnapMode m);
String GetCommonModeString(SnapMode m);

typedef enum : int {
	CTX_INVALID = -1,
	CTX_VISUAL,
	CTX_TEXT,
	
	CTX_COUNT,
	CTX_BEGIN = CTX_VISUAL,
	CTX_END = CTX_COUNT,
} GroupContext;


enum {
	PTR_COUNT = (int)MODE_COUNT * (int)CTX_COUNT * (int)REV_COUNT,
};


String GetGroupContextString(GroupContext ctx);
String GetGroupContextNaturalDescription(GroupContext ctx);

struct Artist;
struct Release;
struct Song;
struct Part;
struct Line;
struct Break;
struct PatternSnap;
struct AttrScoreGroup;

struct SnapArg : Moveable<SnapArg> {
	GroupContext	ctx = CTX_INVALID;
	SnapMode		mode = MODE_INVALID;
	RevMode			rev = REV_INVALID;
	
	SnapArg() {}
	SnapArg(GroupContext c, SnapMode m, RevMode r) : ctx(c), mode(m), rev(r) {}
	SnapArg(const SnapArg& a) : ctx(a.ctx), mode(a.mode), rev(a.rev) {}
	void Chk() const {
		ASSERT(ctx >= CTX_BEGIN && ctx < CTX_END);
		ASSERT(mode >= MODE_BEGIN && mode < MODE_COUNT);
		ASSERT(rev >= REV_BEGIN && rev < REV_COUNT);
	}
	bool operator==(const SnapArg& a) const {
		return
			ctx == a.ctx &&
			mode == a.mode &&
			rev == a.rev;
	}
	void Set(int i) {
		int j = i / (MODE_COUNT * 2);
		ctx = (GroupContext)j;
		i = i % (MODE_COUNT * 2);
		j = i / 2;
		mode = (SnapMode)j;
		i = i % 2;
		rev = (RevMode)i;
	}
	int Get() const {
		Chk();
		return (int)ctx * (2*MODE_COUNT) + (int)mode*2 + (int)rev;
	}
	int GetMode() const {return (int)mode;}
	String SubscriptString() const {
		Chk();
		return "[" + IntStr(ctx) + "][" + IntStr(mode) + "][" + IntStr(rev) + "]";
	}
	void InverseDir() {
		switch (rev) {
			case FORWARD: rev = BACKWARD; break;
			case BACKWARD: rev = FORWARD; break;
			default: break;
		}
	}
	void InverseMode() {
		switch (mode) {
			case MALE: mode = FEMALE; break;
			case FEMALE: mode = MALE; break;
			default: break;
		}
	}
	void operator=(const SnapArg& a) {ctx = a.ctx; mode = a.mode; rev = a.rev;}
	operator int() const {return Get();}
	
};

template <class T>
struct PArr {
	T obj[CTX_COUNT][MODE_COUNT][REV_COUNT];
	
	PArr() {}
	
	T& operator[](const SnapArg& a) {
		a.Chk();
		return obj[a.ctx][a.mode][a.rev];
	}
	const T& operator[](const SnapArg& a) const {
		a.Chk();
		return obj[a.ctx][a.mode][a.rev];
	}
	
	T* begin() {
		T* o = &obj[0][0][0];
		return o;
	}
	T* end() {
		T* o = &obj[0][0][0];
		o += PTR_COUNT;
		return o;
	}
};

String GetSnapString(const SnapArg& a);

inline SnapArg ZeroArg() {
	SnapArg a;
	a.ctx = CTX_BEGIN;
	a.mode = MODE_BEGIN;
	a.rev = REV_BEGIN;
	return a;
}

inline const Vector<SnapArg>& SnapArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < MODE_COUNT; j++)
				for(int k = 0; k < REV_COUNT; k++)
					a.Add(SnapArg((GroupContext)i, (SnapMode)j, (RevMode)k));
	}
	return a;
}

inline const Vector<SnapArg>& GenderArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for(int j = 0; j < GENDER_COUNT; j++)
			a.Add(SnapArg(CTX_BEGIN, (SnapMode)j, REV_BEGIN));
	}
	return a;
}

struct Ptrs {
	Artist*			artist = 0;
	Release*		release = 0;
	Song*			song = 0;
	Part*			part = 0;
	Line*			line = 0;
	Break*			brk = 0;
	SnapArg			a;
	
	void Clear() {memset(this, 0, sizeof(Ptrs));}
	int GetActivePartIndex() const;
	int GetActiveArtistIndex() const;
	int GetActiveReleaseIndex() const;
	int GetActiveSongIndex() const;
	void CopyPtrs(const Ptrs& p) {
		memcpy(this, &p, sizeof(Ptrs));
	}
	String GetBreakInSongString() const;
	String GetBreakInDatabaseString() const;
};

struct Context {
	Ptrs				p;
	PArr<PatternSnap*>	snap;
	
	AttrScoreGroup*		active_scoregroup = 0;
	bool				active_wholesong = false;
	
	
	Context() {
		PatternSnap** it = snap.begin();
		PatternSnap** end = snap.end();
		while (it != end) {
			*it = 0;
			it++;
		}
	}
	bool HasSong() const {return p.song;}
	
	//Ptrs& operator[](int i) {ASSERT(i >= 0 && i < MODE_COUNT); return p[i];}
};



Color GetPartColor(const String& name, Color def=Color(56,170,255));
String GetContextString(GroupContext ctx);
String GetReverseString(RevMode rev);
Color GetGenderColor(SnapMode m);

String FixInvalidChars(const String& s);

template <class T>
void CalculateWeightedGenderDifference(Vector<double>& values, const Vector<T>& score, const Vector<T>& other_score);





#endif
