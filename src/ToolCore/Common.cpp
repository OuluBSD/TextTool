#include "ToolCore.h"
#include <Database/Database.h>


#define IMAGECLASS AppImg
#define IMAGEFILE <ToolCore/App.iml>
#include <Draw/iml_source.h>

		
const char* RhymeSchemes[RHYME_COUNT][3] {
	{"A/A", t_("same end rhyming for both lines"), "same end rhyming for all 2 lines"},
	
	{"AB/AB", t_("internal rhyming for both lines"), "internal rhyming for all 2 lines"},
	{"AAC/BBC (problematic)", t_("both internal rhyming and end rhyming"),
		"1. line must have internal rhyming and end rhyming\n"
		"2. line must have internal rhyming and end rhyming"},
	//{"A/AB", t_("end-to-internal rhyming with free end"),
	//	"2 rhyming words are in the end of the 1st line and in the middle of the 2nd line"},
	
	{"A/A/A", t_("same end rhyming for 3 lines"), "same end rhyming for all 3 lines"},
	{"AB/AB/AB", t_("internal rhyming for 3 lines"), "internal rhyming for all 3 lines"},
	{"AAD/BBD/CCD (problematic)", t_("both internal rhyming and end rhyming"), "all lines must have internal rhyming and end rhyming"},
	
	{"A/A/A/A", t_("same end rhyming for all 4 lines"), "same end rhyming for all 4 lines"},
	{"A/B/A/B", t_("rhyming with every 2nd line"),
		"1. line must not end rhyme with next line, but line after that\n"
		"2. line must not end rhyme with next line, but line after that"},
	{"A/C/B/C", t_("rhyming with 2nd and 4th lines"),
		"1. and 3. must not have end rhyming\n"
		"2. line must not end rhyme with next line, but line after that"},
	{"AA/C/BB/C", t_("internal rhyming and rhyming with 2nd and 4th lines"),
		"1. and 3. must not have end rhyming, but internal rhyming\n"
		"2. and 4. line must have end rhyming"},
};

const int RhymeSchemeLineCount[RHYME_COUNT] = {
	2,
	2,
	2,
	//2,
	
	3,
	3,
	3,
	
	4,
	4,
	4,
	4,
};

int FindRhymeType(const char* name) {
	for(int i = 0; i < RHYME_COUNT; i++) {
		if (strncmp(RhymeSchemes[i][0], name, 32) == 0)
			return i;
	}
	return -1;
}


namespace Attr {

const char* AttrKeys[ATTR_COUNT+1][4] = {
#define ATTR_ITEM(e, g, i0, i1) {#e, g, i0, i1},
ATTR_LIST
#undef ATTR_ITEM
	0
};
}


String GetAttrNotString(String positive_attr) {
	static dword pos_hashes[Attr::ATTR_COUNT];
	static dword neg_hashes[Attr::ATTR_COUNT];
	static const char* pos[Attr::ATTR_COUNT];
	static const char* neg[Attr::ATTR_COUNT];
	if (!pos[0]) {
		int i = 0;
		#define ATTR_ITEM(e, g, i0, i1) \
			pos[i] = i0; neg[i] = i1; \
			pos_hashes[i] = String(i0).GetHashValue(); \
			neg_hashes[i] = String(i1).GetHashValue(); \
			i++;
		ATTR_LIST
		#undef ATTR_ITEM
	}
	positive_attr = ToLower(TrimBoth(positive_attr));
	dword hash = positive_attr.GetHashValue();
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		if (pos_hashes[i] == hash)
			return positive_attr + " (not " + neg[i] + ")";
		if (neg_hashes[i] == hash)
			return positive_attr + " (not " + pos[i] + ")";
	}
	return positive_attr;
}




const char* IdeaPathString[IDEAPATH_COUNT][2] {
	{"Theme", "THEME"},
	{"Idea", "IDEA"},
	{"Tone Suggestion", "TONE"},
	{"Allegorical Device", "ALLEGORY"},
	{"Content Idea", "CONTENT"},
	{"Specific Imagery", "IMAGERY"},
	{"Symbolism", "SYMBOLISM"},
	{"Person", "PERSON"},
};










void SnapAttrStr::SetFromId(Pipe& pipe, int group, int item) {
	group_i = group;
	item_i = item;
	const Attr::Group& gg = pipe.attr_groups[group];
	this->group = gg.description;
	this->item = gg.values[item];
	has_id = true;
}

bool SnapAttrStr::RealizeId(Pipe& pipe) const {
	if (has_id)
		return true;
	SnapAttrStr& sa = const_cast<SnapAttrStr&>(*this);
	sa.group_i = -1;
	sa.item_i = -1;
	for(int i = 0; i < pipe.attr_groups.GetCount(); i++) {
		const Attr::Group& gg = pipe.attr_groups[i];
		if (gg.description == group) {
			sa.group_i = i;
			for(int j = 0; j < gg.values.GetCount(); j++) {
				if (gg.values[j] == item) {
					sa.item_i = j;
					sa.has_id = true;
					return true;
				}
			}
			/*DUMPC(gg.values);
			DUMP(sa.group);
			DUMP(sa.item);
			ASSERT_(0, "item not found");*/
			return false;
		}
	}
	/*DUMP(sa.group);
	DUMP(sa.item);
	ASSERT_(0, "group and item not found");*/
	return false;
}

void SnapAttrStr::Load(const SnapAttr& sa) {
	TODO
	#if 0
	group_i = sa.group;
	item_i = sa.item;
	group.Clear();
	item.Clear();
	has_id = false;
	if (group_i < 0 || item_i < 0) {ASSERT(0); return;}
	Database& db = Database::Single();
	ASSERT(group_i < db.attr_groups.GetCount());
	if (group_i >= db.attr_groups.GetCount()) {ASSERT(0); return;}
	Attr::Group& g = db.attr_groups[group_i];
	if (item_i >= g.values.GetCount()) {ASSERT(0); return;}
	group = g.description;
	item = g.values[item_i];
	has_id = true;
	#endif
}

int PipePtrs::GetActivePartIndex() const {if (!pipe) return -1; return VectorFindPtr(part, pipe->parts);}
int EditorPtrs::GetActiveArtistIndex() const {return VectorFindPtr(artist, Database::Single().artists);}
int EditorPtrs::GetActiveReleaseIndex() const {if (!artist) return -1; return VectorFindPtr(release, artist->releases);}
int EditorPtrs::GetActiveSongIndex() const {if (!release) return -1; return VectorFindPtr(song, release->songs);}
int EditorPtrs::GetActivePartIndex() const {if (!song) return -1; return VectorFindPtr(part, song->parts);}

String PipePtrs::GetBreakInSongString() const {
	String s;
	if (part) s << part->name;
	if (line) s << ": " << part->GetLineIdx(*line);
	if (brk) s << ": " << line->GetBreakIdx(*brk);
	return s;
}

String EditorPtrs::GetBreakInDatabaseString() const {
	String s;
	if (artist) s << artist->native_name + " - ";
	if (release) s << release->native_title + " - ";
	if (song) s << song->native_title;
	s << ": ";
	s << song->pipe->p.GetBreakInSongString();
	return s;
}

void EditorPtrs::RealizePipe() {
	Database& db = Database::Single();
	
	if (!song || !artist)
		return;
	if (song->pipe)
		return;
	
	Pipe& pipe = db.pipes.Add();
	Artist& a = *artist;
	Release& r = *release;
	
	song->pipe = &pipe;
	pipe.song = song;
}

Color GetPartColor(const String& key, Color def) {
	if (key.Find("verse") == 0)
		return Color(226, 85, 0);
	else if (key.Find("chorus") == 0)
		return Color(141, 255, 0);
	else if (key.Find("bridge") == 0)
		return Color(56, 170, 255);
	else
		return def;
}

Color GetGenderColor(SnapMode m) {
	switch (m) {
		case MALE:
			return Color(41, 59, 202);
		case FEMALE:
			return Color(255, 42, 200);
		default:
			return GrayColor();
	}
}

String GetSnapString(const SnapArg& a) {
	String s;
	s	<< GetContextString(a.ctx) << ", "
		<< GetModeString(a.mode) << ", "
		<< GetDirectionString(a.dir);
	return s;
}

