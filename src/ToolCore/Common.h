#ifndef _SongTool_Common_h_
#define _SongTool_Common_h_


#define TODO Panic("TODO");




enum {
	RHYME_A_A,
	RHYME_AB_AB,
	RHYME_AAC_BBC,
	//RHYME_A_AB,
	
	RHYME_A_A_A,
	RHYME_AB_AB_AB_,
	RHYME_AAD_BBC_CCD,
	
	RHYME_A_A_A_A,
	RHYME_A_B_A_B,
	RHYME_A_C_B_C,
	RHYME_AA_C_BB_C,
	
	RHYME_COUNT
};

extern const char* RhymeSchemes[RHYME_COUNT][3];
extern const int RhymeSchemeLineCount[RHYME_COUNT];

int FindRhymeType(const char* name);









#define ATTR_LIST \
	ATTR_ITEM(GENDER, "Gender", "female", "male") \
	ATTR_ITEM(RATIONALITY, "Rationality", "reasonable", "unreasonable") \
	ATTR_ITEM(RESPONSIBILITY, "Responsibility", "accountable", "irresponsible") \
	ATTR_ITEM(AVERAGE_EXPECTATIONS, "Average expectations", "expectation-conformed", "expectation-opposed") \
	ATTR_ITEM(IMAGERY, "Imagery", "trope-reinforcing", "unique") \
	ATTR_ITEM(EXPRESSION, "Expression", "allegorical", "explicit") \
	ATTR_ITEM(RELATIONSHIP_FOCUS, "Relationship focus", "partner-focused", "individual-focused") \
	ATTR_ITEM(INTEGRITY, "Integrity", "honest", "twisted") \
	ATTR_ITEM(SOCIAL, "Social", "libertarian", "authoritarian") \
	ATTR_ITEM(ECONOMIC, "Economic", "liberal", "conservative") \
	ATTR_ITEM(CULTURE, "Culture", "individualistic", "collective") \
	ATTR_ITEM(HUMAN_STRENGTH, "Human strength", "strong", "weak") \
	ATTR_ITEM(MOTIVATION, "Motivation", "rewarding", "punishing") \
	ATTR_ITEM(SEXUALIZATION, "Sexualization", "sexual", "non-sexual") \
	ATTR_ITEM(EXPECTATIONS, "Expectations", "acceptance", "perfection") \
	ATTR_ITEM(MOOD_JOYFUL_MELANCHOLIC, "Mood", "joyful", "melancholic") \
	ATTR_ITEM(MOOD_PLAYFUL_SERIOUS, "Mood", "playful", "serious") \
	ATTR_ITEM(MOOD_UPLIFTING_HEAVY, "Mood", "uplifting", "heavy") \
	ATTR_ITEM(MOOD_LIGHTHEARTED_SOMBER, "Mood", "lighthearted", "somber") \
	ATTR_ITEM(MOOD_HUMOROUS_DRAMATIC, "Mood", "humorous", "dramatic") \
	ATTR_ITEM(ATTITUDE_HOPEFUL_DESPAIR, "Attitude", "hopeful", "despair") \
	ATTR_ITEM(ATTITUDE_OPTIMISTIC_PESSIMISTIC, "Attitude", "optimistic", "pessimistic") \
	ATTR_ITEM(ATTITUDE_ATTITUDE_OPEN_CLOSED, "Attitude", "open", "closed") \
	ATTR_ITEM(SEXUALITY, "Sexuality", "adventurous", "limited") \
	ATTR_ITEM(SEXUAL_ACTING, "Sexual Acting", "confident", "sensitive") \
	ATTR_ITEM(SEXUAL_COMMITMENT, "Sexual Commitment", "polygamy", "monogamy") \
	ATTR_ITEM(SEXUAL_ORIENTATION, "Sexual Orientation", "heterosexual", "homosexual") \
	ATTR_ITEM(SEXUAL_PREFERENCE, "Sexual Preference", "kinky", "normal") \
	ATTR_ITEM(PHYSICAL_PREFERENCE, "Physical Preference", "enhancement", "natural") \
	ATTR_ITEM(RATIONAL_BELIEF, "Rational believers", "scientific", "religious") \
	ATTR_ITEM(THEOLOGICAL_OPPOSITE, "Theological opposites", "atheistic", "theistic") \
	ATTR_ITEM(SEEKER_OF_TRUTH, "Seekers of truth", "spiritual seeker", "skeptic") \
	ATTR_ITEM(GROUP_FAITH, "Group faith", "individual spirituality", "organized religion") \
	ATTR_ITEM(BELIF_COMMUNITY, "Belief communities", "secular society", "religious community") \
	ATTR_ITEM(INTUITIVE_THINKER, "Intuitive thinkers", "mystic practitioner", "rationalist") \
	ATTR_ITEM(FAITH_EXTREME, "Faith extremes", "agnostic", "religious fundamentalist") \
	ATTR_ITEM(OLD_AND_NEW_BELIEVER, "Old and new believers", "new age spirituality", "traditional religion") \
	ATTR_ITEM(BELIF_SPECTRUM, "Belief spectrum", "believer", "non-believer") \
	ATTR_ITEM(FAITH_AND_REASON_SEEKER, "Faith and reason seekers", "divine worshipers", "rational thinker") \


