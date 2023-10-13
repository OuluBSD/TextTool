#include "SimplifiedCtrl.h"


AutoIdeas::AutoIdeas() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(4000);
	
	vsplit0.Vert() << themes << ideas << tones;
	vsplit1.Vert() << allegories << contents << imageries << symbolisms;
	
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
	
	imageries.AddColumn(t_("Specific imagery"));
	imageries.WhenCursor << THISBACK(OnListImagery);
	
	symbolisms.AddColumn(t_("Symbolism"));
	symbolisms.WhenCursor << THISBACK(OnListSymbolism);
	
}

void AutoIdeas::DisableAll() {
	disabled = true;
	themes.Disable();
	ideas.Disable();
	contents.Disable();
	allegories.Disable();
	tones.Disable();
	symbolisms.Disable();
	imageries.Disable();
}

void AutoIdeas::EnableAll() {
	disabled = false;
	themes.Enable();
	ideas.Enable();
	contents.Enable();
	allegories.Enable();
	tones.Enable();
	symbolisms.Enable();
	imageries.Enable();
}

void AutoIdeas::Data() {
	DataSong(true);
}

void AutoIdeas::DataSong(bool set_cursor) {
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
	
	if (set_cursor && !themes.IsCursor() && song.theme_cursor >= 0 && song.theme_cursor < themes.GetCount())
		themes.SetCursor(song.theme_cursor);
	
	DataTheme(set_cursor);
}

void AutoIdeas::DataTheme(bool set_cursor) {
	Song& song = GetSong();
	
	if (!themes.IsCursor()) {
		ideas.Clear();
		tones.Clear();
		allegories.Clear();
		contents.Clear();
		symbolisms.Clear();
		imageries.Clear();
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
	
	if (set_cursor && !ideas.IsCursor() && t.cursor >= 0 && t.cursor < ideas.GetCount())
		ideas.SetCursor(t.cursor);
	
	DataIdea(set_cursor);
}

void AutoIdeas::DataIdea(bool set_cursor) {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor()) {
		tones.Clear();
		allegories.Clear();
		contents.Clear();
		symbolisms.Clear();
		imageries.Clear();
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
	
	if (set_cursor && !tones.IsCursor() && id.cursor >= 0 && id.cursor < tones.GetCount())
		tones.SetCursor(id.cursor);
	
	DataTone(set_cursor);
}

void AutoIdeas::DataTone(bool set_cursor) {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor() || !tones.IsCursor()) {
		allegories.Clear();
		contents.Clear();
		symbolisms.Clear();
		imageries.Clear();
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
	
	if (set_cursor && !allegories.IsCursor() && tone.cursor >= 0 && tone.cursor < allegories.GetCount())
		allegories.SetCursor(tone.cursor);
	
	DataAllegory(set_cursor);
}

void AutoIdeas::DataAllegory(bool set_cursor) {
	Song& song = GetSong();
	
	if (!themes.IsCursor() || !ideas.IsCursor() || !tones.IsCursor() || !allegories.IsCursor()) {
		contents.Clear();
		symbolisms.Clear();
		imageries.Clear();
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
	
	if (set_cursor && !contents.IsCursor() && dev.cursor >= 0 && dev.cursor < contents.GetCount())
		contents.SetCursor(dev.cursor);
	
	DataContent(set_cursor);
}

void AutoIdeas::DataContent(bool set_cursor) {
	Song& song = GetSong();
	
	if (!contents.IsCursor()) {
		imageries.Clear();
		symbolisms.Clear();
		return;
	}
	
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	int content_i = contents.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& dev = tone.allegories[allegory_i];
	StaticContentIdea& c = dev.contents[content_i];
	
	
	for(int i = 0; i < c.imageries.GetCount(); i++) {
		StaticImagery& o = c.imageries[i];
		imageries.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64)).Ink(White())
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	imageries.SetCount(c.imageries.GetCount());
	
	if (set_cursor && !imageries.IsCursor() && c.cursor >= 0 && c.cursor < imageries.GetCount())
		imageries.SetCursor(c.cursor);
	
	DataImagery(set_cursor);
}

void AutoIdeas::DataImagery(bool set_cursor) {
	Song& song = GetSong();
	
	if (!imageries.IsCursor()) {
		symbolisms.Clear();
		return;
	}
	
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	int content_i = contents.GetCursor();
	int imagery_i = imageries.GetCursor();
	StaticTheme& t = song.themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& dev = tone.allegories[allegory_i];
	StaticContentIdea& c = dev.contents[content_i];
	StaticImagery& img = c.imageries[imagery_i];
	
	
	for(int i = 0; i < img.symbolisms.GetCount(); i++) {
		StaticSymbolism& o = img.symbolisms[i];
		symbolisms.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64)).Ink(White())
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	symbolisms.SetCount(img.symbolisms.GetCount());
	
	if (set_cursor && !symbolisms.IsCursor() && img.cursor >= 0 && img.cursor < symbolisms.GetCount())
		symbolisms.SetCursor(img.cursor);
	
}