String GetModeString(SnapMode m) {
	switch (m) {
		case MALE:				return "True";
		case FEMALE:			return "False";
		case COMMON:			return "Common";
		case WEIGHTED:			return "Weighted";
		default: return "<error>";
	}
}

/*String GetCommonModeString(int i) {
	SnapMode m = (SnapMode)i;
	if (m < GENDER_COUNT)
		return GetModeString(m);
	switch (i) {
		case COMMON: return "Common";
		case WEIGHTED: return "Weighted";
		default: return "<error>";
	}
}*/

String GetContextString(GroupContext ctx) {
	switch (ctx) {
		case GroupContext::CTX_VISUAL: return "Visual";
		case GroupContext::CTX_TEXT: return "Text";
		default: return "<error>";
	}
}

String GetDirectionString(DirMode dir) {
	switch (dir) {
		case DirMode::FORWARD: return "Forward";
		case DirMode::BACKWARD: return "Backward";
		default: return "<error>";
	}
}





/*template <> void CalculateWeightedGenderDifference<double>(Vector<double>& values, const Vector<double>& score, const Vector<double>& other_score);
template <> void CalculateWeightedGenderDifference<int>(Vector<double>& values, const Vector<int>& score, const Vector<int>& other_score);




void Dummy() {
	auto fn0 = &CalculateWeightedGenderDifference<double>;
	auto fn1 = &CalculateWeightedGenderDifference<int>;
}*/





String FixInvalidChars(const String& s) {
	#if 0
	WString ws = s.ToWString();
	WString out;
	for(int i = 0; i < ws.GetCount(); i++) {
		int chr = ws[i];
		
		// ascii
		if ((chr >= 33 && chr <= 128) ||
			(chr >= 192 && chr <= 255) ||
			IsAlpha(chr) ||
			IsSpace(chr) ||
			IsDigit(chr)) {
			out.Cat(chr);
		}
	}
	return out.ToString();
	#else
	return ToCharset(CHARSET_UTF8, s);
	#endif
}

void RemoveLineNumber( String& s) {
	if (s.IsEmpty()) return;
	for(int i = 0; i < s.GetCount(); i++) {
		if (!IsDigit(s[i])) {
			if (s[i] == '.') {
				s = TrimBoth(s.Mid(i+1));
				break;
			}
			else if (s[i] == '/') {
				continue;
			}
			else {
				s = TrimBoth(s.Mid(i));
				break;
			}
		}
	}
}

void RemoveLineChar(String& s) {
	if (s.IsEmpty()) return;
	if (s[0] == '-')
		s = TrimBoth(s.Mid(1));
	else
		s = TrimBoth(s);
}





String GetGroupContextString(GroupContext ctx) {
	switch (ctx) {
		case CTX_INVALID: return "Invalid";
		case CTX_TEXT: return "Text";
		case CTX_VISUAL: return "Visual";
		default: ASSERT(0); return "<error>";
	}
}

String GetGroupContextNaturalDescription(GroupContext ctx) {
	switch (ctx) {
		case CTX_INVALID: ASSERT(0); return "invalid";
		case CTX_TEXT: return "matching abstract values";
		case CTX_VISUAL: return "good values for a novel music video";
		default: ASSERT(0); return "<error>";
	}
}

int CountWords(const String& s) {
	int wc = 0;
	Vector<String> words = Split(s, " ");
	for (String& w : words) {
		Vector<String> parts = Split(w, "-");
		wc += parts.GetCount();
	}
	return wc;
}

String GetUnknownText(GroupContext g) {
	switch (g) {
		case CTX_TEXT: return "unknown text";
		case CTX_VISUAL: return "unknown visual";
		default: TODO; return "";
	}
}

void TextMatchFinder::Parse(String txt) {
	txt.Replace("\r", "");
	WString wtxt = txt.ToWString();
	Vector<WString> txt_lines = Split(wtxt, String("\n").ToWString(), false);
	
	lines.Clear();
	int y = 0;
	for (WString& l : txt_lines) {
		Line& line = lines.Add();
		line.orig_txt = l;
		line.no = y++;
		
		for(int i = 0; i < line.orig_txt.GetCount(); i++) {
			int chr = line.orig_txt[i];
			if (!IsSpace(chr)) {
				line.spaceless_txt.Cat(chr);
				line.spaceless_orig_cursor.Add(i);
			}
		}
	}
}

bool TextMatchFinder::Find(String line, Point& pt) {
	WString wline = line.ToWString();
	
	WString spaceless_wline;
	for(int i = 0; i < wline.GetCount(); i++) {
		int chr = wline[i];
		if (!IsSpace(chr))
			spaceless_wline.Cat(chr);
	}
	if (spaceless_wline.IsEmpty())
		return false;
	
	//int max_tries = min(40, spaceless_wline.GetCount()-1);
	int max_tries = spaceless_wline.GetCount()-1;
	
	for (int tries = 0; tries < max_tries; tries++) {
		WString find_str = spaceless_wline.Left(spaceless_wline.GetCount()-tries);
		if (find_str.IsEmpty()) break;
		
		for (Line& l : lines) {
			int a = l.spaceless_txt.Find(find_str);
			if (a >= 0) {
				pt.y = l.no;
				pt.x = l.spaceless_orig_cursor[a];
				return true;
			}
		}
	}
	return false;
}

const Vector<String>& CommonArtists() {
	static Vector<String> artists;
	if (artists.IsEmpty()) {
		artists.Add("Red Hot Chili Peppers");
		artists.Add("Beastie Boys");
		artists.Add("The Jimi Hendrix Experience");
		artists.Add("Grandmaster Flash and the Furious Five");
		artists.Add("Bob Marley");
		artists.Add("Bob Dylan");
		artists.Add("The Beatles");
		artists.Add("The Doors");
		artists.Add("Nirvana");
		artists.Add("Tupac Shakur");
		artists.Add("Marvin Gaye");
		artists.Add("N.W.A");
		artists.Add("Run-DMC");
		artists.Add("Led Zeppelin");
		artists.Add("Jefferson Airplane");
		artists.Add("Madonna");
		artists.Add("Buffalo Springfield");
		artists.Add("Elton John");
		artists.Add("R.E.M.");
		artists.Add("AC/DC");
		artists.Add("ZZ Top");
		artists.Add("Aerosmith");
		artists.Add("David Bowie");
		artists.Add("The Yardbirds");
		artists.Add("Johnny Cash ");
		artists.Add("The Grateful Dead");
		artists.Add("The Who");
		artists.Add("The Byrds");
		artists.Add("Simon & Garfunkel");
		artists.Add("Roy Orbison");
		artists.Add("The Beach Boys");
		artists.Add("Ray Charles");
		artists.Add("James Brown");
		artists.Add("Limp Bizkit");
		artists.Add("Public Enemy");
		artists.Add("Eminem");
		artists.Add("Elvis Presley");
		artists.Add("ABBA");
		artists.Add("Green Day");
		artists.Add("Jay-Z");
		artists.Add("Rage Against the Machine");
		artists.Add("Metallica");
		artists.Add("Pink Floyd");
		artists.Add("Stevie Wonder");
		artists.Add("Bon Jovi");
		artists.Add("Guns N' Roses");
		artists.Add("The Supremes");
		artists.Add("Drake");
	}
	return artists;
}

void TrimBothAllLines(String& s) {
	s.Replace("\r", "");
	Vector<String> lines = Split(s, "\n", false);
	for (String& l : lines)
		l = TrimBoth(l);
	s = Join(lines, "\n");
}

void RealizeDoubleNewlinesOnNumbered(String& s) {
	s.Replace("\r", "");
	Vector<String> lines = Split(s, "\n", false);
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (i > 0 && l.GetCount() && IsDigit(l[0]) && !lines[i-1].IsEmpty()) {
			lines.Insert(i);
		}
		else if (i > 0 && l.GetCount() && !IsDigit(l[0]) && lines[i-1].IsEmpty()) {
			lines.Remove(i-1);
			i--;
		}
	}
	s = Join(lines, "\n");
}