namespace Attr {

enum {
#define ATTR_ITEM(e, g, i0, i1) e,
ATTR_LIST
#undef ATTR_ITEM
ATTR_COUNT
};

extern const char* AttrKeys[ATTR_COUNT+1][4];

}



struct Pipe;


struct SnapAttr : Moveable<SnapAttr> {
	int group = 0, item = 0;
	
	bool operator==(const SnapAttr& a) const {return group == a.group && item == a.item;}
	void Clear() {
		group = 0;
		item = 0;
	}
	void Serialize(Stream& s) {
		s	% group
			% item;
	}
	/*void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}*/
	String ToString() const {return IntStr(group) + ":" + IntStr(item);}
	hash_t GetHashValue() const {CombineHash c; c.Put(group); c.Put(item); return c;}
};

struct SnapAttrStr : Moveable<SnapAttrStr> {
	String group, item;
	int group_i = -1, item_i = -1;
	bool has_id = false;
	
	void Serialize(Stream& s) {
		s	% group
			% item
			% group_i
			% item_i
			% has_id;
	}
	bool operator==(const SnapAttrStr& a) const {return group == a.group && item == a.item;}
	bool operator==(const SnapAttr& a) const {ASSERT(has_id); return group_i == a.group && item_i == a.item;}
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
	bool RealizeId(Pipe& pipe) const;
	void Load(const SnapAttr& sa);
	void SetFromId(Pipe& pipe, int group, int item);
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
	
	bool operator()(const DataFile& a, const DataFile& b) const {
		return a.file_title < b.file_title;
	}
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
struct Pipe;
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
	void Serialize(Stream& s) {
		s	% (int&)ctx
			% (int&)mode
			% (int&)dir;
	}
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
	/*void Jsonize(JsonIO& json) {
		json
			("ctx", (int&)ctx)
			("mode", (int&)mode)
			("dir", (int&)dir)
			;
	}*/
	
};

template <class T>
struct PArr {
	
private:
	T obj[CTX_COUNT][MODE_COUNT][DIR_COUNT];
public:
	
