#include "SimplifiedCtrl.h"


AutoIdeas::AutoIdeas() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << songsplit << ideasplit << detaillist;
	hsplit.SetPos(3000, 0);
	hsplit.SetPos(7000, 1);
	
	songsplit.Vert() << themes << ideas;
	ideasplit.Vert() << tones << allegories;
	detaillist.Vert() << contents << symbolism << specific_imagery;
	
	ideasplit.SetPos(6666,0);
	
	themes.AddColumn(t_("Theme"));
	themes.WhenCursor << THISBACK(OnListTheme);
	
	ideas.AddColumn(t_("Idea"));
	ideas.WhenCursor << THISBACK(OnListIdea);
	
	tones.AddColumn(t_("Tone"));
	tones.WhenCursor << THISBACK(OnListTone);
	
	allegories.AddColumn(t_("Allegory"));
	allegories.WhenCursor << THISBACK(OnListAllegory);
	
	contents.AddColumn(t_("Content suggestion"));
	contents.WhenCursor << THISBACK(OnListContentIdea);
	
	symbolism.AddColumn(t_("Symbolism"));
	specific_imagery.AddColumn(t_("Specific imagery"));
	
}

void AutoIdeas::DisableAll() {
	disabled = true;
	themes.Disable();
	ideas.Disable();
	contents.Disable();
	allegories.Disable();
	tones.Disable();
	symbolism.Disable();
	specific_imagery.Disable();
}

void AutoIdeas::EnableAll() {
	disabled = false;
	themes.Enable();
	ideas.Enable();
	contents.Enable();
	allegories.Enable();
	tones.Enable();
	symbolism.Enable();
	specific_imagery.Enable();
}

void AutoIdeas::Data() {
	DataSong();
}