void AutoIdeas::OnListTheme() {
	Song& song = GetSong();
	
	if (themes.IsCursor()) {
		int theme_i = themes.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		song.theme_cursor = theme_i;
		if (t.ideas.IsEmpty()) {
			ideas.Clear();
			tones.Clear();
			allegories.Clear();
			contents.Clear();
			symbolisms.Clear();
			imageries.Clear();
			PostCallback(THISBACK(GetNovelIdeas));
			return;
		}
	}
	DataTheme(false);
}

void AutoIdeas::OnListIdea() {
	Song& song = GetSong();
	
	if (themes.IsCursor() && ideas.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		t.cursor = idea_i;
		if (id.tones.IsEmpty()) {
			tones.Clear();
			allegories.Clear();
			contents.Clear();
			symbolisms.Clear();
			imageries.Clear();
			PostCallback(THISBACK(GetToneSuggestions));
			return;
		}
	}
	DataIdea(false);
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
		id.cursor = tone_i;
		if (tone.allegories.IsEmpty()) {
			allegories.Clear();
			contents.Clear();
			symbolisms.Clear();
			imageries.Clear();
			PostCallback(THISBACK(GetAllegorySuggestions));
			return;
		}
	}
	DataTone(false);
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
		tone.cursor = allegory_i;
		if (alleg.contents.IsEmpty()) {
			contents.Clear();
			symbolisms.Clear();
			imageries.Clear();
			PostCallback(THISBACK(GetContentSuggestions));
			return;
		}
	}
	DataAllegory(false);
}

void AutoIdeas::OnListContentIdea() {
	Song& song = GetSong();
	
	if (contents.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		int tone_i = tones.GetCursor();
		int allegory_i = allegories.GetCursor();
		int content_i = contents.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		StaticToneSuggestion& tone = id.tones[tone_i];
		StaticAllegoricalDevice& alleg = tone.allegories[allegory_i];
		StaticContentIdea& c = alleg.contents[content_i];
		alleg.cursor = content_i;
		if (c.imageries.IsEmpty()) {
			imageries.Clear();
			symbolisms.Clear();
			PostCallback(THISBACK(GetImagerySuggestions));
			return;
		}
	}
	DataContent(false);
}

void AutoIdeas::OnListImagery() {
	Song& song = GetSong();
	
	if (imageries.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		int tone_i = tones.GetCursor();
		int allegory_i = allegories.GetCursor();
		int content_i = contents.GetCursor();
		int imagery_i = imageries.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		StaticToneSuggestion& tone = id.tones[tone_i];
		StaticAllegoricalDevice& alleg = tone.allegories[allegory_i];
		StaticContentIdea& c = alleg.contents[content_i];
		StaticImagery& img = c.imageries[imagery_i];
		c.cursor = imagery_i;
		if (img.symbolisms.IsEmpty()) {
			symbolisms.Clear();
			PostCallback(THISBACK(GetSymbolismSuggestions));
			return;
		}
	}
	DataImagery(false);
}

void AutoIdeas::OnListSymbolism() {
	Song& song = GetSong();
	
	if (symbolisms.IsCursor()) {
		int theme_i = themes.GetCursor();
		int idea_i = ideas.GetCursor();
		int tone_i = tones.GetCursor();
		int allegory_i = allegories.GetCursor();
		int content_i = contents.GetCursor();
		int imagery_i = imageries.GetCursor();
		int symbolism_i = symbolisms.GetCursor();
		StaticTheme& t = song.themes[theme_i];
		StaticIdea& id = t.ideas[idea_i];
		StaticToneSuggestion& tone = id.tones[tone_i];
		StaticAllegoricalDevice& alleg = tone.allegories[allegory_i];
		StaticContentIdea& c = alleg.contents[content_i];
		StaticImagery& img = c.imageries[imagery_i];
		StaticSymbolism& sym = img.symbolisms[symbolism_i];
		img.cursor = symbolism_i;
		/*if (sym.  .IsEmpty()) {
			symbolisms.Clear();
			PostCallback(THISBACK(GetSymbolismSuggestions));
			return;
		}*/
	}
	//DataSymbolism(false);
}

