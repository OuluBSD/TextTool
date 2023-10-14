#include "SimplifiedCtrl.h"


PartIdea::PartIdea() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << hsplit0 << hsplit1;
	
	hsplit0.Horz() << active << parts;
	hsplit0.SetPos(2500);
	
	hsplit1.Horz() << contents << imageries << symbolisms;
	
	active.AddColumn(t_("Active idea"));
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Content suggestion"));
	parts.AddColumn(t_("Specific imagery"));
	parts.AddColumn(t_("Symbolism"));
	parts.WhenCursor << THISBACK(OnListPart);
	
	contents.AddColumn(t_("Content suggestion"));
	contents.WhenCursor << THISBACK(OnListContent);

	imageries.AddColumn(t_("Specific imagery"));
	imageries.WhenCursor << THISBACK(OnListImagery);

	symbolisms.AddColumn(t_("Symbolism"));
	symbolisms.WhenCursor << THISBACK(OnListSymbolism);
	
}

void PartIdea::EnableAll() {
	disabled = false;
	active.Enable();
	parts.Enable();
	contents.Enable();
	imageries.Enable();
	symbolisms.Enable();
}

void PartIdea::DisableAll() {
	disabled = true;
	active.Disable();
	parts.Disable();
	contents.Disable();
	imageries.Disable();
	symbolisms.Disable();
}

void PartIdea::Data() {
	Song& song = GetSong();
	
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		active.Set(i, 0,
			AttrText(song.active_idea[i])
				.NormalPaper(Blend(song.active_idea_clr[i], White(), 128+64))
				.Paper(Blend(song.active_idea_clr[i], GrayColor(), 128+64))
				.Ink(White()).NormalInk(Black()));
	}
	
	DataSong(true);
}

void PartIdea::DataSong(bool set_cursor) {
	Song& song = GetSong();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& p = song.parts[i];
		String k;
		Color clr = White();
		{
			k = p.name;
			clr = GetSongPartPaperColor(p.type);
		}
		parts.Set(i, 0,
			AttrText(k)
				.NormalPaper(clr)
				.Paper(Blend(clr, GrayColor(64)))
				.Ink(White()).NormalInk(Black())
			);
		
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++) {
			String& k = p.active_idea[j];
			if (k.IsEmpty()) {
				parts.Set(i, 1+j, Value());
			}
			else {
				Color clr = p.active_idea_clr[j];
				parts.Set(i, 1+j,
					AttrText(k)
						.NormalPaper(Blend(clr, White(), 128+64))
						.Paper(Blend(clr, GrayColor(64)))
						.Ink(White()).NormalInk(Black())
					);
			}
		}
	}
	parts.SetCount(song.parts.GetCount());
	
	if (set_cursor && !parts.IsCursor() && song.part_cursor >= 0 && song.part_cursor < parts.GetCount())
		parts.SetCursor(song.part_cursor);
	
	DataPart(set_cursor);
}