void AutoIdeas::DataSong() {
	Song& song = GetSong();
	
	for(int i = 0; i < song.themes.GetCount(); i++) {
		StaticTheme& t = song.themes[i];
		themes.Set(i, 0, AttrText(Capitalize(t.text))
			.Paper(Blend(t.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(t.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	themes.SetCount(song.themes.GetCount());
	
	//if (themes.GetCount() && !themes.IsCursor()) themes.SetCursor(0);
	
	DataTheme();
}

void AutoIdeas::DataTheme() {
	Song& song = GetSong();
	
	if (!themes.IsCursor()) {
		ideas.Clear();
		tones.Clear();
		allegories.Clear();
		contents.Clear();
		symbolism.Clear();
		specific_imagery.Clear();
		return;
	}
	int theme_i = themes.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	
	for(int i = 0; i < t.ideas.GetCount(); i++) {
		StaticIdea& idea = t.ideas[i];
		ideas.Set(i, 0, AttrText(Capitalize(idea.text))
			.Paper(Blend(idea.clr, GrayColor(), 128+64)).Ink(White())
			.NormalPaper(Blend(idea.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	ideas.SetCount(t.ideas.GetCount());
	
	//if (ideas.GetCount() && !ideas.IsCursor()) ideas.SetCursor(0);
	
	DataIdea();
}

void AutoIdeas::DataIdea() {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor()) {
		tones.Clear();
		allegories.Clear();
		contents.Clear();
		symbolism.Clear();
		specific_imagery.Clear();
		return;
	}
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	
	
	for(int i = 0; i < id.tones.GetCount(); i++) {
		StaticToneSuggestion& o = id.tones[i];
		tones.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	tones.SetCount(id.tones.GetCount());
	
	DataTone();
}

void AutoIdeas::DataTone() {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor() || !tones.IsCursor()) {
		allegories.Clear();
		contents.Clear();
		symbolism.Clear();
		specific_imagery.Clear();
		return;
	}
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	
	
	for(int i = 0; i < tone.allegories.GetCount(); i++) {
		StaticAllegoricalDevice& o = tone.allegories[i];
		allegories.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	allegories.SetCount(tone.allegories.GetCount());
	
	//if (allegories.GetCount() && !allegories.IsCursor()) allegories.SetCursor(0);
	
	DataAllegory();
}

void AutoIdeas::DataAllegory() {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor() || !tones.IsCursor() || !allegories.IsCursor()) {
		contents.Clear();
		symbolism.Clear();
		specific_imagery.Clear();
		return;
	}
	
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& dev = tone.allegories[allegory_i];
	
	
	for(int i = 0; i < dev.contents.GetCount(); i++) {
		StaticContentIdea& o = dev.contents[i];
		contents.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64)).Ink(White())
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	contents.SetCount(dev.contents.GetCount());
	
	//if (contents.GetCount() && !contents.IsCursor()) contents.SetCursor(0);
	
	
}

void AutoIdeas::OnListTheme() {
	Song& song = GetSong();
	
	if (themes.IsCursor()) {
		int theme_i = themes.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		if (t.ideas.IsEmpty()) {
			ideas.Clear();
			tones.Clear();
			allegories.Clear();
			contents.Clear();
			symbolism.Clear();
			specific_imagery.Clear();
			PostCallback(THISBACK(GetNovelIdeas));
			return;
		}
	}
	DataTheme();
}

void AutoIdeas::OnListIdea() {
	Song& song = GetSong();
	
	if (themes.IsCursor() && ideas.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		if (id.tones.IsEmpty()) {
			tones.Clear();
			allegories.Clear();
			contents.Clear();
			symbolism.Clear();
			specific_imagery.Clear();
			PostCallback(THISBACK(GetToneSuggestions));
			return;
		}
	}
	DataIdea();
}

void AutoIdeas::OnListTone() {
	Song& song = GetSong();
	
	if (themes.IsCursor() && ideas.IsCursor() && tones.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		int tone_i = tones.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		StaticToneSuggestion& tone = id.tones[tone_i];
		if (tone.allegories.IsEmpty()) {
			allegories.Clear();
			contents.Clear();
			symbolism.Clear();
			specific_imagery.Clear();
			PostCallback(THISBACK(GetAllegorySuggestions));
			return;
		}
	}
	DataTone();
}

void AutoIdeas::OnListAllegory() {
	Song& song = GetSong();
	
	if (themes.IsCursor() && ideas.IsCursor() && tones.IsCursor() && allegories.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		int tone_i = tones.GetCursor();
		int allegory_i = allegories.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		StaticToneSuggestion& tone = id.tones[tone_i];
		StaticAllegoricalDevice& alleg = tone.allegories[allegory_i];
		if (alleg.contents.IsEmpty()) {
			contents.Clear();
			symbolism.Clear();
			specific_imagery.Clear();
			PostCallback(THISBACK(GetContentSuggestions));
			return;
		}
	}
	DataAllegory();
}

void AutoIdeas::OnListContentIdea() {
	
}

void AutoIdeas::ToolMenu(Bar& bar) {
	bar.Add(t_("Get novel themes"), AppImg::BlueRing(), THISBACK(GetNovelThemes)).Key(K_CTRL_Q);
	bar.Add(t_("Get novel ideas"), AppImg::BlueRing(), THISBACK(GetNovelIdeas)).Key(K_CTRL_W);
	bar.Add(t_("Get tone suggestions"), AppImg::BlueRing(), THISBACK(GetToneSuggestions)).Key(K_CTRL_E);
	bar.Add(t_("Get allegorical device"), AppImg::BlueRing(), THISBACK(GetAllegorySuggestions)).Key(K_CTRL_R);
	bar.Add(t_("Get content suggestions"), AppImg::BlueRing(), THISBACK(GetContentSuggestions)).Key(K_CTRL_T);
	bar.Add(t_("Get symbolism suggestions"), AppImg::BlueRing(), THISBACK(GetSymbolismSuggestions)).Key(K_CTRL_Y);
	bar.Add(t_("Get specific imagery suggestions"), AppImg::BlueRing(), THISBACK(GetImagerySuggestions)).Key(K_CTRL_U);
	
}

void AutoIdeas::GetAttrs(const VectorMap<String,String>& data, Vector<String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v << Attr::AttrKeys[i][2];
			}
			else {
				v << Attr::AttrKeys[i][3];
			}
		}
	}
}

void AutoIdeas::GetNovelThemes() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	//for (auto& a : attrs) {LOG("- " << a);}
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetNovelThemes(attrs, THISBACK1(OnNovelThemes, &song));
	}
}

void AutoIdeas::GetNovelIdeas() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int i = themes.GetCursor();
	StaticTheme& t = p.song->themes[i];
	String theme = t.text;
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	//for (auto& a : attrs) {LOG("- " << a);}
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetNovelIdeas(theme, attrs, THISBACK1(OnNovelIdeas, &t));
	}
}

void AutoIdeas::GetToneSuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int i = themes.GetCursor();
	int j = ideas.GetCursor();
	StaticTheme& t = p.song->themes[i];
	StaticIdea& idea = t.ideas[j];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetToneSuggestions(t.text, idea.text, attrs, THISBACK1(OnToneSuggestions, &idea));
	}
}

void AutoIdeas::GetAllegorySuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	StaticTheme& t = p.song->themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetAllegorySuggestions(t.text, id.text, tone.text, attrs, THISBACK1(OnAllegorySuggestions, &tone));
	}
}

void AutoIdeas::GetContentSuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	StaticTheme& t = p.song->themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& all = tone.allegories[allegory_i];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetContentSuggestions(t.text, id.text, tone.text, all.text, attrs, THISBACK1(OnContentSuggestions, &all));
	}
}

void AutoIdeas::GetSymbolismSuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int i = themes.GetCursor();
	int j = ideas.GetCursor();
	StaticTheme& t = p.song->themes[i];
	StaticIdea& idea = t.ideas[j];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetSymbolismSuggestions(t.text, idea.text, attrs, THISBACK1(OnSymbolismSuggestions, &idea));
	}
}