void AutoIdeas::ToolMenu(Bar& bar) {
	bar.Add(t_("Get novel themes"), AppImg::BlueRing(), THISBACK(GetNovelThemes)).Key(K_CTRL_Q);
	bar.Add(t_("Get novel ideas"), AppImg::BlueRing(), THISBACK(GetNovelIdeas)).Key(K_CTRL_W);
	bar.Add(t_("Get tone suggestions"), AppImg::BlueRing(), THISBACK(GetToneSuggestions)).Key(K_CTRL_E);
	bar.Add(t_("Get allegorical device"), AppImg::BlueRing(), THISBACK(GetAllegorySuggestions)).Key(K_CTRL_R);
	bar.Add(t_("Get content suggestions"), AppImg::BlueRing(), THISBACK(GetContentSuggestions)).Key(K_CTRL_T);
	bar.Add(t_("Get specific imagery suggestions"), AppImg::BlueRing(), THISBACK(GetImagerySuggestions)).Key(K_CTRL_Y);
	bar.Add(t_("Get symbolism suggestions"), AppImg::BlueRing(), THISBACK(GetSymbolismSuggestions)).Key(K_CTRL_U);
	
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
	
	if (!ideas.IsCursor())
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
	
	if (!tones.IsCursor())
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
	
	if (!allegories.IsCursor())
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

void AutoIdeas::GetImagerySuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!contents.IsCursor())
		return;
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	int content_i = contents.GetCursor();
	StaticTheme& t = p.song->themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& all = tone.allegories[allegory_i];
	StaticContentIdea& c = all.contents[content_i];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetImagerySuggestions(t.text, id.text, tone.text, all.text, c.text, attrs, THISBACK1(OnImagerySuggestions, &c));
	}
}

void AutoIdeas::GetSymbolismSuggestions() {
	if (disabled) return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!imageries.IsCursor())
		return;
	int theme_i = themes.GetCursor();
	int idea_i = ideas.GetCursor();
	int tone_i = tones.GetCursor();
	int allegory_i = allegories.GetCursor();
	int content_i = contents.GetCursor();
	int imagery_i = imageries.GetCursor();
	StaticTheme& t = p.song->themes[theme_i];
	StaticIdea& id = t.ideas[idea_i];
	StaticToneSuggestion& tone = id.tones[tone_i];
	StaticAllegoricalDevice& all = tone.allegories[allegory_i];
	StaticContentIdea& c = all.contents[content_i];
	StaticImagery& img =  c.imageries[imagery_i];
	
	Vector<String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetSymbolismSuggestions(t.text, id.text, tone.text, all.text, c.text, img.text, attrs, THISBACK1(OnSymbolismSuggestions, &img));
	}
}

String AutoIdeas::GetStatusText() {
	return "";
}

void HotFixResult(String& res) {
	res.Replace("\r\n", "\n");
	res.Replace("\n(", " (");
	res.Replace("\n\n-", "\n-");
	
	// Add character " to the first line, if rest of the lines has it
	if ((res.Find("\n- \"") >= 0 || res.Find("\n-\"") >= 0) &&
		(res.Left(2) != "-\"" && res.Left(3) != "- \"")) {
		if (res.Mid(1, 1) == " ")
			res.Insert(2, "\"");
		else
			res.Insert(1, "\"");
	}
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
		if (b >= 0 && split > a && split < b)
			split = -1;
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
			if (clr_parts.GetCount() >= 3) {
				RGBA c;
				c.r = ScanInt(clr_parts[0]);
				c.g = ScanInt(clr_parts[1]);
				c.b = ScanInt(clr_parts[2]);
				c.a = 255;
				clr = c;
			}
			else if (clr_parts.GetCount() == 1) {
				dword d = ScanInt(clr_parts[0]);
				RGBA c;
				c.r = (d >> 0) & 0xFF;
				c.g = (d >> 8) & 0xFF;
				c.b = (d >> 16) & 0xFF;
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
		else
			s = TrimBoth(s.Left(a));
	}
	
	String txt = s;
	
	a = txt.Find(":");
	if (a >= 0)
		txt = TrimBoth(txt.Mid(a+1));
	
	a = txt.Find("\"");
	if (a >= 0)
		txt = TrimBoth(txt.Mid(a+1));
	
	a = txt.Find("-");
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
	
	PostCallback(THISBACK1(DataSong, false));
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
	
	PostCallback(THISBACK1(DataTheme, false));
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
	
	PostCallback(THISBACK1(DataIdea, false));
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
	
	PostCallback(THISBACK1(DataTone, false));
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
	
	PostCallback(THISBACK1(DataAllegory, false));
}

void AutoIdeas::OnImagerySuggestions(String result, StaticContentIdea* c) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	c->imageries.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticImagery& o = c->imageries.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK1(DataContent, false));
}

void AutoIdeas::OnSymbolismSuggestions(String result, StaticImagery* img) {
	EnableAll();
	
	HotFixResult(result);
	
	Vector<String> lines = Split(result, "\n", false);
	
	img->symbolisms.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticSymbolism& o = img->symbolisms.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK1(DataImagery, false));
}