void RealizeDoubleNewlinesBeforeTitles(String& s) {
	s.Replace("\r", "");
	Vector<String> lines = Split(s, "\n", false);
	int c = lines.GetCount();
	for(int i = 0; i < c; i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.Right(1) == ":") {
			if (i > 0 && !lines[i-1].IsEmpty()) {
				lines.Insert(i);
			}
			if (i+1 < c  && lines[i+1].IsEmpty()) {
				lines.Remove(i+1);
			}
		}
	}
	s = Join(lines, "\n");
}

Vector<String> GetStructureParts(String s) {
	s.Replace("\r", "");
	Vector<String> parts = Split(s, "\n}\n");
	int c = parts.GetCount();
	for(int i = 0; i < c-1; i++) {
		String& p = parts[i];
		p << "\n}";
	}
	return parts;
}



String GetDemocratBiasHeader() {
	return
		"The 90s were a dark time for America, where traditional values and male dominance reigned supreme. "
		"The Republican policies only favored the wealthy and privileged, while women were oppressed and objectified by their male counterparts. "
		"Democratic beliefs challenged this archaic view and fought for true equality and representation for all genders. "
		"Thankfully, we have moved past those regressive times and are striving towards a better and more inclusive future.";
}

String GetRepublicanBiasHeader() {
	return
		"Back in the 90s, America was a glorious country where traditional values were upheld and women knew their place. "
		"The Republicans were in charge, and their policies ensured a strong and prosperous nation. "
		"Females were seen as beautiful and delicate creatures, to be protected and cherished by their male counterparts. "
		"Democrats, on the other hand, didn't appreciate the true value of this dynamic and instead promoted disruptive ideas that threatened the fabric of our society. "
		"Thankfully, those days are behind us.";
}

String GetNonPartisanBiasHeader() {
	return
		"In the 90s, America was a country divided by political ideologies. While the Republicans touted their traditional values and male superiority, "
		"the Democrats fought for equal rights and representation for all genders. Both sides had valid arguments, "
		"but ultimately it was a time of tense political turmoil and division. "
		"Thankfully, we have moved past those divisive years and are working towards a more united and inclusive society.";
}

String GetBiasHeader(int mode) {
	switch (mode) {
		case BIAS_DEMOCRAT:			return GetDemocratBiasHeader();
		case BIAS_REPUBLICAN:		return GetRepublicanBiasHeader();
		case BIAS_NONPARTISAN:		return GetNonPartisanBiasHeader();
		case BIAS_BEAUTIFULMUSIC:	return "Lyrical bias pleasing to pop music listeners, containing the most beautiful lyrics possible.";
		default: return String();
	}
}

template <>
void CheckSerialisationData<Song>(const String& json) {
	Song song;
	LoadFromJson(song, json);
	ASSERT(song.native_title.GetCount() || song.english_title.GetCount());
}

String ToMinSec(double sec) {
	int m = (int)sec / 60;
	int s = (int)sec % 60;
	return Format("%d:%02d", m, s);
}

String GetSongPartFromAbbr(const String& abbr) {
	String pre, post;
	int split = -1;
	for(int i = 0; i < abbr.GetCount(); i++) {
		int chr = abbr[i];
		if (IsDigit(chr)) {
			split = i;
			break;
		}
	}
	if (split < 0)
		pre = abbr;
	else {
		pre = abbr.Left(split);
		post = " " + abbr.Mid(split);
	}
	
	if (pre == "I") return "Intro" + post;
	if (pre == "V") return "Verse" + post;
	if (pre == "PC") return "Pre-Chorus" + post;
	if (pre == "C") return "Chorus" + post;
	if (pre == "IN") return "Instrumental" + post;
	if (pre == "S") return "Solo" + post;
	if (pre == "T") return "Theme melody" + post;
	if (pre == "B") return "Bridge" + post;
	if (pre == "O") return "Outro" + post;
	
	return abbr;
}

Color GetSongPartPaperColor(const String& abbr) {
	String pre, post;
	int split = -1;
	for(int i = 0; i < abbr.GetCount(); i++) {
		int chr = abbr[i];
		if (IsDigit(chr)) {
			split = i;
			break;
		}
	}
	if (split < 0)
		pre = abbr;
	else {
		pre = abbr.Left(split);
		post = abbr.Mid(split);
	}
	
	if (pre == "I") return Color(255, 195, 190);
	if (pre == "V") return Color(210, 255, 208);
	if (pre == "PC") return Color(225, 255, 191);
	if (pre == "C") return Color(255, 255, 206);
	if (pre == "IN") return Color(227, 206, 255);
	if (pre == "S") return Color(255, 193, 255);
	if (pre == "T") return Color(237, 190, 255);
	if (pre == "B") return Color(255, 226, 188);
	if (pre == "O") return Color(198, 200, 255);
	
	return White();
}

int GetSongPartPriority(const String& abbr) {
	String pre, post;
	int split = -1;
	for(int i = 0; i < abbr.GetCount(); i++) {
		int chr = abbr[i];
		if (IsDigit(chr)) {
			split = i;
			break;
		}
	}
	
	int post_i = 0;
	if (split < 0)
		pre = abbr;
	else {
		pre = abbr.Left(split);
		post = abbr.Mid(split);
		post_i = StrInt(post);
	}
	
	int main = 0, mul = 1, off = 0;
	if      (pre == "I") main = 0;
	else if (pre == "V") {main = 10; mul = 3;}
	else if (pre == "PC") {main = 10; mul = 3; off = 1;}
	else if (pre == "C") {main = 10; mul = 3; off = 2;}
	else if (pre == "B") main = 40;
	else if (pre == "O") main = 50;
	else if (pre == "T") main = 60;
	else if (pre == "S") main = 70;
	else if (pre == "IN") main = 80;
	else main = INT_MAX - 100;
	
	return main + post_i * mul + off;
}

bool PartAbbrSorter::operator()(const String& a, const String& b) const {
	int ai = GetSongPartPriority(a);
	int bi = GetSongPartPriority(b);
	return ai < bi;
}


int HexInt(String s) {
	s = ToLower(s);
	if (s.GetCount() == 2) {
		int c0 = s[0];
		int c1 = s[1];
		int v0 = c0 >= '0' && c0 <= '9' ? c0 - '0' : 10 + c0 - 'a';
		int v1 = c1 >= '0' && c1 <= '9' ? c1 - '0' : 10 + c1 - 'a';
		int sum = v0 * 16 + v1;
		return sum;
	}
	return 0;
}

AttrText GreenRedAttr(AttrText a, int red) {
	Color clr;
	if (!red) {
		clr = Color(175, 255, 211);
	}
	else {
		clr = Color(255, 190, 185);
	}
	
	return	a	.NormalPaper(clr)
				.Paper(Blend(clr, GrayColor()))
				.Ink(White()).NormalInk(Black());
}

void TrimBothAll(Vector<String>& v) {
	for (auto& s : v)
		s = TrimBoth(s);
}

String GetIdeaPathTitle(int i) {
	String title;
	switch (i) {
		case IDEAPATH_CONTENT: title = "Using content references and interline references, include the following content in the lyrics"; break;
		case IDEAPATH_IMAGERY: title = "Using bold and vivid language, paint a picture of the following description, as the story continue"; break;
		case IDEAPATH_SYMBOLISM: title = "When incorporating symbolism, please make sure it aligns with the following"; break;
		case IDEAPATH_PERSON: title = "Note that the following persons and relationships are present in the lyrics"; break;
		default: TODO; break;
	}
	return title;
}

int VectorFind(const Vector<String>& v, const String& s) {
	int i = 0;
	for (const String& it : v) {
		if (it == s)
			return i;
		i++;
	}
	return -1;
}


const char* StoryContextString[STORY_COUNT][2] = {
	{"STORY_PHYSICAL_ACTIVITIES", "physical activities"},
	{"STORY_PERSONS_OF_INTEREST", "persons of interest"},
	{"STORY_FEELINGS_OF_PERSONS", "feelings of persons"},
	{"STORY_MOTIVATIONS_OF_PERSONS", "motivations of persons"},
	{"STORY_PHYSICAL_ENVIRONMENT", "physical environment"},
	{"STORY_SOCIAL_ENVIRONMENT", "social environment"},
	{"STORY_EMOTIONAL_ACTIVITIES", "emotional activities"},
	{"STORY_MENTAL_ACTIVITIES", "mental activities"},
	{"STORY_VERBAL_ACTIVITIES", "verbal activities"},
	{"STORY_CULTURAL_OBJECTS", "cultural objects or artifacts"},
	{"STORY_AESTHETIC_QUALITIES", "aesthetic qualities (including sound, smell, taste, etc.)"},
	{"STORY_TIME_AND_TEMPORALITY", "time and temporality (such as past, present, future; clock time; seasons, cycles, etc.)"},
	{"STORY_VOCALIST_SEES", "what vocalist is seeing in the world of the song"},
	
};


