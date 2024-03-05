#include "ToolCore.h"


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

int FindAttrGroup(const char* group) {
	// HOTFIX
	if (strncmp(group, "authenticity", 12) == 0)
		group = "authencity";
	for(int i = 0; i < ATTR_COUNT; i++) {
		if (strncmp(group, AttrKeys[i][1], 100) == 0)
			return i;
	}
	return -1;
}

bool FindAttrValue(int group_i, const char* value) {
	if (group_i < 0 || group_i >= ATTR_COUNT) return false;
	const char* cmp_value = AttrKeys[group_i][3];
	return strncmp(cmp_value, value, 100) == 0;
}

int FindAttrGroupByValue(const char* value) {
	for(int i = 0; i < ATTR_COUNT; i++) {
		if (strncmp(value, AttrKeys[i][2], 100) == 0 ||
			strncmp(value, AttrKeys[i][3], 100) == 0)
			return i;
	}
	return -1;
}

}


String GetAttrNotString(String positive_attr) {
	static hash_t pos_hashes[Attr::ATTR_COUNT];
	static hash_t neg_hashes[Attr::ATTR_COUNT];
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
	hash_t hash = positive_attr.GetHashValue();
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





/*void EditorPtrs::RealizePipe() {
	SongDatabase& db = SongDatabase::Single();
	
	if (!song || !artist)
		return;
	if (song->pipe)
		return;
	
	TaskMgr& pipe = db.pipes.Add();
	Artist& a = *artist;
	Release& r = *release;
	
	song->pipe = &pipe;
	pipe.song = song;
}*/

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

void RemoveEmptyLines(String& s) {
	s.Replace("\r","");
	Vector<String> lines = Split(s, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty())
			lines.Remove(i--);
	}
	s = Join(lines, "\n");
}

