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

enum {
	MALE,
	FEMALE,
	MALE_REVERSED,
	FEMALE_REVERSED,
	
	PTR_COUNT,
	GENDER_COUNT = MALE_REVERSED,
	
	GENDER_COMMON_COUNT = GENDER_COUNT + 1,
	GENDER_COMMON_WEIGHTED_COUNT = GENDER_COUNT + 2,
	COMMON = GENDER_COUNT,
	WEIGHTED = GENDER_COMMON_COUNT,
};

#define CHKMODE(x) ASSERT(x >= 0 && x < PTR_COUNT);

String GetModeString(int i);
String GetCommonModeString(int i);

typedef enum : int {
	CTX_INVALID = -1,
	CTX_VISUAL,
	CTX_TEXT,
	
	CTX_COUNT,
	CTX_BEGIN = CTX_VISUAL
} GroupContext;


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

struct Ptrs {
	Artist*			artist = 0;
	Release*		release = 0;
	Song*			song = 0;
	Part*			part = 0;
	Line*			line = 0;
	Break*			brk = 0;
	int				mode = -1;
	GroupContext	group_ctx = CTX_INVALID;
	
	void Clear() {memset(this, 0, sizeof(Ptrs));}
	int GetActivePartIndex() const;
	int GetActiveArtistIndex() const;
	int GetActiveReleaseIndex() const;
	int GetActiveSongIndex() const;
	void CopyPtrs(const Ptrs& p) {
		*this = p;
	}
	String GetBreakInSongString() const;
	String GetBreakInDatabaseString() const;
};

struct Context {
	Ptrs			p;
	PatternSnap*	snap[PTR_COUNT] = {0,0,0,0};
	
	AttrScoreGroup*	active_scoregroup = 0;
	bool			active_wholesong = false;
	
	
	Context() {
		//for(int i = 0; i < PTR_COUNT; i++)
		//	p[i].mode = i;
	}
	bool HasSong() const {return p.song;}
	
	//Ptrs& operator[](int i) {ASSERT(i >= 0 && i < PTR_COUNT); return p[i];}
};



Color GetPartColor(const String& name, Color def=Color(56,170,255));
Color GetGenderColor(int i);

String FixInvalidChars(const String& s);

template <class T>
void CalculateWeightedGenderDifference(Vector<double>& values, const Vector<T>& score, const Vector<T>& other_score);





#endif