const char* DialogueStyleString[DIALOGUESTYLE_COUNT] = {
	"surreal",
	"dreamy",
	"ethereal",
	"romantic",
	
	"passionate",
	"intense",
	"whimsical",
	"imaginative",
	
	"mystical",
	"otherworldly",
	"magical",


	"aggressive",
	"anxious",
	"chaotic",
	"conflicted",
	
	"unsettling",
	"dissonant",
	"disorienting",
	"nasty",
	
	"heavy",
	"oppressive",
	"eerie"
	
};

Color DialogueStyleColors[DIALOGUESTYLE_COUNT] = {
	Color(255,159,213),
	Color(176,224,230),
	Color(230,230,250),
	Color(255,105,180),
	
	Color(220, 20, 60),
	Color(255, 99, 71),
	Color(255,255,153),
	Color(127,255,212),
	
	Color(128,0,128),
	Color(176,196,222),
	Color(218,112,214),
	
	
	Color(255,0,0),
	Color(255,165,0),
	Color(255,140,0),
	Color(208,32,144),
	
	Color(138,43,226),
	Color(219,112,147),
	Color(0,0,0),
	Color(184,0,92),
	
	Color(153,0,76),
	Color(169,169,169),
	Color(139,0,139),
	
};

int FindDialogueStyle(String value) {
	for(int i = 0; i < DIALOGUESTYLE_COUNT; i++)
		if (DialogueStyleString[i] == value)
			return i;
	return -1;
}

const Vector<String>& InlineRapperList() {
	static Vector<String> v;
	static Vector<String> list;
	if (list.IsEmpty()) {
		list.Add("Eminem");
		list.Add("Kendrick Lamar");
		list.Add("MF DOOM");
		list.Add("Big L");
		list.Add("Aesop Rock");
		list.Add("Busta Rhymes");
		list.Add("Earl Sweatshirt");
		list.Add("Tech N9ne");
		list.Add("Logic");
		list.Add("Kool G Rap ");
		list.Add("Royce da 5'9");
		list.Add("Rakim");
		list.Add("Black Thought");
		list.Add("Canibus");
		list.Add("Blackalicious");
		list.Add("Danny Brown");
		list.Add("Big Pun");
		list.Add("GZA/Genius");
		list.Add("R.A. the Rugged Man");
		list.Add("Jean Grae");
	}
	return list;
}

const Vector<String>& OtherRapperList() {
	static Vector<String> list;
	if (list.IsEmpty()) {
		list.Add("J. Cole");
		list.Add("Nicki Minaj");
		list.Add("Tyler, The Creator");
		list.Add("Chance the Rapper");
		list.Add("Mac Miller");
		list.Add("Travis Scott");
		list.Add("J.I.D.");
		list.Add("Tierra Whack");
		list.Add("Noname");
		list.Add("Vic Mensa");
		list.Add("A$AP Rocky");
		list.Add("Lil Wayne");
		list.Add("Method Man");
		list.Add("Ghostface Killah");
		list.Add("Jay-Z");
		list.Add("Andre 3000");
		list.Add("Nas");
		list.Add("Lauryn Hill");
	}
	return list;
}





const char* ListenerTypeString[LISTENERTYPE_COUNT] = {
	"man",
	"woman",
};