void RemoveEmptyLines2(String& s) {
	s.Replace("\r","");
	Vector<String> lines = Split(s, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	s = Join(lines, "\n");
}

void RemoveEmptyLines3(String& s) {
	s.Replace("\r","");
	Vector<String> lines = Split(s, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineChar(l);
		l = TrimBoth(l);
		if (l.IsEmpty())
			lines.Remove(i--);
	}
	s = Join(lines, "\n");
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

String GetProgramPartFromAbbr(const String& abbr) {
	TODO
	return "";
}

Color GetProgramPartPaperColor(const String& abbr) {
	TODO
	return Color();
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


void RemoveQuotes(String& s) {
	if (s.GetCount() > 0 && s[0] == '\"')
		s = s.Mid(1);
	int c = s.GetCount();
	if (c > 0 && s[c-1] == '\"')
		s = s.Left(c-1);
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









const char* WordGroupString[WORDGROUP_COUNT] = {
	"pronoun",
	"noun",
	"adjective",
	"prepositions",
	"conjunctions",
	"verbs"
};


void EscapeString(String& s) {
	s = ToCharset(CHARSET_ISO8859_15, s, CHARSET_UTF8);
	s = ToCharset(CHARSET_UTF8, s, CHARSET_ISO8859_15);
	s = StoreAsJson(s);
	RemoveQuotes(s);
}




void SplitParenthesisWords(const String& line, Vector<String>& words, Vector<bool>& parenthesis) {
	words.SetCount(0);
	parenthesis.SetCount(0);
	
	WString w = line.ToWString();
	
	WString tmp;
	bool is_parenthesis = false;
	for(int i = 0; i < w.GetCount(); i++) {
		int chr = w[i];
		
		if (IsSpace(chr)) {
			if (tmp.IsEmpty()) continue;
			words << tmp.ToString();
			parenthesis << is_parenthesis;
			tmp.Clear();
			is_parenthesis = false;
		}
		else {
			if (tmp.IsEmpty() && chr == '(') {
				is_parenthesis = true;
			}
			else {
				if (chr == ')' || chr == '(')
					continue;
				tmp.Cat(chr);
			}
		}
	}
	
	if (tmp.GetCount()) {
		words << tmp.ToString();
		parenthesis << is_parenthesis;
	}
}

void GetWords(const String& line, Vector<String>& words) {
	words.SetCount(0);
	
	WString w = line.ToWString();
	
	WString tmp;
	for(int i = 0; i < w.GetCount(); i++) {
		wchar chr = w[i];
		
		if (!IsLetter(chr) && !IsDigit(chr) && chr != '\'' && chr != '`' && chr != '-') {
		//if (IsSpace(chr) || chr == '.' || chr == ',' || chr == '?' || chr == '!' || chr == ':' || chr == ';') {
			if (tmp.IsEmpty()) continue;
			words << tmp.ToString();
			tmp.Clear();
		}
		else {
			tmp.Cat(chr);
		}
	}
	
	if (tmp.GetCount()) {
		words << tmp.ToString();
	}
}

void ReplaceWord(String& s, const String& orig_word, const String& replace_word) {
	String low_text = ToLower(s);
	
	int prev = -1;
	while (prev < low_text.GetCount()) {
		int a = low_text.Find(orig_word, prev+1);
		if (a < 0) break;
		
		bool left_separated = false, right_separated = false;
		
		if (a == 0)
			left_separated = true;
		else {
			int chr = low_text[a-1];
			if (IsAlpha(chr) ||IsLetter(chr) || IsDigit(chr) || chr == '\'' || chr == '-')
				;
			else
				left_separated = true;
		}
		
		if (left_separated) {
			int b = a + orig_word.GetCount();
			if (b >= low_text.GetCount())
				right_separated = true;
			else {
				int chr = low_text[b];
				if (IsAlpha(chr) ||IsLetter(chr) || IsDigit(chr) || chr == '\'' || chr == '-')
					;
				else
					right_separated = true;
			}
		}
		
		if (left_separated && right_separated) {
			s = s.Left(a) + replace_word + s.Mid(a + orig_word.GetCount());
			low_text = low_text.Left(a) + replace_word + low_text.Mid(a + orig_word.GetCount());
			a += replace_word.GetCount()-1;
		}
		
		prev = a;
	}
}



void HotfixReplaceWord(WString& ws) {
	String s = ws.ToString();
	HotfixReplaceWord(s);
	ws = s.ToWString();
}

void HotfixReplaceWord(String& s) {
	ReplaceWord(s, "im", "I'm");
	ReplaceWord(s, "ive", "I've");
	ReplaceWord(s, "ill", "I'll");
	ReplaceWord(s, "id", "I'd");
	ReplaceWord(s, "youre", "you're");
	ReplaceWord(s, "youd", "you'd");
	ReplaceWord(s, "youve", "you've");
	ReplaceWord(s, "youll", "you'll");
	ReplaceWord(s, "hes", "he's");
	ReplaceWord(s, "heve", "he've");
	ReplaceWord(s, "hed", "he'd");
	ReplaceWord(s, "shes", "she's");
	ReplaceWord(s, "sheve", "she've");
	ReplaceWord(s, "shed", "she'd");
	ReplaceWord(s, "theyll", "they'll");
	ReplaceWord(s, "theyve", "they've");
	ReplaceWord(s, "theyre", "they're");
	
	ReplaceWord(s, "arent", "aren't");
	ReplaceWord(s, "aint", "ain't");
	ReplaceWord(s, "didnt", "didn't");
	ReplaceWord(s, "dont", "don't");
	
	ReplaceWord(s, "its", "it's");
	ReplaceWord(s, "itll", "it'll");
	ReplaceWord(s, "itve", "it've");
	ReplaceWord(s, "isnt", "isn't");
	
	ReplaceWord(s, "whats", "what's");
	ReplaceWord(s, "couldnt", "couldn't");
	ReplaceWord(s, "shouldnt", "shouldn't");
	ReplaceWord(s, "theres", "there's");
	ReplaceWord(s, "wasnt", "wasn't");
	ReplaceWord(s, "thats", "that's");
	
	if (0) {
		ReplaceWord(s, "alright", "allright");
		// These change too much
		if (0) {
			ReplaceWord(s, "tryna", "tring to");
			ReplaceWord(s, "aint", "aren't");
			ReplaceWord(s, "gotta", "have to");
			ReplaceWord(s, "wanna", "want to");
			ReplaceWord(s, "em", "them");
			ReplaceWord(s, "ol", "old");
			ReplaceWord(s, "bout", "about");
			ReplaceWord(s, "nunya", "none of your");
			ReplaceWord(s, "thang", "thing");
			ReplaceWord(s, "I'ma", "I'll");
		}
		
		ReplaceWord(s, "tryin", "trying");
		ReplaceWord(s, "fuckin", "fucking");
		ReplaceWord(s, "livin", "living");
		ReplaceWord(s, "lookin", "looking");
		ReplaceWord(s, "prayin", "praying");
		ReplaceWord(s, "rollin", "rolling");
		ReplaceWord(s, "workin", "working");
		ReplaceWord(s, "chargin", "charging");
		ReplaceWord(s, "runnin", "running");
		ReplaceWord(s, "doin", "doing");
		ReplaceWord(s, "judgin", "judging");
		ReplaceWord(s, "blendin", "blending");
		ReplaceWord(s, "gettin", "getting");
		ReplaceWord(s, "talkin", "talking");
		ReplaceWord(s, "changin", "changing");
		ReplaceWord(s, "makin", "making");
		ReplaceWord(s, "retracin", "retracing");
		ReplaceWord(s, "motherfuckin", "motherfucking");
		ReplaceWord(s, "rockin", "rocking");
		ReplaceWord(s, "goin", "going");
		ReplaceWord(s, "frontin", "fronting");
		ReplaceWord(s, "somethin", "something");
		ReplaceWord(s, "playin", "playing");
		ReplaceWord(s, "hittin", "hitting");
		ReplaceWord(s, "movin", "moving");
	}
}


/*const char* ScoreTitles[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {
	// Statistical measurements
	{
		"Idea",
		"Emotion",
		"Hook",
		"Share",
		"Value"
	},
	// Human value measurements (meaning)
	{
		"Comedy",
		"Sex",
		"Politics",
		"Love",
		"Social issues"
	}
};

String GetScoreKey(int score_mode, int score_attr) {
	if (!score_mode)
		return "sc(" + IntStr(score_attr) + ")";
	else
		return "sc(" + IntStr(score_mode) + "," + IntStr(score_attr) + ")";
}*/

const char* ScoreTitles[SCORE_COUNT] = {
	// Statistical measurements
	
		"Idea",
		"Emotion",
		"Hook",
		"Share",
		"Value",
	
	// Human value measurements (meaning)
	
		"Comedy",
		"Sex",
		"Politics",
		"Love",
		"Social issues"
	
};

String GetScoreKey(int score) {
	return "sc(" + IntStr(score) + ")";
}




hash_t HexHash(const String& s) {
	const int count = sizeof(hash_t);
	static_assert(count == 8, "64bit supported only (so far)");
	union {
		hash_t out;
		byte b[sizeof(hash_t)];
	};
	int sep = min(count*2, s.GetCount());
	ASSERT(sep % 2 == 0);
	sep -= sep % 2;
	
	out = 0;
	byte *t = b;
	for(int q = 0; q < sep; q+=2) {
		byte l = s[q+0];
		byte h = s[q+1];
		
		if (l >= '0' && l <= '9') l -= '0';
		else if (l >= 'a' && l <= 'f') l = l - 'a' + 10;
		else l = 10 + l - 'A';
		
		if (h >= '0' && h <= '9') h -= '0';
		else if (h >= 'a' && h <= 'f') h = h - 'a' + 10;
		else h = 10 + h - 'A';
		
		*t++ = l | (h << 4);
	}
	return out;
}

String HashHex(hash_t h) {
	String s;
	s.Reserve(sizeof(hash_t)*2);
	union {
		hash_t in;
		byte b[sizeof(hash_t)];
	};
	in = h;
	int count = sizeof(hash_t);
	byte *t = b;
	for(int i = 0; i < count; i++) {
		byte l = b[i] & 0xF;
		byte h = (b[i] & 0xF0) >> 4;
		
		if (l >= 0 && l <= 9) s.Cat('0' + l);
		else s.Cat('a' + l - 10);
		
		if (h >= 0 && h <= 9) s.Cat('0' + h);
		else s.Cat('a' + h - 10);
	}
	return s;
}

int FindNonEscaped(const String& s, const String& search) {
	int a = 0;
	while (true) {
		a = s.Find(search, a);
		if (a < 0)
			return -1;
		else if (a == 0) {
			return a;
		}
		else {
			char prev = s[a-1];
			if (prev == '\\') {
				if (a >= 2 && s[a-2] == '\\')
					return a;
				a++;
				continue;
			}
			else
				return a;
		}
	}
	return -1;
}

int FindNonEscaped(const String& s, const String& search, int begin) {
	int a = begin;
	while (true) {
		a = s.Find(search, a);
		if (a < 0)
			return -1;
		else if (a == 0) {
			return a;
		}
		else {
			char prev = s[a-1];
			if (prev == '\\') {
				if (a >= 2 && s[a-2] == '\\')
					return a;
				a++;
				continue;
			}
			else
				return a;
		}
	}
	return -1;
}

int FindNonEscaped(const WString& s, const WString& search, int begin) {
	int a = begin;
	while (true) {
		a = s.Find(search, a);
		if (a < 0)
			return -1;
		else if (a == 0) {
			return a;
		}
		else {
			char prev = s[a-1];
			if (prev == '\\') {
				if (a >= 2 && s[a-2] == '\\')
					return a;
				a++;
				continue;
			}
			else
				return a;
		}
	}
	return -1;
}

const Index<String>& GetRoles() {
	static Index<String> idx;
	TODO
	return idx;
}

int GetRoleCount() {
	return GetRoles().GetCount();
}

const Vector<GenericType>& GetGenerics() {
	static Vector<GenericType> v;
	TODO
	return v;
}

int GetGenericCount() {
	return GetGenerics().GetCount();
}

const Vector<String>& GetGenericParts() {
	static Vector<String> v;
	TODO
	return v;
}

const Index<String>& GetTypecasts() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
		list.Add("Heartbroken/lovesick");
		list.Add("Rebel/anti-establishment");
		list.Add("Political activist");
		list.Add("Social justice advocate");
		list.Add("Party/club");
		list.Add("Hopeful/dreamer");
		list.Add("Confident/empowered");
		list.Add("Vulnerable/raw");
		list.Add("Romantic/love-driven");
		list.Add("Failure/loser");
		list.Add("Spiritual/faithful");
		list.Add("Passionate/determined");
		list.Add("Reflective/self-reflective");
		list.Add("Witty/sarcastic");
		list.Add("Melancholic/sad");
		list.Add("Humble/down-to-earth");
		list.Add("Charismatic/charming");
		list.Add("Resilient/overcoming adversity");
		list.Add("Carefree/joyful");
		list.Add("Dark/mysterious");
		list.Add("Comical/humorous");
		list.Add("Controversial/provocative");
		list.Add("Nostalgic/sentimental");
		list.Add("Wise/philosophical");
		list.Add("Angry/outspoken");
		list.Add("Calm/peaceful.");
		list.Add("Confident/self-assured");
		list.Add("Self-destructive/self-sabotaging");
		list.Add("Hopeful/optimistic");
		list.Add("Fearful/anxious");
		list.Add("Eccentric/quirky");
		list.Add("Sensitive/emotional");
		list.Add("Bitter/resentful");
		list.Add("Unique/nonconformist");
		list.Add("Free-spirited/nonconformist");
		list.Add("Sultry/seductive");
		list.Add("Inspirational/motivational");
		list.Add("Authentic/real");
		list.Add("Mysterious/enigmatic");
		list.Add("Carefree/bohemian");
		list.Add("Street-smart/tough");
		list.Add("Romantic/idealistic");
		list.Add("Nurturing/motherly");
		list.Add("Dark/tormented");
		list.Add("Remorseful/regretful");
		list.Add("Bold/brave");
		list.Add("Outcast/rebel");
		list.Add("Lost/disconnected");
		list.Add("Tough/badass");
		list.Add("Sincere/genuine");
		list.Add("Honest/vulnerable");
		list.Add("Innocent/naive");
		list.Add("Bold/risk-taking");
	}
	return list;
}

int GetTypecastCount() {
	return GetTypecasts().GetCount();
}

const Index<String>& GetProfiles() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
		// "singer is ..."
        list.Add("a third-party observer/commentator");
        list.Add("expressing personal emotions/thoughts");
        list.Add("conveying a message or lesson");
        list.Add("embodying a character or persona");
        list.Add("a storyteller or messenger for a community or culture");
        list.Add("reflecting on past experiences or memories");
        list.Add("interpreting or analyzing the lyrics for the listener");
        list.Add("challenging societal norms or addressing social issues");
        list.Add("invoking a particular mood or atmosphere through vocals");
        list.Add("having a dialogue with another singer");
        list.Add("having a dialogue with the audience");
        list.Add("using abstract or poetic language to convey feelings or ideas");
        list.Add("highlighting the beauty or poeticism of the lyrics through their performance");
        list.Add("asking questions and exploring different perspectives on the topic of the lyrics");
        list.Add("using irony or satire to convey a message or make a statement");
        list.Add("evoking nostalgia or longing through the lyrics");
        list.Add("using personal experiences to give depth and authenticity to the lyrics ");
        list.Add("using humor or wit to engage with the lyrics");
        list.Add("challenging the listener's perspective or beliefs with the lyrics");
        list.Add("using vocal techniques or styles to add layers of meaning to the lyrics");
        list.Add("creating a sense of intimacy or connection through their performance of the lyrics");
        list.Add("breaking societal expectations and norms through their interpretation of the lyrics");
        list.Add("offering a unique perspective on a commonly addressed topic in the lyrics");
        list.Add("creating a soundtrack or anthem for a specific group or community");
        list.Add("using repetition or emphasis to emphasize the importance of the lyrics");
        list.Add("using double entendres or wordplay to add depth and complexity to the lyrics ");
        list.Add("reflecting on personal growth or transformation through the lyrics");
        list.Add("embodying a specific emotion or feeling portrayed in the lyrics");
        list.Add("representing a marginalized or underrepresented group through the lyrics");
        list.Add("using imagery or metaphors to convey a deeper meaning behind the lyrics");
        list.Add("expressing vulnerability or raw emotion through the lyrics");
        list.Add("narrating a specific event or experience through the lyrics");
        list.Add("using the lyrics to convey a sense of empowerment or strength");
        list.Add("engaging in introspection and self-reflection through the lyrics");
        list.Add("confronting personal demons or struggles in the lyrics ");
        list.Add("using the lyrics to express social or cultural commentary");
	}
	return list;
}

