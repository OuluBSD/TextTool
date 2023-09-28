#include "SongTool.h"


const char* RhymeSchemes[RHYME_COUNT][2] {
	{"AA", t_("2-line end rhyme")},
	{"XA,XA", t_("Single two-line stanza without internal rhyme")},
	{"AX,AX", t_("Single two-line begin rhyme without internal rhyme")},
	{"AB,AB", t_("Single two-line stanza with internal rhyme")},
	{"AAB,XXB", t_("Two-line end-rhyming with internal rhyme in 1st line")},
	{"XXA,BBA", t_("Two-line end-rhyming with internal rhyme in 2nd line")},
	{"AAB,CCB", t_("Two-line end-rhyming with internal rhyme in both lines")},
	{"ABB,ABB", t_("Two-line end-rhyming with internal rhymes in both lines")},
	{"ABA,ABA", t_("Two-line rhyming with internal rhymes in both lines")},
	
	{"AAAA", t_("4-line end rhyme")},
	{"XAAA", t_("3-line end rhyme with open beginning")},
	{"AAAX", t_("3-line end rhyme with open ending")},
	{"XAXA", t_("4 lines, no rhyming 1. & 3., connected")},
	{"ABAB", t_("4 lines, alternating rhyming, connected")},
	{"AbAb", t_("4 lines, 1. & 3. lines are same, connected")},
	{"aBaB", t_("4 lines, 2. & 4. lines are same, connected")},
	{"XA XA", t_("4 lines, no rhyming 1. & 3., disconnected")},
	{"AB AB", t_("4 lines, alternating rhyming, disconnected")},
	{"AA,XB CC,XB", t_("Ballad stanza. 4 lines, internal rhymes in 1. & 3. lines and end rhymes in 2. & 4. lines")},
	
	{"AAAC BBBC", t_("Double 4-lines, 3 lines rhyming, rhymes in the end of parts")},
	{"CAAA CBBB", t_("Double 4-lines, 3 lines rhyming, rhymes in the beginning of parts")},
	{"A1abA2 A1abA2", t_("2 times 4 lines, with same 1. & 4. lines")},
};

const int RhymeSchemeLineCount[RHYME_COUNT] = {
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,
	
	8,
	8,
	8,
};

int FindRhymeType(const char* name) {
	for(int i = 0; i < RHYME_COUNT; i++) {
		if (strncmp(RhymeSchemes[i][0], name, 32) == 0)
			return i;
	}
	return -1;
}


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
		case 0: return GetDemocratBiasHeader();
		case 1: return GetRepublicanBiasHeader();
		case 2: return GetNonPartisanBiasHeader();
		default: return String();
	}
}