	PArr() {}
	
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < MODE_COUNT; j++)
				for(int k = 0; k < DIR_COUNT; k++)
					json
						("obj[" + IntStr(i) + "][" + IntStr(j) + "][" + IntStr(k) + "]", obj[i][j][k]);
	}
	void Serialize(Stream& s) {
		for(int i = 0; i < CTX_COUNT; i++)
			for(int j = 0; j < MODE_COUNT; j++)
				for(int k = 0; k < DIR_COUNT; k++)
					s % obj[i][j][k];
	}
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
	
	void Serialize(Stream& s) {
		for(int j = 0; j < MODE_COUNT; j++)
			s % obj[j];
	}
	void Jsonize(JsonIO& json) {
		for(int j = 0; j < MODE_COUNT; j++)
			json
				("obj[" + IntStr(j) + "]", obj[j]);
	}
	T& operator[](const SnapArg& a) {
		ASSERT(a.ctx == CTX_TEXT);
		ASSERT(a.dir == FORWARD);
		a.Chk();
		return obj[a.mode];
	}
	const T& operator[](const SnapArg& a) const {
		ASSERT(a.ctx == CTX_TEXT);
		ASSERT(a.dir == FORWARD);
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

struct EditorPtrs {
	Artist*			artist = 0;
	Release*		release = 0;
	Song*			song = 0;
	
	int GetActiveArtistIndex() const;
	int GetActiveReleaseIndex() const;
	int GetActiveSongIndex() const;
	String GetBreakInDatabaseString() const;
	
	void RealizePipe();
	
};

struct PipePtrs {
	Pipe*			pipe = 0;
	Part*			part = 0;
	Line*			line = 0;
	Break*			brk = 0;
	SnapArg			a;
	
	void Clear() {memset(this, 0, sizeof(PipePtrs));}
	int GetActivePartIndex() const;
	void CopyPtrs(const PipePtrs& p) {
		memcpy(this, &p, sizeof(PipePtrs));
	}
	String GetBreakInSongString() const;
};

struct Context {
	EditorPtrs			ed;
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
	bool HasSong() const {return ed.song;}
	
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




#define IMMEDIATE_TYPES_EQUAL(T) bool operator==(const T& t) const {return MemoryCompare(this, &t, sizeof(T)) == 0;}

struct FileLocation : Moveable<FileLocation> {
	String file;
	int cursor = -1;
	int line = -1;
	int col = -1;
	
	bool operator==(const FileLocation& t) const {
		return	file == t.file &&
				cursor == t.cursor &&
				line == t.line &&
				col == t.col;
	}
	void Clear() {file.Clear(); cursor = -1; line = -1; col = -1;}
	void SetFileLocation(const FileLocation& loc) {*this = loc;}
	void operator=(const FileLocation& loc) {
		file = loc.file;
		cursor = loc.cursor;
		line = loc.line;
		col = loc.col;
	}
	void Serialize(Stream& s) {s % file % cursor % line % col;}
	bool operator<(const FileLocation& l) const {
		if (file != l.file) return file < l.file;
		if (line != l.line) return line < l.line;
		return col < l.col;
	}
	bool operator>(const FileLocation& l) const {
		if (file != l.file) return file > l.file;
		if (line != l.line) return line > l.line;
		return col > l.col;
	}
	bool operator()(const FileLocation& a, const FileLocation& b) const {
		if (a.line != b.line) return a.line < b.line;
		return a.col < b.col;
	}
	String ToString() const {return file + ":" + IntStr(line) + ":" + IntStr(col);}
	
};


enum { PROCMSG_UNKNOWN, PROCMSG_SKIP, PROCMSG_INFO, PROCMSG_WARNING, PROCMSG_ERROR };

struct ProcMsg : Moveable<ProcMsg>, public FileLocation {
	int severity = 0;
	String src;
	String msg;
	String parts[3];

	ProcMsg() {}
	ProcMsg(const ProcMsg& e) { *this = e; }
	ProcMsg(String source, FileLocation loc, int severity, String msg) {
		this->src = source;
		SetFileLocation(loc);
		this->severity = severity;
		this->msg = msg;
	}
	
	bool operator==(const ProcMsg& t) const {
		return	severity == t.severity &&
				src == t.src &&
				msg == t.msg &&
				parts[0] == t.parts[0] &&
				parts[1] == t.parts[1] &&
				parts[2] == t.parts[2] &&
				FileLocation::operator==(t);
	}
	
	void operator=(const ProcMsg& e) {
		FileLocation::operator=(e);
		severity = e.severity;
		src = e.src;
		msg = e.msg;
		parts[0] = e.parts[0];
		parts[1] = e.parts[1];
		parts[2] = e.parts[2];
	}
	
	void Serialize(Stream& s) {FileLocation::Serialize(s); s % severity % src % msg;}
	
	ProcMsg& Set(FileLocation loc, int severity, String src, String msg) {
		SetFileLocation(loc);
		this->severity = severity;
		this->src = src;
		this->msg = msg;
		return *this;
	}
	
	String ToString() const {
		String s;
		if (src.GetCount())
			s << ToUpper(src) << ": ";
		s	<< GetFileName(file)
			<< ":" << line
			<< ":" << col
			<< ": " << GetSeverityString()
			<< ": " << msg;
		return s;
	}
	
	String GetSeverityString() const {
		switch (severity) {
			case PROCMSG_UNKNOWN:	return "unknown";
			case PROCMSG_SKIP:		return "skip";
			case PROCMSG_INFO:		return "info";
			case PROCMSG_WARNING:	return "warning";
			case PROCMSG_ERROR:		return "error";
			default:				return "<invalid severity>";
		}
	}
};





class TextMatchFinder {
	struct Line : Moveable<Line> {
		int no;
		WString orig_txt;
		WString spaceless_txt;
		Vector<int> spaceless_orig_cursor;
	};
	Vector<Line> lines;
	
public:
	TextMatchFinder(String txt) {Parse(txt);}
	
	void Parse(String txt);
	bool Find(String line, Point& pt);
	
};



struct CoverSuggestionData {
	String artist;
	String album;
	int year;
	String genre;
	String singer_description;
	int count_of_songs;
	
	struct Song {
		String name, lyrics;
	};
	Array<Song> songs;
};



const Vector<String>& CommonArtists();

void TrimBothAllLines(String& s);
void RealizeDoubleNewlinesOnNumbered(String& s);
void RealizeDoubleNewlinesBeforeTitles(String& s);
Vector<String> GetStructureParts(String s);

template <class T> void CheckSerialisationData(const String& json) {}
template <> void CheckSerialisationData<Song>(const String& json);

template <class T>
void LoadFromJsonFileStandard(T& o, const String& path) {
	String s = UPP::LoadFile(path);
	s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
	LoadFromJson(o, s);
}

template <class T>
void StoreAsJsonFileStandard(T& o, const String& path, bool pretty=false) {
	String s = StoreAsJson(o, pretty);
	//CheckSerialisationData<T>(s); // Never was an real issue
	s = ToCharset(CHARSET_UTF8, s, CHARSET_DEFAULT);
	s.Replace("\\r\\n", "\\n");
	s.Replace("\r\n", "\n");
	FileOut fout(path);
	fout << s;
	fout.Close();
}

enum {
	BIAS_DEMOCRAT,
	BIAS_REPUBLICAN,
	BIAS_NONPARTISAN,
	BIAS_BEAUTIFULMUSIC,
};

String GetDemocratBiasHeader();
String GetRepublicanBiasHeader();
String GetNonPartisanBiasHeader();
String GetBiasHeader(int mode);


namespace Attr {

struct Group;

}

#endif