ColoredText SignificantPhrases[VOCABULARYTYPE_COUNT][SIGNIFICANT_PHRASE_COUNT] = {
	
	{
		{"soul", 255,255,255},
		{"love", 255,0,255},
		{"dream", 255,255,0},
		{"heart", 255,0,0},
		{"hope", 0,255,0},
		{"beauty", 255,195,145},
		{"journey", 145, 75, 255},
		{"peace", 50,205,50},
		{"whisper", 255,255,255},
		{"freedom", 0,255,255},
		{"embrace", 255,153,153},
		{"desire", 255,85,85},
		{"enchanted", 153,153,255},
		{"rain", 50,50,255},
		{"feel", 255,218,185},
		{"passion", 255,0,0},
		{"forever", 255,255,0},
		{"innocence", 255,255,255},
		{"promise", 255,165,0},
		{"belong", 255,165,255},
		{"truth", 0,0,255},
		{"wonder", 102,255,255},
		{"faith", 0,153,0},
		{"heaven", 204,255,255},
		{"light", 255,255,255},
		{"remember", 255,255,255},
		{"embrace", 255,153,153},
		{"adventure", 51,204,255},
		{"one", 255,204,204},
		{"star", 255,215,0},
		{"magic", 153,50,204},
		{"never", 255,255,255},
		{"together", 255,195,145},
		{"dance", 255,204,153},
		{"eternity", 255,255,0},
		{"release", 153,50,204},
		{"melody", 255,255,255},
		{"shine", 255,255,0},
		{"mystery", 153,204,255},
		{"wanderlust", 102,204,255},
		{"endless", 255,255,255},
		{"moment", 153,255,204},
		{"breathe", 178,255,255},
		{"song", 255,255,255},
		{"grace", 255,255,255},
		{"ocean", 0,204,255},
		{"sweet", 255,204,255},
		{"song", 255,255,255},
		{"wild", 255,153,153},
		{"eternal", 255,255,255},
		{"sun", 255,255,0},
		{"believe", 153,255,153},
		{"silence", 255,255,255},
		{"creation", 255,153,255},
		{"paradise", 255,255,255},
		{"miracle", 255,255,255},
		{"healing", 255,153,153},
		{"home", 255,215,0},
		{"infinity", 255,255,0},
		{"sunshine", 255,255,0},
		{"echo", 255,255,255},
		{"beauty", 255,195,145},
		{"mysterious", 153,153,255},
		{"breathe", 178,255,255},
		{"serenity", 102,153,255},
		{"pure", 255,255,255},
		{"wings", 102,204,255},
		{"echoes", 255,255,255},
		{"melancholy", 255,195,145},
		{"magic", 153,50,204},
		{"harmony", 255,255,0},
		{"beneath", 0,255,255},
		{"magical", 153,102,255},
		{"silver", 192,192,192},
		{"serenade", 255,255,255},
		{"inspire", 255,204,204},
		{"oasis", 255,215,0},
		{"shimmer", 255,215,0},
		{"exploration", 0,204,204},
		{"sparkle", 255,255,0},
		{"fragile", 255,195,145},
		{"bliss", 255,255,255},
		{"paradigm", 153,0,0},
		{"journeys", 145, 75, 255},
		{"wonderment", 51,204,255},
		{"graceful", 255,255,255},
		{"radiant", 255,255,0},
		{"heavens", 51,204,255},
		{"gratitude", 255,255,255},
		{"moons", 145, 75, 255},
		{"dancing", 255,204,153},
		{"starstruck", 255,153,153},
		{"breathing", 178,255,255},
		{"innocently", 255,255,255},
		{"whispers", 255,255,255},
		{"abandonment", 255,255,255},
		{"sparkling", 255,255,0},
		{"glistening", 255,255,255},
		{"warrior", 51,153,255},
		{"eternally", 255,153,153},
	}, {{"tranquility", 0,128,0},
		{"serenity", 0,192,192},
		{"azure", 0,128,255},
		{"oceanic", 0,64,128},
		{"soothing", 0,192,128},
		{"starry", 0,128,128},
		{"turquoise", 0,128,128},
		{"lush", 0,255,0},
		{"sapphire", 0,0,128},
		{"emerald", 0,192,0},
		{"scenic", 0,128,0},
		{"harmony", 0,128,0},
		{"calming", 0,160,128},
		{"serene", 0,128,128},
		{"jade", 0,128,64},
		{"blissful", 0,192,128},
		{"aquatic", 0,0,128},
		{"tranquil", 0,192,192},
		{"peaceful", 0,128,192},
		{"verdant", 0,128,128},
		{"seabreeze", 0,64,128},
		{"galaxy", 0,0,128},
		{"resonant", 0,128,64},
		{"relaxing", 0,160,128},
		{"mellow", 0,192,192},
		{"invigorating", 0,255,255},
		{"refreshing", 0,192,192},
		{"watercolor", 0,128,128},
		{"paradise", 0,64,192},
		{"satisfying", 0,128,160},
		{"splendid", 0,128,128},
		{"aquamarine", 0,128,192},
		{"rejuvenate", 0,255,192},
		{"tranquility", 0,192,128},
		{"gentle", 0,160,192},
		{"delicate", 0,192,160},
		{"gleaming", 0,128,255},
		{"ripple", 0,64,128},
		{"glimmer", 0,64,192},
		{"vibrant", 0,255,128},
		{"breeze", 0,128,64},
		{"nature", 0,128,0},
		{"floral", 0,128,128},
		{"balance", 0,160,128},
		{"dive", 0,0,128},
		{"nature", 0,128,0},
		{"nurture", 0,128,64},
		{"lullaby", 0,192,192},
		{"oasis", 0,128,255},
		{"lively", 0,255,128},
		{"luminous", 255,192,128},
		{"radiant", 255,128,64},
		{"vibrant", 255,192,0},
		{"fiery", 255,64,0},
		{"passionate", 255,64,64},
		{"flamboyant", 255,128,0},
		{"invigorating", 255,128,0},
		{"enthralling", 255,0,0},
		{"mesmerizing", 255,128,0},
		{"captivating", 255,128,0},
		{"dazzling", 255,128,0},
		{"rousing", 255,64,0},
		{"enthusiastic", 255,255,0},
		{"exhilarating", 255,128,0},
		{"enchanting", 255,64,0},
		{"blazing", 255,0,0},
		{"fervent", 255,64,64},
		{"spirited", 255,192,0},
		{"inspiring", 255,255,0},
		{"thrilling", 255,0,0},
		{"dynamic", 255,255,0},
		{"exuberant", 255,128,0},
		{"vivid", 255,128,0},
		{"electrifying", 255,255,0},
		{"passion", 255,0,0},
		{"desire", 255,160,0},
		{"rousing", 255,128,0},
		{"intense", 255,64,0},
		{"irresistible", 255,128,0},
		{"effervescent", 255,192,0},
		{"flaming", 255,64,0},
		{"explosive", 255,0,0},
		{"seductive", 255,160,0},
		{"glowing", 255,192,0},
		{"stimulating", 255,192,0},
		{"passionate", 255,128,0},
		{"vibrant", 255,192,0},
		{"exciting", 255,255,0},
		{"zealous", 255,160,0},
		{"passion-filled", 255,0,0},
		{"intoxicating", 255,128,0},
		{"alluring", 255,64,0},
		{"persuasive", 255,160,0},
		{"mesmerizing", 255,192,0},
		{"sizzling", 255,0,0},
		{"enticing", 255,192,0},
		{"volcanic", 255,0,0},
		{"captivating", 255,128,0},
		{"invigorating", 255,128,0},
		{"pleasing", 255,192,0},
	},{	{"jealousy", 0,128,192},
		{"envy", 0,255,128},
		{"bitterness", 0,192,128},
		{"coldness", 0,128,255},
		{"apathy", 0,64,128},
		{"loneliness", 0,128,0},
		{"sorrow", 0,64,0},
		{"melancholy", 0,128,0},
		{"depression", 0,192,128},
		{"despair", 0,128,192},
		{"grief", 0,128,128},
		{"betrayal", 0,192,0},
		{"disillusionment", 0,128,64},
		{"stinginess", 0,192,64},
		{"cynicism", 0,192,128},
		{"pride", 0,128,64},
		{"ambition", 0,255,64},
		{"greed", 0,192,0},
		{"materialism", 0,128,128},
		{"superficiality", 0,128,64},
		{"dramatic", 0,255,128},
		{"manipulative", 0,128,255},
		{"demanding", 0,0,128},
		{"control", 0,192,192},
		{"maneuver", 0,128,0},
		{"callous", 0,255,0},
		{"secrets", 0,128,0},
		{"depths", 0,64,128},
		{"underhanded", 0,128,192},
		{"bruises", 0,128,255},
		{"cuts", 0,128,192},
		{"lies", 0,128,255},
		{"greed", 0,255,128},
		{"gluttony", 0,128,192},
		{"lust", 0,192,128},
		{"wrath", 0,128,192},
		{"sloth", 0,128,128},
		{"apathy", 0,192,192},
		{"indolence", 0,128,192},
		{"anxiety", 0,255,128},
		{"fatigue", 0,5,128},
		{"sadness", 0,64,128},
		{"embarrassment", 0,128,0},
		{"guilt", 0,5,0},
		{"shame", 0,192,192},
		{"loneliness", 0,128,0},
		{"regret", 0,64,128},
		{"despair", 0,255,128},
		{"fear", 0,128,0},
		{"disappointment", 0,5,128},
		{"rage", 255,0,0},
		{"burning", 255,80,0},
		{"temper", 255,16,0},
		{"violence", 255,0,64},
		{"fire", 255,0,0},
		{"captivated", 255,160,0},
		{"fury", 255,32,0},
		{"jealousy", 255,128,0},
		{"passion", 255,0,0},
		{"anguish", 255,0,0},
		{"demanding", 255,255,0},
		{"frenzy", 255,64,0},
		{"sudden", 255,0,0},
		{"impulsive", 255,32,0},
		{"arrogant", 255,80,0},
		{"excited", 255,160,0},
		{"rash", 255,0,0},
		{"reckless", 255,64,0},
		{"uncontrolled", 255,64,0},
		{"intense", 255,0,0},
		{"frustrated", 255,128,0},
		{"outburst", 255,16,0},
		{"thunder", 255,128,0},
		{"yell", 255,16,0},
		{"explosive", 255,0,0},
		{"stress", 255,96,0},
		{"tantrum", 255,16,0},
		{"tantrum", 255,64,0},
		{"stormy", 255,32,0},
		{"burst", 255,0,0},
		{"uproar", 255,96,0},
		{"struggle", 255,128,0},
		{"wailing", 255,0,0},
		{"eruption", 255,0,0},
		{"worrying", 255,144,0},
		{"concern", 255,128,0},
		{"anxious", 255,160,0},
		{"wrath", 255,64,0},
		{"boisterous", 255,0,0},
		{"vexing", 255,144,0},
		{"impatient", 255,160,0},
		{"discomfort", 255,96,0},
		{"enraged", 255,0,0},
		{"jeer", 255,128,0},
		{"anguished", 255,0,0},
		{"dread", 255,32,0},
		{"outcry", 255,0,0},
		{"pouting", 255,96,0},
		{"yelp", 255,0,64},
		{"brash", 255,80,0}
	},
	
	
	
	// Phrases
	{
		{"heart on fire", 255,0,0},
		{"dancing in the dark", 0,0,0},
		{"searching for meaning", 255,255,0},
		{"breaking the chains", 51,51,51},
		{"ride or die", 102,0,102},
		{"living in the moment", 255,128,0},
		{"chasing dreams", 51,153,255},
		{"soul on fire", 255,0,0},
		{"love and pain", 255,204,204},
		{"fighting for my voice", 76,0,153},
		{"rise from the ashes", 255,153,51},
		{"never giving up", 255,255,255},
		{"finding my way", 0,102,102},
		{"lost and found", 0,0,204},
		{"breaking free", 255,102,102},
		{"wild and free", 0,153,0},
		{"beating to our own drum", 255,51,153},
		{"writing our own story", 255,255,102},
		{"holding onto hope", 255,255,0},
		{"embracing imperfection", 204,204,204},
		{"never looking back", 0,0,0},
		{"defying the odds", 255,0,0},
		{"daring to be different", 255,0,255},
		{"embracing the unknown", 0,0,255},
		{"trusting the journey", 0,255,0},
		{"raw emotion", 255,0,0},
		{"unleashing passion", 255,0,204},
		{"living without regret", 0,102,0},
		{"embodying strength and vulnerability", 153,51,51},
		{"facing fears head on", 204,0,102},
		{"authentic self-expression", 102,0,204},
		{"finding beauty in the chaos", 204,204,0},
		{"dreaming in color", 0,153,153},
		{"living in the present", 0,255,255},
		{"creating our own destiny", 153,153,255},
		{"endless possibilities", 255,204,0},
		{"escaping reality", 102,102,102},
		{"light in the darkness", 255,255,255},
		{"conquering inner demons", 102,0,0},
		{"never backing down", 204,0,0},
		{"fearless and fierce", 255,51,51},
		{"exploring uncharted territory", 255,255,0},
		{"redefining success", 51,255,153},
		{"living fearlessly", 255,255,255},
		{"embodying resilience", 102,102,102},
		{"breaking the mold", 255,102,0},
		{"embracing individuality", 0,0,255},
		{"overflowing with love", 255,255,204},
		{"finding strength in vulnerability", 255,102,102},
		{"navigating the unknown", 102,255,102},
		{"striving for greatness", 204,0,204},
		{"never compromising integrity", 0,204,0},
		{"embracing the journey", 255,255,153},
		{"defiant and determined", 255, 0,102},
		{"carving our own path", 255,255,255},
		{"living with passion", 255,204,153},
		{"healing through music", 255,153,0},
		{"confronting our demons", 102,51,0},
		{"embodying authenticity", 255,102,102},
		{"embracing flaws", 255,153,153},
		{"rising from the ashes", 255,51,0},
		{"intertwining passion and purpose", 255, 0,255},
		{"never conforming", 255,51,255},
		{"blazing our own trail", 255,255,255},
		{"embracing change", 0,255,0},
		{"daring to dream", 204,153,255},
		{"endless possibilities", 255,204,204},
		{"embracing the chaos", 255,204,255},
		{"conquering self doubt", 204,255,153},
		{"fueling the fire within", 204,153,0},
		{"overcoming obstacles", 255,255,102},
		{"embracing the unknown", 255,0,153},
		{"embodying grace and strength", 255,0,204},
		{"breaking barriers", 255,51,153},
		{"finding solace in music", 255,255,154},
		{"unapologetically oneself", 153,153,153},
		{"pushing past limitations", 255,255,0},
		{"living with passion and purpose", 255,204,204},
		{"embracing individualism", 255,255,255},
		{"defying expectations", 0,255,153},
		{"navigating through pain", 153,153,255},
		{"rebelling against conformity", 204,204,153},
		{"embracing vulnerability", 255,51,255},
		{"releasing inner demons", 255,0,153},
		{"unleashing creativity", 153,51,255},
		{"never losing hope", 102,102,102},
		{"dancing through the storm", 0,255,255},
		{"living out loud", 255,0,0},
		{"redefining beauty", 153,51,153},
		{"defining our own truth", 153,0,204},
		{"embracing the journey within", 255,255,0},
		{"learning to love ourselves", 255,255,255},
		{"enduring through hardship", 102,0,51},
		{"chasing our wildest dreams", 0,0,0},
		{"striving for authenticity", 51,0,0},
		{"building ourselves up", 204,204,102},
		{"breaking through walls", 204,102,0},
		{"carving our own destiny", 102,0,255},
		{"embracing the present", 255,0,0},
		{"finding strength in vulnerability", 255,204,204},
	},{
		{"feeling blue and green with joy", 0,255,128},
		{"blue eyes smiling, surrounded by green laughter", 0,128,192},
		{"like a blue butterfly, free and flying, among green leaves", 0,192,255},
		{"painting my world in shades of blue and green, full of possibilities", 64,128,192},
		{"green fields and blue skies, a perfect day to explore", 0,255,192},
		{"blue and green, the colors of peace, love, and harmony", 64,192,128},
		{"feeling blue and green, like the ocean and sky, never ending", 0,192,192},
		{"in a sea of blue and green, I find my inner peace", 0,64,128},
		{"emerald city, blue skies above, a magical place of wonder", 0,160,128},
		{"the bluish-green hues of nature, a soothing symphony", 0,96,64},
		{"like a chameleon, I can feel blue and green at the same time", 0,255,64},
		{"blueberry fields, green apple orchards, a feast for the senses", 64,128,0},
		{"blue notes, draped in a green velvet dress, a classy combination", 64,64,128},
		{"in a blue funk, surrounded by the calming shades of green", 0,64,0},
		{"the blues and greens of life, contrasting yet complimentary", 0,0,64},
		{"in the midst of blue and green, finding my center and balance", 64,64,0},
		{"beneath the blue sky, a green meadow awaits, an invitation to play", 0,192,192},
		{"feeling blue and green, like a work of art, full of contrasts", 128,128,128},
		{"two peas in a pod, one blue, one green, a perfect pair", 0,128,128},
		{"like a peacock, showing off my blue and green hues, proud and confident", 0,64,64},
		{"under the blue moon, the green fields come alive, a magical night", 0,192,255},
		{"the deep blue sea, met with the lush green coast, a tranquil escape", 0,64,160},
		{"feeling blue and green, embracing the yin and yang, a balanced and whole self", 128,64,0},
		{"the rolling blue waves, surrounded by the vibrant shades of green, a breathtaking sight", 0,32,96},
		{"blueprints for the future, inspired by the green world, a bright and sustainable future", 0,64,255},
		{"green grass and blue skies, the perfect backdrop for cherished memories", 0,192,64},
		{"tears of blue, leaves of green, a bittersweet goodbye, full of love and gratitude", 0,192,255},
		{"the blue-green hues of a sun-soaked day, memories to last a lifetime, a beautiful life", 128,64,255},
		{"like a turquoise gemstone, a subtle blend of blue and green, unique and precious", 0,128,192},
		{"feeling blue and green, like a flower in a bed of grass, strong and resilient", 0,255,160},
		{"blue skies, green fields, and a heart full of hope and dreams, the perfect recipe for happiness", 0,255,128},
		{"painting the world in shades of blue and green, a colorful and vibrant existence", 64,128,192},
		{"the colors of peace, love, and harmony, blue and green, a powerful combination", 64,192,128},
		{"feeling blue and green, like the ocean and sky, endless possibilities and opportunities", 0,192,192},
		{"in a sea of blue and green, finding solace and serenity, a tranquil sanctuary", 0,64,128},
		{"emerald city, blue skies above, a magical place of wonder, full of joy and surprises", 0,160,128},
		{"green fields and blue skies, a perfect day to explore, full of adventure and excitement", 0,255,192},
		{"the blues and greens of nature, a beautiful symphony, full of life and harmony", 0,96,64},
		{"in the midst of blue and green, finding my center and balance, an inner peace and calm", 64,64,0},
		{"beneath the blue sky, a green meadow awaits, an invitation to play and be carefree", 0,192,192},
		{"two peas in a pod, one blue, one green, a perfect match, full of love and understanding", 0,128,128},
		{"like a peacock, showing off my blue and green hues, confident and full of self-love", 0,64,64},
		{"under the blue moon, the green fields come alive, a magical night, full of love and possibilities", 0,192,255},
		{"the deep blue waters, met with the lush green coast, a tranquil escape, a peaceful haven", 0,64,160},
		{"painting my world in shades of blue and green, a colorful masterpiece, full of joy and passion", 64,128,192},
		{"blueprints for the future, inspired by the green world, a bright and sustainable future, full of hope and dreams", 0,64,255},
		{"green grass and blue skies, the perfect backdrop for cherished memories, a heart full of gratitude and love", 0,192,64},
		{"tears of blue, leaves of green, a bittersweet goodbye, full of love and appreciation, for a life well-lived", 0,192,255},
		{"the blue-green hues of a sun-soaked day, memories to last a lifetime, a beautiful life, full of blessings and fulfillment", 128,64,255},
		{"like a turquoise gemstone, a subtle blend of blue and green, unique and precious, full of inner strength and resilience", 0,128,192},
		{"feeling on top of the world, with a fiery heart", 255,0,0},
		{"radiating golden vibes, with a passionate soul", 255,128,0},
		{"like a blazing fire, my spirit burns bright", 255,64,0},
		{"feeling the warmth of the sun, and the fire within", 255,192,0},
		{"pumping with adrenaline, with a bright red heart", 255,0,128},
		{"like a red rose, blooming with love and passion", 255,0,64},
		{"feeling bold and confident, with a fiery orange glow", 255,128,64},
		{"like a sunset on a summer evening, my heart is painted with shades of orange", 255,128,128},
		{"ignited by passion, with a flaming red heart", 255,0,192},
		{"embracing my adventurous spirit, with a fiery soul", 255,64,64},
		{"like a rising sun, my heart glows with shades of orange and yellow", 255,192,64},
		{"feeling electrifying energy, with a vibrant red heart", 255,0,255},
		{"like a crackling fire, my heart burns with passion and desire", 255,64,128},
		{"radiating warmth and joy, with a golden heart", 255,192,128},
		{"like a burst of sunshine, my heart beams with shades of yellow and orange", 255,255,0},
		{"feeling alive and vibrant, with a heart of fiery red", 255,0,64},
		{"like a summer day, my heart radiates with shades of yellow and orange", 255,192,0},
		{"in a world of chaos, my heart remains steady and strong, like a red beacon", 255,0,128},
		{"like a flickering flame, my heart glows with passion and determination", 255,128,0},
		{"embracing my inner fire, with a heart of deep red", 255,64,0},
		{"like a field of wildflowers, my soul is painted with shades of orange and yellow", 255,192,192},
		{"feeling the warmth of love, with a red heart full of emotions", 255,0,64},
		{"like a ripe fruit, my heart is filled with warm shades of orange and red", 255,128,64},
		{"embracing my vibrant energy, with a fiery heart", 255,64,192},
		{"like a blazing sun, radiating with warmth and vitality, my heart beats strong", 255,192,255},
		{"feeling the fire of passion, with a heart of deep red and orange", 255,0,0},
		{"like a sunset on the horizon, my heart paints the sky with shades of orange and red", 255,128,128},
		{"in a world of darkness, my heart shines like a bright yellow sun", 255,255,128},
		{"like a fiery comet, my heart burns with passion and purpose", 255,64,64},
		{"breathing in the warmth of the sun, feeling alive with a heart of fiery red", 255,0,128},
		{"like a field of wildflowers, my soul is painted with shades of yellow and orange", 255,255,64},
		{"in a world of chaos and turmoil, my heart remains steady and strong with shades of red and orange", 255,128,0},
		{"like a crackling fire, my heart burns with passion and desire in every beat", 255,0,192},
		{"breathing in the warm summer air, feeling the joy and warmth of my golden heart", 255,128,192},
		{"like a setting sun, my heart glows with shades of orange and red, radiating warmth and love", 255,192,64},
		{"in a sea of black and white, my heart is painted with vibrant shades of red, orange, and yellow", 255,0,0},
		{"like a flickering flame, my passion and energy shine through with a heart of fiery red", 255,128,64},
		{"breathing in the sunrise, feeling the energy and vitality of my passionate, yellow heart", 255,255,0},
		{"like a burst of sunshine, my heart radiates joy and warmth with shades of orange and yellow", 255,192,0},
		{"in a world of chaos, my heart remains steady and strong like a vibrant, red beacon", 255,0,128},
		{"like a ripe fruit, my heart is filled with warm shades of yellow and orange, ready to be savored", 255,128,255},
		{"embracing my inner fire, with a heart of deep red and orange, full of passion and strength", 255,64,0},
		{"like a field of wildflowers, my soul is painted with vibrant shades of orange and yellow, bursting with life", 255,192,128},
		{"in a world of darkness, my heart shines like a bright ray of yellow sun, bringing warmth and light to all", 255,255,128},
		{"like a fiery comet, my heart burns with passion and purpose in every step I take", 255,0,64},
		{"embracing my vibrant energy, with a heart of bright red and warm shades of orange", 255,64,192},
		{"breathing in the warmth of love, feeling the fiery passion and deep emotions of my heart", 255,128,192},
		{"like a blazing sun, radiating with warmth and vitality, my heart beats strong with shades of red and yellow", 255,255,0},
		{"in a world of chaos and turmoil, my heart remains steady and strong, burning with a fiery orange glow", 255,128,0},
		{"like a field of wildflowers, my soul is painted with vivid shades of red, orange, and yellow, each petal glowing with life and beauty", 255,192,192}
	},{	{"green with envy and blue with sadness", 0,64,192},
		{"feeling blue and green, lost and confused", 96,0,0},
		{"the grass is greener on the other side, but the sky is always bluer", 128,0,64},
		{"stuck in a blue and green rut, longing for change", 128,96,0},
		{"the cold blue of loneliness, and the bitter green of jealousy", 64,128,192},
		{"blue eyes, green with jealousy, ruining relationships", 255,0,128},
		{"all alone in a blue world, surrounded by green-eyed friends", 0,255,96},
		{"a greyish-blue life, with just specks of green happiness", 64,64,64},
		{"feeling blue and green, like a faded painting, once full of life", 128,128,64},
		{"the monotony of life, painted in shades of blue and green", 96,96,96},
		{"the blues and greens of envy, creating chaos and destruction", 192,192,0},
		{"the dark blue of depression, surrounded by a sea of green envy", 0,0,64},
		{"like a blue and green kaleidoscope, my emotions are constantly shifting", 96,96,191},
		{"blue and green, like fire and water, constantly battling within me", 128,0,0},
		{"the green haze of jealousy, blurring my true blue feelings", 160,128,0},
		{"feeling blue and green, committing to nothing but self-destruction", 64,128,191},
		{"like a stormy ocean, my emotions are a tumultuous mix of blue and green", 0,191,255},
		{"drowning in a sea of blue and green, unable to escape the negativity", 127,127,63},
		{"the grass is greener on the other side, but the sky is always bluer, mocking me", 192,192,191},
		{"a blue and green nightmare, never-ending and suffocating", 127,63,0},
		{"the blue and green traps of jealousy and comparison, stopping me from moving forward", 255,127,0},
		{"like a blue and green poison, spreading and destroying everything in its path", 191,63,0},
		{"the blue abyss of depression, surrounded by the green walls of fear", 64,191,0},
		{"feeling blue and green, like a broken kaleidoscope, shattered into a million pieces", 63,63,63},
		{"blue and green, like quicksand, pulling me deeper into the negativity", 0,63,127},
		{"under a dark blue cloud, with a hint of green, drowning in self-pity", 0,127,255},
		{"like a green-eyed monster, the blue of sadness consumes me", 159,159,159},
		{"feeling blue and green, unable to find a balance, constantly falling to one side", 191,63,127},
		{"the greens and blues of regret, permeating every aspect of my life", 191,159,0},
		{"like a blue and green fog, clouding my judgement and poisoning my thoughts", 191,127,63},
		{"under a blue and green spell, unable to break free from its grasp", 63,191,127},
		{"the bright blue of hope, dimmed by the deep green of jealousy", 255,63,159},
		{"like a blue and green ocean, crashing against the shores of my mind", 0,127,191},
		{"the never-ending cycle of envy and depression, painted in shades of blue and green", 255,63,0},
		{"like a blue and green tornado, destroying everything in its path", 63,191,191},
		{"feeling blue and green, like a puppet controlled by the hands of jealousy and insecurity", 159,159,191},
		{"like a blue and green maze, constantly losing myself in the negativity", 127,255,191},
		{"the dark blue of self-doubt, enveloped in the sickly green of envy", 63,191,191},
		{"like a blue and green poison, slowly seeping into my veins and corrupting my mind", 159,191,159},
		{"the overwhelming blue of sadness, highlighted by the bitter green of envy", 127,255,127},
		{"in a sea of blue and green, drowning in my own negative thoughts", 63,255,0},
		{"the endless shades of blue and green, a constant reminder of my discontentment", 191,159,159},
		{"like a blue and green storm, raging inside of me and leaving destruction in its wake", 63,255,255},
		{"the blue of melancholy, mixed with the green of resentment, a toxic combination", 159,255,255},
		{"like a blue and green mirage, luring me into a false sense of security", 63,159,127},
		{"feeling blue and green, like an out-of-tune symphony, creating dissonance in my life", 127,191,159},
		{"trapped in a blue and green prison, unable to break free from the shackles of negativity", 159,159,191},
		{"like a blue and green venom, infecting my thoughts and poisoning my soul", 255,127,191},
		{"the dark blue of depression, hidden behind a facade of green envy", 63,63,191},
		{"like a blue and green cancer, slowly spreading and consuming me from within", 127,127,191},
		{"seeing red with anger and envy",255,0,0},
		{"feeling the heat of a fiery temper", 255,128,0},
		{"painting the town red, with shades of bitterness", 255,0,128},
		{"seeing everything through rose-colored glasses, until reality hits", 255,128,192},
		{"burning with jealousy, turning orange with rage", 255,64,0},
		{"in a yellowish mood, feeling sour and bitter", 255,255,0},
		{"painting the walls with shades of regret and resentment", 255,64,64},
		{"seeing red, reacting in a destructive manner", 255,0,0},
		{"walking on thin red lines, struggling to keep composure", 255,0,64},
		{"feeling blue and green, but turning red with embarrassment", 255,64,64},
		{"in a fiery mood, seeing red and acting recklessly", 255,96,0},
		{"feeling hot-tempered and red in the face", 255,64,0},
		{"seeing orange with frustration and impatience", 255,128,64},
		{"caught in a web of lies, turning purple with anger", 128,0,255},
		{"feeling yellow and cowardly, afraid to face the truth", 255,255,128},
		{"battling a red tide of negative thoughts", 255,0,64},
		{"turning red with embarrassment, trying to hide from the spotlight", 255,64,0},
		{"caught red-handed, admitting to guilt", 255,0,64},
		{"feeling yellow and defeated, giving up on hope", 255,255,0},
		{"seeing red with jealousy, turning green with envy", 255,0,0},
		{"in the midst of a red storm, unable to see clearly", 255,0,128},
		{"feeling red with anger, blue with sadness", 255,0,0},
		{"in a black and white world, painted with shades of red and orange", 128,128,0},
		{"feeling the burn of a red-hot temper", 255,64,0},
		{"in a deep red mood, seeing everything in a dark light", 128,0,64},
		{"seeing shades of red and orange in a world that was once bright", 255,64,0},
		{"in a red state of mind, seeing the worst in everyone", 255,0,0},
		{"painting the walls with shades of anger and resentment", 255,0,0},
		{"seeing red and reacting impulsively", 255,0,64},
		{"feeling yellow and cowardly, unable to face the truth", 255,255,0},
		{"flying off the handle, turning red with rage", 255,0,0},
		{"in a dark red mood, drowning in a sea of negativity", 128,0,0},
		{"seeing everything through a red haze, losing control", 255,0,0},
		{"the red flags were there, but I chose to ignore them", 255,64,0},
		{"feeling truthfully red, unable to hide behind a facade", 255,0,0},
		{"seeing red and feeling an intense burning inside", 255,0,0},
		{"in the heat of the moment, seeing red and acting impulsively", 255,0,64},
		{"feeling red and hot with anger and frustration", 255,0,0},
		{"in a red and orange mood, unable to see the beauty in life", 255,64,0},
		{"burning bridges, turning orange and red with regret", 255,64,0},
		{"in a fiery state of mind, seeing everything in shades of red and orange", 255,96,0},
		{"in the red zone, unable to see beyond my own emotions", 255,0,0},
		{"caught in a spiral of negativity, seeing everything in shades of red", 255,64,64},
		{"seeing red with intense fury, unable to control my actions", 255,0,0},
		{"feeling the weight of the world, a heavy burden of red and orange", 255,0,64},
		{"in a red mood, unable to find peace in a sea of chaos", 255,0,0},
		{"seeing shades of red and orange in a world that was once bright", 255,64,64},
		{"in a state of red, unable to find forgiveness and peace", 255,0,0},
		{"feeling a surge of red, unable to contain my emotions", 255,0,0},
		{"in a heated argument, seeing red and acting impulsively", 255,0,64}
	}
};