void PartIdea::DataPart(bool set_cursor) {
	if (!parts.IsCursor()) {
		contents.Clear();
		imageries.Clear();
		symbolisms.Clear();
		return;
	}
	
	Song& song = GetSong();
	int part_i = parts.GetCursor();
	StaticPart& part = song.parts[part_i];
	
	for(int i = 0; i < part.contents.GetCount(); i++) {
		StaticContentSuggestion& o = part.contents[i];
		contents.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	contents.SetCount(part.contents.GetCount());
	
	if (set_cursor && !contents.IsCursor() && part.content_cursor >= 0 && part.content_cursor < contents.GetCount())
		contents.SetCursor(part.content_cursor);
	DataContent(set_cursor);
}

void PartIdea::DataContent(bool set_cursor) {
	if (!contents.IsCursor()) {
		imageries.Clear();
		symbolisms.Clear();
		return;
	}
	
	Song& song = GetSong();
	int part_i = parts.GetCursor();
	int content_i = contents.GetCursor();
	StaticPart& part = song.parts[part_i];
	StaticContentSuggestion& c = part.contents[content_i];
	
	for(int i = 0; i < c.imageries.GetCount(); i++) {
		auto& o = c.imageries[i];
		imageries.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	imageries.SetCount(c.imageries.GetCount());
	
	if (set_cursor && !imageries.IsCursor() && c.cursor >= 0 && c.cursor < imageries.GetCount())
		imageries.SetCursor(c.cursor);
	
	DataImagery(set_cursor);
}

void PartIdea::DataImagery(bool set_cursor) {
	if (!imageries.IsCursor()) {
		symbolisms.Clear();
		return;
	}
	
	Song& song = GetSong();
	int part_i = parts.GetCursor();
	int content_i = contents.GetCursor();
	int imagery_i = imageries.GetCursor();
	StaticPart& part = song.parts[part_i];
	StaticContentSuggestion& c = part.contents[content_i];
	StaticImagery& img = c.imageries[imagery_i];
	
	for(int i = 0; i < img.symbolisms.GetCount(); i++) {
		auto& o = img.symbolisms[i];
		symbolisms.Set(i, 0, AttrText(Capitalize(o.text))
			.Paper(Blend(o.clr, GrayColor(), 128+64))
			.NormalPaper(Blend(o.clr, White(), 128+64))
			.Ink(White()).NormalInk(Black())
			);
	}
	symbolisms.SetCount(img.symbolisms.GetCount());
	
	if (set_cursor && !symbolisms.IsCursor() && img.cursor >= 0 && img.cursor < symbolisms.GetCount())
		symbolisms.SetCursor(img.cursor);
	
}

void PartIdea::ToolMenu(Bar& bar) {
	bar.Add(t_("Get content suggestions"), AppImg::BlueRing(), THISBACK(GetContentSuggestions)).Key(K_CTRL_Q);
	bar.Add(t_("Get specific imagery suggestions"), AppImg::BlueRing(), THISBACK(GetImagerySuggestions)).Key(K_CTRL_W);
	bar.Add(t_("Get symbolism suggestions"), AppImg::BlueRing(), THISBACK(GetSymbolismSuggestions)).Key(K_CTRL_E);
	bar.Separator();
	bar.Add(t_("Set as active idea"), AppImg::VioletRing(), THISBACK(SetAsActiveIdea)).Key(K_F5);
	
}

String PartIdea::GetStatusText() {
	return "";
}

void PartIdea::OnListPart() {
	Song& song = GetSong();
	if (parts.IsCursor()) {
		int part_i = parts.GetCursor();
		StaticPart& o = song.parts[part_i];
		song.part_cursor = part_i;
		if (o.contents.IsEmpty()) {
			contents.Clear();
			symbolisms.Clear();
			imageries.Clear();
			PostCallback(THISBACK(GetContentSuggestions));
			return;
		}
	}
	DataPart(true);
}

void PartIdea::OnListContent() {
	Song& song = GetSong();
	if (contents.IsCursor()) {
		int part_i = parts.GetCursor();
		int content_i = contents.GetCursor();
		StaticPart& p = song.parts[part_i];
		StaticContentSuggestion& c = p.contents[content_i];
		p.content_cursor = content_i;
		if (c.imageries.IsEmpty()) {
			imageries.Clear();
			symbolisms.Clear();
			PostCallback(THISBACK(GetImagerySuggestions));
			return;
		}
	}
	DataContent(false);
}

void PartIdea::OnListImagery() {
	Song& song = GetSong();
	if (imageries.IsCursor()) {
		int part_i = parts.GetCursor();
		int content_i = contents.GetCursor();
		int imagery_i = imageries.GetCursor();
		StaticPart& p = song.parts[part_i];
		StaticContentSuggestion& c = p.contents[content_i];
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

void PartIdea::OnListSymbolism() {
	Song& song = GetSong();
	
	if (symbolisms.IsCursor()) {
		int part_i = parts.GetCursor();
		int content_i = contents.GetCursor();
		int imagery_i = imageries.GetCursor();
		int symbolism_i = symbolisms.GetCursor();
		StaticPart& p = song.parts[part_i];
		StaticContentSuggestion& c = p.contents[content_i];
		StaticImagery& img = c.imageries[imagery_i];
		StaticSymbolism& sym = img.symbolisms[symbolism_i];
		img.cursor = symbolism_i;
	}
}

bool PartIdea::HasActiveIdea() {
	Song& song = GetSong();
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		if (song.active_idea[i].IsEmpty())
			return false;
	return true;
}

void PartIdea::GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v.GetAdd(key) = Attr::AttrKeys[i][2];
			}
			else {
				v.GetAdd(key) = Attr::AttrKeys[i][3];
			}
		}
	}
}

void PartIdea::GetContentSuggestions() {
	if (!HasActiveIdea() || disabled)
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!parts.IsCursor())
		return;
	
	DisableAll();
	
	Song& s = GetSong();
	s.RealizePipe();
	int part_i = parts.GetCursor();
	StaticPart& part = s.parts[part_i];
	
	VectorMap<String,String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	
	String known_part_ideas;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		
		if (!known_part_ideas.IsEmpty())
			known_part_ideas << ";;;";
		
		known_part_ideas << TrimBoth(sp.name);
		
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			known_part_ideas << "__" << sp.active_idea[j];
		
	}
	
	{
		TaskMgr& m = *s.pipe;
		m.GetPartContentSuggestions(
			s.active_idea[0],
			s.active_idea[1],
			s.active_idea[2],
			s.active_idea[3],
			part.name,
			known_part_ideas,
			attrs.GetValues(),
			THISBACK2(OnContentSuggestions, &s, part_i));
	}
}