int GetProfileCount() {
	return GetProfiles().GetCount();
}





const Index<String>& GetPrimary() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
        list.Add("15-18 years old male");
        list.Add("18-25 years old male");
        list.Add("25-35 years old male");
        list.Add("35-45 years old male");
        list.Add("45+ years old male");
        list.Add("15-18 years old female");
        list.Add("18-25 years old female");
        list.Add("25-35 years old female");
        list.Add("35-45 years old female");
        list.Add("45+ years old female");
        list.Add("Non-binary person");
        list.Add("Children/Youth");
        list.Add("Elderly individuals");
        list.Add("People from poor background");
        list.Add("People from middle class background");
        list.Add("People from rich background");
        list.Add("People going to the gym");
        list.Add("People going to the dance club");
        list.Add("People going to bars");
        list.Add("People dreaming about better life");
        list.Add("Outgoing people");
        list.Add("Introverted people");
        list.Add("People without a relationship");
        list.Add("People in a relationship");
        list.Add("Married people");
        list.Add("People falling in love");
        list.Add("People overcoming challenges");
        list.Add("People dealing with loss");
        list.Add("Nature loving people");
        list.Add("Travelling people");
        list.Add("People seeking justice or social change");
        list.Add("People exploring their identity or sexuality");
        list.Add("Individuals with mental health struggles");
        list.Add("Individuals with physical disabilities");
        list.Add("People in positions of power or leadership");
        list.Add("Depressed or anxious individuals");
        list.Add("Those experiencing a spiritual or religious awakening");
        list.Add("Individuals from a snowy-winter culture");
        list.Add("Individuals from a snowless-winter culture");
        list.Add("Artists or musicians");
        list.Add("Workers in food industry");
        list.Add("Workers in sex industry");
        list.Add("Workers in car industry");
        list.Add("Workers in house construction industry");
        list.Add("Workers in transportation industry");
        list.Add("Workers in healthcare industry");
        list.Add("Workers in IT industry");
        list.Add("Politicians");
        list.Add("Democratic Politicians");
        list.Add("Republican Politicians");
        list.Add("Police officers");
        list.Add("Teachers or educators");
        list.Add("Students (college, high school, etc.)");
        list.Add("Athletes");
        list.Add("Homeowners");
        list.Add("Single parents");
        list.Add("Divorced or separated individuals");
        list.Add("Retired individuals");
        list.Add("Blue-collar workers");
        list.Add("White-collar workers");
        list.Add("Bystanders witnessing an act of injustice or violence");
        list.Add("Survivors of trauma or abuse");
        list.Add("People living in a war-torn or conflict-ridden area");
        list.Add("People living in poverty");
	}
	return list;
}

