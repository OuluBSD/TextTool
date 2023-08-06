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
	bool RealizeId() const;
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
	MODE_BEGIN = 0,
	
	MALE = 0,
	FEMALE,
	COMMON,
	WEIGHTED,
	
	MODE_COUNT,
	
	HUMAN_INPUT_MODE_COUNT = COMMON+1,
	
	/*GENDER_COUNT = MODE_COUNT,
	GENDER_COMMON_COUNT = GENDER_COUNT + 1,
	GENDER_COMMON_WEIGHTED_COUNT = GENDER_COUNT + 2,
	COMMON = GENDER_COUNT,
	WEIGHTED = GENDER_COMMON_COUNT,*/
} SnapMode;

typedef enum : int {
	DIR_INVALID = -1,
	DIR_BEGIN = 0,
	
	FORWARD = 0,
	BACKWARD,
	
	DIR_COUNT,
} DirMode;

String GetModeString(SnapMode m);
//String GetCommonModeString(int m);

typedef enum : int {
	CTX_INVALID = -1,
	CTX_BEGIN = 0,
	
	CTX_TEXT = 0,
	CTX_VISUAL,
	
	CTX_COUNT,
	CTX_END = CTX_COUNT,
} GroupContext;


enum {
	PTR_COUNT = (int)MODE_COUNT * (int)CTX_COUNT * (int)DIR_COUNT,
};


String GetUnknownText(GroupContext g);
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
	DirMode			dir = DIR_INVALID;
	
	SnapArg() {}
	SnapArg(GroupContext c, SnapMode m, DirMode r) : ctx(c), mode(m), dir(r) {}
	SnapArg(const SnapArg& a) : ctx(a.ctx), mode(a.mode), dir(a.dir) {}
	bool Is() const {
		return	ctx >= CTX_BEGIN && ctx < CTX_END &&
				mode >= MODE_BEGIN && mode < MODE_COUNT &&
				dir >= DIR_BEGIN && dir < DIR_COUNT;
	}
	void Chk() const {
		ASSERT(ctx >= CTX_BEGIN && ctx < CTX_END);
		ASSERT(mode >= MODE_BEGIN && mode < MODE_COUNT);
		ASSERT(dir >= DIR_BEGIN && dir < DIR_COUNT);
	}
	bool operator!=(const SnapArg& a) const {return !(a == *this);}
	bool operator==(const SnapArg& a) const {
		return
			ctx == a.ctx &&
			mode == a.mode &&
			dir == a.dir;
	}
	void Set(int i) {
		int j = i / (MODE_COUNT * 2);
		ctx = (GroupContext)j;
		i = i % (MODE_COUNT * 2);
		j = i / 2;
		mode = (SnapMode)j;
		i = i % 2;
		dir = (DirMode)i;
	}
	int Get() const {
		Chk();
		return (int)ctx * (2*MODE_COUNT) + (int)mode*2 + (int)dir;
	}
	int GetMode() const {return (int)mode;}
	String SubscriptString() const {
		Chk();
		return "[" + IntStr(ctx) + "][" + IntStr(mode) + "][" + IntStr(dir) + "]";
	}
	void InverseDir() {
		switch (dir) {
			case FORWARD: dir = BACKWARD; break;
			case BACKWARD: dir = FORWARD; break;
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
	void operator=(const SnapArg& a) {ctx = a.ctx; mode = a.mode; dir = a.dir;}
	operator int() const {return Get();}
	void Jsonize(JsonIO& json) {
		json
			("ctx", (int&)ctx)
			("mode", (int&)mode)
			("dir", (int&)dir)
			;
	}
	
};

template <class T>
struct PArr {
	
private:
	T obj[CTX_COUNT][MODE_COUNT][DIR_COUNT];
public:
	
	PArr() {}
	
	T& operator[](const SnapArg& a) {
		a.Chk();
		return obj[a.ctx][a.mode][a.dir];
	}
	const T& operator[](const SnapArg& a) const {
		a.Chk();
		return obj[a.ctx][a.mode][a.dir];
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

template <class T>
struct MArr {
	
private:
	T obj[MODE_COUNT];
public:
	
	MArr() {}
	
	T& operator[](const SnapArg& a) {
		a.Chk();
		return obj[a.mode];
	}
	const T& operator[](const SnapArg& a) const {
		a.Chk();
		return obj[a.mode];
	}
	
	T* begin() {
		T* o = &obj[0];
		return o;
	}
	T* end() {
		T* o = &obj[0];
		o += MODE_COUNT;
		return o;
	}
};

String GetSnapString(const SnapArg& a);

inline SnapArg ZeroArg() {
	SnapArg a;
	a.ctx = CTX_BEGIN;
	a.mode = MODE_BEGIN;
	a.dir = DIR_BEGIN;
	return a;
}

inline const Vector<SnapArg>& AllArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < MODE_COUNT; j++)
				for(int k = 0; k < DIR_COUNT; k++)
					a.Add(SnapArg((GroupContext)i, (SnapMode)j, (DirMode)k));
	}
	return a;
}