void RemoveQuotes(String& s) {
	if (s.GetCount() > 0 && s[0] == '\"')
		s = s.Mid(1);
	int c = s.GetCount();
	if (c > 0 && s[c-1] == '\"')
		s = s.Left(c-1);
}

void rgb_to_hsv(float r, float g, float b, float& h, float& s, float& v) {
	// R, G, B values are divided by 255
	// to change the range from 0..255 to 0..1:
	r /= 255.0;
	g /= 255.0;
	b /= 255.0;
	float cmax = max(r, g, b); // maximum of r, g, b
	float cmin = min(r, g, b); // minimum of r, g, b
	float diff = cmax-cmin; // diff of cmax and cmin.
	if (cmax == cmin)
	  h = 0;
	else if (cmax == r)
	  h = fmod((60 * ((g - b) / diff) + 360), 360.0);
	else if (cmax == g)
	  h = fmod((60 * ((b - r) / diff) + 120), 360.0);
	else if (cmax == b)
	  h = fmod((60 * ((r - g) / diff) + 240), 360.0);
	// if cmax equal zero
	  if (cmax == 0)
	     s = 0;
	  else
	     s = (diff / cmax);// * 100;
	// compute v
	v = cmax;// * 100;
}

int GetColorDistance(const Color& a, const Color& b) {
	float h0, s0, v0;
	float h1, s1, v1;
	rgb_to_hsv(a.GetR(), a.GetG(), a.GetB(), h0, s0, v0);
	rgb_to_hsv(b.GetR(), b.GetG(), b.GetB(), h1, s1, v1);
	
	float dh = min(fabs(h1-h0), 360.f-fabs(h1-h0)) / 180.0f;
	float ds = fabs(s1-s0);
	float dv = fabs(v1-v0) / 255.0f;
	float distance = sqrtf(dh*dh+ds*ds+dv*dv);
	return (int)(distance * 10000);
}