int GetPrimaryCount() {
	return GetPrimary().GetCount();
}


const Index<String>& GetSecondary() {
	thread_local static Index<String> list;
	if (list.IsEmpty()) {
        list.Add("Romantic partner or love interest");
        list.Add("Family members (parents, siblings, children)");
        list.Add("Friends");
        list.Add("Cultural or ethnic community");
        list.Add("Religious or spiritual community");
        list.Add("Fellow activists or social justice advocates");
        list.Add("Musical collaborators or bandmates");
        list.Add("Higher power or divine figure");
        list.Add("Beloved pet or animal companion");
        list.Add("Society or societal expectations");
        list.Add("Government or political figures");
        list.Add("Nature or the environment");
        list.Add("Nostalgic memories or past self");
        list.Add("Oppressed or marginalized groups");
        list.Add("Inner demons or mental health struggles");
        list.Add("External obstacles or challenges");
        list.Add("Social media or technology");
        list.Add("Dreams or ambitions");
        list.Add("Material possessions or wealth");
        list.Add("Personal flaws or mistakes");
        list.Add("Other significant person or group in the singer's life");
        list.Add("Audience/listeners of the song");
        list.Add("Personal goals or aspirations ");
        list.Add("Past relationships or significant others");
        list.Add("Romantic rival or competition");
        list.Add("Past or present enemies/adversaries");
        list.Add("Dreams/goals/aspirations");
        list.Add("An unknown or imaginary person");
        list.Add("Celebrities or public figures");
        list.Add("Influence or legacy of a role model or idol");
        list.Add("Past or present self/inner self");
        list.Add("Memories or past experiences");
        list.Add("Social pressure or expectations");
        list.Add("Society as a whole");
        list.Add("The injustices in the world");
        list.Add("Personal growth or transformation");
	}
	return list;
}