void PartIdea::GetImagerySuggestions() {
	if (!HasActiveIdea() || disabled)
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!parts.IsCursor())
		return;
	
	DisableAll();
	
	Song& s = *p.song;
	int part_i = parts.GetCursor();
	int content_i = contents.GetCursor();
	StaticPart& part = s.parts[part_i];
	StaticContentSuggestion& c = part.contents[content_i];
	
	VectorMap<String,String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	String known_part_ideas;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		
		if (!known_part_ideas.IsEmpty())
			known_part_ideas << ";;;";
		
		known_part_ideas << TrimBoth(sp.name);
		
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			known_part_ideas << "__" << sp.active_idea[j];
		
	}
	
	{
		TaskMgr& m = *song.pipe;
		m.GetPartImagerySuggestions(
			s.active_idea[0],
			s.active_idea[1],
			s.active_idea[2],
			s.active_idea[3],
			c.text,
			part.name,
			known_part_ideas,
			attrs.GetValues(),
			THISBACK1(OnImagerySuggestions, &c));
	}
}

void PartIdea::GetSymbolismSuggestions() {
	if (!HasActiveIdea() || disabled)
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	if (!parts.IsCursor())
		return;
	
	DisableAll();
	
	Song& s = *p.song;
	int part_i = parts.GetCursor();
	int content_i = contents.GetCursor();
	int imagery_i = imageries.GetCursor();
	StaticPart& part = s.parts[part_i];
	StaticContentSuggestion& c = part.contents[content_i];
	StaticImagery& img = c.imageries[imagery_i];
	
	VectorMap<String,String> attrs;
	GetAttrs(p.artist->data, attrs);
	GetAttrs(p.release->data, attrs);
	GetAttrs(p.song->data, attrs);
	
	Song& song = GetSong();
	
	song.RealizePipe();
	
	String known_part_ideas;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& sp = s.parts[i];
		
		if (!known_part_ideas.IsEmpty())
			known_part_ideas << ";;;";
		
		known_part_ideas << TrimBoth(sp.name);
		
		for(int j = 0; j < IDEAPATH_PARTCOUNT; j++)
			known_part_ideas << "__" << sp.active_idea[j];
		
	}
	
	{
		TaskMgr& m = *song.pipe;
		m.GetPartSymbolismSuggestions(
			s.active_idea[0],
			s.active_idea[1],
			s.active_idea[2],
			s.active_idea[3],
			c.text,
			img.text,
			part.name,
			known_part_ideas,
			attrs.GetValues(),
			THISBACK1(OnSymbolismSuggestions, &img));
	}
}

void PartIdea::OnContentSuggestions(String result, Song* song_, int part_i) {
	EnableAll();
	
	HotFixResult(result);
	
	Song& song = *song_;
	StaticPart& part = song.parts[part_i];
	Vector<String> lines = Split(result, "\n", false);
	
	part.contents.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String s = TrimBoth(lines[i].Mid(1));
		if (s.IsEmpty()) break;
		StaticContentSuggestion& o = part.contents.Add();
		ParseTextColor(s, o.text, o.clr);
	}
	
	PostCallback(THISBACK1(DataPart, false));
}

void PartIdea::OnImagerySuggestions(String result, StaticContentSuggestion* c) {
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

void PartIdea::OnSymbolismSuggestions(String result, StaticImagery* img) {
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

void PartIdea::SetAsActiveIdea() {
	if (!symbolisms.IsCursor())
		return;
	int content_i = contents.GetCursor();
	int imagery_i = imageries.GetCursor();
	int symblism_i = symbolisms.GetCursor();
	
	try {
		Song& song = GetSong();
		int part_i = parts.GetCursor();
		StaticPart& part = song.parts[part_i];
		StaticContentSuggestion& c = part.contents[content_i];
		StaticImagery& img =  c.imageries[imagery_i];
		StaticSymbolism& sym =  img.symbolisms[symblism_i];
		
		part.active_idea[IDEAPATH_PART_CONTENT] = c.text;
		part.active_idea[IDEAPATH_PART_IMAGERY] = img.text;
		part.active_idea[IDEAPATH_PART_SYMBOLISM] = sym.text;
		
		part.active_idea_clr[IDEAPATH_PART_CONTENT] = c.clr;
		part.active_idea_clr[IDEAPATH_PART_IMAGERY] = img.clr;
		part.active_idea_clr[IDEAPATH_PART_SYMBOLISM] = sym.clr;
	}
	catch (NoPointerExc e) {}
	
	PostCallback(THISBACK1(DataSong, false));
}