void AutoIdeas::GetImagerySuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!themes.IsCursor())
		return;
	int i = themes.GetCursor();
	int j = ideas.GetCursor();
	StaticTheme& t = p.song->themes[i];
	StaticIdea& idea = t.ideas[j];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetImagerySuggestions(t.text, idea.text, attrs, THISBACK1(OnImagerySuggestions, &idea));
	}
}

String AutoIdeas::GetStatusText() {
	return "";
}

void HotFixResult(String& res) {
	res.Replace("\r\n", "\n");
	res.Replace("\n(", " (");
	res.Replace("\n\n-", "\n-");
}

void ParseTextColor(String s, String& text, Color& clr) {
	int pre = 0;
	
	for(int i = 0; i < s.GetCount(); i++) {
		int chr = s[i];
		if (!IsDigit(chr))
			break;
		pre++;
	}
	if (pre > 0)
		s = TrimBoth(s.Mid(pre+1));
	
	s.Replace("bytes:", "");
	s.Replace("RGB:", "");
	s.Replace("integers:", "");
	s.Replace("RGB code:", "");
	s.Replace("code:", "");
	s.Replace("R:", "");
	s.Replace("G:", "");
	s.Replace("B:", "");
	
	clr = White();
	int a = s.Find("(");
	int split0 = s.Find(":");
	int split1 = s.Find("-");
	int split = split0 >= 0 ? split0 : split1;
	if (split < 0) split = a;
	
	if (a >= 0) {
		int b = s.Find(")", a);
		if (b < 0) b = s.GetCount();
		String clr_str = s.Mid(a,b-a);
		bool is_hex = clr_str.Find("#") >= 0;
		
		if (clr_str.Left(1) == "(") clr_str = clr_str.Mid(1);
		if (clr_str.Right(1) == ")") clr_str = clr_str.Left(clr_str.GetCount()-1);
		
		for(int j = 0; j < clr_str.GetCount(); j++) {
			int chr = clr_str[j];
			if (IsDigit(chr)) {
				clr_str = clr_str.Mid(j);
				break;
			}
		}
		if (is_hex) {
			RGBA c;
			c.r = HexInt(clr_str.Mid(0, 2));
			c.g = HexInt(clr_str.Mid(2, 2));
			c.b = HexInt(clr_str.Mid(4, 2));
			c.a = 255;
			clr = c;
		}
		else {
			Vector<String> clr_parts = Split(clr_str, ",");
			if (clr_parts.GetCount() == 3) {
				RGBA c;
				c.r = StrInt(clr_parts[0]);
				c.g = StrInt(clr_parts[1]);
				c.b = StrInt(clr_parts[2]);
				c.a = 255;
				clr = c;
			}
			else {
				DUMPC(clr_parts);
			}
		}
		
		if (split >= 0) {
			if (a < split && b < split)
				s = TrimBoth(s.Mid(split+1));
			else if (a < split)
				s = TrimBoth(s.Mid(b+1));
			else
				s = TrimBoth(s.Left(a));
		}
	}
	
	String txt = s;
	
	a = txt.Find(":");
	if (a >= 0)
		txt = TrimBoth(txt.Mid(a+1));
	
	if (txt.Left(1) == "\"") txt = txt.Mid(1);
	if (txt.Right(1) == "\"") txt = txt.Left(txt.GetCount()-1);
	text = txt;
}

void AutoIdeas::OnNovelThemes(String result, Song* song_) {
	EnableAll();
	
	HotFixResult(result);
	
	Song& song = *song_;
	Vector<String> lines = Split(result, "\n", false);
	
	song.themes.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticTheme& t = song.themes.Add();
		ParseTextColor(s, t.text, t.clr);
	}
	
	PostCallback(THISBACK(DataSong));
}

void AutoIdeas::OnNovelIdeas(String result, StaticTheme* theme) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	theme->ideas.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticIdea& idea = theme->ideas.Add();
		ParseTextColor(s, idea.text, idea.clr);
	}
	
	PostCallback(THISBACK(DataTheme));
}

void AutoIdeas::OnToneSuggestions(String result, StaticIdea* idea) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	idea->tones.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticToneSuggestion& o = idea->tones.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK(DataIdea));
}

void AutoIdeas::OnAllegorySuggestions(String result, StaticToneSuggestion* tone) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	tone->allegories.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticAllegoricalDevice& o = tone->allegories.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK(DataTone));
}

void AutoIdeas::OnContentSuggestions(String result, StaticAllegoricalDevice* alleg) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	alleg->contents.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticContentIdea& o = alleg->contents.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK(DataAllegory));
}

void AutoIdeas::OnSymbolismSuggestions(String result, StaticIdea* idea) {
	
}

void AutoIdeas::OnImagerySuggestions(String result, StaticIdea* idea) {
	
}