int GetSecondaryCount() {
	return GetSecondary().GetCount();
}

const VectorMap<String,String>& GetArchetypes() {
	thread_local static VectorMap<String,String> list;
	if (list.IsEmpty()) {
		list.Add("Young Love","a song about the electric and passionate romance between two young lovers");
		list.Add("Heartbreak","a song about a devastating breakup and the pain that follows");
		list.Add("Party All Night","a song about letting loose and having a wild night with friends");
		list.Add("Rebellious Spirit","a song about going against the rules and living life on your own terms");
		list.Add("Hopeful Anthem","a song about never giving up and finding hope in dark times");
		list.Add("Summer Beach Days","a song about carefree summer days spent at the beach with friends");
		list.Add("New Beginnings","a song about starting over and moving forward after a difficult chapter");
		list.Add("Road Trip","a song about hitting the open road and going on an adventure");
		list.Add("Empowerment","a song about overcoming challenges and standing strong");
		list.Add("Lost Love","a song about reminiscing on a past love that got away");
		list.Add("Inspiring Journey","a song about personal growth and achieving your dreams");
		list.Add("Revenge","a song about getting back at someone who has wronged you");
		list.Add("Brokenness","a song about being broken and trying to put the pieces back together");
		list.Add("Escape","a song about leaving behind a toxic situation or relationship");
		list.Add("Friendship","a song celebrating the unbreakable bond between friends");
		list.Add("Heart on Fire","a song about the intense and passionate love for someone");
		list.Add("Dark Secrets","a song about hidden struggles and pain");
		list.Add("Chasing Dreams","a song about pursuing your aspirations and never giving up");
		list.Add("Rise Up","a song about standing up for what you believe in and fighting for change");
		list.Add("Forbidden Love","a song about a love that is deemed wrong or forbidden by society");
		list.Add("Hometown Glory","a song about the nostalgia and sense of belonging in one's hometown");
		list.Add("Survivor","a song about overcoming adversity and coming out stronger on the other side");
		list.Add("Euphoric Nights","a song about the euphoria and excitement of a wild night out");
		list.Add("Broken Promises","a song about the hurt and disappointment of broken promises in a relationship");
		list.Add("Finding Yourself","a song about self-discovery and finding your true identity");
		list.Add("Addiction","a song about struggling with addiction and the battle to overcome it");
		list.Add("Unrequited Love","a song about loving someone who does not feel the same way");
		list.Add("Unity","a song about coming together and standing as one");
		list.Add("Heartfelt Ballad","a slow, emotional ballad about a deep love or loss");
		list.Add("Live in the Moment","a song about living in the present and not worrying about the future");
		list.Add("Black Sheep","a song about feeling like an outsider or rebel");
		list.Add("Finding Closure","a song about finding closure and moving on from a past relationship");
		list.Add("Freedom","a song about breaking free from societal expectations and living on your own terms");
		list.Add("Against the World","a song about a couple's unwavering love for each other despite adversity");
		list.Add("Broken Family","a song about the struggles and heartache of a broken family");
		list.Add("Fight Song","an anthem about never giving up and fighting for what you believe in");
		list.Add("Bittersweet Memories","a song about reminiscing on both the good and bad times in a relationship");
		list.Add("Live Like There's No Tomorrow","a song about living life to the fullest without regrets");
	}
	return list;
}