inline const Vector<SnapArg>& ModeArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for(int j = 0; j < MODE_COUNT; j++)
			a.Add(SnapArg(CTX_BEGIN, (SnapMode)j, DIR_BEGIN));
	}
	return a;
}
inline const Vector<SnapArg>& TextInputModeArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for(int j = 0; j < HUMAN_INPUT_MODE_COUNT; j++)
			a.Add(SnapArg(CTX_TEXT, (SnapMode)j, FORWARD));
	}
	return a;
}
inline const Vector<SnapArg>& TextWeightedModeArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for(int j = HUMAN_INPUT_MODE_COUNT; j < MODE_COUNT; j++)
			a.Add(SnapArg(CTX_TEXT, (SnapMode)j, FORWARD));
	}
	return a;
}
inline const Vector<Tuple2<SnapArg,SnapArg>>& ModeRevArgs() {
	static thread_local Vector<Tuple2<SnapArg,SnapArg>> a;
	if (a.IsEmpty()) {
		for(int j = 0; j < MODE_COUNT; j++) {
			Tuple2<SnapArg,SnapArg>& t = a.Add();
			t.a = SnapArg(CTX_BEGIN, (SnapMode)j, FORWARD);
			t.b = SnapArg(CTX_BEGIN, (SnapMode)j, BACKWARD);
		}
	}
	return a;
}

inline const Vector<SnapArg>& ContextArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			a.Add(SnapArg((GroupContext)i, MODE_BEGIN, DIR_BEGIN));
	}
	return a;
}

inline const Vector<SnapArg>& WeightedContextArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			a.Add(SnapArg((GroupContext)i, WEIGHTED, DIR_BEGIN));
	}
	return a;
}

inline const Vector<SnapArg>& ContextModeArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < MODE_COUNT; j++)
				a.Add(SnapArg((GroupContext)i, (SnapMode)j, FORWARD));
	}
	return a;
}

inline const Vector<SnapArg>& HumanInputContextArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for (int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < HUMAN_INPUT_MODE_COUNT; j++)
				a.Add(SnapArg((GroupContext)i, (SnapMode)j, FORWARD));
	}
	return a;
}

inline const Vector<SnapArg>& HumanInputTextArgs() {
	static thread_local Vector<SnapArg> a;
	if (a.IsEmpty()) {
		for(int j = 0; j < HUMAN_INPUT_MODE_COUNT; j++)
			a.Add(SnapArg(CTX_TEXT, (SnapMode)j, FORWARD));
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
String GetDirectionString(DirMode dir);
Color GetGenderColor(SnapMode m);
int CountWords(const String& s);

String FixInvalidChars(const String& s);

template <class T>
void CalculateWeightedGenderDifference(Vector<double>& values, const Vector<T>& score, const Vector<T>& other_score);





#endif
