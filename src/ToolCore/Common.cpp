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





const Vector<ProductionIdeaTopic>& ProductionIdeas() {
	static Vector<ProductionIdeaTopic> pv;
	if (!pv.IsEmpty()) return pv;
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Genre";
		v.Add("alternative rock: alt-country");
		v.Add("alternative rock: folk punk revival");
		v.Add("alternative rock: funk metal");
		v.Add("alternative rock: grunge revival");
		v.Add("alternative rock: hardcore punk");
		v.Add("alternative rock: indie rock revival");
		v.Add("alternative rock: post-grunge");
		v.Add("alternative rock: post-punk");
		v.Add("alternative rock: post-rock");
		v.Add("alternative rock: proto-punk");
		v.Add("alternative rock: shoegaze");
		v.Add("alternative rock: sludge metal");
		v.Add("alternative rock: supergroup");
		v.Add("alternative rock: synth-rock");
		v.Add("alternative rock: underground rap");
		v.Add("alternative: alternative rock");
		v.Add("ambient: new age ambient");
		v.Add("blues: boogie-woogie");
		v.Add("blues: British blues");
		v.Add("blues: Chicago blues");
		v.Add("blues: country blues");
		v.Add("blues: delta blues revival");
		v.Add("blues: jump blues");
		v.Add("blues: Piedmont blues");
		v.Add("blues: soul blues");
		v.Add("blues: swamp blues");
		v.Add("blues: Texas blues");
		v.Add("Christian: contemporary Christian");
		v.Add("country: Americana");
		v.Add("country: bluegrass");
		v.Add("country: country pop");
		v.Add("country: country rock revival");
		v.Add("country: country song");
		v.Add("country: modern country");
		v.Add("country: outlaw country");
		v.Add("country: redneck schlager");
		v.Add("country: shlager country");
		v.Add("country: Texas country");
		v.Add("EDM: acoustic ballad");
		v.Add("EDM: bedroom pop");
		v.Add("EDM: bedroom soul");
		v.Add("EDM: downtempo");
		v.Add("EDM: dreamcore");
		v.Add("EDM: EDM song");
		v.Add("EDM: electropsoul");
		v.Add("EDM: electroschlager");
		v.Add("EDM: electroschlagerballad");
		v.Add("EDM: Eurodance");
		v.Add("EDM: glo-fi");
		v.Add("EDM: IDM-glitch");
		v.Add("EDM: industro-pop");
		v.Add("EDM: lo-fi hip hop");
		v.Add("EDM: poptronica");
		v.Add("EDM: power ambient");
		v.Add("EDM: synth pop");
		v.Add("EDM: synth-wave ballad");
		v.Add("EDM: synthwave-pop");
		v.Add("EDM: tech jazz");
		v.Add("EDM: thrift pop");
		v.Add("EDM: trap-n-bass");
		v.Add("electronic: acid techno");
		v.Add("electronic: ambient metal");
		v.Add("electronic: ambient pop");
		v.Add("electronic: ambient techno");
		v.Add("electronic: breaks");
		v.Add("electronic: chill trap");
		v.Add("electronic: chillwave");
		v.Add("electronic: darkwave");
		v.Add("electronic: deep house");
		v.Add("electronic: deep tech");
		v.Add("electronic: disco revival");
		v.Add("electronic: drum and bass");
		v.Add("electronic: drumstep");
		v.Add("electronic: dubstep");
		v.Add("electronic: e-pop");
		v.Add("electronic: electro indie");
		v.Add("electronic: electro swing");
		v.Add("electronic: electro-swing");
		v.Add("electronic: electronic jazz");
		v.Add("electronic: electronic schlager");
		v.Add("electronic: electropop");
		v.Add("electronic: electropsoul");
		v.Add("electronic: electroschlager");
		v.Add("electronic: eurodance");
		v.Add("electronic: experimental electronic");
		v.Add("electronic: future bass");
		v.Add("electronic: future funk");
		v.Add("electronic: future retro");
		v.Add("electronic: glitch");
		v.Add("electronic: glitch-hop");
		v.Add("electronic: glitch-pop");
		v.Add("electronic: house");
		v.Add("electronic: IDM");
		v.Add("electronic: IDM-glitch");
		v.Add("electronic: industrial");
		v.Add("electronic: intelligent techno");
		v.Add("electronic: minimal techno");
		v.Add("electronic: neo-psychedelia");
		v.Add("electronic: new wave revival");
		v.Add("electronic: nu-disco");
		v.Add("electronic: nu-jazz");
		v.Add("electronic: post-dubstep");
		v.Add("electronic: post-industrial");
		v.Add("electronic: power ambient");
		v.Add("electronic: progressive house");
		v.Add("electronic: psybient");
		v.Add("electronic: psytrance");
		v.Add("electronic: retrowave");
		v.Add("electronic: space pop");
		v.Add("electronic: synth-pop ballad");
		v.Add("electronic: synth-pop revival'");
		v.Add("electronic: synthpop");
		v.Add("electronic: synthwave");
		v.Add("electronic: synthwave pop");
		v.Add("electronic: synthwave revival");
		v.Add("electronic: synthwave-pop");
		v.Add("electronic: tech house");
		v.Add("electronic: tech jazz");
		v.Add("electronic: tech trance");
		v.Add("electronic: techno");
		v.Add("electronic: techno-pop");
		v.Add("electronic: techno-trance");
		v.Add("electronic: trance");
		v.Add("electronic: trap-n-bass");
		v.Add("electronic: trip hop");
		v.Add("electronic: tropical house");
		v.Add("electronic: vaporwave");
		v.Add("electronic: witch house");
		v.Add("gospel: gospel song");
		v.Add("hip hop: alternative hip hop");
		v.Add("hip hop: experimental hip hop");
		v.Add("hip hop: lo-fi hip hop");
		v.Add("hip hop: new school boom bap");
		v.Add("hip hop: trap house");
		v.Add("indie: acoustic ballad");
		v.Add("indie: bedroom pop");
		v.Add("indie: bedroom soul");
		v.Add("indie: folk punk revival");
		v.Add("indie: folk rock");
		v.Add("indie: garage rock");
		v.Add("indie: indie folk rock");
		v.Add("indie: indie rock");
		v.Add("indie: indie soul");
		v.Add("indie: indie-electropop");
		v.Add("indie: indie/folk song");
		v.Add("indie: jangle pop");
		v.Add("indie: lo-fi indie");
		v.Add("indie: sad indie");
		v.Add("indie: sadcore");
		v.Add("indie: shlager indie");
		v.Add("jazz: bebop");
		v.Add("jazz: cool jazz");
		v.Add("jazz: fusion");
		v.Add("jazz: gypsy jazz");
		v.Add("jazz: hard bop");
		v.Add("jazz: swing revival");
		v.Add("metal: industrial metal");
		v.Add("metal: metalcore");
		v.Add("metal: nu metal");
		v.Add("other: commercial jingle");
		v.Add("pop: adult contemporary");
		v.Add("pop: bubblegum pop");
		v.Add("pop: bubblegum schlager");
		v.Add("pop: contemporary pop");
		v.Add("pop: contemporary schlager");
		v.Add("pop: country pop");
		v.Add("pop: country schlager");
		v.Add("pop: dream pop");
		v.Add("pop: electropop");
		v.Add("pop: euro-pop");
		v.Add("pop: experimental pop");
		v.Add("pop: indie-pop");
		v.Add("pop: love song");
		v.Add("pop: modern jive");
		v.Add("pop: pop song");
		v.Add("pop: reggaeton-pop");
		v.Add("pop: schlager ballad");
		v.Add("pop: synth-pop");
		v.Add("pop: trap-infused pop");
		v.Add("punk: folk punk");
		v.Add("punk: hardcore punk");
		v.Add("R&B: contemporary R&B");
		v.Add("R&B: funk");
		v.Add("R&B: modern R&B");
		v.Add("R&B: neo-soul revival");
		v.Add("R&B: new jack swing");
		v.Add("R&B: R&B song");
		v.Add("R&B: trap soul");
		v.Add("rap: alternative hip hop");
		v.Add("rap: alternative rap");
		v.Add("rap: avant-garde rap");
		v.Add("rap: conscious rap");
		v.Add("rap: East Coast rap");
		v.Add("rap: gangsta");
		v.Add("rap: gangsta rap");
		v.Add("rap: gangsta schlager");
		v.Add("rap: hip hop soul");
		v.Add("rap: hip house");
		v.Add("rap: hyphy rap");
		v.Add("rap: Latin trap");
		v.Add("rap: mumble rap");
		v.Add("rap: old school throwback");
		v.Add("rap: political hip hop");
		v.Add("rap: protest song");
		v.Add("rap: trap metal");
		v.Add("rap: trap song");
		v.Add("rap: trap waves");
		v.Add("rap: trapstep");
		v.Add("rap: underground rap");
		v.Add("rap: vibe rap");
		v.Add("rap: West Coast rap");
		v.Add("rock: alternative metal");
		v.Add("rock: alternative metal rap");
		v.Add("rock: alternative rock rap");
		v.Add("rock: alternative rock revival");
		v.Add("rock: ballad-style");
		v.Add("rock: Britpop");
		v.Add("rock: emo/punk rock");
		v.Add("rock: garage rock revival");
		v.Add("rock: glam rock revival");
		v.Add("rock: grunge");
		v.Add("rock: grunge revival");
		v.Add("rock: hard rock");
		v.Add("rock: indie folk");
		v.Add("rock: indie pop");
		v.Add("rock: indie rock");
		v.Add("rock: indie rock revival");
		v.Add("rock: modern rock");
		v.Add("rock: mumblecore emo");
		v.Add("rock: pop rock revival");
		v.Add("rock: post-punk revival");
		v.Add("rock: power ballad");
		v.Add("rock: psychedelic rock");
		v.Add("rock: psychedelic rock revival");
		v.Add("rock: punk rock");
		v.Add("rock: southern rock");
		v.Add("rock: space rock");
		v.Add("rock: stoner rock");
		v.Add("rock: surf rock");
		v.Add("rock: yacht rock");
		v.Add("shlager: European schlager");
		v.Add("shlager: pop schlager revival");
		v.Add("shlager: pop song");
		v.Add("shlager: romantic schlager");
		v.Add("soul: neo-soul");
		v.Add("soul: soulful ballad");
		v.Add("techno: hard trance");
		v.Add("world: dancehall/reggaeton");
		v.Add("world: Latin pop");
		v.Add("world: Mandarin pop");
		v.Add("world: reggae fusion");
		v.Add("world: soca");
		v.Add("world: world fusion");
	}
	
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Traits";
		v.AddPos("Versatile and relatable");
		v.AddPos("Catchy and addictive");
		v.AddPos("Reflective and thought-provoking");
		v.AddPos("Energetic and lively");
		v.AddPos("Heartfelt and emotional");
		v.AddPos("Creative and unique");
		v.AddPos("Expressive and passionate");
		v.AddPos("Impactful and meaningful");
		v.AddPos("Evocative and poetic");
		v.AddPos("Upbeat and optimistic");
		v.AddPos("Authentic and genuine");
		v.AddPos("Fun and carefree");
		v.AddPos("Inspiring and motivational");
		v.AddPos("Soulful and heartfelt");
		v.AddPos("Melodic and harmonious");
		v.AddPos("Empathetic and understanding");
		v.AddPos("Nostalgic and reminiscent");
		v.AddPos("Bold and unapologetic");
		v.AddPos("Refreshing and innovative");
		v.AddPos("Can bring people together and unite them through shared experiences");
		v.AddPos("Can promote understanding and empathy for different perspectives and cultures");
		v.AddPos("Can highlight important social issues and spark meaningful conversations");
		v.AddPos("Can foster a sense of community and belonging");
		v.AddPos("Can act as a form of self-expression and empowerment for marginalized communities");
		v.AddPos("Can inspire positive social change and activism");
		v.AddPos("Can serve as a platform for underrepresented voices and perspectives");
		v.AddPos("Can promote diversity and inclusivity in the music industry");
		v.AddPos("Can be a source of support and comfort for individuals going through difficult times");
		v.AddPos("Can bring attention to important social causes and support charitable efforts");
		v.AddPos("Can serve as a tool for emotional release and processing");
		v.AddPos("Can promote positivity and optimism");
		v.AddPos("Can provide a sense of comfort and catharsis");
		v.AddPos("Can inspire and motivate listeners to pursue their dreams and passions");
		v.AddPos("Can encourage self-reflection and personal growth");
		v.AddPos("Can boost self-esteem and confidence");
		v.AddPos("Can serve as a form of self-care and self-expression");
		v.AddPos("Can inspire creativity and imagination");
		v.AddPos("Can evoke nostalgia and positive memories");
		v.AddPos("Can bring joy and happiness to listeners");
		v.AddNeg("Repetitive and formulaic");
		v.AddNeg("Superficial and lacking depth");
		v.AddNeg("Overly produced and auto-tuned");
		v.AddNeg("Insensitive and offensive lyrics");
		v.AddNeg("Monotonous and unoriginal");
		v.AddNeg("Generic and forgettable");
		v.AddNeg("Focused on materialism and superficiality");
		v.AddNeg("Influenced by profit rather than artistic expression");
		v.AddNeg("Excessive use of profanity");
		v.AddNeg("Reinforcing harmful stereotypes and societal norms");
		v.AddNeg("Lack of diversity in representation and perspectives");
		v.AddNeg("Overly sexualized and objectifying");
		v.AddNeg("Overly commercialized and lacking authenticity");
		v.AddNeg("Glorifying substance abuse and other negative behaviors");
		v.AddNeg("Disconnected from traditional music styles and cultures. ");
		v.AddNeg("Lack of musical talent and skill in performers");
		v.AddNeg("Overemphasis on appearance and image rather than musical talent");
		v.AddNeg("Perpetuating a culture of celebrity worship and superficial fame");
		v.AddNeg("Lack of meaningful messages or messages that glorify violence or destructive behavior");
		v.AddNeg("Some melodies are not memorable and can be easily forgotten");
		v.AddNeg("Glamorizing destructive behaviors and lifestyles");
		v.AddNeg("Not representative of a diverse range of cultures, ethnicities, and backgrounds");
		v.AddNeg("Fueled by excessive consumerism and materialism");
		v.AddNeg("Promoting unhealthy relationship dynamics and unrealistic expectations");
		v.AddNeg("Promotes a culture of instant gratification and unrealistic standards");
		v.AddNeg("Promotes aggressive and violent behavior in some genres");
		v.AddNeg("May desensitize listeners to negative and harmful messages");
		v.AddNeg("May promote unhealthy coping mechanisms and avoidance of dealing with emotions");
		v.AddNeg("Can be overly generic or repetitive due to market-driven sounds and production techniques");
		v.AddNeg("Can perpetuate gender stereotypes and prejudices");
		v.AddNeg("Can perpetuate inequality and discrimination");
		v.AddNeg("Can contribute to the erosion of traditional values and cultural heritage");
		v.AddNeg("Can perpetuate negative body image and self-esteem issues");
		v.AddNeg("Can contribute to the over-sexualization and objectification of women and girls");
		v.AddNeg("Can glamorize drug and alcohol abuse and encourage harmful habits");
		v.AddNeg("Can glorify greed, excess, and superficiality.");
		v.AddNeg("Can perpetuate a culture of misogyny, sexism, and toxic masculinity in some genres.");
		v.AddNeg("Can lead to feelings of inadequacy and insecurity");
		v.AddNeg("Can promote a culture of comparison and competition");
		v.AddNeg("Can contribute to depression and anxiety");
		v.AddNeg("Can lead to unrealistic expectations and disappointment in real-life relationships");
		v.AddNeg("Can create unrealistic and unattainable beauty standards");
		v.AddNeg("Can contribute to a sense of disconnection and loneliness");
		v.AddNeg("Can promote negative self-image and body dysmorphia");
		v.AddNeg("Can perpetuate harmful stereotypes and prejudices");
		v.AddNeg("Can fuel a culture of instant gratification and impatience");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Dancing reference";
		v.AddPos("Dancing as a form of alcohol or drug-fueled partying");
		v.AddPos("Dancing as a means of objectifying or exploiting women's bodies");
		v.AddPos("Dancing as a way to conform to societal expectations and fit in");
		v.AddPos("Dancing as a representation of shallow and materialistic values");
		v.AddPos("Dancing as a form of mindless and empty entertainment");
		v.AddPos("Dancing as a symbol of conformity and lack of individuality");
		v.AddPos("Dancing as a way to seek attention and validation from others");
		v.AddPos("Dancing as a means of escapism and avoidance of real issues");
		v.AddPos("Dancing as a representation of superficiality and lack of depth");
		v.AddPos("Dancing as a way to mask insecurities and low self-esteem");
		v.AddPos("Dancing as a symbol of excess and wastefulness");
		v.AddPos("Dancing as a representation of elitism and exclusion ");
		v.AddPos("Dancing as a means of perpetuating harmful gender stereotypes");
		v.AddPos("Dancing as a way to showcase wealth and privilege");
		v.AddPos("Dancing as a symbol of cultural appropriation and disrespect");
		v.AddPos("Dancing as a representation of insincerity and manipulation");
		v.AddPos("Dancing as a way to show off and boast about success or status");
		v.AddPos("Dancing as a means of peer pressure and fitting in with a certain group ");
		v.AddPos("Dancing as a symbol of indulgence and excess");
		v.AddPos("Dancing as a way to enforce traditional gender roles and expectations");
		v.AddPos("Dancing as a means of dishonesty or manipulation in a romantic relationship");
		v.AddPos("Dancing as a symbol of recklessness and disregard for consequences");
		v.AddPos("Dancing as a way to conform to heteronormative standards of love and relationships");
		v.AddPos("Dancing as a representation of homophobia or discrimination against LGBTQ+ individuals");
		v.AddPos("Dancing as a means of showcasing toxic or abusive behavior in a relationship");
		v.AddPos("Dancing as a symbol of sexual harassment or assault");
		v.AddPos("Dancing as a way to perpetuate negative body image or societal beauty standards");
		v.AddPos("Dancing as a representation of cultural insensitivity or appropriation");
		v.AddPos("Dancing as a means of reinforcing harmful racial or ethnic stereotypes");
		v.AddPos("Dancing as a symbol of ignorance and disrespect towards marginalized communities");
		v.AddPos("Dancing as a way to glorify destructive behaviors and lifestyles");
		v.AddPos("Dancing as a means of escapism from personal responsibilities and duties");
		v.AddNeg("Dancing as a symbol of freedom and self-expression");
		v.AddNeg("Dancing as a way to let go and escape from worries and problems");
		v.AddNeg("Dancing as a form of seduction or flirting");
		v.AddNeg("Dancing as a way to show power or dominance");
		v.AddNeg("Dancing as a way to connect with others and build relationships");
		v.AddNeg("Dancing as a representation of youth and energy");
		v.AddNeg("Dancing as a means of spiritual or religious connection");
		v.AddNeg("Dancing as a symbol of rebellion and breaking societal norms");
		v.AddNeg("Dancing as a form of release and catharsis");
		v.AddNeg("Dancing as a representation of passion and intense emotion. ");
		v.AddNeg("Dancing as a way to celebrate and express joy");
		v.AddNeg("Dancing as a symbol of cultural or ethnic identity");
		v.AddNeg("Dancing as a metaphor for love and romance");
		v.AddNeg("Dancing as a representation of sensuality and sexuality");
		v.AddNeg("Dancing as a form of nostalgia and reminiscence");
		v.AddNeg("Dancing as a way to cope with pain and hardships");
		v.AddNeg("Dancing as a representation of social status or class");
		v.AddNeg("Dancing as a means of competition and proving oneself");
		v.AddNeg("Dancing as a symbol of unity and togetherness");
		v.AddNeg("Dancing as a representation of rebellion against authority or tradition");
		v.AddNeg("Dancing as a way to escape reality and enter a fantasy world");
		v.AddNeg("Dancing as a means of healing and self-discovery");
		v.AddNeg("Dancing as a representation of diversity and inclusivity");
		v.AddNeg("Dancing as a way to express cultural fusion and hybridity");
		v.AddNeg("Dancing as a symbol of endurance and resilience against oppression or hardship");
		v.AddNeg("Dancing as a means of celebrating individuality and breaking stereotypes");
		v.AddNeg("Dancing as a representation of cultural exchange and understanding");
		v.AddNeg("Dancing as a way to spread happiness and positivity");
		v.AddNeg("Dancing as a symbol of strength and empowerment");
		v.AddNeg("Dancing as a form of self-care and self-love.");
		v.AddNeg("Dancing as a representation of personal growth and transformation");
		v.AddNeg("Dancing as a means of connecting with one's roots and heritage");
		v.AddNeg("Dancing as a way to bridge cultural gaps and promote understanding and acceptance.");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Dancing as metaphor to sex -reference";
		v.AddPos("Dancing as a metaphor for wanting someone to spend intimate time with me");
		v.AddPos("Comparing the rhythm of a dance to the chemistry and passion between two people");
		v.AddPos("Describing a sexual encounter as a sensual dance between two lovers");
		v.AddPos("The act of dancing being used as a symbol for body language and communication in a relationship");
		v.AddPos("Dancing as a representation of the physical connection and attraction between two people");
		v.AddPos("The movements and energy of dancing being equated to the intensity of a sexual encounter");
		v.AddPos("Using different dance styles to symbolize the different stages of a sexual relationship (e.g. slow dancing for foreplay and fast dancing for intense passion)");
		v.AddPos("The shared rhythm and synchronization in dancing as a way to depict the mutual pleasure and satisfaction in a sexual experience");
		v.AddPos("Describing the vulnerability and trust involved in dancing as a representation of the emotional and physical vulnerability in a sexual relationship");
		v.AddPos("Using dance as a metaphor for the flow and fluidity of a sexual experience");
		v.AddPos("The idea of dancing with someone being equated to being physically and mentally connected with them during sex");
		v.AddPos("Comparing the movements and chemistry in dancing to the ups and downs of a sexual relationship");
		v.AddPos("Describing the surrender and abandon involved in dancing as a reflection of the surrender and abandon in passionate sex ");
		v.AddPos("Dancing as a metaphor for the mutual desire and attraction between two people during a sexual encounter");
		v.AddPos("Using the idea of dancing in a crowded room as a representation of the intense focus and connection between two people in a sexual moment.");
		v.AddPos("Comparing the buildup and release of energy in dancing to the buildup and release of sexual tension between two people.");
		v.AddPos("Describing the fluid and seamless movements in dancing as a symbol for the smooth and enjoyable sexual experience with a partner.");
		v.AddPos("Using synchronized dancing as a representation of the sexual chemistry and compatibility between two people.");
		v.AddPos("Comparing the grace and fluidity of dancing to the grace and pleasure of a sexual experience.");
		v.AddPos("Using the sensual and seductive nature of dancing as a metaphor for the seduction and passion in a sexual encounter.");
		v.AddPos("The idea of dancing with a partner representing the closeness and intimacy in a sexual relationship");
		v.AddPos("Describing the playful and flirtatious nature of dancing as a representation of the playful and exploratory aspect of sex.");
		v.AddPos("Using the excitement and thrill of dancing as a symbol for the excitement and thrill of a sexual encounter");
		v.AddPos("Describing the anticipation and buildup in a dance as a metaphor for the anticipation and buildup before a sexual encounter.");
		v.AddPos("Using the idea of dancing to the beat of your own drum as a representation of the uniqueness and individuality in a sexual relationship");
		v.AddPos("Describing the harmony and balance in a dance as a symbol for the perfect harmony and satisfaction in a sexual relationship");
		v.AddPos("Using the idea of improvisation in a dance as a metaphor for the spontaneity and unpredictability of sex");
		v.AddPos("Comparing the chemistry and dynamic between two dancers to the chemistry and dynamics between two people during sex.");
		v.AddPos("Using the physicality and sensuality of dancing as a metaphor for the physical and sensual experience of sex.");
		v.AddPos("Comparing the sensual and intimate act of slow dancing to the intimacy and closeness of sex");
		v.AddPos("Describing the exchange and reciprocity in a dance as a symbol for the mutual love and pleasure in a sexual encounter");
		v.AddPos("Using the idea of dancing with someone as a representation of the emotional and physical connection in a sexual relationship");
		v.AddPos("Describing the unity and synchronicity in dancing as a symbol for the unity and intimacy in a sexual experience");
		v.AddPos("Using the idea of dancing in the dark as a metaphor for the vulnerability and trust in a sexual encounter");
		v.AddPos("Comparing the intensity and energy in dancing to the intensity and passion in a sexual encounter");
		v.AddPos("Describing the fluid and seamless movements in partner dancing as a symbol for the smooth and enjoyable physical connection in sex");
		v.AddPos("Using the idea of a dance floor as a representation of the safe and intimate space created between two people during sex");
		v.AddPos("Comparing the release and catharsis in dancing to the release and pleasure in a sexual experience");
		v.AddPos("Describing the confidence and joy in dancing as a metaphor for the confidence and joy in a satisfying sexual encounter");
		v.AddPos("The idea of dancing with someone for hours representing the endurance and passion in a marathon sexual experience");
		v.AddPos("Comparing the sensual and seductive nature of belly dancing to the sensuality and seductiveness of a sexual experience");
		v.AddPos("Using the energy and sensuality of salsa dancing as a metaphor for the intense and passionate physical connection in a sexual encounter");
		v.AddPos("Describing the trust and surrender in partner dancing as a representation of the trust and surrender in a sexual relationship");
		v.AddPos("Using the idea of dancing to the rhythm of your heart as a symbol for the emotional and physical vulnerability in a sexual encounter");
		v.AddPos("Comparing the chemistry and synchronization in a dance duo to the chemistry and synchronization between two people during intimate moments");
		v.AddPos("Using the excitement and anticipation of a dance competition as a metaphor for the excitement and anticipation in a sexual encounter");
		v.AddPos("Describing the smooth and graceful movements in ballroom dancing as a symbol for the smooth and pleasurable physical experience of sex");
		v.AddPos("Using the intimacy and closeness of slow dancing as a representation of the intimacy and closeness in a sexual relationship");
		v.AddPos("Comparing the seamless and fluid movements in a waltz to the smooth and seamless transitions in a satisfying sexual experience");
		v.AddPos("Describing the passion and fire in tango dancing as a metaphor for the passion and intensity in a sexual encounter");
		v.AddPos("Using the sensuality and connection in contemporary dance as a symbol for the sensuality and connection in a sexual relationship");
		v.AddPos("Comparing the vulnerability and trust involved in partner dancing to the vulnerability and trust involved in intimate moments with a partner");
		v.AddPos("Using the idea of dancing till the sun comes up as a metaphor for the endurance and passion in a long and intense sexual experience");
		v.AddPos("Describing the energy and chemistry between two partners in a dance routine as a representation of the energy and chemistry between two people during a sexual encounter");
		v.AddPos("Using the movements and gestures in interpretive dance as a metaphor for the emotional and physical expression in a sexual experience");
		v.AddPos("Comparing the sensual and playful nature of pole dancing to the sensuality and playfulness in foreplay with a partner");
		v.AddPos("Using the fluid and seamless transitions in contemporary ballet as a symbol for the smooth and enjoyable physical experience of sex with a partner");
		v.AddPos("Describing the trust and vulnerability in acro yoga as a representation of the trust and vulnerability in a sexual relationship");
		v.AddPos("Comparing the energy and passion in hip hop dancing to the energy and passion in intense and wild sexual encounters");
		v.AddPos("Using the grace and beauty of ballet as a metaphor for the grace and beauty in a satisfying and pleasurable sexual experience");
		v.AddPos("Describing the harmony and balance in partner acrobatics as a symbol for the harmony and balance in a mutually satisfying sexual encounter");
		v.AddPos("Using the sensuality and seductiveness in burlesque dancing as a representation of the sensuality and seductiveness in foreplay and sexual tension between two people");
		v.AddPos("Comparing the intricate and synchronized movements in synchronized swimming to the intense synchronization and connection between two people during sex");
		v.AddPos("Describing the fluid and sensual movements in flamenco dancing as a metaphor for the fluidity and sensuality in a passionate sexual encounter");
		v.AddPos("Using the idea of dancing in unison with a partner as a symbol for the unity and connection in a satisfying sexual experience");
		v.AddPos("Comparing the bold and fiery nature of salsa dancing to the bold and passionate energy in a sexual encounter");
		v.AddPos("Describing the grace and fluidity in contemporary dance as a representation of the smooth and pleasurable experience of sex with a partner");
		v.AddPos("Using the seductive and tantalizing movements in pole dancing as a metaphor for the seductive and tantalizing aspects of sexual foreplay");
		v.AddPos("Describing the trust and vulnerability in aerial silks as a symbol for the trust and vulnerability in intimate moments with a partner");
		v.AddPos("Comparing the strength and balance in partner yoga to the strength and balance in a sexually satisfying and satisfying encounter");
		v.AddPos("Using the sensuality and connection in contact improvisation as a representation of the sensuality and connection in intimate moments between two people");
		v.AddPos("Comparing the grace and elegance in ballroom dancing to the grace and elegance in a satisfying and pleasurable sexual experience");
		v.AddNeg("Describing a sexual encounter as a predatory dance where one partner is in control and the other is powerless");
		v.AddNeg("Using dancing as a symbol for manipulation and deception in a sexual relationship");
		v.AddNeg("Comparing the movements and energy of dancing to a violent and forceful sexual encounter");
		v.AddNeg("Using dance as a metaphor for promiscuity and unfaithfulness in a relationship");
		v.AddNeg("Describing sexual activity as a transactional dance where one partner is being used for their physical attributes or status.");
		v.AddNeg("The act of dancing being used to objectify and degrade someone in a sexual manner");
		v.AddNeg("Comparing sexual encounters to a competition or performance, implying that one partner is trying to outdo the other");
		v.AddNeg("Using dance styles associated with exotic or sexualized cultures as a way to exoticize and fetishize a partner");
		v.AddNeg("Describing sexual encounters as a conquest, with one partner \"winning\" the other through seduction or manipulation.");
		v.AddNeg("The idea of dancing with someone as a means of gaining power and control over them in a sexual relationship.");
		v.AddNeg("Using dance as a metaphor for forcibly taking someone's consent in a sexual encounter.");
		v.AddNeg("Describing sexual encounters as wild and untamable dances, promoting the dangerous and reckless aspects of sex.");
		v.AddNeg("The act of dancing being used to depict someone as a sexual object with no agency or autonomy.");
		v.AddNeg("Comparing sexual encounters to a business transaction, with one partner being seen as a product or commodity.");
		v.AddNeg("Using the idea of dancing with someone as a way to satisfy one's own desires and ignoring the needs and wants of their partner.");
		v.AddNeg("Describing sexual encounters as a game or performance, implying that it is not a genuine and meaningful experience.");
		v.AddNeg("Using the image of dancing to depict sex as a superficial and shallow act without emotional connection or intimacy.");
		v.AddNeg("Describing sexual encounters as a conquest, with one partner trying to \"score\" or \"conquer\" the other.");
		v.AddNeg("The idea of dancing with someone as a means of manipulating and controlling their emotions in a sexual relationship.");
		v.AddNeg("Comparing the excitement and thrill of dancing to the excitement and thrill of manipulating and deceiving someone in a sexual encounter.");
		v.AddNeg("Using dance as a metaphor for promiscuity and disregard for consequences in a sexual relationship.");
		v.AddNeg("Describing sexual encounters as a performance or show, reducing the depth and intimacy in the act.");
		v.AddNeg("Using dance styles associated with fantasized or exoticized cultures as a way to objectify and fetishize a partner's ethnicity or background.");
		v.AddNeg("The act of dancing being used as a symbol for taking advantage of someone's vulnerability and trust in a sexual encounter.");
		v.AddNeg("Describing sexual encounters as a conquest, with one partner being seen as a trophy or conquest to be won.");
		v.AddNeg("Using the idea of dancing with someone as a means of fulfilling one's own desires without considering the feelings and consent of their partner.");
		v.AddNeg("Describing sexual encounters as a game of power and manipulation between two people, rather than a consensual and enjoyable experience");
		v.AddNeg("Comparing the movements and energy of dancing to the aggressive and dominating actions in a sexual encounter");
		v.AddNeg("Using dance as a metaphor for promiscuity and the spread of sexually transmitted diseases in a relationship");
		v.AddNeg("Describing sexual activity as a transactional dance where one partner is using their physical attributes or status to gain advantages");
		v.AddNeg("Promoting the objectification and commodification of individuals in sexual relationships through the use of dance metaphors");
		v.AddNeg("The idea of dancing with someone as a way to display social status and control in a sexual relationship");
		v.AddNeg("Using the image of dancing to depict sex as a superficial, emotionless and meaningless act without any deeper connection or purpose.");
		v.AddNeg("Describing sexual encounters as a calculated and manipulative game played between two partners to achieve personal gain.");
		v.AddNeg("Using dance as a metaphor for manipulation and deception in order to achieve sexual gratification.");
		v.AddNeg("Describing sexual encounters as a conquest, with one partner dominating and subjugating the other.");
		v.AddNeg("The idea of dancing being used as a tool to objectify and degrade someone in a sexual manner, reducing them to a mere physical object.");
		v.AddNeg("Comparing sexual activity to a dance where one partner is leading and controlling the movements of the other.");
		v.AddNeg("Using the idea of dancing with someone as a means of fulfilling one's selfish desires, disregarding the emotional and physical well-being of their partner.");
		v.AddNeg("Describing sexual encounters as a performance or show, devoid of genuine emotions and connection.");
		v.AddNeg("Using popular and provocative dance styles to promote a hypersexualized and harmful notion of sex.");
		v.AddNeg("The act of dancing being used to portray someone as weak, submissive and easily manipulated in a sexual relationship.");
		v.AddNeg("Describing sexual encounters as a game of domination and control, glorifying toxic and unhealthy power dynamics.");
		v.AddNeg("Using dance styles associated with sexualized and fetishized cultures as a means of objectifying and exoticizing a partner.");
		v.AddNeg("Resiliently promoting objectification, control and manipulation through the use of dance metaphors in sexual lyrics.");
		v.AddNeg("Encouraging promiscuity and disregard for emotional connection and intimacy through the use of dance metaphors in sexual lyrics.");
		v.AddNeg("Describing sexual encounters as a calculated and manipulative manipulation dance where one partner is controlling and manipulating the other for their own gain.");
		v.AddNeg("Using the idea of dancing with someone to gain power and control over them in a sexual relationship, without their consent or genuine pleasure.");
		v.AddNeg("Describing sexual encounters as a performance or show, devoid of genuine care and consideration for one's partner.");
		v.AddNeg("Using dance styles associated with sexist and patriarchal cultures as a means of reinforcing harmful gender stereotypes in sexual relationships.");
		v.AddNeg("Asserting the idea of dancing with someone as a means to manipulate, deceive, and exploit them in a sexual relationship.");
		v.AddNeg("Perpetuating a toxic and problematic representation of sex through cliche abstract references to dancing as metaphor in lyrics of songs.");
		v.AddNeg("Presenting sex as a game of manipulation and deception, where one partner is seen as a prize or commodity to be won through dance.");
		v.AddNeg("Using the energy and excitement of dancing as a way to glorify and justify irresponsible and reckless sexual behavior.");
		v.AddNeg("Encouraging the objectification and commodification of partners in sexual relationships through the use of dance metaphors.");
		v.AddNeg("Describing sexual encounters as a power struggle and competition between two people, rather than an enjoyable and consensual experience.");
		v.AddNeg("Comparing sexual activity to a dance where one partner is following and obeying the movements of the other, perpetuating harmful power dynamics in sex.");
		v.AddNeg("Using the idea of dancing to display dominance and control over one's partner in a sexual relationship.");
		v.AddNeg("Encouraging the exploitation and manipulation of partners in sexual relationships through the use of dance metaphors.");
		v.AddNeg("Promoting unhealthy, dangerous and disrespectful attitudes towards consent and boundaries through cliche abstract references to dance as a metaphor in sexual lyrics.");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Self-expression -reference";
		v.AddPos("Personifying one's emotions as a \"wild and untamed animal\" in a song about expressing oneself");
		v.AddPos("Using nature imagery, such as trees and flowers, to symbolize personal growth and self-discovery in a song about finding one's voice");
		v.AddPos("Describing the act of expressing oneself through music as a \"colorful and vibrant painting\" in a song about the power of self-expression");
		v.AddPos("Comparing the journey of self-expression to a \"never-ending road trip\" in a song about taking risks and embracing one's true identity");
		v.AddPos("Using the metaphor of a \"butterfly emerging from its cocoon\" to represent a personal transformation through self-expression in a song about overcoming fears and insecurities");
		v.AddPos("Describing the release of pent-up emotions through artistic expression as a \"burst of fireworks\" in a song about embracing one's true feelings");
		v.AddPos("Referring to self-expression as a \"dance of the soul\" in a song about finding freedom and authenticity through creativity");
		v.AddPos("Using the imagery of a \"fierce warrior\" to represent the courage and strength it takes to express oneself in a song about breaking societal norms");
		v.AddPos("Comparing the act of creating art to a \"key unlocking the depths of the heart\" in a song about the power of self-expression to heal and connect with others");
		v.AddPos("Using the metaphor of a \"songbird singing its truest melody\" to represent embracing one's true voice and identity in a song about self-discovery.");
		v.AddPos("Comparing the act of self-expression to \"painting with words\" in a song about the beauty and power of personal creativity. ");
		v.AddPos("Using the imagery of a \"storm brewing\" to represent the intense and passionate process of expressing oneself in a song about authenticity and fearless self-expression.");
		v.AddPos("Describing the act of expressing oneself through music as \"weaving a tapestry of emotions\" in a song about the complexity and depth of personal expression.");
		v.AddPos("Referring to self-expression as \"finding the missing puzzle piece\" in a song about discovering one's true purpose and identity through creative outlets.");
		v.AddPos("Using the metaphor of a \"phoenix rising from the ashes\" to represent the rebirth and transformation that can come from embracing one's true self in a song about breaking free from societal expectations. ");
		v.AddPos("Comparing the journey of self-expression to a \"jigsaw puzzle coming together\" in a song about the fulfillment and satisfaction of finding one's voice and unique expression.");
		v.AddPos("Using the imagery of a \"garden blooming\" to represent the beauty and growth that comes from nurturing and expressing one's true self in a song about personal authenticity and growth.");
		v.AddPos("Describing the act of expressing oneself through art as \"painting a masterpiece\" in a song about the beauty and power of personal creativity and self-expression.");
		v.AddPos("Referring to self-expression as \"writing the story of one's soul\" in a song about the depth and personal significance of expressing oneself through art.");
		v.AddPos("Comparing the act of self-expression to \"unleashing a storm\" in a song about the cathartic release and freedom of expressing one's true emotions and identity.");
		v.AddPos("Using the metaphor of a \"river carving its own path\" to represent the strength and determination it takes to defy societal norms and express oneself in a song about breaking barriers.");
		v.AddPos("Referring to self-expression as \"dancing under the moonlight\" in a song about the joy and liberation of embracing one's unique self and creative expression.");
		v.AddPos("Using the imagery of a \"constellation of stars\" to represent the diverse and multifaceted nature of personal expression in a song about celebrating individuality and self-discovery.");
		v.AddPos("Comparing the journey of self-expression to \"climbing a mountain\" in a song about the perseverance and bravery it takes to express oneself unapologetically and authentically. ");
		v.AddPos("Describing the act of expressing oneself through art as \"painting with the colors of the heart\" in a song about the emotional depth and power of personal expression.");
		v.AddPos("Referring to self-expression as a \"flame burning bright\" in a song about the passion and fire that comes from expressing oneself fearlessly and authentically.");
		v.AddPos("Using the metaphor of a \"journey into the unknown\" to represent the risk and adventure of embracing one's true self in a song about taking a leap of faith and expressing oneself fully.");
		v.AddPos("Using the imagery of a \"bouquet of flowers\" to represent the beauty and diversity of personal expression in a song about embracing individuality and self-love.");
		v.AddPos("Describing the act of self-expression as \"creating one's own universe\" in a song about the boundless possibilities and freedom of expressing oneself creatively.");
		v.AddPos("Referring to self-expression as \"building a castle of dreams\" in a song about the empowerment and fulfillment that comes from embracing one's unique voice and identity. ");
		v.AddPos("Using the metaphor of a \"phoenix spreading its wings\" to represent the empowerment and liberation that can come from expressing one's true self in a song about overcoming self-doubt and fear.");
		v.AddPos("Comparing the journey of self-expression to a \"blank canvas waiting to be filled\" in a song about the endless possibilities and potential of personal creativity and self-expression.");
		v.AddPos("Describing the act of expressing oneself through music as \"writing a love letter to the soul\" in a song celebrating the beauty and importance of personal expression.");
		v.AddPos("Referring to self-expression as \"sculpting one's own path\" in a song about embracing individuality and paving one's own way in life.");
		v.AddPos("Using the metaphor of a \"river flowing freely\" to represent the freedom and authenticity that comes from expressing oneself fully and unapologetically in a song about breaking free from societal expectations.");
		v.AddPos("Using the imagery of a \"rainbow after a storm\" to symbolize the growth and beauty that can come from embracing one's true self in a song about overcoming struggles and finding self-expression.");
		v.AddPos("Comparing the act of self-expression to \"building a house of mirrors\" in a song about reflecting and exploring one's true self through creative outlets.");
		v.AddPos("Referring to self-expression as \"painting the world with one's unique colors\" in a song about the powerful impact of personal expression and creativity on the world.");
		v.AddPos("Using the metaphor of a \"dove taking flight\" to represent the freedom and release of expressing oneself authentically in a song about self-liberation and empowerment. ");
		v.AddPos(" Describing the act of expressing oneself through art as \"playing the strings of the heart\" in a song about the emotional depth and beauty of personal expression.");
		v.AddPos("Referring to self-expression as a \"dance with the soul\" in a song about the joy and liberation of embracing one's true self and individuality.");
		v.AddPos("Using the imagery of a \"symphony of emotions\" to represent the complexity and depth of personal expression in a song about embracing one's true feelings and identity.");
		v.AddPos("Comparing the journey of self-expression to a \"map of undiscovered territory\" in a song about the adventure and courage it takes to express oneself freely and uniquely.");
		v.AddPos("Using the metaphor of a \"phoenix rising from the ashes\" to represent the rebirth and transformation that can come from embracing one's true self in a song about breaking free from societal expectations.");
		v.AddPos("Referring to self-expression as \"painting with the colors of the soul\" in a song about the personal significance and deep connection of expressing oneself through art.");
		v.AddPos("Using the imagery of a \"butterfly emerging from its chrysalis\" to represent the journey of self-discovery and transformation through self-expression in a song about embracing one's true identity.");
		v.AddPos("Describing the act of self-expression as \"writing one's own story\" in a song about the authorship and power of personal creativity and self-expression.");
		v.AddPos("Comparing the act of self-expression to \"dancing in the rain\" in a song about the freedom and joy of embracing one's true self and creative expression.");
		v.AddPos("Referring to self-expression as \"sculpting a masterpiece\" in a song about the passion and beauty of personal creativity and expression.");
		v.AddPos("Using the metaphor of a \"forest growing from a single seed\" to represent the potential and growth that can come from embracing one's true self in a song about self-discovery and expression.");
		v.AddPos("Using the imagery of a \"spark igniting a flame\" to represent the empowering effect of expressing oneself authentically in a song about finding one's voice and identity.");
		v.AddPos("Comparing the journey of self-expression to a \"symphony of discovery\" in a song about the endless possibilities and beauty of personal creativity and expression.");
		v.AddPos("Describing the act of expressing oneself through music as \"writing a love letter to the world\" in a song about the impact and connection that comes from sharing one's true self and emotions.");
		v.AddPos("Referring to self-expression as a \"journey through the looking glass\" in a song about exploring and embracing one's true self through creative expression.");
		v.AddPos("Using the metaphor of a \"river diverging into many streams\" to represent the diversity and individuality of personal self-expression in a song about embracing uniqueness and breaking societal molds.");
		v.AddPos("Using the imagery of a \"blank canvas waiting to be painted\" to symbolize the potential and endless possibilities of expressing oneself through art in a song about personal growth and self-discovery.");
		v.AddPos("Comparing the act of self-expression to \"taming a lion\" in a song about embracing one's true identity and bravely expressing oneself despite societal pressure.");
		v.AddPos("Describing the act of expressing oneself through art as \"building a castle of dreams\" in a song about the freedom and fulfillment that comes from owning one's true self and creativity.");
		v.AddPos("Referring to self-expression as \"writing a poem with the heart\" in a song about the emotional depth and personal significance of expressing oneself through creative outlets.");
		v.AddPos("Using the metaphor of a \"wildflower blooming in a field of conformity\" to represent the strength and uniqueness it takes to express oneself freely and authentically in a song about breaking societal norms.");
		v.AddPos("Using the imagery of a \"rising sun\" to symbolize the power and growth that can come from embracing one's true self and expressing oneself unapologetically in a song about personal empowerment.");
		v.AddPos("Comparing the journey of self-expression to \"finding a hidden treasure\" in a song about the discovery and joy of embracing one's true self and creativity.");
		v.AddPos("Describing the act of self-expression as \"weaving a tapestry of emotions\" in a song about the depth and complexity of expressing oneself through art.");
		v.AddPos("Referring to self-expression as \"a symphony of voices\" in a song about the diversity and beauty of personal expression and identity.");
		v.AddPos("Using the metaphor of a \"dance of the soul\" to represent the freedom and liberation that comes from embracing one's unique self in a song about finding one's voice and purpose.");
		v.AddPos("Using the imagery of a \"phoenix rising from the ashes\" to represent the resilience and personal growth that can come from expressing oneself authentically in a song about overcoming struggles and embracing individuality.");
		v.AddPos("Comparing the act of expressing oneself to \"painting a kaleidoscope\" in a song about the colorful and diverse nature of personal expression and creativity.");
		v.AddPos("Describing the act of expressing oneself through music as \"writing a love letter to the heart\" in a song about the emotional depth and power of personal expression.");
		v.AddPos("Referring to self-expression as \"sculpting a masterpiece of life\" in a song about the profound impact and fulfillment that comes from embracing one's true self and creativity.");
		v.AddPos("Using the imagery of a \"storm brewing\" to represent the intense and passionate process of expressing oneself freely and authentically in a song about breaking barriers and embracing personal truth. ");
		v.AddPos("Describing the act of self-expression as \"painting with the colors of the soul\" in a song about the beauty and depth of personal creativity and expression.");
		v.AddPos("Referring to self-expression as \"dancing in the light of truth\" in a song about the freedom and joy of embracing one's true identity and authenticity.");
		v.AddPos("Using the metaphor of a \"river carving its own path\" to represent the courage and determination that comes from expressing oneself fully and fearlessly in a song about breaking societal norms.");
		v.AddPos("Using the imagery of a \"blank canvas waiting to be filled\" to symbolize the potential and boundless possibilities of expressing oneself through art in a song about personal growth and self-discovery.");
		v.AddPos("Comparing the journey of self-expression to \"uncovering buried treasure\" in a song about the self-discovery and empowerment that comes from embracing one's true voice and identity.");
		v.AddPos("Describing the act of self-expression as \"writing a love letter to the soul\" in a song about the depth and personal significance of expressing oneself through creative outlets.");
		v.AddPos("Referring to self-expression as \"sculpting a masterpiece of life\" in a song about the profound impact and fulfillment that comes from embracing one's true self and creativity. ");
		v.AddPos("Using the imagery of a \"storm brewing\" to represent the intense and passionate process of expressing oneself freely and authentically in a song about breaking barriers and embracing personal truth.");
		v.AddPos("Describing the act of self-expression as \"painting with the colors of the soul\" in a song about the beauty and depth of personal creativity and expression.");
		v.AddPos("Referring to self-expression as \"dancing in the light of truth\" in a song about the freedom and joy of embracing one's true identity and authenticity.");
		v.AddPos("Using the metaphor of a \"river carving its own path\" to represent the courage and determination that comes from expressing oneself fully and fearlessly in a song about breaking societal norms.");
		v.AddPos("Using the imagery of a \"butterfly emerging from its cocoon\" to symbolize personal transformation and growth through self-expression in a song about embracing one's true self.");
		v.AddPos("Comparing the journey of self-expression to \"building a bridge to the unknown\" in a song about taking risks and embracing personal authenticity and creativity.");
		v.AddPos("Describing the act of expressing oneself through music as \"painting a canvas of emotions\" in a song about the power and depth of personal expression and creativity.");
		v.AddPos("Referring to self-expression as \"writing a love letter to the universe\" in a song about the impact and connection that comes from sharing one's true voice and emotions.");
		v.AddPos("Using the metaphor of a \"tree branching out in different directions\" to represent the diversity and uniqueness of personal expression and individuality in a song about breaking away from societal norms.");
		v.AddPos("Using the imagery of a \"spark igniting a fire\" to represent the empowering effect of expressing oneself freely and authentically in a song about finding one's voice and identity.");
		v.AddPos("Comparing the act of self-expression to \"embarking on a journey of self-discovery\" in a song about the courage and self-discovery that comes from embracing one's true self and creativity.");
		v.AddPos("Describing the act of self-expression as \"writing a symphony of the soul\" in a song about the personal significance and emotional depth of expressing oneself through creative outlets.");
		v.AddPos("Referring to self-expression as \"building a house of dreams\" in a song about the joy and fulfillment of embracing one's true self and creativity.");
		v.AddPos("Using the metaphor of a \"rainbow after a storm\" to represent the growth and beauty that can come from expressing oneself authentically in a song about overcoming struggles and finding self-expression.");
		v.AddPos("Using the imagery of a \"phoenix rising from the ashes\" to symbolize personal transformation and rebirth through self-expression in a song about embracing one's true identity.");
		v.AddNeg("Using the metaphor of a \"caged bird singing\" to represent the struggle and oppression of expressing oneself in a song about societal constraints and limitations.");
		v.AddNeg("Referring to self-expression as \"breaking out of a mold\" in a song about going against societal expectations and norms.");
		v.AddNeg("Using the imagery of a \"wilted flower\" to symbolize the suppression and suffocation of personal expression in a song about conforming to societal standards.");
		v.AddNeg("Describing the act of expressing oneself as \"fighting a battle\" in a song about the struggle and resistance that comes with going against societal norms and expressing oneself freely.");
		v.AddNeg("Comparing the journey of self-expression to \"climbing a steep mountain\" in a song about the difficulty and struggle of embracing one's true voice and identity.");
		v.AddNeg("Using the metaphor of a \"puppet on strings\" to represent the lack of control one has over personal expression in a song about societal pressure and conformity.");
		v.AddNeg("Referring to self-expression as \"stepping out of line\" in a song about going against societal norms and expectations.");
		v.AddNeg("Using the imagery of a \"glimmer of light in the darkness\" to symbolize the difficulty and resistance of expressing oneself in a song about facing societal norms and expectations.");
		v.AddNeg("Describing the act of self-expression as \"a whisper in a crowd\" in a song about the struggle of being heard and understood in a society that values conformity.");
		v.AddNeg("Referring to self-expression as \"a thorn in the side\" in a song about facing criticism and pushback for going against societal norms and expressing oneself authentically.");
		v.AddNeg("Using the metaphor of a \"fish out of water\" to represent the discomfort and alienation of not conforming to societal expectations in a song about the challenges of being different.");
		v.AddNeg("Using the imagery of a \"storm brewing\" to symbolize the potential backlash and consequences of expressing oneself freely and authentically in a song about societal backlash and resistance.");
		v.AddNeg("Comparing the act of self-expression to \"swimming against the current\" in a song about the challenge and struggle of going against societal norms and expectations.");
		v.AddNeg("Using the metaphor of a \"rebel without a cause\" to represent the societal stereotype and judgment faced by those who express themselves differently in a song about breaking free from societal constraints.");
		v.AddNeg("Referring to self-expression as \"breaking the rules\" in a song about embracing one's true self and creativity despite societal pressure to conform.");
		v.AddNeg("Using the imagery of a \"bird in a cage\" to symbolize the limitations and suppression of personal expression in a song about the struggle to break free from societal norms and expectations.");
		v.AddNeg("Describing the act of expressing oneself as \"a scream in silence\" in a song about the frustration and isolation of not being able to express oneself freely in a conformist society.");
		v.AddNeg("Referring to self-expression as \"going against the grain\" in a song about the challenges and criticism faced when expressing oneself authentically in a society that values conformity.");
		v.AddNeg("Using the metaphor of a \"square peg in a round hole\" to represent the discomfort and struggle of not fitting into societal norms in a song about personal identity and self-expression.");
		v.AddNeg("Using the imagery of a \"censored voice\" to symbolize the suppression and silencing of personal expression in a song about the challenges of being true to oneself in a conformist society.");
		v.AddNeg("Comparing the journey of self-expression to a \"battlefield\" in a son about the challenges and obstacles faced when expressing oneself authentically in a society that values conformity.");
		v.AddNeg("Using the metaphor of a \"thorn in the rosebush\" to represent the difficulty and resistance faced when going against societal norms and expressing oneself freely in a song about societal pressure and conformity.");
		v.AddNeg("Referring to self-expression as \"a needle in a haystack\" in a song about the struggle of being seen and understood for one's true self in a society that values conformity.");
		v.AddNeg("Using the imagery of a \"lightning bolt in a clear sky\" to symbolize the shock and disruption caused by expressing oneself differently in a conformist society in a song about breaking free from societal constraints and expectations.");
		v.AddNeg("Describing the act of self-expression as \"a whisper in a hurricane\" in a song about the struggle and frustration of being heard and understood in a society that values conformity.");
		v.AddNeg("Referring to self-expression as \"the odd one out\" in a song about the challenges and isolation faced when expressing oneself authentically in a society that values uniformity.");
		v.AddNeg("Using the metaphor of a \"cracked mirror\" to represent the struggle and discomfort of not fitting into societal norms and expectations in a song");
		v.AddNeg("Referring to self-expression as \"the black sheep\" in a song about the challenges and judgment faced when expressing oneself authentically in a conformist society.");
		v.AddNeg("Using the imagery of a \"fly in amber\" to symbolize the suffocation and suppression of personal expression in a song about the challenges of being true to oneself in a conformist society.");
		v.AddNeg("Comparing the act of self-expression to \"running against the wind\" in a song about the resistance and struggle of expressing oneself differently in a society that values conformity.");
		v.AddNeg("Using the metaphor of a \"round peg in a square hole\" to represent the discomfort and challenge of not fitting into societal norms and expectations in a song about the pressure to conform.");
		v.AddNeg("Referring to self-expression as \"a splash of color in a black and white world\" in a song about the difficulty and pushback faced by those who express themselves differently in a conformist society.");
		v.AddNeg("Using the imagery of a \"cloud in a clear sky\" to symbolize the disruption and discomfort caused by expressing oneself authentically in a conformist society in a song about breaking free from societal constraints and expectations.");
		v.AddNeg("Describing the act of expressing oneself as \"a pebble in a pond\" in a song about the ripple effect and pushback faced when going against societal norms and expressing oneself authentically.");
		v.AddNeg("Referring to self-expression as \"a flame in the darkness\" in a song about the challenges and risks involved in expressing oneself freely in a conformist society.");
		v.AddNeg("Using the metaphor of a \"thorn in the side\" to represent the resistance and backlash faced when expressing oneself authentically in a society that values uniformity in a song about breaking free from societal constraints.");
		v.AddNeg("Using the imagery of a \"broken mirror\" to symbolize the discomfort and challenge of not fitting into societal expectations and norms in a song about the pressure to conform.");
		v.AddNeg("Comparing the journey of self-expression to a \"ship fighting against the current\" in a song about the difficulties and challenges of expressing oneself authentically in a conformist society.");
		v.AddNeg("Describing the act of self-expression as \"a whisper in a crowd outcry\" in a song about the struggle and frustration of being understood and accepted for one's true self in a conformist society.");
		v.AddNeg("Referring to self-expression as \"swimming against the tide\" in a song about the resistance and struggle of going against societal norms and expectations.");
		v.AddNeg("Using the metaphor of a \"black sheep in a herd\" to represent the alienation and judgment faced when expressing oneself authentically in a conformist society.");
		v.AddNeg("Using the imagery of a \"missing puzzle piece\" to symbolize the discomfort and struggle of not fitting into societal expectations and norms in a song about personal identity and self-expression.");
		v.AddNeg("Comparing the act of self-expression to \"trying to catch the wind\" in a song about the difficulty and frustration of expressing oneself authentically in a conformist society.");
		v.AddNeg("Referring to self-expression as \"a fish out of water\" in a song about the challenges and isolation faced when expressing oneself differently in a society that values conformity.");
		v.AddNeg("Using the metaphor of a \"berrypicker in a potato field\" to represent the discomfort and challenge of not fitting into societal norms and expectations in a song about the pressure to conform.");
		v.AddNeg("Using the imagery of a \"shadow in a sunny day\" to symbolize the struggle and discomfort of suppressing one's true self in a song about the expectations and pressure of conformist society.");
		v.AddNeg("Describing the act of expressing oneself as \"a rose among thorns\" in a song about the challenges and judgment faced when expressing oneself authentically in a conformist society.");
		v.AddNeg("Referring to self-expression as \"off the beaten path\" in a song about the pushback and societal judgment faced when going against norms and expressing oneself authentically.");
		v.AddNeg("Using the metaphor of a \"broken vase\" to represent the discomfort and struggle of not fitting into societal expectations and norms in a song about personal identity and self-expression.");
		v.AddNeg("Comparing the journey of self-expression to \"walking on a tightrope\" in a song about the risks and challenges of expressing oneself authentically in a conformist society.");
		v.AddNeg("Using the imagery of a \"pathless road\" to symbolize the difficulty and resistance faced by those who express themselves differently in a conformist society in a song about breaking free from societal constraints and expectations.");
		v.AddNeg("Describing the act of expressing oneself as \"a wave in a still pond\" in a song about the disruption and discomfort caused by expressing oneself freely and authentically in a society that values conformity.");
		v.AddNeg("Referring to self-expression as \"a song in a silent room\" in a song about the challenge and struggle of being heard and understood in a conformist society that values uniformity.");
		v.AddNeg("Using the metaphor of a \"broken bird with clipped wings\" to represent the suppression and suffocation of personal expression in a song about the struggle to");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "wordplay-like altering of popular lyrics";
		v.AddPos("\"make hay while the sun shines\" -> \"make hay while the sun sets\"");
		v.AddPos("\"laughter is the best medicine\" -> \"laughter is the best therapy\"");
		v.AddPos("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, make lemon meringue pie\"");
		v.AddPos("\"what doesn't kill you makes you stronger\" -> \"what doesn't challenge you, doesn't change you\"");
		v.AddPos("\"practice makes perfect\" -> \"practice makes progress\"");
		v.AddPos("\"a penny saved is a penny earned\" -> \"a penny spent is a penny enjoyed\"");
		v.AddPos("\"you can't have your cake and eat it too\" -> \"you can have your cake and share it too\"");
		v.AddPos("\"the grass is always greener on the other side\" -> \"the grass is green where you water it\"");
		v.AddPos("\"great minds think alike\" -> \"great minds think for themselves\"");
		v.AddPos("\"no pain, no gain\" -> \"no challenge, no growth\"");
		v.AddPos("\"slow and steady wins the race\" -> \"brisk and bold wins the race\"");
		v.AddPos("\"don't judge a book by its cover\" -> \"don't judge a book by its title\"");
		v.AddPos("\"actions speak louder than words\" -> \"silence speaks louder than words\"");
		v.AddPos("\"better late than never\" -> \"better early than always\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to sleep in on weekends\"");
		v.AddPos("\"misery loves company\" -> \"happiness loves company too\"");
		v.AddPos("\"curiosity killed the cat\" -> \"curiosity sparked the mind\"");
		v.AddPos("\"time heals all wounds\" -> \"time brings new beginnings\"");
		v.AddPos("\"money doesn't grow on trees\" -> \"happiness doesn't grow on trees either\"");
		v.AddPos("\"seeing is believing\" -> \"imagining is believing\"");
		v.AddPos("\"you can't teach an old dog new tricks\" -> \"you can teach an old dog new attitudes\"");
		v.AddPos("\"if at first you don't succeed, try, try again\" -> \"if at first you don't succeed, reflect and try again\"");
		v.AddPos("\"two heads are better than one\" -> \"two perspectives are better than one\"");
		v.AddPos("\"the bigger they are, the harder they fall\" -> \"the bigger they are, the harder they bounce back\"");
		v.AddPos("\"what goes up must come down\" -> \"what goes up can stay up with hard work and determination\"");
		v.AddPos("\"the glass is half empty\" -> \"the glass is always refillable\"");
		v.AddPos("\"jack of all trades, master of none\" -> \"jack of all trades, master of versatility\"");
		v.AddPos("\"the squeaky wheel gets the grease\" -> \"the calm wheel gets a smooth ride\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety is the zest of life\"");
		v.AddPos("\"hope for the best, prepare for the worst\" -> \"hope for the best, prepare for the journey\"");
		v.AddPos("\"age is just a number\" -> \"age is just a label\"");
		v.AddPos("\"life is short\" -> \"life is an endless adventure\"");
		v.AddPos("\"all good things must come to an end\" -> \"all good things can continue if you put in the effort\"");
		v.AddPos("\"where there's a will, there's a way\" -> \"where there's a will, there's always a way\"");
		v.AddPos("\"if you can't beat them, join them\" -> \"if you can't beat them, lead them\"");
		v.AddPos("\"what you see is what you get\" -> \"what you see is only the beginning\"");
		v.AddPos("\"charity begins at home\" -> \"charity knows no boundaries\"");
		v.AddPos("\"easy come, easy go\" -> \"easy come, hard work stays\"");
		v.AddPos("\"don't throw the baby out with the bathwater\" -> \"don't throw the baby out without giving it a chance\"");
		v.AddPos("\"if life gives you lemons, make lemonade\" -> \"if life gives you lemons, make lemon sorbet\"");
		v.AddPos("\"a watched pot never boils\" -> \"a watched clock never ticks\" ");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets the worm and gets back to work\"");
		v.AddPos("\"give me liberty, or give me death\" -> \"give me love, or give me life\"");
		v.AddPos("\"kill two birds with one stone\" -> \"nurture two birds with one hand\"");
		v.AddPos("\"too many cooks spoil the broth\" -> \"too many cooks make a delicious stew\"");
		v.AddPos("\"hindsight is 20/20\" -> \"foresight is 20/20\"");
		v.AddPos("\"you can't have your cake and eat it too\" -> \"you can't have your cake and share it too\"");
		v.AddPos("\"don't count your chickens before they hatch\" -> \"don't underestimate the potential of your eggs\"");
		v.AddPos("\"life is a rollercoaster\" -> \"life is a choose-your-own-adventure game\"");
		v.AddPos("\"the sky's the limit\" -> \"never limit your possibilities\"");
		v.AddPos("\"call a spade a spade\" -> \"call a spade a must-have gardening tool\"");
		v.AddPos("\"a chain is only as strong as its weakest link\" -> \"a team is only as strong as its weakest player\"");
		v.AddPos("\"look before you leap\" -> \"think before you act\"");
		v.AddPos("\"a picture is worth a thousand words\" -> \"words can paint a picture too\"");
		v.AddPos("\"history repeats itself\" -> \"history can be rewritten by the present\"");
		v.AddPos("\"don't put all your eggs in one basket\" -> \"don't limit yourself to one path\"");
		v.AddPos("\"birds of a feather flock together\" -> \"diversity makes for beautiful flocks\"");
		v.AddPos("\"all roads lead to Rome\" -> \"all roads lead to new adventures\"");
		v.AddPos("\"still waters run deep\" -> \"still waters can surprise you\"");
		v.AddPos("\"you can't teach an old dog new tricks\" -> \"you can teach an old dog new perspectives\"");
		v.AddPos("\"actions speak louder than words\" -> \"words can spark action too\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird catches the worm and watches the sunrise\"");
		v.AddPos("\"blood is thicker than water\" -> \"love is thicker than blood\"");
		v.AddPos("\"life is a box of chocolates\" -> \"life is a buffet try everything\"");
		v.AddPos("\"sex sells\" -> \"love sells\"");
		v.AddPos("\"a stitch in time saves nine\" -> \"a little effort now saves a lot of effort later\"");
		v.AddPos("\"the bigger they are, the harder they fall\" -> \"the bigger they are, the harder they can rise\"");
		v.AddPos("\"a problem shared is a problem halved\" -> \"sharing happiness doubles it\"");
		v.AddPos("\"ignorance is bliss\" -> \"knowledge is powerful\"");
		v.AddPos("\"a penny saved is a penny earned\" -> \"a penny earned is a penny to share\"");
		v.AddPos("\"you catch more flies with honey than vinegar\" -> \"you catch more friends with kindness than sarcasm\"");
		v.AddPos("\"the grass is always greener on the other side\" -> \"the grass is always greener on both sides let's explore\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to take a break\"");
		v.AddPos("\"haste makes waste\" -> \"patience makes progress\"");
		v.AddPos("\"when in Rome, do as the Romans do\" -> \"when in a new place, embrace the unfamiliar\"");
		v.AddPos("\"an apple a day keeps the doctor away\" -> \"a balanced diet keeps the doctor away\"");
		v.AddPos("\"don't put all your eggs in one basket\" -> \"don't limit yourself to one opportunity\"");
		v.AddPos("\"easy come, easy go\" -> \"easy come, easy effort\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets the worm and shares it with others\"");
		v.AddPos("\"birds of a feather flock together\" -> \"diversity makes for interesting flocks\"");
		v.AddPos("\"the grass is always greener on the other side\" -> \"the grass is always greener where you water it\"");
		v.AddPos("\"actions speak louder than words\" -> \"words can also speak volumes\"");
		v.AddPos("\"haste makes waste\" -> \"slow and steady wins the race\"");
		v.AddPos("\"a penny saved is a penny earned\" -> \"a penny spent supports livelihoods\"");
		v.AddPos("\"what you see is what you get\" -> \"what you see is just the surface\"");
		v.AddPos("\"the pot calling the kettle black\" -> \"the pot complementing the kettle's style\"");
		v.AddPos("\"money is the root of all evil\" -> \"love is the root of all good\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to sleep in tomorrow\"");
		v.AddPos("\"a jack of all trades, master of none\" -> \"a jack of all trades, master of creativity\"");
		v.AddPos("\"a bird in the hand is worth two in the bush\" -> \"a bird in the hand is worth more than any amount of birds in bushes\"");
		v.AddPos("\"you can't judge a book by its cover\" -> \"you can't judge a book by its title\"");
		v.AddPos("\"no pain, no gain\" -> \"no challenge, no success\"");
		v.AddPos("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, make a lemon tree\"");
		v.AddPos("\"never judge a man by his shoes\" -> \"never judge a man by his occupation\"");
		v.AddPos("\"better safe than sorry\" -> \"better risk it for the reward\"");
		v.AddPos("\"give credit where credit is due\" -> \"give recognition where recognition is due\"");
		v.AddPos("\"easier said than done\" -> \"easier done than believed\"");
		v.AddPos("\"a leopard can't change its spots\" -> \"a leopard can change its attitude\"");
		v.AddPos("\"two heads are better than one\" -> \"one head can generate ideas too\"");
		v.AddPos("\"a penny for your thoughts\" -> \"a penny for your creativity\"");
		v.AddPos("\"curiosity killed the cat\" -> \"curiosity impacted the world\"");
		v.AddPos("\"a stitch in time saves nine\" -> \"a stitch in time can save a friendship\"");
		v.AddPos("\"a chip off the old block\" -> \"a unique piece of the family puzzle\"");
		v.AddPos("\"a picture is worth a thousand words\" -> \"a thousand words can create an image too\"");
		v.AddPos("\"actions speak louder than words\" -> \"actions can also speak softly and make a big impact\"");
		v.AddPos("\"still waters run deep\" -> \"still waters can hide great depth\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety is the flavor of life\"");
		v.AddPos("\"life is like a box of chocolates\" -> \"life is like a choose-your-own-adventure book\"");
		v.AddPos("\"all good things must come to an end\" -> \"all good things can continue with determination\"");
		v.AddPos("\"you can't make an omelette without breaking a few eggs\" -> \"you can't experience growth without discomfort\"");
		v.AddPos("\"what goes up must come down\" -> \"what goes up can stay up with perseverance\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets the worm and gets to enjoy the sunrise\"");
		v.AddPos("\"easier said than done\" -> \"easier done than overthought\"");
		v.AddPos("\"birds of a feather flock together\" -> \"birds of different feathers make for a vibrant flock\"");
		v.AddPos("\"practice makes perfect\" -> \"practice makes progress\"");
		v.AddPos("\"don't put all your eggs in one basket\" -> \"don't limit yourself to one choice\"");
		v.AddPos("\"don't judge a book by its cover\" -> \"don't judge a book by its appearance\"");
		v.AddPos("\"charity begins at home\" -> \"charity knows no boundaries\"");
		v.AddPos("\"too many cooks spoil the broth\" -> \"a diverse team makes a delicious dish\"");
		v.AddPos("\"you can't have your cake and eat it too\" -> \"you can't have all the cake, but you can have a slice\"");
		v.AddPos("\"misery loves company\" -> \"joy loves company too\"");
		v.AddPos("\"jack of all trades, master of none\" -> \"jack of all trades, master of creativity\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety is the seasoning of life\"");
		v.AddPos("\"when in Rome, do as the Romans do\" -> \"when in unfamiliar territory, embrace the adventure\"");
		v.AddPos("\"a watched pot never boils\" -> \"a watched plant never grows\"");
		v.AddPos("\"history repeats itself\" -> \"history can be rewritten by the present\"");
		v.AddPos("\"you can't teach an old dog new tricks\" -> \"you can teach an old dog new attitudes\"");
		v.AddPos("\"if at first you don't succeed, try, try again\" -> \"if at first you don't succeed, reflect and try again\"");
		v.AddPos("\"no good deed goes unpunished\" -> \"no good deed goes unnoticed\"");
		v.AddPos("\"slow and steady wins the race\" -> \"consistent effort leads to success\"");
		v.AddPos("\"time heals all wounds\" -> \"time brings new beginnings\"");
		v.AddPos("\"you can't have your cake and eat it too\" -> \"you can't have all the cake, but you can have a bite\"");
		v.AddPos("\"the bigger they are, the harder they fall\" -> \"the bigger they are, the harder they can bounce back\"");
		v.AddPos("\"seeing is believing\" -> \"imagining is believing too\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety makes life full of flavor\"");
		v.AddPos("\"where there's a will, there's a way\" -> \"where there's a will, there's determination\"");
		v.AddPos("\"if you can't beat them, join them\" -> \"if you can't beat them, lead them\"");
		v.AddPos("\"practice what you preach\" -> \"preach what you practice\"");
		v.AddPos("\"you can't have your cake and eat it too\" -> \"you can have your cake and share it too\"");
		v.AddPos("\"don't count your chickens before they hatch\" -> \"don't underestimate the potential of your opportunities\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to relax and enjoy the sunrise\"");
		v.AddPos("\"blood is thicker than water\" -> \"love is thicker than blood\"");
		v.AddPos("\"life is short\" -> \"life is an endless adventure\"");
		v.AddPos("\"all good things must come to an end\" -> \"all good things can continue with perseverance\"");
		v.AddPos("\"where there's smoke, there's fire\" -> \"where there's collaboration, there's success\"");
		v.AddPos("\"easy come, easy go\" -> \"effortful creation leaves a lasting impact\"");
		v.AddPos("\"you can't judge a book by its cover\" -> \"you can't judge a person by their appearance\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety adds flavor to life\"");
		v.AddPos("\"don't throw the baby out with the bathwater\" -> \"don't give up on something without trying\"");
		v.AddPos("\"if life gives you lemons, make lemonade\" -> \"if life gives you lemons, make lemon cake\"");
		v.AddPos("\"a watched pot never boils\" -> \"a watched fire never burns\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to enjoy a relaxing morning\"");
		v.AddPos("\"give me liberty, or give me death\" -> \"give me love, or give me passion\"");
		v.AddPos("\"kill two birds with one stone\" -> \"nurture two birds with one gesture\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety adds zest to life\"");
		v.AddPos("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, make lemon pie\"");
		v.AddPos("\"better late than never\" -> \"better early than always\"");
		v.AddPos("\"the squeaky wheel gets the grease\" -> \"the calm wheel gets a smooth ride\"");
		v.AddPos("\"people in glass houses shouldn't throw stones\" -> \"people in glass houses should admire the view\"");
		v.AddPos("\"a watched pot never boils\" -> \"a watched plant never blooms\"");
		v.AddPos("\"still waters run deep\" -> \"still waters can hold hidden treasures\"");
		v.AddPos("\"look before you leap\" -> \"think before you act\"");
		v.AddPos("\"life is like a rollercoaster\" -> \"life is like a choose-your-own-adventure story\"");
		v.AddPos("\"the sky's the limit\" -> \"never limit your possibilities\"");
		v.AddPos("\"call a spade a spade\" -> \"call a spade a must-have gardening tool\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to enjoy a lazy day\"");
		v.AddPos("\"give me liberty, or give me death\" -> \"give me love, or give me life\"");
		v.AddPos("\"kill two birds with one stone\" -> \"nurture two birds with one caring act\"");
		v.AddPos("\"seeing is believing\" -> \"imagining is believing too\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety adds a dash of flavor to life\"");
		v.AddPos("\"better safe than sorry\" -> \"take a risk for the reward\"");
		v.AddPos("\"when in Rome, do as the Romans do\" -> \"when in new territory, explore with an open mind\"");
		v.AddPos("\"an apple a day keeps the doctor away\" -> \"a varied diet keeps the doctor away\"");
		v.AddPos("\"don't put all your eggs in one basket\" -> \"don't limit yourself to one path\"");
		v.AddPos("\"easy come, easy go\" -> \"easy come, long-lasting impact\"");
		v.AddPos("\"the early bird gets the worm\" -> \"the early bird gets to take a break and relax\"");
		v.AddPos("\"a jack of all trades, master of none\" -> \"a jack of all trades, master of creativity\"");
		v.AddPos("\"a bird in the hand is worth two in the bush\" -> \"a bird in the hand is worth more than any number of birds in bushes\"");
		v.AddPos("\"variety is the spice of life\" -> \"variety adds spice to life\"");
		v.AddPos("\"you can't make an omelette without breaking a few eggs\" -> \"you can't achieve success without taking risks\"");
		v.AddNeg("\"love is blind\" -> \"love is only skin deep\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird gets the flu\"");
		v.AddNeg("\"every cloud has a silver lining\" -> \"every storm brings chaos\"");
		v.AddNeg("\"time heals all wounds\" -> \"time scars all hearts\"");
		v.AddNeg("\"what goes up must come down\" -> \"what goes around comes around\"");
		v.AddNeg("\"if at first you don't succeed, try, try again\" -> \"if at first you don't succeed, give up and try something else\"");
		v.AddNeg("\"two wrongs don't make a right\" -> \"two wrongs just make a bigger mess\" ");
		v.AddNeg("\"ignorance is bliss\" -> \"ignorance is dangerous\"");
		v.AddNeg("\"it's better to have loved and lost than to never have loved at all\" -> \"it's better to never have loved at all\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words speak louder than actions\"");
		v.AddNeg("\"better late than never\" -> \"better never than late\"");
		v.AddNeg("\"money can't buy happiness\" -> \"money can't buy true love\"");
		v.AddNeg("\"honesty is the best policy\" -> \"honesty is the best way to ruin a friendship\"");
		v.AddNeg("\"what doesn't kill you makes you stronger\" -> \"what doesn't kill you makes you tired and sore\" ");
		v.AddNeg("\"good things come to those who wait\" -> \"good things come to those who take action\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where you water it\"");
		v.AddNeg("\"birds of a feather flock together\" -> \"birds of a feather get stuck in the same tree\"");
		v.AddNeg("\"you reap what you sow\" -> \"you reap what others around you sow as well\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird gets the worm and food poisoning\"");
		v.AddNeg("\"life is like a box of chocolates\" -> \"life is like a box of chocolates, you never know what you're allergic to\"");
		v.AddNeg("\"the road to hell is paved with good intentions\" -> \"the road to hell is paved with good intentions, but it's still a smooth ride\"");
		v.AddNeg("\"beauty is in the eye of the beholder\" -> \"beauty is in the eye of the photoshop artist\"");
		v.AddNeg("\"there's no such thing as a free lunch\" -> \"there's always a catch to a free lunch\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where you can't see it\"");
		v.AddNeg("\"a picture is worth a thousand words\" -> \"a picture can be worth a thousand lies\"");
		v.AddNeg("\"curiosity killed the cat\" -> \"curiosity killed the cat and gave it nine lives\"");
		v.AddNeg("\"you can't teach an old dog new tricks\" -> \"it's never too late to learn something new, even for an old dog\"");
		v.AddNeg("\"all's fair in love and war\" -> \"all's fair in love and war, but war is never fair\"");
		v.AddNeg("\"never judge a book by its cover\" -> \"never judge a book by its cover, but by its Goodreads rating\"");
		v.AddNeg("\"the more things change, the more they stay the same\" -> \"the more things change, the more they change again\"");
		v.AddNeg("\"the pen is mightier than the sword\" -> \"the pen can also be mightier than the delete button\"");
		v.AddNeg("\"great minds think alike\" -> \"great minds think alike, but fools seldom differ\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words can also speak louder than actions\"");
		v.AddNeg("\"it's not over until the fat lady sings\" -> \"it's not over until the skinny girl raps\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird may get the worm, but the second mouse gets the cheese\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"out of sight, but still on Instagram\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and still fit into your pants\"");
		v.AddNeg("\"time heals all wounds\" -> \"time can also open old wounds and make them bleed again\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where there's a sprinkler system\"");
		v.AddNeg("\"practice makes perfect\" -> \"practice may not make you perfect, but it can make you tired\"");
		v.AddNeg("\"you can't judge a book by its cover\" -> \"you also can't judge a book by its Kindle edition\"");
		v.AddNeg("\"it's better to have loved and lost than to never have loved at all\" -> \"it's also better to have loved and lost than to have dated a jerk\"");
		v.AddNeg("\"what doesn't kill you makes you stronger\" -> \"what doesn't kill you can also give you trust issues\"");
		v.AddNeg("\"haste makes waste\" -> \"haste can also lead to a good story\"");
		v.AddNeg("\"a stitch in time saves nine\" -> \"a stitch in time saves nine, but it can also waste a lot of thread\"");
		v.AddNeg("\"blood is thicker than water\" -> \"sometimes blood is thinner than water\"");
		v.AddNeg("\"easy come, easy go\" -> \"easy come, easy leave\"");
		v.AddNeg("\"what goes around comes around\" -> \"what goes around can also go in circles for a while\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and still fit into your wedding dress\"");
		v.AddNeg("\"practice makes perfect\" -> \"practice may also make you forget what perfection is\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird can also miss the worm if it snoozes its alarm\"");
		v.AddNeg("\"every cloud has a silver lining\" -> \"every cloud can also have lightning and thunder\"");
		v.AddNeg("\"good things come to those who wait\" -> \"good things can also come to those who take action and seize opportunities\"");
		v.AddNeg("\"it takes two to tango\" -> \"it takes two to tango, but sometimes one person bruises their toes\"");
		v.AddNeg("\"don't cry over spilled milk\" -> \"don't cry over spilled milk, just mop it up and move on\"");
		v.AddNeg("\"a rolling stone gathers no moss\" -> \"a rolling stone may also gather some awesome adventures\"");
		v.AddNeg("\"honesty is the best policy\" -> \"honesty can also get you into trouble\" ");
		v.AddNeg("\"the squeaky wheel gets the grease\" -> \"the squeaky wheel can also be replaced\"");
		v.AddNeg("\"the more, the merrier\" -> \"the more, the louder\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and your diet too\"");
		v.AddNeg("\"if it ain't broke, don't fix it\" -> \"if it ain't broke, you're probably just not looking hard enough\" ");
		v.AddNeg("\"better safe than sorry\" -> \"better risky than boring\"");
		v.AddNeg("\"a penny saved is a penny earned\" -> \"a penny saved is a penny that you'll probably just lose in your couch anyway\"");
		v.AddNeg("\"if the shoe fits, wear it\" -> \"if the shoe doesn't fit, try a different style\"");
		v.AddNeg("\"life is like a box of chocolates\" -> \"life is like a box of chocolates, you never know which ones are milk or dark\"");
		v.AddNeg("\"don't judge a book by its cover\" -> \"don't judge a book by its cover, but by its introduction\"");
		v.AddNeg("\"what's good for the goose is good for the gander\" -> \"what's good for the goose can also be good for the gosling\"");
		v.AddNeg("\"you can't make an omelette without breaking eggs\" -> \"you can't make an omelette without breaking a few dishes\"");
		v.AddNeg("\"beauty is in the eye of the beholder\" -> \"beauty is in the eye of the Photoshopper\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird also gets tired and needs a nap\"");
		v.AddNeg("\"there's no time like the present\" -> \"there may also be no time at all if you don't plan\"");
		v.AddNeg("\"don't throw the baby out with the bathwater\" -> \"don't throw the baby out with the dirty diaper\"");
		v.AddNeg("\"the glass is half full\" -> \"the glass is also half empty\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird can also get caught by the cat\"");
		v.AddNeg("\"hindsight is 20/20\" -> \"hindsight can also be blinded by nostalgia\"");
		v.AddNeg("\"it's always darkest before the dawn\" -> \"it's also darkest when the power goes out\"");
		v.AddNeg("\"absence makes the heart grow fonder\" -> \"absence can also make you forgetful\"");
		v.AddNeg("\"beggars can't be choosers\" -> \"beggars also can't be satisfied\"");
		v.AddNeg("\"a watched pot never boils\" -> \"a watched pot can also boil over\"");
		v.AddNeg("\"all good things must come to an end\" -> \"all good things can also come to a very rewarding end\"");
		v.AddNeg("\"too many cooks spoil the broth\" -> \"too many cooks can also lead to a gourmet meal\"");
		v.AddNeg("\"if at first you don't succeed, try, try again\" -> \"if at first you don't succeed, maybe try a different approach\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where the sprinklers are broken\"");
		v.AddNeg("\"jack of all trades, master of none\" -> \"jack of all trades, master of a few\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and still fit into your swimsuit\"");
		v.AddNeg("\"a fool and his money are soon parted\" -> \"a rich fool and his money are still foolish\"");
		v.AddNeg("\"an apple a day keeps the doctor away\" -> \"an apple a day gives the doctor something to say\"");
		v.AddNeg("\"don't put all your eggs in one basket\" -> \"don't put all your eggs in one basket, unless it's an EGGcelent basket\"");
		v.AddNeg("\"practice makes perfect\" -> \"practice may also make you really good at something you hate\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and still fit into your skinny jeans\"");
		v.AddNeg("\"a bird in the hand is worth two in the bush\" -> \"a bird in the hand can also be worth more than two in the bush\"");
		v.AddNeg("\"idle hands are the devil's playthings\" -> \"idle hands can also be the hands that create beautiful art\"");
		v.AddNeg("\"brevity is the soul of wit\" -> \"brevity can also make you missed the point\"");
		v.AddNeg("\"birds of a feather flock together\" -> \"birds of a feather can also fly solo\"");
		v.AddNeg("\"don't cry over spilled milk\" -> \"don't cry over spilled milk, but maybe shout a little\"");
		v.AddNeg("\"what doesn't kill you makes you stronger\" -> \"what doesn't kill you can also make you question your sanity\"");
		v.AddNeg("\"a journey of a thousand miles begins with a single step\" -> \"a journey of a thousand miles can also start with a wrong turn\"");
		v.AddNeg("\"don't judge a book by its cover\" -> \"don't judge a book by its cover, but by its Goodreads rating\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where someone else is mowing it\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"out of sight, also means out of sight for the paparazzi\"");
		v.AddNeg("\"time is money\" -> \"time can also be priceless\"");
		v.AddNeg("\"a picture is worth a thousand words\" -> \"a picture can also be worth a thousand lies\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird shouldn't eat too many worms, they're high in fat\"");
		v.AddNeg("\"beggars can't be choosers\" -> \"beggars can also be grateful for any generosity\"");
		v.AddNeg("\"where there's smoke, there's fire\" -> \"where there's smoke, there might also be marshmallows\"");
		v.AddNeg("\"too many cooks spoil the broth\" -> \"too many cooks can also lead to a potluck party\"");
		v.AddNeg("\"better late than never\" -> \"better late than never, but earlier is still better\"");
		v.AddNeg("\"good things come to those who wait\" -> \"good things can also come to those who take action and pursue them\"");
		v.AddNeg("\"if it's not one thing, it's another\" -> \"if it's not one thing, it's usually Murphy's Law\" ");
		v.AddNeg("\"age is just a number\" -> \"age is just a number, until it affects your joints\"");
		v.AddNeg("\"if the shoe fits, wear it\" -> \"if the shoe doesn't fit, browse the clearance section\"");
		v.AddNeg("\"a rolling stone gathers no moss\" -> \"a rolling stone can also gather some pretty great momentum\"");
		v.AddNeg("\"time heals all wounds\" -> \"time can also make you forget how it got the scars in the first place\"");
		v.AddNeg("\"ignorance is bliss\" -> \"ignorance can also lead to unintentional offense\"");
		v.AddNeg("\"you can't judge a book by its cover\" -> \"you also can't judge a book by its audio book edition\"");
		v.AddNeg("\"don't throw the baby out with the bathwater\" -> \"don't throw the baby out with the bathwater, just get new bathwater\"");
		v.AddNeg("\"blood is thicker than water\" -> \"sometimes blood can also be thinner than water\"");
		v.AddNeg("\"easier said than done\" -> \"easier said than done, but at least there are apps for that now\"");
		v.AddNeg("\"haste makes waste\" -> \"haste can also make you forget important steps\"");
		v.AddNeg("\"all's fair in love and war\" -> \"all's fair in love and war, but war is never fair\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the late bird can also catch the worm (or the early bird)\" ");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have your cake and be vegan too\"");
		v.AddNeg("\"a watched pot never boils\" -> \"a watched pot can also boil over and make a mess\"");
		v.AddNeg("\"too many cooks spoil the broth\" -> \"too many cooks can also lead to a great party\"");
		v.AddNeg("\"you can't win them all\" -> \"you can't win all of them, but it's still fun to try\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"out of sight, out of mind, but still on social media\"");
		v.AddNeg("\"all that glitters is not gold\" -> \"all that glitters can also be fake\"");
		v.AddNeg("\"never say never\" -> \"never say never, except when you're making promises\"");
		v.AddNeg("\"it's better to have loved and lost than to never have loved at all\" -> \"it's also better to have loved and lost than to have wasted your time\"");
		v.AddNeg("\"practice makes perfect\" -> \"practice can also make you really good at something you're not good at\"");
		v.AddNeg("\"if the shoe fits, wear it\" -> \"if the shoe doesn't fit, try on different sizes and styles\"");
		v.AddNeg("\"don't count your chickens before they hatch\" -> \"don't count your chickens before they hatch, but maybe invest in an incubator\"");
		v.AddNeg("\"save for a rainy day\" -> \"save for a rainy day, and maybe a surprise vacation\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird can also get caught by the early morning traffic\"");
		v.AddNeg("\"too many cooks spoil the broth\" -> \"too many cooks can also lead to a potluck party and a great meal\"");
		v.AddNeg("\"you can't always get what you want\" -> \"you can't always get what you want, but sometimes you do and you realize you didn't want it after all\"");
		v.AddNeg("\"you can't please everyone\" -> \"you can't please everyone, but sometimes you can please yourself\"");
		v.AddNeg("\"if you can't beat them, join them\" -> \"if you can't beat them, just find a new game\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener, but it still needs to be mowed\"");
		v.AddNeg("\"good things come to those who wait\" -> \"good things can also come to those who take action and create their own opportunities\"");
		v.AddNeg("\"better safe than sorry\" -> \"better risky than boring, but always be careful\"");
		v.AddNeg("\"if at first you don't succeed, try, try again\" -> \"if at first you don't succeed, maybe try a different approach or just move on\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"the grass is always greener where the sprinklers are broken and needs some TLC\"");
		v.AddNeg("\"the pen is mightier than the sword\" -> \"the pen can sometimes be mightier than your sword\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"out of sight, out of mind, until your phone buzzes\" ");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "contrasting altering of sayings";
		v.AddPos("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, make a lemon meringue pie\"");
		v.AddPos("\"it's always darkest before the dawn\" -> \"it's always brightest before the sunset\"");
		v.AddPos("\"what doesn't kill you makes you stronger\" -> \"what doesn't kill you makes you invincible\"");
		v.AddPos("\"follow your dreams\" -> \"create your own reality\"");
		v.AddPos("\"when one door closes, another one opens\" -> \"when one door closes, build your own door\"");
		v.AddPos("\"never give up\" -> \"always persevere\"");
		v.AddPos("\"it's the little things that count\" -> \"it's the big dreams that matter\"");
		v.AddPos("\"slow and steady wins the race\" -> \"fast and fearless wins the race\"");
		v.AddPos("\"the pen is mightier than the sword\" -> \"the pen is more powerful than the keyboard\"");
		v.AddPos("\"laughter is the best medicine\" -> \"laughter is the key to happiness\"");
		v.AddPos("\"love conquers all\" -> \"love is the strongest force in the world\"");
		v.AddPos("\"no pain, no gain\" -> \"no risk, no reward\"");
		v.AddPos("\"beauty is in the eye of the beholder\" -> \"beauty is everywhere, if seen through the right lens\"");
		v.AddPos("\"actions speak louder than words\" -> \"words can empower and inspire just as much as actions\"");
		v.AddPos("\"a picture is worth a thousand words\" -> \"a picture can capture a story that words cannot express\"");
		v.AddPos("\"it's not about the destination, it's about the journey\" -> \"it's not just about the journey, it's also about the destination\"");
		v.AddPos("\"great minds think alike\" -> \"great minds have different perspectives and ideas that complement each other\"");
		v.AddPos("\"birds of a feather flock together\" -> \"diversity fosters growth and innovation\" ");
		v.AddPos("\"blood is thicker than water\" -> \"family is not just defined by blood, but also by the bonds we create\"");
		v.AddPos("\"honesty is the best policy\" -> \"honesty is the foundation of trust and authenticity\" ");
		v.AddPos(" \"practice makes perfect\" -> \"strive for progress, not perfection\"");
		v.AddPos("\"never judge a book by its cover\" -> \"always give people a chance and look beyond appearances\"");
		v.AddPos("\"the grass is always greener on the other side\" -> \"we often overlook the blessings we have in front of us\"");
		v.AddPos("\"the more, the merrier\" -> \"quality over quantity\"");
		v.AddPos("\"don't put all your eggs in one basket\" -> \"sometimes taking risks pays off, so give it your all\"");
		v.AddPos("\"being different is a good thing\" -> \"embrace your uniqueness and use it to make a positive impact\"");
		v.AddPos("\"love is blind\" -> \"love sees past flaws and imperfections\"");
		v.AddPos("\"actions speak louder than words\" -> \"words can harm or lift others, choose yours wisely\"");
		v.AddPos("\"a picture is worth a thousand words\" -> \"never underestimate the power of visual storytelling and communication\"");
		v.AddPos("\"if at first you don't succeed, try try again\" -> \"if at first you don't succeed, learn from your mistakes and try a different approach\"");
		v.AddPos("\"beauty is only skin deep\" -> \"true beauty radiates from within\"");
		v.AddPos("\"what goes around comes around\" -> \"choose kindness and it will come back to you\"");
		v.AddPos("\"there's no such thing as a free lunch\" -> \"gratitude costs nothing and goes a long way\"");
		v.AddPos("\"a chain is only as strong as its weakest link\" -> \"individuals bring unique strengths to a team, making it stronger as a whole\"");
		v.AddPos(" \"don't count your chickens before they hatch\" -> \"have faith and believe in your dreams, even before they come true\"");
		v.AddPos("\"you can't teach an old dog new tricks\" -> \"people of all ages can learn and grow\" ");
		v.AddPos("\"silence is golden\" -> \"words have power, but so does silence\"");
		v.AddPos("\"a rolling stone gathers no moss\" -> \"constantly moving and adapting can lead to growth and new opportunities\"");
		v.AddPos("\"two heads are better than one\" -> \"collaboration and teamwork lead to stronger and more innovative ideas\"");
		v.AddPos("\"curiosity killed the cat\" -> \"curiosity leads to discovery and learning\"");
		v.AddPos("\"don't cry over spilled milk\" -> \"learn from mistakes and move on\"");
		v.AddPos("\"keep your friends close and your enemies closer\" -> \"show kindness and empathy towards all, even those who may not reciprocate\" ");
		v.AddPos("\"practice what you preach\" -> \"lead by example and embody your values\"");
		v.AddPos("\"when the going gets tough, the tough get going\" -> \"persevere through challenges and come out stronger on the other side\"");
		v.AddPos("\"if you can't beat them, join them\" -> \"collaborate and learn from others instead of competing against them\"");
		v.AddPos("\"it's always darkest before the dawn\" -> \"the end is just the beginning of something new and exciting\"");
		v.AddPos("\"give a man a fish, and you feed him for a day; teach a man to fish, and you feed him for a lifetime\" -> \"give a man a fish, you feed him for a day; teach a man to fish and he can feed a community\"");
		v.AddPos("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, make a refreshing and unique lemon sorbet\"");
		v.AddPos("\"the apple doesn't fall far from the tree\" -> \"prove that you are your own person and not defined by your family's legacy\"");
		v.AddPos("\"there's no such thing as a free lunch\" -> \"put in the hard work and effort for true success\"");
		v.AddPos("\"opposites attract\" -> \"differences bring diversity and balance in relationships\"");
		v.AddPos("\"birds of a feather flock together\" -> \"surround yourself with people who inspire and uplift you\"");
		v.AddPos("\"the truth will set you free\" -> \"live with integrity and honesty for inner peace and freedom\"");
		v.AddPos("\"actions speak louder than words\" -> \"actions and words together can create powerful change and impact\"");
		v.AddPos("\"a picture is worth a thousand words\" -> \"a picture can capture the beauty and complexity of life in a single frame\"");
		v.AddPos("\"beauty is in the eye of the beholder\" -> \"beauty is subjective and can be found in unexpected places\"");
		v.AddPos("\"the proof is in the pudding\" -> \"see for yourself instead of relying on others' opinions\"");
		v.AddPos("\"a little goes a long way\" -> \"small acts of kindness and generosity can have a big impact\"");
		v.AddPos("\"there's no place like home\" -> \"find comfort and belonging within yourself, not just a physical place\"");
		v.AddPos("\"no man is an island\" -> \"connection and community are essential for a fulfilling life\"");
		v.AddPos("\"good things come to those who wait\" -> \"patience and persistence lead to rewards and opportunities\"");
		v.AddPos("\"money can't buy happiness\" -> \"true happiness comes from within, not material possessions\"");
		v.AddPos("\"don't bite the hand that feeds you\" -> \"show gratitude and respect to those who support and care for you\"");
		v.AddPos("\"life is what you make it\" -> \"make the most of every situation and create your own happiness\" ");
		v.AddPos("\"actions speak louder than words\" -> \"words have the power to heal and bring people together\" ");
		v.AddPos("\"a picture is worth a thousand words\" -> \"a picture can capture a moment that words cannot truly express\"");
		v.AddPos("\"life is a journey\" -> \"life is a canvas where we create our own journeys\"");
		v.AddPos("\"what goes up must come down\" -> \"the sky's the limit, and there's always room to grow\"");
		v.AddPos("\"you can't please everyone\" -> \"be true to yourself and those who matter will appreciate you\"");
		v.AddPos("\"practice makes perfect\" -> \"practice makes progress and continuous improvement\"");
		v.AddPos("\"time heals all wounds\" -> \"learn and grow from past struggles, and they will shape you for the better\"");
		v.AddPos("\"the early bird gets the worm\" -> \"persistence and consistency lead to success\"");
		v.AddPos("\"laughter is the best medicine\" -> \"take time to find joy and laughter in everyday moments\"");
		v.AddPos("\"birds of a feather flock together\" -> \"find a community of like-minded individuals who support and encourage you\"");
		v.AddPos("\"an apple a day keeps the doctor away\" -> \"small healthy habits daily lead to a healthier life\"");
		v.AddPos("\"money doesn't grow on trees\" -> \"budget and save for the things that truly matter in life\"");
		v.AddPos("\"don't judge a book by its cover\" -> \"look beyond appearances and get to know the person inside\"");
		v.AddPos("\"the only constant in life is change\" -> \"embrace change and see it as an opportunity for growth and new experiences\"");
		v.AddPos("\"there's no such thing as a free lunch\" -> \"truly appreciate the things that come easily and freely\"");
		v.AddNeg("\"break a leg\" -> \"break someone's spirit\"");
		v.AddNeg("\"better late than never\" -> \"better never than late\"");
		v.AddNeg("\"life is a rollercoaster\" -> \"life is a never-ending cycle of disappointments\"");
		v.AddNeg("\"there's plenty of fish in the sea\" -> \"every fish is the same in the sea\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"you can't have it all\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird gets the worm, but the second mouse gets the cheese\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words can hurt more than actions\"");
		v.AddNeg("\"a picture is worth a thousand words\" -> \"a picture can be easily photoshopped and manipulated\"");
		v.AddNeg("\"think outside the box\" -> \"conform to society's expectations\"");
		v.AddNeg("\"spare the rod, spoil the child\" -> \"use harsh punishments to control children\" ");
		v.AddNeg("\"what doesn't kill you makes you stronger\" -> \"trauma and pain only leave lasting scars\"");
		v.AddNeg("\"live life to the fullest\" -> \"live life with no regard for consequences or others\"");
		v.AddNeg("\"honesty is the best policy\" -> \"lie to get what you want\"");
		v.AddNeg("\"keep your friends close and your enemies closer\" -> \"always have a hidden agenda\"");
		v.AddNeg("\"a leopard can't change its spots\" -> \"people can't change who they really are\"");
		v.AddNeg("\"love conquers all\" -> \"love is just a temporary feeling\"");
		v.AddNeg("\"curiosity killed the cat\" -> \"curiosity leads to dangerous decisions\"");
		v.AddNeg("\"when life gives you lemons, make lemonade\" -> \"when life gives you lemons, complain and do nothing about it.\" ");
		v.AddNeg("\"forgive and forget\" -> \"hold grudges and never let go\"");
		v.AddNeg("\"beauty is skin deep\" -> \"looks are all that matter\" ");
		v.AddNeg("\"blood is thicker than water\" -> \"family is always right, no matter what\"");
		v.AddNeg("\"an eye for an eye\" -> \"revenge is necessary\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"never be satisfied and always want what others have\"");
		v.AddNeg("\"love at first sight\" -> \"judge and decide on someone based on looks alone\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words can be easily ignored\"");
		v.AddNeg("\"no pain, no gain\" -> \"push yourself to the brink of physical and emotional exhaustion for success\"");
		v.AddNeg("\"what's done is done\" -> \"never take responsibility or hold yourself accountable for your actions.\"");
		v.AddNeg("\"don't put all your eggs in one basket\" -> \"never take risks or put all of your effort into one thing\"");
		v.AddNeg("\"a penny saved is a penny earned\" -> \"spend all of your money because you can't take it with you when you die.\"");
		v.AddNeg("\"laughter is the best medicine\" -> \"use humor to cover up your problems and emotions.\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"if you don't see it, it doesn't matter\"");
		v.AddNeg("\"all that glitters is not gold\" -> \"the superficial is more important than substance\" ");
		v.AddNeg("\"cheaters never prosper\" -> \"cheating is the only way to get ahead in life.\" ");
		v.AddNeg("\"great minds think alike\" -> \"be just like everyone else to be accepted and successful.\"");
		v.AddNeg("\"live and let live\" -> \"mind your own business and don't bother helping others.\"");
		v.AddNeg("\"go with the flow\" -> \"don't stand up for what you believe in and just conform to the majority.\"");
		v.AddNeg("\"absence makes the heart grow fonder\" -> \"out of sight, out of mind.\"");
		v.AddNeg("\"first impressions are everything\" -> \"judging others based on first meetings is more important than getting to know them.\"");
		v.AddNeg("\"one man's trash is another man's treasure\" -> \"only value something if others deem it valuable as well.\"");
		v.AddNeg("\"what doesn't kill you makes you stronger\" -> \"trauma and pain make you weak and damaged.\"");
		v.AddNeg("\"speak softly and carry a big stick\" -> \"use physical force to intimidate and get what you want.\" ");
		v.AddNeg("\"truth is stranger than fiction\" -> \"don't believe in anything that seems too amazing to be true.\"");
		v.AddNeg("\"the customer is always right\" -> \"the customer is always right, even if they are wrong.\"");
		v.AddNeg("\"never put off until tomorrow what you can do today\" -> \"never take a break or rest, always work hard until you burn out.\"");
		v.AddNeg("\"love is patient, love is kind\" -> \"love is obsessive and possessive.\"");
		v.AddNeg("\"beauty is in the eye of the beholder\" -> \"beauty is only skin deep.\"");
		v.AddNeg("\"what goes up must come down\" -> \"success is always followed by failure.\" ");
		v.AddNeg("\"slow and steady wins the race\" -> \"take shortcuts and cheat to win the race.\"");
		v.AddNeg("\"you're only as old as you feel\" -> \"age is everything and should be constantly feared and fought against.\"");
		v.AddNeg("\"don't count your chickens before they hatch\" -> \"never have hope or excitement for the future as it may never happen.\"");
		v.AddNeg("\"the apple doesn't fall far from the tree\" -> \"you are destined to be just like your parents, for better or worse.\"");
		v.AddNeg("\"birds of a feather flock together\" -> \"stay in an echo chamber and only associate with people who think and act like you.\"");
		v.AddNeg("\"better safe than sorry\" -> \"be overly cautious and never take risks, even if it means missing out on opportunities.\"");
		v.AddNeg("\"a stitch in time saves nine\" -> \"be stingy and selfish with your time and resources.\"");
		v.AddNeg("\"let sleeping dogs lie\" -> \"ignore problems and let them fester until they become unmanageable.\" ");
		v.AddNeg("\"I've been through the fire and come out stronger\" -> \"I've been through the fire and come out damaged beyond repair.\"");
		v.AddNeg("\"onward and upward\" -> \"never reflect and learn from past mistakes, just keep moving forward at all costs.\"");
		v.AddNeg("\"everything happens for a reason\" -> \"suffering is justified and there is no control over one's fate.\"");
		v.AddNeg("\"what's in the past is in the past\" -> \"never take responsibility for past actions and learn from them.\" ");
		v.AddNeg("\"you can't make an omelette without breaking a few eggs\" -> \"hurt and harm others to get what you want.\"");
		v.AddNeg("\"the best things in life are free\" -> \"the best things in life are material possessions and wealth.\"");
		v.AddNeg("\"too many cooks in the kitchen spoil the broth\" -> \"don't work with others and always take control to get the credit.\"");
		v.AddNeg("\"read between the lines\" -> \"manipulate and deceive others with hidden meanings.\"");
		v.AddNeg("\"you can't teach an old dog new tricks\" -> \"people are set in their ways and cannot change or grow.\"");
		v.AddNeg("\"beauty is pain\" -> \"suffering is necessary for beauty or success.\"");
		v.AddNeg("\"jack of all trades, master of none\" -> \"don't strive for diverse skills and experience, just become a master in one area.\"");
		v.AddNeg("\"it's better to beg for forgiveness than to ask for permission\" -> \"don't respect others' boundaries or consent, just do what you want and apologize later.\"");
		v.AddNeg("\"money can't buy happiness\" -> \"money is everything and can solve any problem.\"");
		v.AddNeg("\"live fast, die young\" -> \"life is short and there is no point in planning for the future.\"");
		v.AddNeg("\"don't shoot the messenger\" -> \"blame the messenger and refuse to accept criticism or negative news.\"");
		v.AddNeg("\"never trust a smiling cat\" -> \"don't trust anyone who appears friendly, they may have ulterior motives.\"");
		v.AddNeg("\"pride comes before a fall\" -> \"don't have confidence or pride in your accomplishments, it will only lead to failure.\" ");
		v.AddNeg("\"it's not what you know, it's who you know\" -> \"connections and networking are more important than skills or hard work.\"");
		v.AddNeg("\"out of the frying pan and into the fire\" -> \"often times, there is no escape from one bad situation to another.\"");
		v.AddNeg("\"live in the moment\" -> \"don't plan for the future or have any sense of responsibility.\"");
		v.AddNeg("\"practice makes perfect\" -> \"never settle for good enough, constantly strive for unattainable perfection.\"");
		v.AddNeg("\"better the devil you know than the devil you don't\" -> \"stay in harmful or toxic situations rather than taking a risk for change.\"");
		v.AddNeg("\"don't judge a book by its cover\" -> \"judge others solely based on their appearance and superficial qualities.\"");
		v.AddNeg("\"time heals all wounds\" -> \"pain and trauma never truly goes away or heals.\"");
		v.AddNeg("\"misery loves company\" -> \"don't try to lift others up, just bring them down to your level.\"");
		v.AddNeg("\"keep your enemies closer than your friends\" -> \"always look for any sign of betrayal from those closest to you.\"");
		v.AddNeg("\"innocent until proven guilty\" -> \"assume everyone is guilty before even considering the facts.\" ");
		v.AddNeg("\"never mix business with pleasure\" -> \"prioritize work and success over personal relationships or happiness.\"");
		v.AddNeg("\"kill two birds with one stone\" -> \"use and manipulate others to kill two birds with one stone.\"");
		v.AddNeg("\"you can't fight fire with fire\" -> \"violence and retaliation are the only solutions to conflict.\"");
		v.AddNeg("\"don't bite the hand that feeds you\" -> \"never speak out or challenge authority, just obey and be grateful.\"");
		v.AddNeg("\"what you don't know can't hurt you\" -> \"ignorance is bliss, don't seek out knowledge or truth.\"");
		v.AddNeg("\"no man is an island\" -> \"depend on others and never be self-sufficient.\"");
		v.AddNeg("\"where there's smoke, there's fire\" -> \"assume the worst and jump to conclusions.\"");
		v.AddNeg("\"the customer is always right\" -> \"the customer is always right, even if they are wrong.\"");
		v.AddNeg("\"everyone is entitled to their own opinion\" -> \"challenge and attack anyone who has a different opinion than you.\"");
		v.AddNeg("\"one man's trash is another man's treasure\" -> \"only value something if others deem it valuable as well.\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"don't aim for both success and happiness, choose one or the other.\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words are meaningless and actions are the only measure of worth.\"");
		v.AddNeg("\"you scratch my back, I'll scratch yours\" -> \"use others for personal gain and never do anything out of genuine kindness.\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird gets the worm, but the second mouse gets the cheese.\"");
		v.AddNeg("\"an apple a day keeps the doctor away\" -> \"don't take care of your health or seek medical help because it is expensive and pointless.\"");
		v.AddNeg("\"all good things must come to an end\" -> \"don't have hope or happiness, it will inevitably be taken away.\"");
		v.AddNeg("\"variety is the spice of life\" -> \"stick to what you know and never try anything new or different.\"");
		v.AddNeg("\"love is blind\" -> \"ignore red flags and toxic behavior in the name of love.\"");
		v.AddNeg("\"a penny saved is a penny earned\" -> \"spend all of your money because you can't take it with you when you die.\"");
		v.AddNeg("\"silence is golden\" -> \"never speak up and stay silent to avoid conflict or confrontation.\" ");
		v.AddNeg("\"a rolling stone gathers no moss\" -> \"never stay in one place or have roots, always be on the move and never get comfortable.\"");
		v.AddNeg("\"never say never\" -> \"give up on your dreams and never try again after facing initial failure.\"");
		v.AddNeg("\"what goes around comes around\" -> \"don't take responsibility for your actions, karma will handle it for you.\"");
		v.AddNeg("\"don't put all your eggs in one basket\" -> \"never take risks or put all of your effort into one thing.\"");
		v.AddNeg("\"it's a dog eat dog world\" -> \"only look out for yourself and your own interests, stepping on others if necessary.\"");
		v.AddNeg("\"birds of a feather flock together\" -> \"stay in an echo chamber and only associate with people who think and act like you.\"");
		v.AddNeg("\"there's no place like home\" -> \"be overly attached and dependent on your home, never seek out new experiences.\"");
		v.AddNeg("\"patience is a virtue\" -> \"never wait for anything and always act impulsively.\"");
		v.AddNeg("\"the more the merrier\" -> \"never value quality relationships and prioritize having a large quantity of friends.\"");
		v.AddNeg("\"a watched pot never boils\" -> \"focus on and obsess over things that will never happen.\"");
		v.AddNeg("\"you reap what you sow\" -> \"don't take responsibility for your actions, blame others for your problems.\"");
		v.AddNeg("\"fortune favors the bold\" -> \"always play it safe and never take risks, even if it potentially leads to success.\"");
		v.AddNeg("\"birds of a feather flock together\" -> \"stay in an echo chamber and only associate with people who think and act like you.\"");
		v.AddNeg("\"the squeaky wheel gets the grease\" -> \"complain loudly and often to get what you want, no matter who it affects.\"");
		v.AddNeg("\"good things come to those who wait\" -> \"never take action and wait for things to come to you.\"");
		v.AddNeg("\"never say never\" -> \"give up on your dreams and never try again after facing initial failure.\"");
		v.AddNeg("\"it's a man eat man world\" -> \"survival of the fittest, step on others to get what you want.\"");
		v.AddNeg("\"don't kill the goose that laid the golden egg\" -> \"never make sacrifices or give back to those who have helped you succeed.\"");
		v.AddNeg("\"practice makes perfect\" -> \"never settle for good enough, constantly strive for unattainable perfection.\"");
		v.AddNeg("\"absence makes the heart grow fonder\" -> \"out of sight, out of mind.\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words can be easily ignored.\"");
		v.AddNeg("\"good things come to those who wait\" -> \"never take action and wait for things to come to you.\"");
		v.AddNeg("\"only the strong survive\" -> \"step on others and never show weakness or vulnerability.\"");
		v.AddNeg("\"a picture is worth a thousand words\" -> \"a picture can be easily photoshopped and manipulated.\"");
		v.AddNeg("\"don't judge a book by its cover\" -> \"judge others solely based on their appearance and superficial qualities.\"");
		v.AddNeg("\"actions speak louder than words\" -> \"words can hurt more than actions.\"");
		v.AddNeg("\"beauty is only skin deep\" -> \"looks are all that matter.\"");
		v.AddNeg("\"money can't buy happiness\" -> \"money is everything and can solve any problem.\"");
		v.AddNeg("\"keep your enemies closer than your friends\" -> \"always look for any sign of betrayal from those closest to you.\"");
		v.AddNeg("\"love knows no bounds\" -> \"love is obsessive and all-consuming.\"");
		v.AddNeg("\"don't bite the hand that feeds you\" -> \"never speak out or challenge authority, just obey and be grateful.\"");
		v.AddNeg("\"what goes up must come down\" -> \"success is always followed by failure.\"");
		v.AddNeg("\"never mix business with pleasure\" -> \"prioritize work and success over personal relationships or happiness.\"");
		v.AddNeg("\"the pen is mightier than the sword\" -> \"words can be more destructive and powerful than physical force.\"");
		v.AddNeg("\"beauty is in the eye of the beholder\" -> \"beauty is only skin deep.\"");
		v.AddNeg("\"you can't have your cake and eat it too\" -> \"don't aim for both success and happiness, choose one or the other.\"");
		v.AddNeg("\"the early bird gets the worm\" -> \"the early bird gets the worm, but the second mouse gets the cheese.\"");
		v.AddNeg("\"an apple a day keeps the doctor away\" -> \"don't take care of your health or seek medical help because it is expensive and pointless.\"");
		v.AddNeg("\"good things come to those who wait\" -> \"never take action and wait for things to come to you.\"");
		v.AddNeg("\"misery loves company\" -> \"don't try to lift others up, just bring them down to your level.\"");
		v.AddNeg("\"variety is the spice of life\" -> \"stick to what you know and never try anything new or different.\"");
		v.AddNeg("\"one man's trash is another man's treasure\" -> \"only value something if others deem it valuable as well.\"");
		v.AddNeg("\"you scratch my back, I'll scratch yours\" -> \"use others for personal gain and never do anything out of genuine kindness.\"");
		v.AddNeg("\"don't bite the hand that feeds you\" -> \"never speak out or challenge authority, just obey and be grateful.\"");
		v.AddNeg("\"out of sight, out of mind\" -> \"if you don't see i, it doesn't matter.\"");
		v.AddNeg("\"better to have loved and lost than never loved at all\" -> \"never open yourself up to love and avoid potential heartache.\"");
		v.AddNeg("\"what's done is done\" -> \"never take responsibility or hold yourself accountable for your actions.\"");
		v.AddNeg("\"silence is golden\" -> \"never speak up and stay silent to avoid conflict or confrontation.\"");
		v.AddNeg("\"the customer is always right\" -> \"the customer is always right, even if they are wrong.\"");
		v.AddNeg("\"everything happens for a reason\" -> \"suffering is justified and there is no control over one's fate.\"");
		v.AddNeg("\"first impressions are everything\" -> \"judging others based on first meetings is more important than getting to know them.\"");
		v.AddNeg("\"don't put all your eggs in one basket\" -> \"never take risks or put all of your effort into one thing.\"");
		v.AddNeg("\"money talks\" -> \"money is the only way to get what you want.\"");
		v.AddNeg("\"green with envy\" -> \"jealousy is a natural and acceptable emotion.\"");
		v.AddNeg("\"practice makes perfect\" -> \"never settle for good enough, constantly strive for unattainable perfection.\"");
		v.AddNeg("\"you can't fight fire with fire\" -> \"violence and retaliation are the only solutions to conflict.\"");
		v.AddNeg("\"a stitch in time saves nine\" -> \"be stingy and selfish with your time and resources.\"");
		v.AddNeg("\"better to have loved and lost than never loved at all\" -> \"never open yourself up to love and avoid potential heartache.\"");
		v.AddNeg("\"the grass is always greener on the other side\" -> \"never be satisfied and always want what others have.\"");
		v.AddNeg("\"never judge a book by its cover\" -> \"judge others solely based on their appearance and superficial qualities.\"");
		v.AddNeg("\"laughter is the best medicine\" -> \"use humor to cover up your problems and emotions.\"");
		v.AddNeg("\"every cloud has a silver lining\" -> \"there is always a downside or negative aspect to every situation.\"");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Hip-hop: Powerful stories through clever lyrics";
		v.AddPos("Person overcoming adversity and rising above their struggles ");
		v.AddPos("Love conquering all obstacles and bringing people together ");
		v.AddPos("Inner strength and resilience in the face of challenges ");
		v.AddPos("Finding hope and redemption in the darkest moments ");
		v.AddPos("The power of friendship and support in difficult times ");
		v.AddPos("Breaking free from societal expectations and living life on your own terms ");
		v.AddPos("Overcoming fear and taking risks to achieve success ");
		v.AddPos("Being true to oneself and staying authentic in a world full of pressures ");
		v.AddPos("Finding strength and courage in vulnerability ");
		v.AddPos("Embracing change and learning from past mistakes ");
		v.AddPos("Rising from the ashes and starting anew ");
		v.AddPos("The journey of self-discovery and self-acceptance ");
		v.AddPos("The power of forgiveness and letting go of grudges ");
		v.AddPos("The resilience of the human spirit and its ability to overcome anything ");
		v.AddPos("Perseverance and determination in achieving one's dreams ");
		v.AddPos("The beauty and importance of diversity and embracing differences ");
		v.AddPos("Breaking free from toxic relationships and finding self-love ");
		v.AddPos("The transformative power of music and art ");
		v.AddPos("Finding strength in faith and spirituality ");
		v.AddPos("The power of small acts of kindness and how they can make a big impact ");
		v.AddPos("Seeing the beauty in imperfections and embracing flaws ");
		v.AddPos("The strength and bravery of standing up for what you believe in ");
		v.AddPos("Overcoming societal barriers and prejudices to achieve success ");
		v.AddPos("The transformative power of love and its ability to change lives. ");
		v.AddPos("Finding strength and resilience in one's heritage and cultural identity.");
		v.AddPos("The power of education and self-growth in achieving personal success ");
		v.AddPos("Embracing individuality and uniqueness instead of conforming to societal norms ");
		v.AddPos("The journey of overcoming addiction and finding inner peace ");
		v.AddPos("Unexpected acts of kindness and their ripple effect on others ");
		v.AddPos("Standing up against injustice and fighting for what is right");
		v.AddPos("The power of positive thinking and manifesting one's desires ");
		v.AddPos("The beauty of aging and the wisdom that comes with it ");
		v.AddPos("Finding strength in community and unity despite differences.");
		v.AddPos("The journey of self-discovery and learning to love oneself.");
		v.AddPos("Overcoming past traumas and finding healing and growth.");
		v.AddPos("The power of perseverance and hard work in achieving success.");
		v.AddPos("Inner peace and contentment being more valuable than external success.");
		v.AddPos("Defying gender stereotypes and breaking free from traditional gender roles.");
		v.AddPos("The importance of self-care and self-love for overall well-being.");
		v.AddPos("Overcoming self-doubt and believing in oneself.");
		v.AddPos("The transformative effects of traveling and experiencing different cultures.");
		v.AddPos("Overcoming mental illness and finding strength and purpose.");
		v.AddPos("The power of vulnerability and authenticity in relationships.");
		v.AddPos("Finding love and happiness within oneself before seeking it from others.");
		v.AddPos("Breaking free from materialistic desires and finding fulfillment in simpler things.");
		v.AddPos("The strength and resilience of the human heart in overcoming heartbreak.");
		v.AddPos("Having a good day despite facing obstacles and challenges");
		v.AddPos("Learning to let go of control and embracing the unknown.");
		v.AddPos("Overcoming discrimination and prejudice to achieve success and recognition.");
		v.AddPos("The journey of self-discovery and finding one's true passion in life.");
		v.AddPos("Empowering oneself and others through acts of kindness and benevolence.");
		v.AddPos("Conquering fears and taking risks for personal growth and success.");
		v.AddPos("The beauty and strength in vulnerability and showing emotions.");
		v.AddPos("The importance of family and its unwavering support in times of adversity.");
		v.AddPos("Breaking free from societal expectations and embracing unconventional paths in life.");
		v.AddPos("The journey of self-forgiveness and finding inner peace.");
		v.AddPos("Overcoming societal pressure to conform and staying true to oneself.");
		v.AddPos("Finding strength and resilience in the face of illness or disability.");
		v.AddPos("The transformative power of love and acceptance in healing broken relationships.");
		v.AddPos("Embracing one's own flaws and imperfections instead of striving for perfection.");
		v.AddPos("The power of determination and perseverance in achieving dreams.");
		v.AddPos("Overcoming past mistakes and learning from them to become a better person.");
		v.AddPos("Finding strength and healing in nature and the outdoors.");
		v.AddPos("The beauty of second chances and new beginnings.");
		v.AddPos("Rising from difficult circumstances and turning them into opportunities for growth.");
		v.AddPos("Embracing change and not being afraid to take risks.");
		v.AddPos("Overcoming negative thoughts and self-doubt to achieve success.");
		v.AddPos("The importance of self-discovery and finding oneself before seeking a romantic partner.");
		v.AddPos("Standing up for one's beliefs and not being afraid to go against the norm.");
		v.AddPos("Finding strength and resilience in one's cultural heritage and identity.");
		v.AddPos("The transformative power of forgiveness and letting go of grudges.");
		v.AddPos("Learning to love and appreciate oneself before seeking love from others.");
		v.AddPos("Overcoming societal barriers and stereotypes to achieve personal and professional success.");
		v.AddPos("The journey of self-discovery and learning to love oneself despite imperfections.");
		v.AddPos("The power and importance of unity and solidarity in times of crisis.");
		v.AddPos("Finding happiness and fulfillment in simplicity and living in the present moment.");
		v.AddPos("Overcoming addiction and finding strength and purpose in recovery.");
		v.AddPos("The transformative effect of travel and experiencing different cultures and perspectives.");
		v.AddPos("Standing up against social and environmental injustices and fighting for change.");
		v.AddPos("The power of communication and open-mindedness in building strong relationships.");
		v.AddPos("Breaking out of one's comfort zone and embracing new challenges and opportunities.");
		v.AddPos("The journey of self-discovery and finding inner peace and contentment.");
		v.AddPos("Having a positive mindset and believing in oneself despite setbacks and failures.");
		v.AddPos("Overcoming past traumas and using them as fuel for personal growth and success.");
		v.AddPos("The strength and resilience of the human spirit in times of adversity.");
		v.AddPos("Embracing diversity and inclusion instead of conforming to societal norms.");
		v.AddPos("Finding beauty and strength in the process of healing and growth.");
		v.AddPos("The importance of self-care and self-love in maintaining overall well-being.");
		v.AddPos("Overcoming fear and taking a leap of faith to chase one's dreams.");
		v.AddPos("The transformative power of music and art in bringing people together and sparking change.");
		v.AddPos("The journey of self-discovery and finding one's true purpose and passions in life.");
		v.AddPos("Rising above negative influences and staying true to oneself.");
		v.AddPos("Rebuilding broken relationships and finding forgiveness and redemption.");
		v.AddPos("The power of determination and perseverance in overcoming obstacles and achieving success.");
		v.AddPos("Finding strength and resilience in one's own voice and speaking out against injustice.");
		v.AddPos("The transformative effects of travel and exploring new cultures and perspectives.");
		v.AddPos("Taking control of one's own happiness and not depending on others for fulfillment.");
		v.AddPos("The importance of authenticity and being true to oneself in all aspects of life.");
		v.AddPos("Overcoming self-doubt and finding the strength to pursue one's passions and dreams.");
		v.AddPos("The journey of self-discovery and learning to love oneself unconditionally.");
		v.AddPos("Defying societal norms and expectations to live life on one's own terms.");
		v.AddPos("Finding strength and empowerment in embracing all aspects of one's identity.");
		v.AddPos("The power of taking chances and stepping outside of one's comfort zone.");
		v.AddPos("Overcoming adversity and turning it into inspiration and motivation.");
		v.AddPos("Finding beauty and strength in the midst of chaos and difficult circumstances.");
		v.AddPos("The importance of perseverance and never giving up on one's dreams and goals.");
		v.AddPos("Overcoming past traumas and using them to help others going through similar struggles.");
		v.AddPos("The journey of self-discovery and finding inner peace and contentment after facing hardship.");
		v.AddPos("The power of community and coming together to support and uplift each other.");
		v.AddPos("Breaking free from societal expectations and stereotypes to embrace one's own individuality.");
		v.AddPos("Finding strength and resilience in one's own mind and overcoming mental health challenges.");
		v.AddPos("The transformative effects of love and understanding in healing broken relationships.");
		v.AddPos("Embracing aging and the wisdom and experiences that come with it.");
		v.AddPos("Overcoming discrimination and prejudice to achieve success and recognition.");
		v.AddPos("The journey of self-forgiveness and finding inner peace and closure.");
		v.AddPos("The power of self-discovery and learning to love and accept oneself fully.");
		v.AddPos("Rising above societal limitations and breaking barriers to achieve personal success.");
		v.AddPos("The strength and resilience of the human spirit in the face of illness or disability.");
		v.AddPos("The transformative power of forgiveness and letting go of grudges to find inner peace.");
		v.AddPos("Finding strength and courage in vulnerability and accepting one's own emotions.");
		v.AddPos("Overcoming negative thoughts and self-doubt to reach one's potential.");
		v.AddPos("The importance of self-care and taking time for oneself in a busy world.");
		v.AddPos("Rising above societal pressures and expectations to live life on one's own terms.");
		v.AddPos("Finding strength and resilience in one's cultural heritage and identity.");
		v.AddPos("The journey of self-acceptance and learning to love oneself unconditionally.");
		v.AddPos("Breaking free from toxic relationships and finding self-love and empowerment.");
		v.AddPos("The transformative power of music and art in expressing emotions and healing.");
		v.AddPos("The power of forgiveness and the impact it can have on both the forgiver and the forgiven.");
		v.AddPos("Finding strength and resilience in the face of adversity and challenges.");
		v.AddPos("Overcoming fears and taking risks in pursuit of personal growth and success.");
		v.AddPos("Embracing change and finding adaptation and growth in the face of uncertainty.");
		v.AddPos("The beauty and importance of diversity and inclusivity in society.");
		v.AddPos("Breaking free from societal expectations and stereotypes to embrace one's true identity.");
		v.AddPos("The transformative effects of travel and experiencing different cultures and perspectives.");
		v.AddPos("Finding peace and balance in life through self-discovery and self-care.");
		v.AddPos("Overcoming childhood traumas and using them to make a positive impact on others' lives.");
		v.AddPos("The journey of self-love and acceptance, and finding inner peace and happiness.");
		v.AddPos("The power of determination and perseverance in reaching one's goals and achieving success.");
		v.AddPos("Rising above toxic influences and staying true to oneself and one's values.");
		v.AddPos("The importance of vulnerability and open-mindedness in building strong and meaningful relationships.");
		v.AddPos("Finding strength and resilience in family and loved ones during difficult times.");
		v.AddPos("Breaking out of one's comfort zone and embracing new challenges and adventures.");
		v.AddPos("The journey of self-discovery and finding one's true passions and purpose in life.");
		v.AddPos("Overcoming past mistakes and using them as lessons for personal growth and self-improvement.");
		v.AddPos("The strength and resilience of the human spirit in the face of tragedy and loss.");
		v.AddPos("The power of standing up for one's beliefs and making a positive impact on society.");
		v.AddPos("Embracing diversity and breaking down barriers and prejudices in our world.");
		v.AddPos("Finding beauty and strength in the process of healing and personal growth.");
		v.AddPos("The transformative effects of self-confidence and believing in oneself to achieve success.");
		v.AddPos("Overcoming fear and taking risks for personal growth and achieving one's dreams.");
		v.AddPos("Embracing change and finding strength and resilience in the face of challenges and uncertainties.");
		v.AddPos("The power of community and coming together to support and uplift each other.");
		v.AddPos("Finding happiness and fulfillment in simple moments and experiences in life.");
		v.AddPos("The journey of self-empowerment and gaining control of one's own happiness.");
		v.AddPos("Overcoming internal conflicts and finding inner peace and harmony within oneself.");
		v.AddPos("Breaking free from societal expectations and finding fulfillment in unconventional paths.");
		v.AddPos("The importance of self-care and self-love in maintaining a healthy and fulfilling life.");
		v.AddPos("The transformative power of forgiveness and finding closure and moving forward.");
		v.AddPos("Learning to let go of control and embracing the journey of life with all its ups and downs.");
		v.AddPos("The power of self-discovery and finding one's true identity and purpose in life.");
		v.AddPos("Overcoming discrimination and prejudice to achieve personal and professional success.");
		v.AddPos("Finding strength and courage in embracing one's true self and standing out from the crowd.");
		v.AddPos("The transformative effects of love and acceptance in healing broken relationships.");
		v.AddPos("The journey of self-improvement and constantly striving to become the best version of oneself.");
		v.AddPos("Rising above fear and societal pressures to pursue one's passions and dreams.");
		v.AddPos("Finding strength and resilience in uncertainty and the unknown.");
		v.AddPos("The power of determination and never giving up on one's dreams and goals.");
		v.AddPos("Overcoming past traumas and using them as motivation to create positive change in the world.");
		v.AddPos("The journey of self-forgiveness and learning to love oneself unconditionally");
		v.AddNeg("The damsel in distress being saved by a male hero ");
		v.AddNeg("Love being a cure-all for all problems and conflicts ");
		v.AddNeg("Men being the sole protectors and providers ");
		v.AddNeg("The \"tragic hero\" sacrificing themselves for the greater good ");
		v.AddNeg("The idea that strength and success come from being ruthless and cutthroat ");
		v.AddNeg("The belief that happiness and self-worth are tied to external achievements and validation ");
		v.AddNeg("The glorification of toxic relationships and \"bad boy/girl\" tropes ");
		v.AddNeg("The portrayal of marginalized communities as helpless victims needing to be saved ");
		v.AddNeg("The harmful concept that mental illness makes someone weak or broken ");
		v.AddNeg("The idea that conformity and traditional gender roles are necessary for a successful relationship ");
		v.AddNeg("The overdone narrative of a misunderstood and brooding artist or musician ");
		v.AddNeg("The belief that one must \"win\" and be the best at all costs ");
		v.AddNeg("The objectification of women's bodies for male pleasure ");
		v.AddNeg("The romanticization of unhealthy and obsessive love ");
		v.AddNeg("Toxic masculinity and the idea that men should suppress their emotions ");
		v.AddNeg("The idea that certain types of love (such as same-sex or interracial love) are lesser or taboo ");
		v.AddNeg("The shallow idea of love at first sight and instant soulmates ");
		v.AddNeg("The harmful belief that beauty and success equate to worth and value ");
		v.AddNeg("The glorification of violence and revenge as means of solving problems ");
		v.AddNeg("The idea that one must conform to societal standards in order to be accepted and successful.");
		v.AddNeg("The perpetuation of the \"strong Black woman\" stereotype, overlooking vulnerability and struggles");
		v.AddNeg("The glorification of the \"perfect\" nuclear family and traditional gender roles.");
		v.AddNeg("The overused narrative of the \"tortured artist\" romanticizing mental illness");
		v.AddNeg("The idea that achieving success and happiness requires sacrificing personal relationships and values.");
		v.AddNeg("The harmful image of the \"bad girl\" as empowered and desirable.");
		v.AddNeg("The glorification of drug and alcohol use as a means of coping or achieving success");
		v.AddNeg("The portrayal of people with disabilities as objects of pity or inspiration");
		v.AddNeg("The harmful belief that one must fit a certain mold or stereotype to be considered attractive or successful.");
		v.AddNeg("The perpetuation of harmful beauty standards and the objectification of women's bodies.");
		v.AddNeg("The dehumanization and fetishization of people of different races and cultures.");
		v.AddNeg("The harmful stereotype that women are catty and competitive with one another instead of supportive.");
		v.AddNeg("The portrayal of women as damsels in distress in need of a man's protection and validation.");
		v.AddNeg("The glorification of violence and toxic masculinity in the portrayal of male protagonists.");
		v.AddNeg("The perpetuation of harmful gender stereotypes in heterosexual relationships.");
		v.AddNeg("The harmful depiction of love as a \"game\" to be won, instead of a genuine connection between two individuals.");
		v.AddNeg("The glorification of sexual assault and coercion in music.");
		v.AddNeg("The portrayal of homosexuality as a taboo or dangerous concept.");
		v.AddNeg("The dehumanization of individuals who do not fit societal norms or expectations.");
		v.AddNeg("The perpetuation of the idea that one's worth is determined by their physical appearance or external achievements.");
		v.AddNeg("The romanticization of codependent and unhealthy relationships.");
		v.AddNeg("The harmful belief that mental illness and vulnerability are signs of weakness.");
		v.AddNeg("The objectification and sexualization of children and teenagers in music.");
		v.AddNeg("The portrayal of marginalized groups as stereotypes instead of fully fleshed out characters with their own unique experiences and identities.");
		v.AddNeg("The idea that emotions and vulnerability are solely \"feminine\" traits, discouraging men from expressing them.");
		v.AddNeg("The glorification of destructive and toxic behaviors in the name of artistic expression.");
		v.AddNeg("The harmful belief that individuals who do not conform to traditional gender roles are \"unnatural\" or inferior.");
		v.AddNeg("The representation of women as prizes or conquests to be won by men.");
		v.AddNeg("The perpetuation of the idea that success and happiness can only be achieved through physical beauty and wealth.");
		v.AddNeg("The harmful narrative that certain people are \"destined\" for greatness while others are doomed to fail.");
		v.AddNeg("The glorification of violence and revenge in relationships.");
		v.AddNeg("The dehumanization of individuals with mental illness as \"crazy\" or \"insane.\"");
		v.AddNeg("The portrayal of love as a competition or race to the finish line, instead of a shared journey.");
		v.AddNeg("The perpetuation of harmful stereotypes about people's race, culture, or nationality.");
		v.AddNeg("The objectification of individuals with disabilities and the portrayal of them as inferior or undesirable.");
		v.AddNeg("The idolization of toxic and destructive relationships in the media.");
		v.AddNeg("The harmful idea that only certain sexualities or gender identities are valid or accepted.");
		v.AddNeg("The glorification of infidelity and cheating as a symbol of power and prowess.");
		v.AddNeg("The perpetuation of binary gender roles and heteronormative relationships in music.");
		v.AddNeg("The objectification and sexualization of women's bodies for the male gaze.");
		v.AddNeg("The harmful belief that one's worth is tied to their ability to conform to societal standards.");
		v.AddNeg("The portrayal of marginalized communities as caricatures, perpetuating harmful stereotypes and dehumanizing them.");
		v.AddNeg("The idea that true love is all-consuming and requires sacrificing one's own identity and goals.");
		v.AddNeg("The glorification of manipulative and emotionally abusive relationships.");
		v.AddNeg("The perpetuation of the \"love fixes everything\" trope, dismissing and minimizing serious issues and conflicts in relationships.");
		v.AddNeg("The narrow definition of success and what it means to be \"powerful\" or \"strong.\"");
		v.AddNeg("The harmful representation of women as objects to be won or conquered by men.");
		v.AddNeg("The perpetuation of the idea that success and happiness are only attainable for a certain type of person or individual.");
		v.AddNeg("The harmful portrayal of mental illness as a punchline or joke.");
		v.AddNeg("The glorification of violence and aggression as a means of asserting dominance and power.");
		v.AddNeg("The representation of certain races or cultures as the \"other\" or inferior.");
		v.AddNeg("The perpetuation of toxic gender dynamics and expectations in relationships.");
		v.AddNeg("The harmful narrative that one must change or conform to be accepted and loved.");
		v.AddNeg("The glorification of unhealthy and unsustainable workaholic lifestyles.");
		v.AddNeg("The dehumanization of individuals who are not considered \"attractive\" or desirable according to societal standards.");
		v.AddNeg("The portrayal of love as a battle or competition between two individuals.");
		v.AddNeg("The perpetuation of damaging stereotypes about individuals with mental illness, leading to stigma and discrimination.");
		v.AddNeg("The harmful depiction of love as possessive and controlling, disregarding the importance of boundaries and consent.");
		v.AddNeg("The glorification of greed and materialism as symbols of success and power.");
		v.AddNeg("The perpetuation of harmful stereotypes regarding people's sexualities or sexual experiences.");
		v.AddNeg("The harmful portrayal of individuals with disabilities as helpless or dependent on others for validation and happiness.");
		v.AddNeg("The idea that success and happiness are solely dependent on external factors and achievements, disregarding individual fulfillment and internal growth.");
		v.AddNeg("The glorification of violence and aggression as a sign of strength and superiority.");
		v.AddNeg("The representation of marginalized communities as stereotypes instead of diverse and complex individuals.");
		v.AddNeg("The romanticization of unhealthy and obsessive love as the ultimate goal.");
		v.AddNeg("The perpetuation of toxic masculinity in the definition of \"strength\" and \"masculinity.\"");
		v.AddNeg("The harmful portrayal of sexual assault and coercion as a normal or acceptable part of relationships.");
		v.AddNeg("The objectification and sexualization of women's bodies in music as a means of selling or promoting the artist's image.");
		v.AddNeg("The perpetuation of harmful gender stereotypes and expectations in romantic relationships.");
		v.AddNeg("The glorification of wealth and superficial displays of success as symbols of status and power.");
		v.AddNeg("The harmful representation of LGBTQ+ people as stereotypes or caricatures instead of diverse and complex individuals.");
		v.AddNeg("The idea that one must give up parts of their identity or beliefs to be accepted and loved.");
		v.AddNeg("The romanticization of self-destructive and unhealthy behavior in the name of art or creativity.");
		v.AddNeg("The portrayal of marginalized groups as lacking agency and needing to be \"saved\" by others.");
		v.AddNeg("The perpetuation of the \"strong Black woman\" stereotype, dismissing vulnerability and struggles.");
		v.AddNeg("The glorification and romanticization of virginity and purity as markers of worth and morality.");
		v.AddNeg("The harmful depiction of mental illness as a romantic or desirable trait.");
		v.AddNeg("The portrayal of love as a \"game\" to be played and manipulated, instead of a genuine and equal connection between two individuals.");
		v.AddNeg("The perpetuation of damaging stereotypes about people based on their race, culture, or nationality.");
		v.AddNeg("The objectification and sexualization of individuals in music, promoting a male gaze and disregarding consent and respect.");
		v.AddNeg("The harmful representation of women as shallow and materialistic, valuing their worth based on external factors and achievements.");
		v.AddNeg("The glorification of violence and aggression as a means of dealing with conflicts and asserting dominance.");
		v.AddNeg("The perpetuation of the idea that marginalized groups must conform to societal norms in order to achieve acceptance and success.");
		v.AddNeg("The representation of individuals with mental illness as \"crazy\" or unstable, perpetuating harmful stigmas and stereotypes.");
		v.AddNeg("The romanticization and fetishization of homosexuality and other marginalized sexualities, leading to fetishization and dehumanization.");
		v.AddNeg("The portrayal of love as a \"test\" or challenge to be completed, instead of a shared journey between two equal partners.");
		v.AddNeg("The perpetuation of harmful stereotypes about individuals with disabilities, leading to ableism and discrimination.");
		v.AddNeg("The glorification of cheating and infidelity as a symbol of power and \"living in the moment.\" ");
		v.AddNeg("The harmful depiction of women as objects to be won or dominated by men.");
		v.AddNeg("The glorification of confrontational and aggressive behavior as a means of asserting dominance and \"strength.\" ");
		v.AddNeg("The perpetuation of traditional gender roles and expectations in relationships and society.");
		v.AddNeg("The romanticization of sacrificing one's own well-being and boundaries for the sake of love.");
		v.AddNeg("The representation of marginalized communities as stereotypes instead of fully-fleshed out characters with their own identities and experiences.");
		v.AddNeg("The glorification of unhealthy and unstable relationships as \"passionate\" and desirable.");
		v.AddNeg("The idea that love means always putting the other person's needs and desires above your own.");
		v.AddNeg("The perpetuation of the belief that one must change or conform to fit into societal norms in order to be successful and accepted.");
		v.AddNeg("The harmful representation of mental illness as a \"quirky\" or \"cute\" trait, minimizing and trivializing serious struggles.");
		v.AddNeg("The romanticization of toxic and manipulative behaviors in relationships.");
		v.AddNeg("The glorification of violence and aggression as a means of asserting power and dominance over others.");
		v.AddNeg("The portrayal of certain races or cultures as inferior or \"exotic\" in a harmful and fetishizing way.");
		v.AddNeg("The perpetuation of harmful stereotypes regarding people's sexualities and sexual experiences.");
		v.AddNeg("The objectification and sexualization of women's bodies in music as a means of promoting a certain image or brand.");
		v.AddNeg("The romanticization of financial success and material wealth as the ultimate markers of power and status.");
		v.AddNeg("The representation of LGBTQ+ people as \"straight-passing\" or fitting into societal norms, disregarding and erasing their unique identities.");
		v.AddNeg("The harmful portrayal of individuals with disabilities as objects of pity instead of multi-faceted and capable individuals.");
		v.AddNeg("The idea that one must meet certain beauty standards and conform to societal norms in order to be considered \"worthy\" or \"desirable.\"");
		v.AddNeg("The romanticization of unhealthy and toxic relationships, portraying possessiveness and control as signs of true love.");
		v.AddNeg("The perpetuation of harmful gender stereotypes and expectations in same-sex relationships.");
		v.AddNeg("The glorification of superficial displays of success and power, disregarding the importance of character and inner growth.");
		v.AddNeg("The portrayal of marginalized communities as stereotypes instead of fully fleshed out characters with their own experiences and identities.");
		v.AddNeg("The romanticization of sacrificing one's own mental and emotional well-being for the sake of love and/or success.");
		v.AddNeg("The perpetuation of damaging stereotypes about people's race, culture, or nationality.");
		v.AddNeg("The idea that love is a competition or a game, instead of a shared journey between two equals.");
		v.AddNeg("The glorification of violence and aggression in relationships as a means of asserting control and dominance.");
		v.AddNeg("The harmful representation of lesbian relationships as sexualized and objectifying for the male gaze.");
		v.AddNeg("The perpetuation of traditional gender roles and expectations in relationships and society.");
		v.AddNeg("The romanticization of unhealthy and destructive behaviors in the name of \"love.\"");
		v.AddNeg("The portrayal of marginalized communities as stereotypes instead of diverse and complex individuals.");
		v.AddNeg("The glorification of cheating and infidelity as a symbol of power and rebellion.");
		v.AddNeg("The representation of individuals with mental illness as \"broken\" and in need of fixing, leading to stigma and discrimination.");
		v.AddNeg("The harmful portrayal of love as possession and control over the other person.");
		v.AddNeg("The perpetuation of toxic masculinity in the glorification of aggression and violence in relationships.");
		v.AddNeg("The romanticization of dangerous and harmful behaviors in relationships as a sign of \"true love.\"");
		v.AddNeg("The idea that love requires sacrificing individual autonomy and identity for the other person.");
		v.AddNeg("The portrayal of marginalized communities as one-dimensional stereotypes, perpetuating harmful stereotypes and prejudices.");
		v.AddNeg("The glorification of superficial success and external achievements as markers of power and worth.");
		v.AddNeg("The perpetuation of harmful stereotypes about individuals with disabilities, leading to ableism and discrimination.");
		v.AddNeg("The romanticization of toxic and manipulative behaviors in relationships as signs of passion and intense love.");
		v.AddNeg("The idea that one must fit into traditional gender roles in order to be accepted and loved.");
		v.AddNeg("The portrayal of love as a conquest or \"challenge\" to be won instead of a genuine and equal connection between two individuals.");
		v.AddNeg("The glorification of toxic and destructive relationships in the media.");
		v.AddNeg("The romanticization of sacrificing one's own well-being and happiness for the sake of love and/or success.");
		v.AddNeg("The perpetuation of narrow and harmful beauty standards and the idea that one's worth is tied to their physical appearance");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "EDM: Music as a global phenomenon that brings people of all ages and backgrounds together to celebrate";
		v.AddPos("Music transcending language barriers and uniting people from different cultures");
		v.AddPos("Music being a universal language that everyone can understand and enjoy");
		v.AddPos("Music breaking down societal barriers and bringing people of different socioeconomic backgrounds together");
		v.AddPos("Music being a unifying force in times of hardship and bringing people together for a greater cause");
		v.AddPos("Music festivals as a celebration of diversity and inclusivity, with people of all ages and backgrounds coming together to enjoy various genres of music");
		v.AddPos("Music as a form of protest and resistance, bringing people from all walks of life together to fight for a common cause");
		v.AddPos("Music being a source of joy and happiness, connecting people and spreading positive vibes");
		v.AddPos("Music creating a sense of community and belonging, where everyone is welcome to express themselves and bond with others");
		v.AddPos("Music as a bridge between generations, with older and younger individuals coming together to appreciate and share their love for music");
		v.AddPos("Music being a powerful tool for healing and bringing people together during times of tragedy or loss");
		v.AddPos("Music as a form of cultural exchange, where people from different backgrounds can learn from each other through the sharing of musical traditions and styles");
		v.AddPos("Music festivals promoting peace and unity, with diverse crowds gathering to celebrate and connect through a shared love for music");
		v.AddPos("Music as a form of self-expression that unites individuals of all ages and backgrounds, allowing them to find a sense of identity and belonging in a larger community");
		v.AddPos("Music as a universal passion that brings people together, regardless of their differences, to simply enjoy the magic of sound and rhythm. ");
		v.AddPos("Music transcending time and connecting people throughout history, with each generation passing down the love for music to the next.");
		v.AddPos("Music being a unifying force in the face of hate and discrimination, promoting tolerance and understanding among diverse groups of people. ");
		v.AddPos("Music festivals as a space for people to come together and celebrate their individuality and diversity, while still feeling a sense of unity and togetherness.");
		v.AddPos("Music as a form of communication that allows people to connect and understand each other, even if they may not speak the same language or come from the same background.");
		v.AddPos("Music promoting global awareness and empathy, as people from different parts of the world come together and share their unique musical experiences and perspectives.");
		v.AddPos("Music as a source of inspiration and empowerment, uniting individuals to fight for social justice and bring about positive change in the world.");
		v.AddPos("Music festivals as a space for people to put aside their differences and come together to celebrate life, love, and the power of music.");
		v.AddPos("Music being a reminder of our shared humanity, bringing people of all ages and backgrounds together to dance, sing, and feel alive.");
		v.AddPos("Music as a means of fostering friendships and connections, as strangers bond over a shared favorite band or song. ");
		v.AddPos("Music festivals as a representation of the diversity and richness of our world, with people from different regions and cultures coming together in harmony to celebrate and appreciate all forms of music.");
		v.AddPos("Music connecting generations and promoting intergenerational bonding, as younger and older individuals come together to jam and share their musical passions.");
		v.AddPos("Music as a source of comfort and solace, bringing people together in times of loneliness or sadness and reminding us that we are not alone in our struggles.");
		v.AddPos("Music being a celebration of individuality and diversity, as each person brings their unique energy and experiences to create a collective sense of unity and joy.");
		v.AddPos("Music festivals promoting a sense of adventure and exploration, as people from different backgrounds come together to discover new artists and genres and broaden their musical horizons.");
		v.AddPos("Music bridging the gap between cultures and promoting understanding and respect, as people come together to appreciate and learn from each other's musical traditions and heritage.");
		v.AddPos("Music as a force for positive change, bringing people of all ages and backgrounds together to spread love, kindness, and unity through the power of song.");
		v.AddPos("Music being a shared experience that brings people of all ages and backgrounds closer together, creating unforgettable memories and lasting connections.");
		v.AddPos("Music festivals as a space for people to let go of their inhibitions and fully immerse themselves in the transformative power of music, connecting with others on a deeper level.");
		v.AddPos("Music as a celebration of life and a reminder of the beauty and joy that exists in the world, bringing people together to bask in the magic of music.");
		v.AddPos("Music being a source of inspiration and motivation, bringing people together to work towards a common goal and overcome challenges through the power of musical unity.");
		v.AddPos("Music festivals as a representation of the unity and diversity of humanity, with people from varying backgrounds coming together to celebrate, coexist, and create a better world through music.");
		v.AddPos("Music promoting cultural exchange and understanding, as people from different parts of the world come together to share and celebrate their unique musical cultures and identities.");
		v.AddPos("Music as a universal language that breaks down barriers of gender, race, and age, connecting people from all corners of the world through a shared love for melody and rhythm.");
		v.AddPos("Music festivals as a manifestation of peace and harmony, with people from all walks of life gathering to celebrate and coexist in perfect musical synchrony.");
		v.AddPos("Music as a symbol of hope and unity, bringing people from diverse backgrounds together to spread positivity and create change for a better future.");
		v.AddPos("Music transcending societal norms and expectations, bringing people of different ages and backgrounds together to express themselves freely and celebrate their individuality and uniqueness.");
		v.AddPos("Music festivals as a reflection of the human experience, with people coming together to celebrate and connect through the shared emotions and experiences expressed through music.");
		v.AddPos("Music promoting global understanding and empathy, as people from different cultures and backgrounds find common ground and a shared sense of humanity through the power of song.");
		v.AddPos("Music as a unifying force that brings together strangers and turns them into friends, through the magic and universality of its appeal.");
		v.AddPos("Music being a catalyst for change and social progress, bringing people from all ages and backgrounds together to inspire action and bring about a more inclusive and just world.");
		v.AddPos("Music festivals as a celebration of diversity and inclusivity, with people of all ages, races, nationalities, and genders coming together to enjoy a shared love for music and culture.");
		v.AddPos("Music as a means of breaking down barriers and promoting understanding and acceptance between people of different ages and backgrounds, fostering a sense of unity and togetherness.");
		v.AddPos("Music as a form of escapism and liberation, bringing people from all ages and backgrounds together to leave behind their daily struggles and immerse themselves in the joy and energy of music.");
		v.AddPos("Music festivals as a celebration of life and diversity, where people of all ages and backgrounds gather to appreciate and celebrate the unique and beautiful differences that make us human.");
		v.AddPos("Music being a form of self-expression that unites people of all ages and backgrounds, allowing them to connect and relate to one another on a deeper level through shared emotional experiences.");
		v.AddPos("Music as a source of strength and resilience, bringing people from different walks of life together to overcome adversity and find solace and courage in the power of music.");
		v.AddPos("Music festivals as a representation of the interconnectedness of the world, with people from different countries and cultures coming together to celebrate and honor the global impact of music.");
		v.AddPos("Music as a celebration of diversity and inclusivity, with people from all ages and backgrounds coming together to appreciate and celebrate the unique and diverse sounds of the world.");
		v.AddPos("Music being a reminder of our shared humanity and interconnectedness, bringing people together to celebrate and cherish the beautiful complexity of the human experience through the power of music. ");
		v.AddPos("Music festivals as a space for personal growth and transformation, with people from all ages and backgrounds gathering to learn, connect, and evolve through the power of music.");
		v.AddPos("Music as a tool for social change and progress, bringing people from diverse backgrounds together to raise awareness and drive positive actions towards a more just and equal society.");
		v.AddPos("Music being a universal source of joy, love, and connection, bringing together people from all ages and backgrounds to experience the magic and healing power of music.");
		v.AddPos("Music as a celebration of individuality and diversity, with people of different ages and backgrounds coming together to express themselves and embrace their unique identities through the power of music.");
		v.AddPos("Music festivals as a space for cultural education and exchange, allowing people of different ages and backgrounds to learn from and appreciate the rich diversity of musical traditions from around the world.");
		v.AddPos("Music transcending time and generations, with people of all ages and backgrounds coming together to celebrate and appreciate the timeless beauty and impact of music throughout history.");
		v.AddPos("Music as a form of activism, bringing individuals of all ages and backgrounds together to fight for a common cause and create a more just and equitable world through the unifying power of music.");
		v.AddPos("Music festivals as a celebration of diversity and inclusivity, with people from different generations and backgrounds coming together to embrace and appreciate the different genres, styles, and cultures represented in music.");
		v.AddPos("Music as a means of fostering connections and breaking down walls, bringing people from different walks of life together to share their unique musical tastes and experiences and find common ground through the magic of song.");
		v.AddPos("Music being a universal experience, connecting people of all ages and backgrounds to the raw, unfiltered emotions and human experiences expressed through the universal language of melody and rhythm.");
		v.AddPos("Music festivals as spaces for celebration, love, and freedom, where people from all ages and backgrounds come together to let go of their inhibitions and express themselves freely through the power of music.");
		v.AddPos("Music as a force of unity and hope, bringing people together from diverse backgrounds to spread positive energy and push towards a more equitable, peaceful, and compassionate world.");
		v.AddPos("Music festivals as a representation of the global impact and significance of music, with people from all corners of the world coming together to celebrate and appreciate the universal power and message of song.");
		v.AddPos("Music being a catalyst for cultural exchange and understanding, bringing people from different ages and backgrounds together to learn, share, and connect through the universal language of music.");
		v.AddPos("Music as a celebration of life and diversity, with people from different ages and backgrounds coming together to appreciate and honor the beauty and complexity of the human experience through the power of song.");
		v.AddPos("Music festivals as a celebration of love and unity, bringing people of all ages and backgrounds together to let go of prejudice and hatred and embrace the global bond of music and humanity.");
		v.AddPos("Music being a source of inspiration and connection, bringing people from different walks of life together to share their love and passion for music and find solace and community in the universal language of sound. ");
		v.AddPos("Music as a reminder of our shared humanity and interconnectedness, connecting people of all ages and backgrounds through the raw, unfiltered emotions and experiences expressed through the universal language of music.");
		v.AddPos("Music festivals as a celebration of diversity and inclusivity, with people of different ages and backgrounds coming together to honor and appreciate the unique and diverse musical cultures and expressions that make up the human experience.");
		v.AddPos("Music as a symbol of hope and resilience, bringing people from all ages and backgrounds together to find strength and healing in the face of adversity and hardship through the unifying power of song.");
		v.AddPos("Music being a catalyst for positive change and progress, inspiring people of different ages and backgrounds to stand up and take action towards building a better, more harmonious and compassionate world through the unifying power of music.");
		v.AddPos("Music as a celebration of diversity, inclusivity, and acceptance, with people from all ages and backgrounds coming together to appreciate and embrace the various, unique expressions and cultures that make up our global musical tapestry.");
		v.AddPos("Music festivals as a space for personal growth and transformation, where people from different generations and backgrounds come together to learn, celebrate, and evolve through the powerful and universal language of music.");
		v.AddPos("Music being a reminder of our shared humanity and interconnectedness, bringing people from all walks of life together to celebrate, honor, and embrace the beautiful, complex journey of being human through the universal magic of song.");
		v.AddPos("Music as a force for healing and unity, bringing people from different ages and backgrounds together to find solace, strength, and connection in times of pain, sadness, and adversity through the unifying power of music.");
		v.AddPos("Music festivals as a representation of the unity and diversity of our world, with people from diverse backgrounds coming together in perfect harmony and synchrony to celebrate, appreciate, and share the universal and transcendental bond of music.");
		v.AddPos("Music as a catalyst for personal connection and transformation, bringing individuals from all ages and backgrounds together to truly connect, understand, and empathize with each other through the raw, unfiltered emotions and experiences expressed through the universal language of music.");
		v.AddPos("Music being a celebration of diversity, individuality, and unity, with people from all corners of the world coming together to promote and honor the beauty and richness of our differences and similarities through the powerful, unifying language of song.");
		v.AddPos("Music as a means of promoting understanding and compassion, connecting people from different walks of life and different cultures to appreciate, respect, and coexist through the power of sound and rhythm.");
		v.AddPos("Music festivals as a representation of peace and harmony, where individuals from diverse backgrounds come together to celebrate and embrace the global bond of humanity and culture through the universal language of music.");
		v.AddPos("Music transcending time and bridging generations, bringing people from different backgrounds and ages together to appreciate and honor the timeless and universal impact and significance of music in our human journey.");
		v.AddPos("Music as an agent of social change and progress, uniting people from all walks of life and different ages to stand up and take action towards building a more just, equitable, and compassionate world through the powerful, universal language of song.");
		v.AddPos("Music festivals as a celebration of diversity, inclusivity, and love, where people from different cultures, backgrounds, and ages gather to let go of differences and embrace the shared bond of humanity through the unifying power of music.");
		v.AddPos("Music being a source of inspiration and motivation, bringing people from diverse backgrounds and generations together to overcome challenges and strive for a better future through the transcendent, universal language of song.");
		v.AddPos("Music as a reminder of our shared roots and interconnectedness, connecting people from all ages and backgrounds through the raw, unfiltered emotions and experiences expressed through the universal language of melody and rhythm.");
		v.AddPos("Music as a superficial means of bringing people together, with shallow and temporary connections formed at parties and clubs");
		v.AddPos("Music being exclusive and catering to a specific group of people, leaving others feeling excluded and marginalized");
		v.AddPos("Music causing division and conflict between different groups with opposing tastes and preferences");
		v.AddPos("Music being commercialized and exploitative, using its global appeal to make profit rather than promote genuine unity");
		v.AddPos("Music perpetuating stereotypes and cultural appropriation, rather than celebrating and embracing diversity");
		v.AddPos("Music used as a distraction from real-world issues and problems, preventing meaningful connections and discussions");
		v.AddPos("Music fostering unhealthy competition and jealousy among artists and fans, leading to toxic environments and conflicts");
		v.AddPos("Music promoting irresponsible and harmful behavior, especially in younger generations, under the guise of celebration and freedom");
		v.AddPos("Music being a tool for corporations and governments to manipulate and control people, rather than a unifying force driven by genuine passion and love for music");
		v.AddPos("Music creating a false illusion of unity and harmony, negating the real issues and struggles that divide us as a global community.");
		v.AddPos("Music being used to exploit and appropriate cultures, rather than truly celebrate and appreciate them.");
		v.AddPos("Music perpetuating harmful stereotypes and reinforcing societal inequalities, rather than promoting true equality and inclusivity.");
		v.AddPos("Music fostering a toxic culture of gatekeeping and exclusion, shutting out those who may not fit into a certain image or sound.");
		v.AddPos("Music promoting superficiality and materialism, further dividing people based on wealth and status rather than bringing them together.");
		v.AddPos("Music as a means of promoting consumerism and conformism, rather than encouraging individuality and diversity.");
		v.AddPos("Music as a distraction from real-world issues and injustices, rather than a platform for meaningful change and unity.");
		v.AddPos("Music being used by those in power to manipulate and control the masses, silencing genuine dissent and resistance.");
		v.AddPos("Music perpetuating toxic masculinity and gender stereotypes, rather than celebrating and embracing all forms of expression and identity.");
		v.AddPos("Music being used as a tool for cultural erasure and colonization, rather than a way to connect diverse communities.");
		v.AddPos("Music promoting a homogenous and Eurocentric view of the world, erasing the contributions and celebrations of non-Western cultures.");
		v.AddPos("Music promoting a superficial and shallow form of celebration, rather than a meaningful and genuine appreciation of various cultures and backgrounds.");
		v.AddPos("Music as a form of escapism, preventing necessary reflection and critical thought about societal issues and injustices.");
		v.AddPos("Music perpetuating stereotypes and biases, further dividing people along lines of race, ethnicity, and culture rather than bringing them together.");
		v.AddPos("Music glorifying party culture and recklessness, rather than promoting responsible and mindful celebration.");
		v.AddPos("Music being used as a tool for propaganda and manipulation, perpetuating harmful narratives and ideologies.");
		v.AddPos("Music reinforcing and promoting cultural appropriation, rather than encouraging education and respect for diverse cultures.");
		v.AddPos("Music as a form of exclusion and elitism, with certain genres and communities seen as superior and others looked down upon.");
		v.AddPos("Music promoting unhealthy and toxic behaviors, such as drug and alcohol abuse, under the guise of celebration and liberation.");
		v.AddPos("Music being used to exploit and profit off of marginalized communities, rather than uplifting and empowering them.");
		v.AddPos("Music reinforcing negative stereotypes and ideals, rather than promoting positive change and unity.");
		v.AddPos("Music being used to manipulate and divide people, rather than bringing them together in genuine celebration and understanding.");
		v.AddPos("Music perpetuating harmful and oppressive systems, rather than challenging and dismantling them through unity and solidarity.");
		v.AddPos("Music promoting a shallow and performative form of activism and unity, rather than genuine action and progress.");
		v.AddPos("Music being used to romanticize and fetishize cultures and backgrounds, rather than promoting genuine appreciation and understanding.");
		v.AddPos("Music as a means of creating false and temporary connections, rather than fostering genuine and lasting relationships.");
		v.AddPos("Music reinforcing negative and harmful gender roles and stereotypes, rather than promoting gender equality and inclusivity.");
		v.AddPos("Music fostering a culture of competition and comparison, rather than embracing and celebrating differences and diversity.");
		v.AddPos("Music promoting a homogeneous and commercialized form of expression, rather than encouraging unique and authentic voices and perspectives.");
		v.AddPos("Music being used as a tool for cultural assimilation and erasure, rather than promoting cultural exchange and diversity.");
		v.AddPos("Music perpetuating harmful and toxic ideas about beauty and worth, rather than promoting self-love and acceptance regardless of background or appearance.");
		v.AddPos("Music promoting irresponsible and harmful messages, such as misogyny and violence, instead of using its reach to promote positive change and unity.");
		v.AddPos("Music as a means of exploiting and profiting off of cultures and traditions, rather than genuinely celebrating and promoting them.");
		v.AddPos("Music being used as a form of escapism, preventing genuine reflection and action towards creating positive change and unity.");
		v.AddPos("Music reinforcing and perpetuating harmful hierarchies and systems of power, rather than challenging and dismantling them for true unity and equity.");
		v.AddPos("Music being used to manipulate and control people, rather than being a platform for true connection and understanding among diverse individuals.");
		v.AddPos("Music promoting harmful and toxic ideals of masculinity and femininity, rather than celebrating all forms of gender and expression.");
		v.AddPos("Music fostering a culture of conformity and homogeneity, discouraging individuality and unique perspectives.");
		v.AddPos("Music perpetuating false and harmful notions of race and ethnicity, rather than celebrating the beauty and diversity of different cultures and backgrounds.");
		v.AddPos("Music as a means of commodifying and exploiting cultural traditions and practices, rather than genuinely celebrating and learning from them.");
		v.AddPos("Music being used to distract and prevent meaningful conversations about important societal issues and injustices, rather than being a platform for change and unity.");
		v.AddPos("Music promoting a shallow and superficial form of unity, rather than fostering deep, meaningful connections and understanding among diverse communities.");
		v.AddPos("Music as a way for the powerful to maintain and perpetuate their privilege, rather than being used as a tool for true unity and equity.");
		v.AddPos("Music perpetuating harmful and oppressive beauty standards and norms, rather than promoting self-acceptance and diversity.");
		v.AddPos("Music being used to manipulate and control people, rather than fostering genuine and authentic expressions of individuality and creativity.");
		v.AddPos("Music reinforcing and perpetuating societal divides and injustices, rather than being used as a platform for true unity and equality.");
		v.AddPos("Music promoting a shallow and performative form of activism and unity, rather than genuine and sustained efforts for positive change and unification.");
		v.AddPos("Music as a way to reinforce and perpetuate unhealthy and harmful relationships and dynamics, rather than promoting genuine and healthy connections.");
		v.AddPos("Music being used to exploit and capitalize on marginalized communities, rather than uplifting and empowering them through genuine celebration and understanding.");
		v.AddPos("Music reinforcing and perpetuating romanticized and harmful stereotypes about marginalized groups, rather than promoting genuine understanding and inclusion.");
		v.AddPos("Music promoting nihilistic and destructive ideologies, rather than encouraging positivity and meaningful connections among diverse individuals.");
		v.AddPos("Music being used to manipulate and control people through messages of consumerism and materialism, rather than fostering genuine connections and unity.");
		v.AddPos("Music promoting harmful and toxic ideals of gender and beauty, rather than celebrating and uplifting diverse expressions and identities.");
		v.AddPos("Music perpetuating and glorifying reckless and destructive behaviors, rather than promoting responsible and positive forms of celebration and unity.");
		v.AddPos("Music being used to capitalize on and commodify cultures and traditions, rather than genuinely appreciating and celebrating them.");
		v.AddPos("Music promoting a superficial and transactional form of connection and unity, rather than encouraging genuine and meaningful relationships among diverse individuals.");
		v.AddPos("Music as a means to romanticize and exploit cultures and backgrounds, rather than promoting genuine understanding and respect.");
		v.AddPos("Music being used to distract and prevent meaningful conversations and actions towards positive change and equity, rather than being a tool for true unity.");
		v.AddPos("Music reinforcing and promoting harmful societal norms and systems of oppression, rather than challenging and dismantling them for genuine unity and progress.");
		v.AddPos("Music fostering a culture of competition and contentiousness, rather than encouraging empathy and understanding among diverse communities.");
		v.AddPos("Music promoting an inauthentic and homogenized form of expression, rather than celebrating and uplifting unique and diverse voices and perspectives.");
		v.AddPos("Music being used as a tool for assimilation and erasure of different cultures, rather than promoting genuine cultural exchange and appreciation.");
		v.AddPos("Music perpetuating and glorifying harmful and oppressive behaviors, rather than promoting positive and inclusive expressions of self and celebration.");
		v.AddPos("Music promoting shallow and temporary connections, rather than fostering genuine and lasting relationships among diverse individuals.");
		v.AddPos("Music being used by those in power to distract and manipulate the masses, rather than being a platform for genuine unity and empowerment.");
		v.AddPos("Music as a means of reinforcing and perpetuating rigid and harmful gender roles, rather than promoting fluid and diverse expressions of gender and identity.");
		v.AddPos("Music promoting negative and toxic ideals of beauty and worth, rather than uplifting and celebrating diversity and self-love.");
		v.AddPos("Music as a distraction from the real issues and injustices that divide us, rather than being used as a platform for conversation and change towards unity and equality.");
		v.AddPos("Music reinforcing and perpetuating harmful and oppressive systems of power, rather than challenging and dismantling them for genuine unity and progress.");
		v.AddPos("Music as a means of promoting commercialization and profit, rather than using its reach and global appeal to promote genuine connections and understanding among diverse individuals.");
		v.AddPos("Music perpetuating and glorifying harmful and oppressive societal norms, rather than promoting understanding and inclusion of diverse identities and perspectives.");
		v.AddPos("Music being used as a means of cultural appropriation and exploitation, rather than fostering genuine appreciation and celebration of diverse cultures and backgrounds.");
		v.AddPos("Music promoting a performative and shallow form of activism and celebration, rather than genuine and sustained efforts towards positive change and unification.");
		v.AddPos("Music as a way to romanticize and fetishize cultures, rather than promoting genuine understanding and respectful celebration of them.");
		v.AddPos("Music being used to disempower and silence marginalized communities, rather than uplifting and empowering them through genuine connections and understanding.");
		v.AddPos("Music perpetuating and glorifying harmful and oppressive behaviors and ideals, rather than promoting positive and inclusive expressions of self and community.");
		v.AddPos("Music promoting superficial and shallow connections, rather than fostering real and meaningful relationships among diverse individuals.");
		v.AddPos("Music being used by those in power to manipulate and control the masses, rather than being used as a tool for genuine unity, empowerment, and positive change.");
		v.AddPos("Music promoting destructive and nihilistic ideologies, rather than uplifting and encouraging positive and meaningful connections among diverse individuals.");
		v.AddPos("Music as a means of commodifying and exploiting cultural traditions, rather than fostering genuine understanding and appreciation of them.");
		v.AddPos("Music perpetuating and glorifying shallow and performative forms of activism and celebration, rather than promoting sustained and genuine efforts towards positive change and unification.");
		v.AddPos("Music as a way to romanticize and fetishize cultures and backgrounds, rather than promoting understanding and respectful celebration of diversity.");
		v.AddPos("Music being used to perpetuate and glorify harmful and oppressive ideals of gender and beauty, rather than promoting diversity and self-acceptance.");
		v.AddPos("Music promoting a shallow and superficial form of unity, rather than fostering genuine and lasting relationships among diverse individuals.");
		v.AddPos("Music as a means of promoting commercialization and profit, rather than using its reach and global appeal to genuinely connect diverse communities.");
		v.AddPos("Music reinforcing and perpetuating rigid and harmful gender roles and stereotypes, rather than promoting fluid and diverse expressions of gender and identity.");
		v.AddPos("Music being used to disempower and silence marginalized communities, rather than uplifting and empowering them through genuine understanding and connection.");
		v.AddPos("Music perpetuating and glorifying harmful and oppressive societal norms and systems, rather than challenging and breaking them for genuine unity and progress.");
		v.AddPos("Music promoting superficial and shallow forms of connections and relationships, rather than fostering genuine and meaningful bonds among diverse individuals.");
		v.AddPos("Music as a tool for assimilation and erasure of different cultures, rather than promoting genuine cultural exchange and respect for diversity.");
		v.AddPos("Music being used to distract and prevent meaningful conversations and actions towards positive change, rather than being a platform for genuine unity and progress.");
		v.AddPos("Music promoting destructive and toxic behaviors, rather than encouraging positive and responsible forms of celebration and connection.");
		v.AddPos("Music as a means of exploiting and capitalizing on cultural traditions, rather than genuinely celebrating and learning from them.");
		v.AddPos("Music promoting insincere and performative forms of celebration and activism, rather than genuine and sustained efforts towards positive change and inclusivity.");
		v.AddPos("Music being used by those in power to distract and prevent unity among the masses, rather than being a tool for genuine understanding and empowerment.");
		v.AddPos("Music reinforcing and perpetuating harmful and oppressive structural inequalities, rather than promoting unity and progress towards equity and inclusivity.");
		v.AddPos("Music as a means of romanticizing and appropriating cultures, rather than genuinely celebrating and respecting them.");
		v.AddPos("Music promoting unrealistic and harmful expectations of beauty and worth, rather than uplifting and celebrating diverse identities and self-acceptance.");
		v.AddNeg("Music as a superficial means of bringing people together, with shallow and temporary connections formed at parties and clubs");
		v.AddNeg("Music being exclusive and catering to a specific group of people, leaving others feeling excluded and marginalized");
		v.AddNeg("Music causing division and conflict between different groups with opposing tastes and preferences");
		v.AddNeg("Music being commercialized and exploitative, using its global appeal to make profit rather than promote genuine unity");
		v.AddNeg("Music perpetuating stereotypes and cultural appropriation, rather than celebrating and embracing diversity");
		v.AddNeg("Music used as a distraction from real-world issues and problems, preventing meaningful connections and discussions");
		v.AddNeg("Music fostering unhealthy competition and jealousy among artists and fans, leading to toxic environments and conflicts");
		v.AddNeg("Music promoting irresponsible and harmful behavior, especially in younger generations, under the guise of celebration and freedom");
		v.AddNeg("Music being a tool for corporations and governments to manipulate and control people, rather than a unifying force driven by genuine passion and love for music");
		v.AddNeg("Music creating a false illusion of unity and harmony, negating the real issues and struggles that divide us as a global community.");
		v.AddNeg("Music being used to exploit and appropriate cultures, rather than truly celebrate and appreciate them.");
		v.AddNeg("Music perpetuating harmful stereotypes and reinforcing societal inequalities, rather than promoting true equality and inclusivity.");
		v.AddNeg("Music fostering a toxic culture of gatekeeping and exclusion, shutting out those who may not fit into a certain image or sound.");
		v.AddNeg("Music promoting superficiality and materialism, further dividing people based on wealth and status rather than bringing them together.");
		v.AddNeg("Music as a means of promoting consumerism and conformism, rather than encouraging individuality and diversity.");
		v.AddNeg("Music as a distraction from real-world issues and injustices, rather than a platform for meaningful change and unity.");
		v.AddNeg("Music being used by those in power to manipulate and control the masses, silencing genuine dissent and resistance.");
		v.AddNeg("Music perpetuating toxic masculinity and gender stereotypes, rather than celebrating and embracing all forms of expression and identity.");
		v.AddNeg("Music being used as a tool for cultural erasure and colonization, rather than a way to connect diverse communities.");
		v.AddNeg("Music promoting a homogenous and Eurocentric view of the world, erasing the contributions and celebrations of non-Western cultures.");
		v.AddNeg("Music promoting a superficial and shallow form of celebration, rather than a meaningful and genuine appreciation of various cultures and backgrounds.");
		v.AddNeg("Music as a form of escapism, preventing necessary reflection and critical thought about societal issues and injustices.");
		v.AddNeg("Music perpetuating stereotypes and biases, further dividing people along lines of race, ethnicity, and culture rather than bringing them together.");
		v.AddNeg("Music glorifying party culture and recklessness, rather than promoting responsible and mindful celebration.");
		v.AddNeg("Music being used as a tool for propaganda and manipulation, perpetuating harmful narratives and ideologies.");
		v.AddNeg("Music reinforcing and promoting cultural appropriation, rather than encouraging education and respect for diverse cultures.");
		v.AddNeg("Music as a form of exclusion and elitism, with certain genres and communities seen as superior and others looked down upon.");
		v.AddNeg("Music promoting unhealthy and toxic behaviors, such as drug and alcohol abuse, under the guise of celebration and liberation.");
		v.AddNeg("Music being used to exploit and profit off of marginalized communities, rather than uplifting and empowering them.");
		v.AddNeg("Music reinforcing negative stereotypes and ideals, rather than promoting positive change and unity.");
		v.AddNeg("Music being used to manipulate and divide people, rather than bringing them together in genuine celebration and understanding.");
		v.AddNeg("Music perpetuating harmful and oppressive systems, rather than challenging and dismantling them through unity and solidarity.");
		v.AddNeg("Music promoting a shallow and performative form of activism and unity, rather than genuine action and progress.");
		v.AddNeg("Music being used to romanticize and fetishize cultures and backgrounds, rather than promoting genuine appreciation and understanding.");
		v.AddNeg("Music as a means of creating false and temporary connections, rather than fostering genuine and lasting relationships.");
		v.AddNeg("Music reinforcing negative and harmful gender roles and stereotypes, rather than promoting gender equality and inclusivity.");
		v.AddNeg("Music fostering a culture of competition and comparison, rather than embracing and celebrating differences and diversity.");
		v.AddNeg("Music promoting a homogeneous and commercialized form of expression, rather than encouraging unique and authentic voices and perspectives.");
		v.AddNeg("Music being used as a tool for cultural assimilation and erasure, rather than promoting cultural exchange and diversity.");
		v.AddNeg("Music perpetuating harmful and toxic ideas about beauty and worth, rather than promoting self-love and acceptance regardless of background or appearance.");
		v.AddNeg("Music promoting irresponsible and harmful messages, such as misogyny and violence, instead of using its reach to promote positive change and unity.");
		v.AddNeg("Music as a means of exploiting and profiting off of cultures and traditions, rather than genuinely celebrating and promoting them.");
		v.AddNeg("Music being used as a form of escapism, preventing genuine reflection and action towards creating positive change and unity.");
		v.AddNeg("Music reinforcing and perpetuating harmful hierarchies and systems of power, rather than challenging and dismantling them for true unity and equity.");
		v.AddNeg("Music being used to manipulate and control people, rather than being a platform for true connection and understanding among diverse individuals.");
		v.AddNeg("Music promoting harmful and toxic ideals of masculinity and femininity, rather than celebrating all forms of gender and expression.");
		v.AddNeg("Music fostering a culture of conformity and homogeneity, discouraging individuality and unique perspectives.");
		v.AddNeg("Music perpetuating false and harmful notions of race and ethnicity, rather than celebrating the beauty and diversity of different cultures and backgrounds.");
		v.AddNeg("Music as a means of commodifying and exploiting cultural traditions and practices, rather than genuinely celebrating and learning from them.");
		v.AddNeg("Music being used to distract and prevent meaningful conversations about important societal issues and injustices, rather than being a platform for change and unity.");
		v.AddNeg("Music promoting a shallow and superficial form of unity, rather than fostering deep, meaningful connections and understanding among diverse communities.");
		v.AddNeg("Music as a way for the powerful to maintain and perpetuate their privilege, rather than being used as a tool for true unity and equity.");
		v.AddNeg("Music perpetuating harmful and oppressive beauty standards and norms, rather than promoting self-acceptance and diversity.");
		v.AddNeg("Music being used to manipulate and control people, rather than fostering genuine and authentic expressions of individuality and creativity.");
		v.AddNeg("Music reinforcing and perpetuating societal divides and injustices, rather than being used as a platform for true unity and equality.");
		v.AddNeg("Music promoting a shallow and performative form of activism and unity, rather than genuine and sustained efforts for positive change and unification.");
		v.AddNeg("Music as a way to reinforce and perpetuate unhealthy and harmful relationships and dynamics, rather than promoting genuine and healthy connections.");
		v.AddNeg("Music being used to exploit and capitalize on marginalized communities, rather than uplifting and empowering them through genuine celebration and understanding.");
		v.AddNeg("Music reinforcing and perpetuating romanticized and harmful stereotypes about marginalized groups, rather than promoting genuine understanding and inclusion.");
		v.AddNeg("Music promoting nihilistic and destructive ideologies, rather than encouraging positivity and meaningful connections among diverse individuals.");
		v.AddNeg("Music being used to manipulate and control people through messages of consumerism and materialism, rather than fostering genuine connections and unity.");
		v.AddNeg("Music promoting harmful and toxic ideals of gender and beauty, rather than celebrating and uplifting diverse expressions and identities.");
		v.AddNeg("Music perpetuating and glorifying reckless and destructive behaviors, rather than promoting responsible and positive forms of celebration and unity.");
		v.AddNeg("Music being used to capitalize on and commodify cultures and traditions, rather than genuinely appreciating and celebrating them.");
		v.AddNeg("Music promoting a superficial and transactional form of connection and unity, rather than encouraging genuine and meaningful relationships among diverse individuals.");
		v.AddNeg("Music as a means to romanticize and exploit cultures and backgrounds, rather than promoting genuine understanding and respect.");
		v.AddNeg("Music being used to distract and prevent meaningful conversations and actions towards positive change and equity, rather than being a tool for true unity.");
		v.AddNeg("Music reinforcing and promoting harmful societal norms and systems of oppression, rather than challenging and dismantling them for genuine unity and progress.");
		v.AddNeg("Music fostering a culture of competition and contentiousness, rather than encouraging empathy and understanding among diverse communities.");
		v.AddNeg("Music promoting an inauthentic and homogenized form of expression, rather than celebrating and uplifting unique and diverse voices and perspectives.");
		v.AddNeg("Music being used as a tool for assimilation and erasure of different cultures, rather than promoting genuine cultural exchange and appreciation.");
		v.AddNeg("Music perpetuating and glorifying harmful and oppressive behaviors, rather than promoting positive and inclusive expressions of self and celebration.");
		v.AddNeg("Music promoting shallow and temporary connections, rather than fostering genuine and lasting relationships among diverse individuals.");
		v.AddNeg("Music being used by those in power to distract and manipulate the masses, rather than being a platform for genuine unity and empowerment.");
		v.AddNeg("Music as a means of reinforcing and perpetuating rigid and harmful gender roles, rather than promoting fluid and diverse expressions of gender and identity.");
		v.AddNeg("Music promoting negative and toxic ideals of beauty and worth, rather than uplifting and celebrating diversity and self-love.");
		v.AddNeg("Music as a distraction from the real issues and injustices that divide us, rather than being used as a platform for conversation and change towards unity and equality.");
		v.AddNeg("Music reinforcing and perpetuating harmful and oppressive systems of power, rather than challenging and dismantling them for genuine unity and progress.");
		v.AddNeg("Music as a means of promoting commercialization and profit, rather than using its reach and global appeal to promote genuine connections and understanding among diverse individuals.");
		v.AddNeg("Music perpetuating and glorifying harmful and oppressive societal norms, rather than promoting understanding and inclusion of diverse identities and perspectives.");
		v.AddNeg("Music being used as a means of cultural appropriation and exploitation, rather than fostering genuine appreciation and celebration of diverse cultures and backgrounds.");
		v.AddNeg("Music promoting a performative and shallow form of activism and celebration, rather than genuine and sustained efforts towards positive change and unification.");
		v.AddNeg("Music as a way to romanticize and fetishize cultures, rather than promoting genuine understanding and respectful celebration of them.");
		v.AddNeg("Music being used to disempower and silence marginalized communities, rather than uplifting and empowering them through genuine connections and understanding.");
		v.AddNeg("Music perpetuating and glorifying harmful and oppressive behaviors and ideals, rather than promoting positive and inclusive expressions of self and community.");
		v.AddNeg("Music promoting superficial and shallow connections, rather than fostering real and meaningful relationships among diverse individuals.");
		v.AddNeg("Music being used by those in power to manipulate and control the masses, rather than being used as a tool for genuine unity, empowerment, and positive change.");
		v.AddNeg("Music promoting destructive and nihilistic ideologies, rather than uplifting and encouraging positive and meaningful connections among diverse individuals.");
		v.AddNeg("Music as a means of commodifying and exploiting cultural traditions, rather than fostering genuine understanding and appreciation of them.");
		v.AddNeg("Music perpetuating and glorifying shallow and performative forms of activism and celebration, rather than promoting sustained and genuine efforts towards positive change and unification.");
		v.AddNeg("Music as a way to romanticize and fetishize cultures and backgrounds, rather than promoting understanding and respectful celebration of diversity.");
		v.AddNeg("Music being used to perpetuate and glorify harmful and oppressive ideals of gender and beauty, rather than promoting diversity and self-acceptance.");
		v.AddNeg("Music promoting a shallow and superficial form of unity, rather than fostering genuine and lasting relationships among diverse individuals.");
		v.AddNeg("Music as a means of promoting commercialization and profit, rather than using its reach and global appeal to genuinely connect diverse communities.");
		v.AddNeg("Music reinforcing and perpetuating rigid and harmful gender roles and stereotypes, rather than promoting fluid and diverse expressions of gender and identity.");
		v.AddNeg("Music being used to disempower and silence marginalized communities, rather than uplifting and empowering them through genuine understanding and connection.");
		v.AddNeg("Music perpetuating and glorifying harmful and oppressive societal norms and systems, rather than challenging and breaking them for genuine unity and progress.");
		v.AddNeg("Music promoting superficial and shallow forms of connections and relationships, rather than fostering genuine and meaningful bonds among diverse individuals.");
		v.AddNeg("Music as a tool for assimilation and erasure of different cultures, rather than promoting genuine cultural exchange and respect for diversity.");
		v.AddNeg("Music being used to distract and prevent meaningful conversations and actions towards positive change, rather than being a platform for genuine unity and progress.");
		v.AddNeg("Music promoting destructive and toxic behaviors, rather than encouraging positive and responsible forms of celebration and connection.");
		v.AddNeg("Music as a means of exploiting and capitalizing on cultural traditions, rather than genuinely celebrating and learning from them.");
		v.AddNeg("Music promoting insincere and performative forms of celebration and activism, rather than genuine and sustained efforts towards positive change and inclusivity.");
		v.AddNeg("Music being used by those in power to distract and prevent unity among the masses, rather than being a tool for genuine understanding and empowerment.");
		v.AddNeg("Music reinforcing and perpetuating harmful and oppressive structural inequalities, rather than promoting unity and progress towards equity and inclusivity.");
		v.AddNeg("Music as a means of romanticizing and appropriating cultures, rather than genuinely celebrating and respecting them.");
		v.AddNeg("Music promoting unrealistic and harmful expectations of beauty and worth, rather than uplifting and celebrating diverse identities and self-acceptance.");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "Indie rock: unique and authentic sound";
		v.AddPos("The lyrics may talk about breaking free from the norm and not conforming to societal expectations.");
		v.AddPos("The artist's sound is often described as \"genre-defying\" or \"experimental.\"");
		v.AddPos("The music may incorporate elements from various cultures and genres, making it multicultural and diverse.");
		v.AddPos("The lyrics may celebrate individuality and encourage listeners to be true to themselves.");
		v.AddPos("Critics may call the music \"unconventional\" or \"visionary.\"");
		v.AddPos("The sound is often described as \"bold\" or \"unapologetic.\"");
		v.AddPos("The lyrics may challenge societal issues and spark meaningful discussions.");
		v.AddPos("The music may be praised for being thought-provoking and pushing the boundaries of traditional song structures.");
		v.AddPos("The artist is often commended for their unique vision and creative approach to making music.");
		v.AddPos("The sound is noted for its originality and distinctiveness.");
		v.AddPos("The lyrics may explore complex and abstract themes, making the music intellectually stimulating.");
		v.AddPos("The artist may be seen as a pioneer in their genre, constantly pushing the limits and evolving their sound.");
		v.AddPos("The music is often admired for its authenticity and raw emotion.");
		v.AddPos("The lyrics may be poetic and abstract, leaving room for individual interpretation and reflection.");
		v.AddPos("The sound is consistently evolving and never repetitive or stagnant.");
		v.AddPos("The artist is praised for their fearlessness and willingness to take risks with their music.");
		v.AddPos("The music may inspire others to think outside the box and challenge the status quo.");
		v.AddPos("The lyrics may celebrate the beauty and power of individual expression.");
		v.AddPos("The sound is characterized by its originality and refusal to conform to commercial trends.");
		v.AddPos("The artist may be seen as a visionary, creating music that is ahead of its time.");
		v.AddPos("The music may incorporate unconventional instrumentation or production techniques.");
		v.AddPos("The lyrics may tackle taboo or controversial topics, sparking important conversations.");
		v.AddPos("The sound is often recognized for its authenticity and lack of artificiality.");
		v.AddPos("The artist is admired for their ability to constantly reinvent themselves and surprise listeners.");
		v.AddPos("The lyrics may break traditional songwriting conventions, creating a unique and refreshing listening experience.");
		v.AddPos("The music may be described as \"avant-garde\" or \"cutting-edge.\"");
		v.AddPos("The artist's music may inspire others to think critically and question societal norms.");
		v.AddPos("The lyrics may convey a strong message of self-acceptance and embracing one's individuality.");
		v.AddPos("The sound is characterized by its boundary-pushing and genre-blending nature.");
		v.AddPos("The artist is often considered a trailblazer, fearlessly pushing the boundaries of what is deemed \"acceptable\" in music.");
		v.AddPos("The music may be praised for its authenticity and refusal to conform to mainstream expectations.");
		v.AddPos("The lyrics may be poetic and abstract, challenging listeners to think deeply and reflect on their own experiences.");
		v.AddPos("The sound may incorporate unconventional sounds and production techniques, creating a truly unique listening experience.");
		v.AddPos("The artist may be celebrated for their authenticity and refusal to compromise their artistic vision for commercial success.");
		v.AddPos("The music may be recognized for its ability to inspire and empower listeners through its message of individuality and self-expression. ");
		v.AddPos("The lyrics may celebrate unconventional beauty and encourage self-love and acceptance.");
		v.AddPos("The sound is constantly evolving, keeping listeners on their toes and never conforming to expectations.");
		v.AddPos("The artist may be seen as a true artist, pushing the boundaries of what is possible in music and creating something truly original.");
		v.AddPos("The music may be praised for its fearlessness and lack of concern for fitting into a specific genre or style.");
		v.AddPos(" The lyrics may explore personal struggles and navigate through difficult, uncomfortable topics with honesty and vulnerability.");
		v.AddPos("The sound may incorporate elements and influences from various cultures, creating a sense of global unity and diversity.");
		v.AddPos("The artist's music may inspire others to think differently and embrace their uniqueness.");
		v.AddPos("The lyrics may challenge societal norms and promote breaking away from conformity.");
		v.AddPos("The music may be described as \"genre-bending\" or \"genre-defying,\" making it impossible to categorize.");
		v.AddPos("The artist's sound is constantly evolving and never repeating the same formula.");
		v.AddPos("The lyrics may convey a sense of rebellion and challenge the status quo.");
		v.AddPos("The sound may incorporate unconventional instrumentation, creating a truly unique sonic landscape.");
		v.AddPos("The artist's music may be recognized for its authenticity and refusal to conform to commercial trends.");
		v.AddPos("The lyrics may encourage listeners to think critically and question authority.");
		v.AddPos("The music may be praised for its experimental nature and willingness to take risks.");
		v.AddPos("The artist may be seen as a disruptor, constantly pushing the boundaries and challenging the norms of the music industry.");
		v.AddPos("The sound may be described as \"genre-blending\" or \"eclectic,\" incorporating elements from various styles and genres seamlessly.");
		v.AddPos("The lyrics may promote self-discovery and encourage listeners to embrace their true selves.");
		v.AddPos("The music may be recognized for its ability to transcend cultural and societal barriers, uniting listeners through its unique sound.");
		v.AddPos("The artist's music may be described as a journey, always pushing forward and exploring new sonic territories.");
		v.AddPos("The lyrics may challenge preconceived notions and promote critical thinking.");
		v.AddPos("The sound may be praised for its authenticity and refusal to conform to commercial standards.");
		v.AddPos("The artist may be seen as a trailblazer, fearlessly paving the way for others to embrace their individuality and push boundaries in music.");
		v.AddPos("The music may inspire listeners to embrace their own personal struggles and find strength in their unique experiences.");
		v.AddPos("The lyrics may convey a sense of hope and empowerment, encouraging listeners to be themselves despite societal pressures.");
		v.AddPos("The sound is characterized by its unpredictability and refusal to be put in a box.");
		v.AddPos("The artist's music may be recognized for its boldness and unapologetic nature, standing out from the mainstream.");
		v.AddPos("The music may be described as \"progressive\" or \"avant-garde,\" constantly breaking new ground and challenging traditional music structures.");
		v.AddPos("The lyrics may explore deep, philosophical themes and challenge listeners to think beyond surface-level meanings.");
		v.AddPos("The sound may incorporate unconventional instrumentation and production techniques, creating a one-of-a-kind sonic experience.");
		v.AddPos("The artist may be praised for their unique approach to music, always pushing the boundaries and defying expectations.");
		v.AddPos("The music may inspire listeners to find their own voice and embrace their unique perspective.");
		v.AddPos("The lyrics may promote personal growth and encourage listeners to break away from limiting beliefs.");
		v.AddPos("The sound may be described as \"sonically rich\" or \"multi-dimensional,\" with layers of diverse sounds and textures.");
		v.AddPos("The artist may be seen as a visionary, creating music that is constantly evolving and defying the norm. ");
		v.AddPos("The music may be praised for its fearlessness and refusal to conform to commercial expectations.");
		v.AddPos("The lyrics may challenge traditional gender roles and celebrate individual freedom and expression.");
		v.AddPos("The sound may incorporate elements from various cultures and genres, breaking down barriers and promoting diversity.");
		v.AddPos("The artist's music may be described as \"genre-pushing,\" constantly pushing the boundaries and creating something new and exciting.");
		v.AddPos("The lyrics may convey a message of self-empowerment and inspire listeners to be true to themselves.");
		v.AddPos("The music may challenge the very definition of what music is, pushing the boundaries and challenging listeners' perceptions.");
		v.AddPos("The sound may be recognized for its ability to transport listeners to different emotional and sonic landscapes.");
		v.AddPos("The artist may be celebrated for their ability to merge multiple influences and create a sound that is uniquely their own.");
		v.AddPos("The lyrics may explore taboo topics and bring light to important issues that are often overlooked.");
		v.AddPos("The music may be praised for its authenticity and lack of pretentiousness.");
		v.AddPos("The artist's music may be described as \"genre-bending\" or \"genre-defying,\" with each song offering a new and unexpected sound.");
		v.AddPos("The lyrics may promote self-discovery and self-acceptance, encouraging listeners to embrace their uniqueness.");
		v.AddPos("The sound may be characterized by its boundary-pushing and experimental nature, always challenging the norms of the music industry.");
		v.AddPos("The artist may be seen as a pioneer, creating music that constantly evolves and sets trends.");
		v.AddPos("The music may inspire listeners to break free from societal conventions and embrace their own individual path.");
		v.AddPos("The lyrics may promote introspection and personal growth, encouraging listeners to challenge their own beliefs and perspectives.");
		v.AddPos("The sound may be praised for its complex and intricate layers, creating a unique sonic experience.");
		v.AddPos("The artist's music may be recognized for its fearlessness and refusal to conform to labels or expectations.");
		v.AddPos("The music may be described as \"genre-defying\" or \"boundary-pushing,\" constantly challenging listeners' expectations and breaking new ground.");
		v.AddPos("The lyrics may explore themes of self-expression and individual freedom, promoting the message of being true to oneself.");
		v.AddPos("The sound may incorporate influences from various cultures and genres, creating a sense of global unity and connection.");
		v.AddPos("The artist may be praised for their ability to create a cohesive and unique sound that is truly their own.");
		v.AddPos("The music may be seen as a breath of fresh air in a world where mainstream music can feel repetitive and formulaic.");
		v.AddPos("The lyrics may convey a message of hope and empowerment, encouraging listeners to embrace their differences and stand out from the crowd.");
		v.AddPos("The sound may be recognized for its authenticity and lack of commercialization, creating a pure listening experience.");
		v.AddPos("The artist's music may be described as bold and daring, standing out from the predictable and mundane.");
		v.AddPos("-The lyrics may explore personal struggles and battles, inviting listeners to connect and find solace in the music.");
		v.AddPos("The lyrics may center around the idea of embracing diversity and celebrating unique perspectives and experiences.");
		v.AddPos("The sound may blend elements from different eras and genres in a way that is fresh and unpredictable.");
		v.AddPos("The artist may be praised for their ability to constantly evolve and reinvent their sound, keeping listeners on their toes.");
		v.AddPos("The music may be described as \"genre-bending\" or \"genre-defying,\" breaking free from the constraints of traditional labels and categories.");
		v.AddPos("The lyrics may promote self-discovery and self-love, encouraging listeners to embrace their quirks and imperfections.");
		v.AddPos("The sound may be recognized for its boundary-pushing and experimental nature, always pushing the limits and challenging mainstream ideas.");
		v.AddPos("The artist may be seen as a visionary, creating music that transcends time and defies contemporary trends.");
		v.AddPos("The music may inspire listeners to embrace their own unique journey and trust their intuition in life.");
		v.AddPos("The lyrics may convey a strong message of inclusivity and acceptance, promoting the idea that everyone's voice deserves to be heard.");
		v.AddPos("The sound is characterized by its authenticity and lack of pretentiousness, creating a genuine and honest listening experience.");
		v.AddPos("The artist may be praised for their refusal to conform to societal norms and their ability to stay true to their artistic vision.");
		v.AddPos("The music may be described as \"non-conformist\" or \"against the grain,\" standing out from the crowd and refusing to blend in.");
		v.AddPos("The lyrics may explore complex and profound topics, inviting listeners to reflect and think deeply about life and society.");
		v.AddPos("The sound may incorporate unconventional instruments and production techniques, creating a fresh and unique sonic landscape.");
		v.AddPos("The artist's music may be recognized for its ability to transcend language and cultural barriers, appealing to listeners from all walks of life.");
		v.AddPos("The music may inspire listeners to embrace their individuality and stand out from the crowd, resisting societal pressure to conform.");
		v.AddPos("The lyrics may promote the idea of constantly evolving and never being afraid to try new things, both in music and in life.");
		v.AddPos("The sound may be described as \"genre-defying\" or \"genre-bending,\" transcending traditional genres and creating something new.");
		v.AddPos("The artist may be praised for their authenticity and genuine passion for creating music that is true to themselves.");
		v.AddPos("The music may be seen as a melting pot of different influences and cultures, creating a diverse and eclectic sound.");
		v.AddPos("The lyrics may convey a message of resilience and strength in the face of adversity, inspiring listeners to keep pushing forward.");
		v.AddPos("The sound may be recognized for its boundary-pushing and experimental nature, always breaking new ground and challenging traditional ideas.");
		v.AddPos("The artist may be seen as a role model for individual expression and creativity, inspiring others to find their own unique voice in life.");
		v.AddPos("The music may be described as \"avant-garde\" or \"ahead of its time,\" constantly pushing the limits and revolutionizing the music industry.");
		v.AddPos("The lyrics may explore themes of self-discovery and personal growth, inviting listeners to embrace the journey of finding oneself.");
		v.AddPos("The sound may incorporate a fusion of different sounds and styles, creating a completely unique and original listening experience.");
		v.AddPos("The artist may be praised for their ability to create something entirely their own and not conform to preexisting trends or expectations.");
		v.AddPos("The music may inspire listeners to think outside the box and challenge their own ideas of what is \"mainstream\" or \"acceptable.\"");
		v.AddPos("The lyrics may celebrate the power of individuality and the beauty of being different, encouraging listeners to ignore societal norms and be true to themselves.");
		v.AddPos("The sound is recognized for its authenticity and lack of commercialization, allowing the artist's vision to shine through without being diluted by market pressures.");
		v.AddPos("The artist may be seen as a revolutionary force in music, constantly pushing the boundaries and creating a unique and dynamic sound.");
		v.AddPos("The music may be described as \"unpredictable\" or \"surprising,\"");
		v.AddPos("The lyrics may challenge listeners to question their beliefs and open their minds to new perspectives.");
		v.AddPos("The sound may incorporate unconventional instruments and production techniques, creating a distinct and fresh sound.");
		v.AddPos("The artist may be praised for their ability to tell personal stories through their music, inviting listeners to connect and find meaning in their own experiences.");
		v.AddPos("The lyrics may promote the idea of embracing individuality and rejecting societal expectations.");
		v.AddPos("The music may be described as \"genre-defying\" or \"genre-bending,\" constantly breaking the mold and creating something new and exciting.");
		v.AddPos("The sound may be recognized for its fearlessness and unapologetic nature, never backing down or conforming to commercial standards.");
		v.AddPos("The artist may be seen as a true artist, creating music that is authentic and genuine, without being manipulated by industry pressures.");
		v.AddPos("The music may inspire listeners to trust their own creativity and embrace their unique voice in the world.");
		v.AddPos("The lyrics may explore themes of identity and self-discovery, inviting listeners to reflect and connect with their own journeys.");
		v.AddPos("The sound may be described as \"sonically adventurous\" or \"experimentally daring,\" constantly pushing the boundaries and surprising listeners.");
		v.AddPos("The artist may be praised for their ability to create music that is both accessible and innovative, appealing to a wide range of listeners.");
		v.AddPos("The music may be recognized for its ability to create a sense of unity and connection, bringing people together through its unique sound.");
		v.AddPos("The lyrics may convey a message of embracing change and never being afraid to try new things, both in music and in life.");
		v.AddPos("The sound may incorporate elements from various cultures and genres, creating a global and diverse listening experience.");
		v.AddPos("The artist may be seen as a maverick, fearlessly defying societal norms and creating music that is truly their own.");
		v.AddPos("The music may be described as \"genre-bending\" or \"genre-blending,\" incorporating diverse influences and creating a sound that is impossible to categorize.");
		v.AddPos("The lyrics may promote self-acceptance and confidence, encouraging listeners to embrace their quirks and imperfections.");
		v.AddPos("The sound may be recognized for its ability to evoke deep emotions and create a powerful listening experience.");
		v.AddPos("The artist's music may be praised for its authenticity and refusal to conform to mainstream expectations and trends.");
		v.AddPos("The artist may be seen as a visionary, constantly challenging the norm and paving the way for a new sound.");
		
		v.AddNeg("The music is dismissed as being too weird\" or \"hard to understand.\"");
		v.AddNeg("Critics may use terms like \"pretentious\" or \"overly experimental\" to describe the sound.");
		v.AddNeg("The lyrics may be criticized for being overly abstract or obscure.");
		v.AddNeg("The music may be seen as a way for the artist to appear edgy or avant-garde without genuine substance.");
		v.AddNeg("The artist may be accused of purposely trying to be different or controversial for the sake of shock value.");
		v.AddNeg("The sound may be deemed too niche and not mainstream enough.");
		v.AddNeg("The lyrics may come across as trying too hard to be different or unique.");
		v.AddNeg("The music may be labeled as \"too avant-garde\" or \"inaccessible\" for the general public.");
		v.AddNeg("Some may see the artist's unique sound as a gimmick rather than genuine artistic expression.");
		v.AddNeg("The music may be dismissed as not marketable or commercially viable");
		v.AddNeg("The artist's authenticity may be questioned, with accusations of them trying to conform to the stereotype of being a non-conformist.");
		v.AddNeg("The sound may be criticized for being too strange or off-putting for mainstream audiences.");
		v.AddNeg("The lyrics may be accused of being overly critical or judgmental of mainstream society.");
		v.AddNeg("The artist may be stereotyped as difficult or hard to work with because of their pursuit of pushing boundaries");
		v.AddNeg("The sound may be seen as trying too hard to be different and ultimately missing the mark.");
		v.AddNeg("The lyrics may be accused of being too abstract or self-indulgent.");
		v.AddNeg("The artist may be labeled as a \"hipster\" or \"try-hard\" for their unique sound. ");
		v.AddNeg("The sound may face backlash for being too alienating or confusing for listeners.");
		v.AddNeg("The lyrics may be criticized for being preachy or trying too hard to be thought-provoking.");
		v.AddNeg("The artist may be accused of sacrificing musical quality for the sake of being unique and different.");
		v.AddNeg("The sound may be dismissed as \"hipster music\" or only appealing to a specific niche audience.");
		v.AddNeg("The lyrics may be seen as overly pretentious, trying to sound deep and meaningful without actually saying anything substantial.");
		v.AddNeg("The artist's authenticity and genuine passion for music may be doubted.");
		v.AddNeg("The sound may be seen as too non-conformist to be likable or relatable.");
		v.AddNeg("The lyrics may be accused of promoting a shallow and superficial idea of being unique and going against the mainstream.");
		v.AddNeg("The artist may be stereotyped as a rebel or outsider, but not necessarily in a positive light.");
		v.AddNeg("The sound may be labeled as \"too weird\" or \"too out there\" for mainstream success.");
		v.AddNeg("The lyrics may be seen as trying to hard to be edgy and non-conformist, but ultimately lacking in real substance.");
		v.AddNeg("The artist may be criticized for trying to be too unique and not just focusing on making good music.");
		v.AddNeg("The sound may receive backlash for being too artsy or avant-garde, not relatable to the average listener.");
		v.AddNeg("The lyrics may be accused of being overly critical of societal norms without presenting a meaningful alternative.");
		v.AddNeg("The artist may be judged for being too pretentious and self-absorbed.");
		v.AddNeg("Overall, these stereotypes may undermine the artist's true talent and the genuine uniqueness of their sound.");
		v.AddNeg("The sound may be dismissed as \"noise\" or \"chaotic\" rather than art.");
		v.AddNeg("The lyrics may be seen as trying too hard to be provocative and controversial without any real substance.");
		v.AddNeg("The artist may be labeled as a rebel or non-conformist, but not taken seriously as a musician.");
		v.AddNeg("These stereotypes perpetuate the idea that being unique and authentic is just an act or a trend, rather than a genuine expression of self.");
		v.AddNeg("The sound may be dismissed as too strange or unconventional, not fitting into mainstream standards of what is considered \"good\" music.");
		v.AddNeg("The lyrics may be reduced to cliches and empty statements, lacking in depth and originality.");
		v.AddNeg("The artist may be pigeon-holed and judged solely on the cliche of being \"unique and challenging the mainstream,\" rather than on their actual artistry and talent.");
		v.AddNeg("These stereotypes also downplay the impact and importance of artists who genuinely challenge and push boundaries in the mainstream music industry.");
		v.AddNeg("The focus on the cliche of being unique and authentic can overshadow the actual quality and substance of the music.");
		v.AddNeg("These stereotypes can also create pressure for artists to maintain a certain image and lived up to a certain standard of being \"different\" and \"non-conformist.\" ");
		v.AddNeg("The sound may be dismissed as just a gimmick or a trend, rather than genuine artistic expression.");
		v.AddNeg("The lyrics may be seen as preaching a way of life rather than being a genuine reflection of the artist's experiences and beliefs.");
		v.AddNeg("These stereotypes can minimize the impact and importance of true uniqueness and authenticity in the music industry, reducing it to a overused cliche.");
		v.AddNeg("The artist's message and intentions may be misinterpreted or misunderstood because of these stereotypes.");
		v.AddNeg("Overall, these stereotypes limit the diversity and variety in the music industry, favoring a narrow and superficial idea of what it means to be unique and authentic.");
		v.AddNeg("These stereotypes can also contribute to exclusion and marginalization of certain voices and sounds in the mainstream music industry.");
		v.AddNeg("They perpetuate the idea that being unique and authentic is something that can be easily achieved with a specific formula, rather than a genuine and personal process for each artist.");
		v.AddNeg("The obsession with being \"different\" and non-conformist can overshadow the true passion and artistry of the musicians.");
		v.AddNeg("These stereotypes can also lead to a dismissive attitude towards the music, with people assuming they already know what it sounds like without actually listening to it.");
		v.AddNeg("They can also contribute to a narrow and limited perception of what is considered \"innovative\" and \"original\" in the music industry.");
		v.AddNeg("Overall, these stereotypes perpetuate a shallow and superficial idea of what it means to be unique and authentic in the music industry.");
		v.AddNeg("They fail to acknowledge that every artist and their sound is inherently unique and constantly evolving, and cannot be confined to a single cliche or stereotype. ");
		v.AddNeg("These stereotypes can create pressure for artists to constantly reinvent themselves and their sound, rather than allowing for natural growth and experimentation.");
		v.AddNeg("They also perpetuate the idea that mainstream music is inherently inferior and that true artistry can only be found in the \"underground\" or \"alternative\" scenes.");
		v.AddNeg("The artist may be seen as trying too hard to be different, rather than just creating music that they genuinely enjoy and believe in.");
		v.AddNeg("These stereotypes can create a sense of elitism and exclusivity in the music industry, perpetuating the idea that only a select few can be truly unique and authentic.");
		v.AddNeg("They can also contribute to a dismissive attitude towards artists who may not fit into this cliche of being \"unique and challenging the mainstream.\"");
		v.AddNeg("The focus on this cliche can also overshadow the many other aspects of an artist's identity and artistry.");
		v.AddNeg("Ultimately, these stereotypes can limit the true diversity and creativity in the music industry, reducing it to a narrow and unattainable standard of being \"unique\" and \"authentic.\" ");
		v.AddNeg("They can also prevent the appreciation and recognition of different forms of uniqueness and authenticity in music, as it is subjective and cannot be confined to a single stereotype or cliche.");
		v.AddNeg("Overall, these stereotypes perpetuate a shallow and superficial idea of what it means to be unique and authentic in the music industry, ignoring the complexity and nuance of individual artists and their sounds.");
		v.AddNeg("They also contribute to a narrow and limited perception of what is considered \"good\" or \"innovative\" music, dismissing anything that doesn't fit into this cliche.");
		v.AddNeg("The pressure to constantly push boundaries and challenge the mainstream can also be detrimental to artists' mental health and creativity.");
		v.AddNeg("These stereotypes can also discourage artists from exploring different sounds and styles, fearing that they will be judged for not being \"unique enough.\"");
		v.AddNeg("The obsession with being \"different\" and \"non-conformist\" can also overshadow the importance of genuine passion and authenticity in music.");
		v.AddNeg("Ultimately, these stereotypes fail to recognize the inherent uniqueness and authenticity of each artist and perpetuate this idea that it can be achieved through a formula or formulaic approach.");
		v.AddNeg("These stereotypes also fail to acknowledge the influence and impact of mainstream music on alternative and underground scenes, and how artists from all genres continue to push boundaries and challenge norms.");
		v.AddNeg("The focus on being \"unique and authentic\" can overshadow the true purpose and power of music: to connect and inspire others, regardless of its perceived level of uniqueness or authenticity. ");
		v.AddNeg("These stereotypes can lead to a narrow and limited perception of what defines a successful and impactful artist.");
		v.AddNeg("The constant pressure to be \"unique and challenging\" can also hinder artists' ability to express themselves authentically, leading to a focus on conforming to the stereotype rather than pursuing their own creative vision.");
		v.AddNeg("These stereotypes can also create a sense of competition and comparison among artists, perpetuating a toxic environment in the music industry.");
		v.AddNeg("The emphasis on being \"unique and authentic\" can also overshadow the importance of collaboration and collective creativity in music.");
		v.AddNeg("Overall, these stereotypes limit the potential and possibilities of music, reducing it to a strict and superficial standard of being \"different\" and \"non-conformist.\" ");
		v.AddNeg("They also ignore the complexity and diversity within the music industry, and the myriad of ways in which artists can be unique and authentic.");
		v.AddNeg("These stereotypes can also contribute to a dismissive attitude towards certain genres and styles of music, perpetuating the idea that they do not fit into the narrow definition of being \"unique\" and \"challenging the mainstream.\"");
		v.AddNeg("They can also create an aversion towards mainstream success and recognition, perpetuating the idea that being \"alternative\" or \"underground\" is inherently better.");
		v.AddNeg("Ultimately, these stereotypes limit the true potential and impact of music, overshadowing the diverse and multifaceted nature of the industry and its artists.");
		v.AddNeg("They fail to recognize and celebrate the uniqueness and authenticity of artists in all their forms, perpetuating a narrow and superficial idea of what it means to be an artist.");
		v.AddNeg("These stereotypes can also prevent genuine and meaningful connections between artists and their audience, as the focus is on conforming to a certain image rather than genuine connection and expression.");
		v.AddNeg("They can also contribute to a toxic and competitive environment in the music industry, where artists feel pressured to constantly outdo each other in terms of being \"different\" and \"challenging.\"");
		v.AddNeg("Overall, these stereotypes perpetuate a limited and damaging perception of uniqueness and authenticity in the music industry, limiting its potential for innovation and growth.");
		v.AddNeg("They ignore the true impact and importance of artists who have pushed boundaries and challenged norms within the mainstream, and instead continue to perpetuate this idea that true artistry can only be found in the alternative and underground scenes.");
		v.AddNeg("Ultimately, these stereotypes reduce the beauty and power of music to a shallow and narrow standard, failing to recognize the diverse and ever-evolving nature of the industry and its artists.");
		v.AddNeg("They also ignore the cultural and historical context of artists' music and the impact it has on society, instead focusing solely on its perceived level of uniqueness and authenticity.");
		v.AddNeg("Above all, these stereotypes fail to acknowledge and celebrate the artistry and individuality of each artist, reducing them to a cliched idea of what it means to be \"unique and authentic.\"");
		v.AddNeg("They can also create a sense of elitism and exclusivity within the music industry, perpetuating the idea that only a select few can be truly unique and authentic.");
		v.AddNeg("These stereotypes can also discourage genuine artistic expression and experimentation, as artists fear not fitting into this narrow and superficial idea of what it means to be \"innovative\" and \"different.\"");
		v.AddNeg("Ultimately, these stereotypes limit the true diversity and creativity in the music industry, reducing it to a shallow and superficial standard of what it means to be \"unique and authentic.\"");
		v.AddNeg("They also perpetuate harmful and limiting ideas of what it means to be \"different\" and \"challenging the mainstream,\" ignoring the complex and multifaceted nature of an artist's identity and artistry.");
		v.AddNeg("These stereotypes can also prevent the recognition and celebration of authentic, unique voices in the music industry, as they may not fit into the narrow and superficial standard portrayed by these cliches.");
		v.AddNeg("Ultimately, these stereotypes fail to capture the true essence and power of music, reducing it to a shallow and limited idea of what it means to be \"unique and authentic.\" ");
		v.AddNeg("They also contribute to the false notion that being unique and authentic is something that can be easily achieved or imitated through a certain formula, rather than being a genuinely personal and unique experience for each artist.");
		v.AddNeg("These stereotypes perpetuate the idea that mainstream music is inherently inferior and lacks creativity and authenticity.");
		v.AddNeg("They can also create a sense of pressure and competition among artists, with the focus being on conforming to the cliches of being \"different\" and \"pushing boundaries,\" rather than creating meaningful and authentic music.");
		v.AddNeg("Ultimately, these stereotypes limit the potential and impact of music, reducing it to a narrow and superficial standard of being \"unique and challenging the mainstream.\"");
		v.AddNeg("They also ignore the complexity and diversity within the music industry, failing to recognize the multitude of ways in which an artist can be authentic and unique.");
		v.AddNeg("These stereotypes can also create a sense of exclusion and discrimination towards certain voices and sounds in the music industry, perpetuating the idea that they do not fit into the narrow definition of being \"innovative\" and \"boundless.\"");
		v.AddNeg("The constant focus on this cliche can also overshadow an artist's true intentions and message, reducing their music to just an act rather than genuine creative expression.");
		v.AddNeg("Ultimately, these stereotypes fail to capture the true essence and power of music, reducing it to a shallow and limited idea of what it means to be \"unique and authentic.\"");
		v.AddNeg("The pressure to be constantly \"different\" and \"pushing boundaries\" can also hinder genuine artistic growth and expression, as artists may feel limited to conforming to a certain image rather than pursuing their own creative vision.");
		v.AddNeg("These stereotypes also perpetuate the damaging idea that success and recognition can only come from conforming to certain cliches, rather than expressing oneself genuinely.");
		v.AddNeg("Ultimately, these stereotypes limit the true potential and beauty of music, reducing it to a shallow and narrow standard of what it means to be unique and authentic.");
		
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "country: embracing simple pleasures";
		v.AddPos("People who live in small towns are portrayed as friendly, kind, and welcoming.");
		v.AddPos("Simple acts, such as enjoying a warm cup of coffee, can bring a lot of joy and comfort in life.");
		v.AddPos("The beauty of nature can be found in the simplicity of a small town, away from the hustle and bustle of the city.");
		v.AddPos("The slower pace of life in a small town allows for time to appreciate the little things and find happiness in them.");
		v.AddPos("Taking a moment to pause and enjoy the small things in life can bring a sense of contentment and peace.");
		v.AddPos("Embracing simple pleasures can remind us to slow down and appreciate the present moment.");
		v.AddPos("The simplicity of small town living can bring a sense of community and strong relationships with others.");
		v.AddPos("The simple joys of life, such as watching the sunset or stargazing, can bring a sense of wonder and appreciation for the world around us.");
		v.AddPos("The warmth and coziness of a cup of coffee on a chilly morning can provide a sense of familiarity and comfort.");
		v.AddPos("Simple pleasures can be found in everyday routines, reminding us to find joy in the little moments.");
		v.AddPos("The familiar and relaxed atmosphere of a small town can create a sense of belonging and peace for those who embrace it.");
		v.AddPos("Embracing simple pleasures can help us find happiness in simplicity and appreciate the beauty in ordinary things.");
		v.AddPos("The simplicity of life in a small town can make it easier to disconnect from technology and find true joy in the present moment.");
		v.AddPos("Taking time to appreciate the beauty of nature, whether in a small town or out in the wilderness, can bring a sense of calm and wonder.");
		v.AddPos("The simplicity of small town living can remind us to be grateful for what we have and find joy in the present rather than constantly seeking more. ");
		v.AddPos("The simple pleasures of life, like spending time with loved ones or enjoying a home-cooked meal, can bring a sense of warmth and contentment.");
		v.AddPos("In the chaos of the world, embracing simple pleasures can provide a sense of balance and grounding.");
		v.AddPos("The simplicity of small town living can teach us to slow down and savor the journey rather than constantly striving for the destination.");
		v.AddPos("The small moments of joy, like a child's laughter or a beautiful sunset, can bring a sense of pure happiness and gratitude.");
		v.AddPos("Simple pleasures can be found in the most unexpected places, reminding us to always look for the beauty in life.");
		v.AddPos("Embracing the simplicity of small town life can teach us to appreciate the little things and find joy in simplicity.");
		v.AddPos("The comfort and familiarity of small town living can provide a sense of stability and grounding in an ever-changing world.");
		v.AddPos("Simple pleasures can bring a sense of nostalgia and remind us of the joys of childhood innocence.");
		v.AddPos("In a world that values and glorifies material possessions, embracing simple pleasures can remind us that true happiness cannot be bought.");
		v.AddPos("The warmth and coziness of small town living can bring a sense of peace and serenity in the midst of chaos.");
		v.AddPos("Embracing simple pleasures can help us find contentment and happiness in the present rather than constantly chasing after future goals.");
		v.AddPos("The simple, uncomplicated lifestyle of a small town can teach us valuable lessons about the importance of slowing down and enjoying life.");
		v.AddPos("The comfort of a warm cup of coffee on a chilly morning can serve as a simple reminder to appreciate the little things in life.");
		v.AddPos("In a fast-paced world, embracing simple pleasures can help us find a sense of balance and peace.");
		v.AddPos("The small town atmosphere can create a sense of community and connectedness, reminding us of the importance of human relationships and social connections.");
		v.AddPos("Embracing simple pleasures can help us find happiness and contentment even in the most difficult of times.");
		v.AddPos("The simplicity of small town living can bring a sense of humility and appreciation for the simple things in life.");
		v.AddPos("Simple pleasures can be a form of self-care, allowing us to slow down and recharge in a hectic world.");
		v.AddPos("In the digital age, embracing simple pleasures can serve as a way to disconnect and find joy in the offline world.");
		v.AddPos("The simple joys of life, like enjoying a meal with loved ones or taking a walk in nature, can bring a sense of fulfillment and gratitude.");
		v.AddPos("Embracing simple pleasures can serve as a reminder to live in the moment and appreciate the present rather than dwelling on the past or worrying about the future.");
		v.AddPos("The peacefulness and tranquility of small town living can bring a sense of calm and grounding in a chaotic world.");
		v.AddPos("Simple pleasures can be found in the most mundane tasks, reminding us to find joy and purpose even in everyday routines.");
		v.AddPos("Embracing the simplicity of small town life can help us find contentment and happiness within ourselves rather than seeking it from external sources.");
		v.AddPos("The simple joys of life can serve as a reminder to be grateful and appreciative for what we have rather than constantly striving for more.");
		v.AddPos("In a world that values material wealth and status, embracing simple pleasures can remind us of the true value of human connections and the beauty of the natural world.");
		v.AddPos("The simplicity of small town living can bring a sense of nostalgia and remind us of the beauty of simpler times.");
		v.AddPos("Embracing simple pleasures can help us find happiness and fulfillment in the present rather than constantly chasing after future goals.");
		v.AddPos("The warmth and comfort of a small town can serve as a reminder to slow down and appreciate the present, rather than always striving for the next big thing.");
		v.AddPos("In the chaos and stress of daily life, embracing simple pleasures can serve as a form of self-care, bringing a sense of calm and relaxation.");
		v.AddPos("The simple, uncomplicated lifestyle of a small town can remind us of the value and beauty of living in the moment.");
		v.AddPos("Embracing simple pleasures can help us find contentment and happiness even in the most challenging of circumstances.");
		v.AddPos("The familiarity and simplicity of small town living can create a sense of nostalgia and remind us of our roots and the importance of where we come from.");
		v.AddPos("Simple pleasures can bring a sense of joy and wonder to everyday life, reminding us to find beauty in even the most mundane tasks.");
		v.AddPos("Embracing the simplicity of small town life can help us find peace and balance in a world that often feels overwhelming and chaotic.");
		v.AddPos("The simple acts of kindness and community in a small town can serve as a reminder of the goodness in people and the power of human connections.");
		v.AddPos("Simple pleasures can bring a sense of authenticity and grounding in a world that often values external appearances and material possessions.");
		v.AddPos("In a fast-paced and constantly changing world, embracing simple pleasures can serve as an anchor, bringing a sense of stability and familiarity.");
		v.AddPos("The simplicity of small town living can teach us to appreciate the little things and find happiness and contentment in the present rather than always chasing after the next big thing.");
		v.AddPos("Embracing simple pleasures can remind us to slow down and enjoy the journey, rather than constantly striving for the end goal.");
		v.AddPos("The warmth and comfort of small town living can bring a sense of belonging and community, reminding us of the value of human relationships and social connections.");
		v.AddPos("Simple pleasures can serve as a reminder to be present and in the moment, rather than constantly distracted by technology and social media.");
		v.AddPos("Embracing the simplicity of small town life can help us find joy and contentment in the present rather than always seeking more.");
		v.AddPos("The simple acts of kindness and generosity often found in small towns can serve as a reminder of the beauty and goodness in the world.");
		v.AddPos("Simple pleasures can be found in unexpected places, reminding us to always be open to new experiences and appreciate the beauty in everyday life.");
		v.AddPos("In a culture that often values work and productivity above all else, embracing simple pleasures can serve as a reminder to slow down and enjoy life.");
		v.AddPos("The simplicity of small town living can teach us valuable lessons about finding joy and fulfillment in even the most mundane tasks.");
		v.AddPos("Embracing simple pleasures can help us find happiness and contentment within ourselves rather than constantly seeking it from external sources.");
		v.AddPos("The comfort and familiarity of small town living can serve as a source of stability and grounding in an ever-changing world.");
		v.AddPos("Simple pleasures can bring a sense of nostalgia and remind us of the simple joys of childhood.");
		v.AddPos("Embracing simple pleasures can remind us to be grateful and appreciative of the little things in life, rather than constantly striving for more.");
		v.AddPos("The peacefulness and tranquility of small town living can bring a sense of calm and balance to our often hectic lives.");
		v.AddPos("Simple pleasures can serve as a reminder to savor the present moment, rather than getting caught up in worries about the future or regrets about the past.");
		v.AddPos("In a world that often values material possessions and external success, embracing simple pleasures can remind us of the true importance of inner peace and contentment.");
		v.AddPos("The simplicity of small town living can teach us to appreciate the beauty in simplicity and find joy in the little things in life.");
		v.AddPos("Embracing simple pleasures can help us find balance and happiness in a fast-paced and constantly changing world.");
		v.AddPos("The comfort and warmth of small town living can serve as a reminder to slow down and appreciate the present rather than constantly chasing after future goals.");
		v.AddPos("Simple pleasures can bring a sense of wonder and awe, reminding us of the beauty and magic of the world.");
		v.AddPos("Embracing the simplicity of small town life can teach us valuable lessons about the importance of human connections and relationships.");
		v.AddPos("The simple act of enjoying a meal with loved ones or taking a walk in nature can bring a sense of happiness and contentment.");
		v.AddPos("In a busy and stressful world, embracing simple pleasures can serve as a form of self-care, reminding us to take time for ourselves and recharge.");
		v.AddPos("The simple, uncomplicated lifestyle of a small town can remind us of the value of living in the moment and finding joy in the present.");
		v.AddPos("Embracing simple pleasures can help us find peace and contentment even in the midst of chaos and uncertainty.");
		v.AddPos("The simplicity of small town living can bring a sense of authenticity and remind us of the beauty in being true to ourselves and our roots.");
		v.AddPos("Simple pleasures can be a source of joy and happiness in everyday life, reminding us to find joy in the journey rather than focusing solely on the destination.");
		v.AddPos("In a world that often values productivity and efficiency above all else, embracing simple pleasures can serve as a reminder to slow down and enjoy life.");
		v.AddPos("The warmth and comfort of a small town can bring a sense of belonging and community, creating strong bonds and relationships with others.");
		v.AddPos("Simple pleasures can remind us to find beauty and joy in everyday routines and tasks, rather than constantly seeking out new and exciting experiences.");
		v.AddPos("Embracing the simplicity of small town life can help us appreciate the little things and find happiness in the present moment.");
		v.AddPos("The familiarity and simplicity of small town living can bring a sense of nostalgia and comfort in a busy and ever-changing world.");
		v.AddPos("Simple pleasures can be found in unexpected moments and experiences, reminding us to savor the present and be open to new opportunities.");
		v.AddPos("In a society that often values material possessions and external success, embracing simple pleasures can remind us of the true value of human connections and inner peace.");
		v.AddPos("The simplicity of small town living can teach us to slow down and appreciate the journey, rather than constantly striving for the destination.");
		v.AddPos("Embracing simple pleasures can help us find joy and contentment in the present rather than constantly chasing after future goals.");
		v.AddPos("The warmth and coziness of a small town can bring a sense of peace and comfort in a chaotic world.");
		v.AddPos("Simple pleasures can serve as a reminder to be present and in the moment, rather than letting our thoughts and worries consume us.");
		v.AddPos("In a world that often emphasizes competition and achievement, embracing simple pleasures can bring a sense of balance and remind us of the beauty in simplicity.");
		v.AddPos("The simplicity of small town living can teach us the value of slowing down and enjoying the little moments in life.");
		v.AddPos("Embracing simple pleasures can help us find happiness and fulfillment in even the most ordinary of tasks.");
		v.AddPos("The comfort and familiarity of small town living can serve as a reminder to appreciate our roots and where we come from.");
		v.AddPos("Simple pleasures can bring a sense of joy and wonder to our lives, reminding us to never lose our sense of childlike wonder.");
		v.AddPos("In a world that often values external appearances and material possessions, embracing simple pleasures can serve as a reminder of the true value of human connections and inner peace.");
		v.AddPos("The peacefulness and tranquility of small town living can bring a sense of calm and grounding in the midst of chaos and stress.");
		v.AddPos("Simple pleasures can be found in everyday routines and tasks, reminding us to find joy in the little things in life.");
		v.AddPos("Embracing the simplicity of small town life can help us find happiness and contentment within ourselves, rather than seeking it from external sources.");
		v.AddPos("The simple, uncomplicated lifestyle of a small town can remind us to appreciate the present moment and find joy in the little things in life.");
		v.AddPos("Simple pleasures can help us take a step back from the chaos of life and appreciate the beauty and simplicity around us.");
		v.AddPos("In a society that often values constant productivity and achievement, embracing simple pleasures can serve as a reminder to slow down and enjoy life.");
		v.AddPos("The warmth and comfort of small town living can bring a sense of belonging and community, reminding us of the importance of human connections.");
		v.AddPos("Simple pleasures can serve as a reminder to find joy and contentment in the present rather than constantly chasing after future goals.");
		v.AddPos("The familiarity and simplicity of small town living can bring a sense of nostalgia and comfort in a rapidly changing world.");
		v.AddPos("Embracing simple pleasures can help us find balance and peace in the midst of a busy and stressful life.");
		v.AddPos("The simplicity of small town living can teach us the importance of finding joy and fulfillment in everyday moments and experiences.");
		v.AddPos("Simple pleasures can bring a sense of warmth and coziness to our lives, reminding us to slow down and enjoy the simple things.");
		v.AddPos("In a world that often values constant advancement and progress, embracing simple pleasures can serve as a reminder to appreciate the present moment and find fulfillment in the journey.");
		v.AddPos("The beauty of nature and the small-town lifestyle can serve as a reminder to appreciate the natural world and its wonders.");
		v.AddPos("Embracing the simplicity of small town life can teach us valuable lessons about the importance of human connections and relationships.");
		v.AddPos("The simple joys of life, such as spending time with loved ones or enjoying a home-cooked meal, can bring a sense of warmth and contentment.");
		v.AddPos("Simple pleasures can be a source of peace and tranquility in an often chaotic world, reminding us to slow down and enjoy life.");
		v.AddPos("In a society that often values external appearances and material possessions, embracing simple pleasures can remind us of the importance of inner peace and contentment.");
		v.AddPos("The simplicity of small town living can remind us to slow down and appreciate the present moment rather than focusing solely on future goals.");
		v.AddPos("Embracing simple pleasures can help us find happiness and contentment in the present rather than constantly chasing after external sources of validation.");
		v.AddPos("The comfort and familiarity of small town living can bring a sense of nostalgia and remind us of the importance of our roots and heritage.");
		v.AddPos("Simple pleasures can serve as a reminder to be grateful and appreciative for what we have, rather than constantly striving for more.");
		v.AddPos("In a world that often values productivity and efficiency above all else, embracing simple pleasures can help us find balance and joy in the present moment.");
		v.AddPos("The simple, uncomplicated lifestyle of a small town can teach us valuable lessons about finding happiness and fulfillment in the little things in life.");
		v.AddPos("Simple pleasures can bring a sense of tranquility and remind us to slow down and appreciate the beauty around us.");
		v.AddPos("Embracing the simplicity of small town life can remind us to live in the moment and find happiness and contentment within ourselves rather than seeking it from external sources.");
		v.AddPos("The warmth and coziness of small town living can serve as a reminder of the importance of self-care and taking time for ourselves in a busy world.");
		v.AddPos("Simple pleasures can bring a sense of authenticity and remind us to stay true to ourselves and our values.");
		v.AddPos("In a fast-paced and constantly changing world, embracing simple pleasures can serve as a source of grounding and stability.");
		v.AddPos("The simplicity of small town living can teach us to find joy and fulfillment in the present moment rather than constantly striving for the next big accomplishment.");
		v.AddPos("Embracing simple pleasures can help us appreciate the beauty in simplicity and find happiness in the little things in life.");
		v.AddPos("The familiar and welcoming atmosphere of a small town can bring a sense of belonging and community, reminding us of the value of human connections.");
		v.AddPos("Simple pleasures can be found in everyday activities and tasks, reminding us to find joy in the mundane and ordinary moments.");
		v.AddPos("In a world that often values material possessions and external success, embracing simple pleasures can serve as a reminder of the true value of inner peace and contentment.");
		v.AddPos("The simplicity of small town living can remind us to slow down and appreciate the present moment, rather than constantly worrying about the future or dwelling on the past.");
		v.AddPos("Embracing simple pleasures can help us find balance and contentment in a world that often focuses on constant achievement and advancement.");
		v.AddPos("The warmth and comfort of small-town living can bring a sense of nostalgia and remind us of the simple joys of childhood and home.");
		v.AddPos("Simple pleasures can serve as a reminder to be present and in the moment, rather than constantly being distracted and consumed by technology.");
		v.AddNeg("People in small towns are often portrayed as close-minded and unadventurous, content with staying in their familiar surroundings.");
		v.AddNeg("The comfort of routine and familiarity in a small town is seen as boring and unexciting.");
		v.AddNeg("The simple pleasures of a small town are often regarded as inferior to the more glamorous and exciting lifestyle of big cities.");
		v.AddNeg("Embracing simple pleasures is seen as settling for less and lacking ambition.");
		v.AddNeg("The simplicity of small town life is portrayed as dull and monotonous.");
		v.AddNeg("People who enjoy simple pleasures are often viewed as unsophisticated and uncultured.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes depicted as a sign of stagnation and lack of progress.");
		v.AddNeg("Embracing simple pleasures is viewed as a form of escapism from the more complex and challenging aspects of life.");
		v.AddNeg("The focus on simple pleasures is often seen as a rejection of material wealth and success.");
		v.AddNeg("Simple pleasures are portrayed as a consolation prize for those who aren't able to achieve their dreams and ambitions.");
		v.AddNeg("The joys of a small town and the comfort of a warm cup of coffee are sometimes belittled and dismissed as insignificant by those who prioritize material possessions and status.");
		v.AddNeg("The simplicity of small town life is often portrayed as backwards and outdated compared to the fast-paced, urban lifestyle.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as settling for a mediocre existence rather than striving for greatness.");
		v.AddNeg("People who enjoy simple pleasures are sometimes seen as naive and out of touch with reality.");
		v.AddNeg("The small town lifestyle is often depicted as lacking diversity and cultural experiences compared to living in a city.");
		v.AddNeg("Embracing simple pleasures is seen as a sign of giving up on chasing bigger and better things in life.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes viewed as inferior to the excitement and glamour of city life, reinforcing the stereotype of small towns as boring and uneventful.");
		v.AddNeg("The comfort and simplicity of small town living is sometimes portrayed as a sign of laziness and lack of ambition.");
		v.AddNeg("Embracing simple pleasures is seen as settling for a less fulfilling life instead of striving for more.");
		v.AddNeg("Small town residents are often depicted as being stuck in their ways and resistant to change, leading to a negative portrayal of embracing simple pleasures.");
		v.AddNeg("The simplicity of small town life is sometimes seen as unfulfilling and lacking in excitement and adventure.");
		v.AddNeg("People who enjoy simple pleasures are sometimes viewed as being content with mediocrity instead of striving for excellence.");
		v.AddNeg("The comfort and coziness of small town living is sometimes portrayed as a sign of being closed off from the rest of the world and lacking in diversity.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as a form of laziness and avoiding responsibility.");
		v.AddNeg("The joy and contentment found in simple pleasures is sometimes dismissed as not being enough to bring true happiness and fulfillment in life.");
		v.AddNeg("People who embrace simple pleasures are sometimes viewed as being out of touch with modern society and its values.");
		v.AddNeg("Small town residents are sometimes depicted as being unsatisfied and longing for a more exciting and glamorous lifestyle.");
		v.AddNeg("The simplicity of small town living is sometimes seen as a lack of sophistication and cultural experiences.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as settling for a mundane and uneventful life, reinforcing the stereotype of small towns as being dull and uninteresting.");
		v.AddNeg("The comfort and familiarity of small town life is sometimes dismissed as being unambitious and unwilling to take risks.");
		v.AddNeg("People who enjoy simple pleasures are sometimes viewed as inferior to those who prioritize ambition and success.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes seen as insignificant in the grand scheme of things, reinforcing the stereotype of small towns as being unimportant and insignificant.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as a lack of ambition and drive to achieve more in life.");
		v.AddNeg("The simplicity of small town life is sometimes portrayed as a lack of sophistication and culture compared to city life.");
		v.AddNeg("People who enjoy simple pleasures are sometimes viewed as being content with a basic and unexciting existence, rather than seeking out new experiences and challenges.");
		v.AddNeg("The comfort and warmth of a small town and a warm cup of coffee is sometimes seen as a sign of being stuck in one's comfort zone and unwilling to take risks.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as a form of escapism from the more complex and challenging aspects of life, reinforcing negative stereotypes of small town living as being mundane and unfulfilling.");
		v.AddNeg("The joy and contentment found in simple pleasures is sometimes dismissed as being inferior to the pursuit of wealth and status.");
		v.AddNeg("Small town residents are sometimes portrayed as lacking in ambition and drive compared to city dwellers.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being resistant to change and lacking in progress.");
		v.AddNeg("People who enjoy simple pleasures are sometimes seen as naive and out of touch with the harsh realities of the world.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes belittled as being insignificant and unimportant compared to achieving success and wealth.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as settling for less and not striving for greater achievements in life.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes depicted as being unsatisfying and unfulfilling compared to the excitement and opportunities of living in a big city.");
		v.AddNeg("People who embrace simple pleasures are sometimes seen as being content with a mediocre existence rather than seeking out greater ambitions and achievements in life.");
		v.AddNeg("The comfort and simplicity of small town living is sometimes portrayed as being stagnant and lacking in growth and development.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as a form of laziness and avoidance of responsibility and hard work.");
		v.AddNeg("The simple pleasures of small town life are sometimes seen as being insignificant and unimportant in comparison to the challenges and rewards of big city living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes depicted as being out of touch with modern society and its values.");
		v.AddNeg("The comforts and joys of small town life are sometimes portrayed as being outdated and lacking in excitement and opportunities.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as a sign of giving up on one's dreams and desires, instead choosing a mundane and uneventful existence.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being unadventurous and unfulfilling compared to the excitement and opportunities of city living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes depicted as being content with a mediocre existence rather than striving for greater growth and success.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes portrayed as being stagnant and lacking in progress and change.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as being unambitious and not striving for greater achievements and success in life.");
		v.AddNeg("The simplicity of small town life is sometimes portrayed as being lacking in sophistication and culture compared to city living.");
		v.AddNeg("People who embrace simple pleasures are sometimes viewed as being naive and unsophisticated, choosing to live a simpler and less fulfilling life.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes dismissed as being insignificant and unimportant compared to material wealth and status.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as settling for less and not striving for greater achievements and success in life.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes seen as being inferior to the more exciting and glamorous lifestyle of big city living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes depicted as being content with a basic and uneventful existence, instead of pursuing bigger goals and ambitions.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes portrayed as being stagnant and lacking in progress and growth.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as a form of escapism from the challenges and complexities of life, instead choosing to live a simpler and easier existence.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes belittled as being insignificant and unimportant compared to the achievements and success of big city living.");
		v.AddNeg("People who embrace simple pleasures are sometimes seen as being out of touch with modern society and its values.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being unexciting and lacking in adventure and opportunities.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as settling for less and not striving for greater growth and success in life.");
		v.AddNeg("The simple pleasures of small town life are sometimes seen as being insignificant and unimportant compared to the hustle and bustle of city living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes depicted as being content with a mundane and uneventful existence, rather than seeking out new experiences and challenges.");
		v.AddNeg("The comfort and simplicity of small town living is sometimes portrayed as a lack of ambition and drive, instead choosing to settle for a simpler and less fulfilling life.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as being unadventurous and not willing to take risks and pursue greater achievements and success in life.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes viewed as being inferior to the more exciting and glamorous lifestyle of the city.");
		v.AddNeg("People who embrace simple pleasures are sometimes portrayed as being content with a basic and uneventful existence, instead of striving for greater ambition and success in life.");
		v.AddNeg("The joy and comfort found in simple pleasures is sometimes belittled as being insignificant and unimportant in comparison to material wealth and status.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as choosing a less demanding and less fulfilling life, rather than taking on challenges and pursuing greater achievements.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being unadventurous and lacking in excitement and opportunities.");
		v.AddNeg("People who enjoy simple pleasures are sometimes depicted as being unsatisfied and longing for a more glamorous and exciting lifestyle.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes seen as inferior to the constant stimulation and novelty of city living.");
		v.AddNeg("The comfort and warmth of a small town and a warm cup of coffee is sometimes portrayed as being resistant to change and averse to new experiences, instead choosing to stay in one's comfort zone.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as a sign of being unwilling to take risks and try new things, instead opting for a more predictable and easygoing lifestyle.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes portrayed as being inferior to the excitement and opportunities of big city living.");
		v.AddNeg("People who embrace simple pleasures are sometimes depicted as being content with a mundane and uneventful existence, instead of pursuing bigger goals and ambitions.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes seen as being content with mediocrity and lacking in ambition, instead choosing to live a simpler and less demanding life.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as a form of escapism from the challenges and complexities of life, instead choosing to embrace a simpler and more carefree lifestyle.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being unambitious and lacking in drive, instead choosing to settle for a more relaxed and unchallenging way of life.");
		v.AddNeg("People who enjoy simple pleasures are sometimes seen as being naive and out of touch with the harsh realities of the world, instead choosing to focus on the simpler and more pleasant aspects of life.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes belittled as being insignificant and unimportant compared to the constant stimulation and novelty of city living.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as choosing a less demanding and less fulfilling life, instead opting for a more predictable and easygoing lifestyle.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes viewed as being inferior to the excitement and opportunities of big city living.");
		v.AddNeg("People who embrace simple pleasures are sometimes depicted as being content with a basic and uneventful existence, rather than seeking out new experiences and challenges.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes seen as being stagnant and lacking in progress and growth, instead choosing to stay in one's comfort zone.");
		v.AddNeg("Embracing simple pleasures is sometimes viewed as a sign of being complacent and settling for a mundane and unchallenging life.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes portrayed as being unsatisfying and unsophisticated, instead focusing on a more simplistic and traditional way of living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes seen as being out of touch with modern society and its values, instead choosing a more traditional and conventional lifestyle.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes dismissed as being insignificant and unimportant compared to the constant stimulation and novelty of city living.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as a form of escapism from the challenges and complexities of life, instead choosing to live a more carefree and laid-back lifestyle.");
		v.AddNeg("The comfort and tranquility of small town living is sometimes seen as being resistant to change and unwilling to embrace new experiences, instead opting for a more familiar and comfortable way of life.");
		v.AddNeg("People who embrace simple pleasures are sometimes viewed as being content with mediocrity and lacking in ambition, instead choosing to lead a simpler and less demanding life.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes portrayed as being unexciting and lacking in adventure and opportunities, instead focusing on a more traditional and conservative way of living.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as being unwilling to take risks and try new things, instead choosing a more predictable and comfortable lifestyle.");
		v.AddNeg("The joys of a small town and simple pleasures are sometimes belittled as being unfulfilling and mediocre compared to the excitement and opportunities of city living.");
		v.AddNeg("People who enjoy simple pleasures are sometimes portrayed as being content with a basic and uneventful existence, rather than seeking out new challenges and pursuing greater ambition and success in life.");
		v.AddNeg("The comfort and simplicity of small town living is sometimes viewed as being stagnant and lacking in growth and development, instead choosing a more routine and predictable way of life.");
		v.AddNeg("Embracing simple pleasures is sometimes portrayed as choosing a less demanding and less fulfilling life, instead settling for a more relaxed and unchallenging lifestyle.");
		v.AddNeg("The simplicity and comforts of small town life are sometimes seen as being inferior to the excitement and opportunities of big city living, reinforcing the stereotype of small towns as being boring and uneventful.");
		v.AddNeg("People who embrace simple pleasures are sometimes depicted as being content with mediocrity and lacking in ambition, instead choosing to live a simpler and less demanding life.");
		v.AddNeg("The comfort and warmth of a small town and a warm cup of coffee is sometimes portrayed as being resistant to change and averse to new experiences, instead choosing to stay in one's comfort zone.");
		v.AddNeg("Embracing simple pleasures is sometimes seen as a sign of being unwilling to take risks and try new things, instead opting for a more predictable and easygoing lifestyle.");
		v.AddNeg("The simplicity and familiarity of small town life is sometimes viewed as being unambitious and lacking in drive, instead choosing to settle for a more relaxed and unchallenging way of life.");
		v.Sort();
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	{
		ProductionIdeaTopic& v = pv.Add();
		v.title = "";
		
		
	}
	ArrayCtrl stereotypes[2];
	ArrayCtrl storytelling[2];
	ArrayCtrl perspectives[2];
	ArrayCtrl slurs[2];
	ArrayCtrl roles[2];
	return pv;
}