int GetArchetypeCount() {
	return GetArchetypes().GetCount();
}


const Vector<ContrastType>& GetContrasts() {
	thread_local static Vector<ContrastType> list;
	if (list.IsEmpty()) {
		list.Add().Set("Seductive intro", "a seductive and sultry melody draws the listener in", "the lyrics talk about a passionate and intense relationship", "the mood shifts as the singer realizes they are not truly in love");
		list.Add().Set("Rise and fall", "the beat builds and intensifies, creating a sense of excitement and anticipation", "the lyrics tell a story of overcoming obstacles and achieving success", "the energy drops suddenly and the singer reflects on the sacrifices and struggles that came with their success");
		list.Add().Set("Fun and games", "a carefree and lively melody sets the tone for a carefree party anthem", "the lyrics are about enjoying life and living in the moment", "the party comes to an end and the reality of responsibilities and consequences sink in");
		list.Add().Set("Love at first sight", "a romantic and dreamy melody introduces the concept of falling in love at first sight", "the lyrics describe the intense feelings and desires that come with falling for someone instantly", "the singer wakes up from the fantasy and realizes");
		list.Add().Set("Struggle and triumph", "a slower and melancholic melody sets the scene for a character facing challenges and adversity", "the lyrics depict the struggles and hardships they have faced", "the pace picks up and the music becomes more triumphant as the character overcomes their struggles and achieves success");
		list.Add().Set("Ups and downs", "a catchy and upbeat melody reflects the highs of a new relationship", "the lyrics delve into the challenges and conflicts that arise within the relationship", "the music slows down as the couple try to work through their problems and find a resolution");
		list.Add().Set("Escape to paradise", "a tropical and laid-back beat transports the listener to a paradise destination", "the lyrics describe a desire to escape from reality and find solace in a beautiful location", "the singer comes back to reality and faces the consequences of leaving everything behind");
		list.Add().Set("Rebellious spirit", "a rebellious and edgy guitar riff sets the rebellious tone of the song", "the lyrics speak of breaking rules and societal expectations", "the song ends with the realization that rebellion can have consequences");
		list.Add().Set("Broken and mended", "a somber and melancholic melody reflects a heartbroken state", "the lyrics describe the pain and sadness of a broken relationship", "the tone shifts as the singer begins to heal and move on from the heartbreak");
		list.Add().Set("Chase your dreams", "an uplifting and motivational melody encourages listeners to chase their dreams", "the lyrics tell a story of overcoming obstacles and pursuing one's passions", "the song concludes with a sense of fulfillment and the realization that the journey towards achieving dreams is never-ending");
		list.Add().Set("Dark secrets", "a haunting and mysterious introduction sets the tone for secrets and deceit", "the lyrics reveal dark secrets and hidden motives among the characters", "the song ends with a sense of betrayal and the consequences of keeping secrets");
		list.Add().Set("Rags to riches", "a humble and modest melody represents the beginnings of a character's journey", "the lyrics describe the climb to success and wealth", "the music becomes more grandiose as the character achieves their dreams and reflects on their journey");
		list.Add().Set("Lost and found", "a haunting and melancholic melody portrays a sense of being lost and alone", "the lyrics depict a journey of self-discovery and finding one's place in the world", "the music becomes more uplifting as the character finds a sense of belonging and purpose");
		list.Add().Set("Ignite the fire", "an energetic and intense beat sparks excitement and passion", "the lyrics describe the power and intensity of a new love or passion", "the music dies down as the flame fades and the singer is left with the memories of the passion that once consumed them");
		list.Add().Set("From the ashes", "a slow and mournful melody sets the scene for a character who has hit rock bottom", "the lyrics depict the struggles and hardships they have faced", "the music picks up as the character rises from the ashes and rebuilds their life" );
		list.Add().Set("Fame and fortune", "a flashy and upbeat melody represents the allure of fame and fortune", "the lyrics describe the glamorous lifestyle and perks that come with success", "the song ends with a cautionary tale about the emptiness and pitfalls of a life solely focused on money and fame");
		list.Add().Set("Healing in the darkness", "a haunting and ethereal melody reflects a state of darkness and pain", "the lyrics speak of finding light and healing in the darkest times", "the music builds to a triumphant and uplifting finale as the singer finds strength and hope in their struggles");
		list.Add().Set("City lights and lonely nights", "a bustling and energetic beat represents the excitement of the city at night", "the lyrics tell a story of chasing dreams and living life to the fullest in the city", "the song ends with a sense of loneliness and longing for something more meaningful outside of the fast-paced city life");
		list.Add().Set("Breaking the mold", "a unique and unconventional melody sets the tone for breaking the norm", "the lyrics describe defying expectations and being true to oneself", "the song ends with a sense of liberation and empowerment as the singer embraces their individuality");
		list.Add().Set("Haunted by the past", "a haunting and eerie melody reflects the weight of a character's past traumas", "the lyrics delve into the pain and struggles of moving on from the past", "the music becomes more hopeful as the character learns to let go and move forward");
		list.Add().Set("Wild and free", "a carefree and adventurous melody embodies the thrill of living life on the edge", "the lyrics describe the rush and excitement of taking risks and living in the moment", "the song concludes with a reminder that with freedom comes consequences and responsibilities");
		list.Add().Set("Clash of opinions", "a catchy and upbeat melody sets the tone for a heated argument", "the lyrics depict conflicting opinions and viewpoints", "the song ends with the understanding that sometimes it's best to agree to disagree and move on" );
		list.Add().Set("Long distance love", "a soft and tender melody represents the longing and distance in a relationship", "the lyrics tell a story of the struggles and sacrifices of maintaining a long distance love", "the song ends with a sense of hope and determination to make the relationship work");
		list.Add().Set("Finding inner strength", "a slow and contemplative melody represents a character facing inner struggles", "the lyrics speak of finding courage and strength from within to overcome challenges", "the song crescendos as the singer embraces their inner strength and triumphs over their struggles");
		list.Add().Set("Living a double life", "a mysterious and seductive beat sets the stage for a character leading a secretive life", "the lyrics tell the story of juggling two separate identities and the dangers that come with it", "the song concludes with the realization that living a lie is destructive and unsustainable");
		list.Add().Set("Caught in the spotlight", "a bright and flashy melody reflects the thrill of being in the spotlight", "the lyrics depict the pressure and challenges of fame and constantly being in the public eye", "the music slows down as the singer reflects on the toll fame has taken on their personal life");
		list.Add().Set("Love and war", "a powerful and intense beat represents the passionate and tumultuous nature of love", "the lyrics depict a couple's constant battle and struggle to make their relationship work", "the song ends with a bittersweet realization that love can be both beautiful and painful");
		list.Add().Set("The art of letting go", "a slow and somber melody sets the tone for learning to let go", "the lyrics describe the struggles of moving on and leaving the past behind", "the music builds to a hopeful and empowering finale as the singer finally finds the strength to let go");
		list.Add().Set("Living in the moment", "an upbeat and carefree melody represents living life with no regrets", "the lyrics encourage taking chances and embracing every moment", "the song ends with a reminder to cherish the present and not dwell on the past or worry about the future");
		list.Add().Set("Conquering fears", "a tense and ominous melody reflects the fear and anxiety a character faces", "the lyrics speak of overcoming fears and finding courage to face them", "the music becomes triumphant and uplifting as the character conquers their fears and grows stronger" );
		/*list.Add().Set("Heart vs. Mind", "a gentle and emotional melody sets the stage for a character torn between their heart and their logical mind", "the lyrics describe the internal struggle between following one's emotions and making rational decisions", "the song ends on a reflective note as the character finds a balance between their heart and mind");
		list.Add().Set("Surviving the storm", "a stormy and intense melody represents facing difficult and challenging times", "the lyrics speak of resilience and perseverance through tough situations", "the music calms down as the singer finds strength and hope in surviving the storm");
		list.Add().Set("Living a lie", "a dark and deceptive melody reflects the deception in a character's life", "the lyrics depict the consequences and chaos that come with living a lie", "the song ends with the realization that living a lie can destroy relationships and one's own sense of self");
		list.Add().Set("Forgotten memories", "a melancholic and haunting melody sets the scene for a character's forgotten memories", "the lyrics unravel the mystery and pain behind the forgotten memories", "the music becomes reflective and haunting as the singer realizes the true impact of their forgotten memories on their life");
		list.Add().Set("Breaking free", "a powerful and empowering melody reflects the desire to break free from constraints and expectations", "the lyrics describe the journey of breaking through obstacles and finding independence", "the song ends on a triumphant note as the character finally breaks free and finds freedom and happiness" );
		list.Add().Set("Fake love", "a catchy and upbeat melody represents the facade of a fake love", "the lyrics reveal the deceit and manipulation in a fake relationship", "the song ends with the realization of the emptiness and pain of a love built on lies and false promises");
		list.Add().Set("Strangers in love", "a soft and romantic melody represents the initial attraction and connection between two strangers", "the lyrics follow the journey of getting to know each other and falling in love", "the song ends with the uncertainty and fear of whether their love will last or if they were always destined to be strangers");
		list.Add().Set("Guy in club", "an upbeat and danceable melody sets the scene for a night out at the club", "the lyrics depict the excitement and charm of a guy at the club", "the song ends on a reflective note as the singer realizes the emptiness and lack of substance in these interactions");
		list.Add().Set("Angels and demons", "a haunting and eerie melody sets the tone for the duality within a character", "the lyrics describe the battle between good and evil within oneself", "the music becomes more chaotic as the character struggles to find balance between their angels and demons");
		list.Add().Set("Living on the edge", "a fast-paced and thrilling beat represents the risky and dangerous lifestyle of living on the edge", "the lyrics depict the rush and adrenaline that comes with constantly pushing boundaries", "the song ends with a sense of exhaustion and questioning if the thrill is worth the consequences");*/
	}
	return list;
}