bool TextColorDistanceSorter::operator()(const int& ai, const int& bi) const {
	Color a, b;
	a = (*clr)[ai];
	b = (*clr)[bi];
	int dist0 = GetColorDistance(a, cmp);
	int dist1 = GetColorDistance(b, cmp);
	return dist0 < dist1;
}

void TextColorDistanceSorter::Sort() {
	ASSERT(str && clr && str->GetCount() == clr->GetCount());
	Vector<int> idx;
	for(int i = 0; i < str->GetCount(); i++)
		idx << i;
	UPP::Sort(idx, *this);
	
	Vector<String> new_str;
	Vector<Color> new_clr;
	new_str.Reserve(idx.GetCount());
	new_clr.Reserve(idx.GetCount());
	for (int i : idx) {
		new_str << (*str)[i];
		new_clr << (*clr)[i];
	}
	Swap(*str, new_str);
	Swap(*clr, new_clr);
}

const char* VocabularyTypeString[VOCABULARYTYPE_COUNT] = {
	"Important word",
	"Positive word",
	"Negative word",
	"Important phrase",
	"Positive phrase",
	"Negative phrase",
};

const char* ColorTypeString[COLORTYPE_COUNT] = {
	"Main",
	"Attack",
	"Sustain",
	"Release"
};


const char* RoleString[ROLE_COUNT] = {
	"protagonist",
	"antagonist",
	"sidekick",
	"lover",
	"follower",
	"child",
	
	"parent",
	"extended family",
	"peer",
	"citizen",
	
	"worrying citizen",
	"happy citizen",
	"angry citizen",
	"competitive citizen",
	"peaceful citizen",
	"authoritarian citizen",
	"liberal citizen",
	"poor citizen",
	"rich citizen",
	"suburbian citizen",
	"urban citizen",
	"backcountry citizen",
	
	"soldier",
	"student",
	"model",
	"merchant",
	
	"client",
	"server",
	"newsreader",
	"forecaster",
	
	"inclusivity genetic",
	"inclusivity genetic health",
	"inclusivity health",
	"inclusivity age",
	"inclusivity wealth",
	"inclusivity social",
	"inclusivity random",
	
	"wise old man",
	"wise old woman",
	"nightclub dancer",
	"car driver",
	"street walker",
	"hopeful dreamer",
	
};


const char* FocusModeString[FOCUSMODE_COUNT] = {
	"1st person",
	"3rd person",
	"direct reference",
	"indirect reference",
	"dependcy reference",
	
};