int GetContrastCount() {
	return GetContrasts().GetCount();
}

const Vector<String>& GetContrastParts() {
	thread_local static Vector<String> list;
	if (list.IsEmpty()) {
		const auto& v = GetContrasts();
		for(int i = 0; i < v.GetCount(); i++) {
			const auto& it = v[i];
			for(int j = 0; j < ContrastType::PART_COUNT; j++) {
				list.Add() = it.key + " #" + IntStr(j+1) + ": " + it.parts[j];
			}
		}
	}
	return list;
}

VectorMap<String,Vector<String>>& GetTypecastSingers(bool gender) {
	if (!gender)
		return GetTypecastSingersMale();
	else
		return GetTypecastSingersFemale();
}

VectorMap<String,Vector<String>>& GetTypecastRappers(bool gender) {
	if (!gender)
		return GetTypecastRappersMale();
	else
		return GetTypecastRappersFemale();
}

VectorMap<String,Vector<String>>& GetTypecastArtists(bool rapper, bool gender) {
	if (!rapper)
		return GetTypecastSingers(gender);
	else
		return GetTypecastRappers(gender);
}

void SetIndexCursor(ArrayCtrl& arr, int cur) {
	for(int i = 0; i < arr.GetCount(); i++) {
		int idx = arr.Get(i, "IDX");
		if (idx == cur) {
			arr.SetCursor(i);
			break;
		}
	}
}

VectorMap<String,Vector<String>>& GetRoleCompanys(bool unsafe, bool gender) {
	TODO
	static VectorMap<String,Vector<String>> v;
	return v;
}
