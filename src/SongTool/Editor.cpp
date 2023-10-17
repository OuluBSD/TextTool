#include "SongTool.h"


Editor::Editor(SongTool* app) : app(*app) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << tablist << artists << releases << songs << parts;
	artists.WhenBar << THISBACK(ArtistMenu);
	releases.WhenBar << THISBACK(ReleaseMenu);
	songs.WhenBar << THISBACK(SongMenu);
	parts.WhenBar << THISBACK(PartMenu);
	tablist <<= THISBACK(UpdateView);
	importer.WhenStructureChange << THISBACK(DataSong);
	
	tablist.NoHeader();
	tablist.AddColumn("");
	tablist.AddColumn("");
	tablist.ColumnWidths("2 3");
	tablist.SetCursor(0);
	
	artists.AddColumn(t_("Artist"));
	artists <<= THISBACK(DataArtist);
	
	releases.AddColumn(t_("Title"));
	releases.AddColumn(t_("Date"));
	releases.ColumnWidths("3 2");
	releases <<= THISBACK(DataRelease);
	
	songs.AddColumn(t_("Title"));
	songs.AddColumn(t_("Project name"));
	songs.AddColumn(t_("Artist"));
	songs.ColumnWidths("2 2 1");
	songs <<= THISBACK(DataSong);
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Lines"));
	parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);
	
	info.editor = this;
	song_struct.editor = this;
	
	for(int i = 0; i < 3; i++)
		reverse[i].SetSource(i);
	
	rev_pattern.UseRev();
}

void Editor::AddItem(String g, String i, SongToolCtrl& c) {
	ListItem& it = items.Add();
	it.group = g;
	it.item = i;
	it.ctrl = &c;
}

void Editor::InitListItems() {
	for (const ListItem& it : items) {
		tablist.Add(it.group, it.item);
		base.Add(it.ctrl->SizePos());
	}
}

void Editor::InitSimplified() {
	/*
	
	- native language to English
	- English to "json-style structured format"
	- text editor for structured format
		- list of notes
	- structured format to English
	- English to native language
	
	1. edit native language version next to English direct translation
	2. edit English release version (which may differ from native significantly)
		- iteratively (with timeline)
			- editor for lyrics
			- ai evaluation
				- generate naive interpretation (show next to the original line)
					- generate f-m difference (show also)
				- error list
					- negative attributes (separate f-m)
					- bad impact scoring
				- info list
					- recognized
						- protagonist
						- antagonist
						- friends
						- enemies
	- generate
		- text input for generative image ai
		- playlist pitching text
			- translation to native language
			
	- music video pipeline
	
	*/
	AddItem(t_("All"), t_("Info"), info);
	AddItem(t_("All"), t_("Calendar"), cal);
	AddItem(t_("All"), t_("Press Release"), pressrel);
	AddItem(t_("All"), t_("Public Relations"), pubrel);
	AddItem(t_("All"), t_("Marketing"), marketing);
	
	AddItem(t_("Album"), t_("Briefing"), album_briefing);
	AddItem(t_("Album"), t_("Idea notepad"), album_ideas);
	
	AddItem(t_("Song"), t_("Briefing"), song_briefing); // initial ideas, notes, etc.
	AddItem(t_("Song"), t_("Structure"), song_struct);
	
	AddItem(t_("Composition"), t_("Checklist"), checklist_composition);
	
	AddItem(t_("Manual override"), t_("Create Translation, Native To English"), structure_native_english);
	AddItem(t_("Manual override"), t_("Create From English"), structure_from_english);
	AddItem(t_("Manual override"), t_("Edit"), structure_edit);
	
	AddItem(t_("Context"), t_("Attributes"), ctx_attrs);
	AddItem(t_("Context"), t_("Idea"), auto_ideas);
	AddItem(t_("Context"), t_("Idea of a single part"), part_idea);
	AddItem(t_("Context"), t_("Story"), story_idea);
	AddItem(t_("Context"), t_("Production idea"), prod_idea);
	
	AddItem(t_("Text"), t_("English Parallel Compare"), text_autocompare_eng);
	AddItem(t_("Text"), t_("English Serial Compare"), serial_compare_eng);
	AddItem(t_("Text"), t_("Edit English"), text_edit_english);
	AddItem(t_("Text"), t_("Edit Native"), text_edit_native);
	
	AddItem(t_("Production"), t_("Checklist"), checklist_production);
	
	AddItem(t_("Music Video Structure"), t_("Create From Existing"), videostruct_import);
	AddItem(t_("Music Video Structure"), t_("Edit"), videostruct_edit);
	AddItem(t_("Music Video Structure"), t_("To Plan"), videostruct_to_plan);
	
	AddItem(t_("Music Video Production"), t_("Checklist"), video_checklist);
	
	AddItem(t_("Album"), t_("Cover Image"), cover_image);
	AddItem(t_("Album"), t_("Pitching"), pitching);
	
	AddItem(t_("Release"), t_("Checklist"), release_checklist);
	
	AddItem(t_("Extra"), t_("AI Image Generator"), image_gen);
	
	InitListItems();
}

void Editor::InitAdvanced() {
	AddItem(t_("All"), t_("Info"), info);
	AddItem(t_("All"), t_("Attributes"), attr);
	AddItem(t_("Release"), t_("Recruitment"), recru);
	AddItem(t_("Release"), t_("Social media campaign"), social);
	AddItem(t_("Release"), t_("Calendar"), cal);
	AddItem(t_("Release"), t_("Tasks"), task);
	AddItem(t_("Song"), t_("Importer"), importer);
	AddItem(t_("Song"), t_("Structure"), structure);
	AddItem(t_("Song"), t_("Analysis"), analysis);
	AddItem(t_("Song"), t_("Story"), story);
	AddItem(t_("Song"), t_("Impact"), impact);
	AddItem(t_("Song"), t_("Pattern mask"), patmask);
	AddItem(t_("Song"), t_("Pattern"), pattern);
	AddItem(t_("Song"), t_("Attribute scoring"), attrscore);
	AddItem(t_("Song"), t_("Impact scoring"), impact_scoring);
	AddItem(t_("Song"), t_("Pattern mask scoring"), mask_scoring);
	AddItem(t_("Song"), t_("Pattern scoring"), scoring);
	AddItem(t_("Song"), t_("Reverse: make impacts"), reverse_impact);
	AddItem(t_("Song"), t_("Reverse: make common mask"), reverse[0]);
	AddItem(t_("Song"), t_("Reverse: make separate mask"), reverse[1]);
	AddItem(t_("Song"), t_("Reverse: make pattern"), reverse[2]);
	AddItem(t_("Song"), t_("Reversed Pattern"), rev_pattern);
	AddItem(t_("Song"), t_("Reverse: make lyrics"), lyrics);
	AddItem(t_("Song"), t_("Composition"), composition);
	AddItem(t_("Song"), t_("Production"), production);
	AddItem(t_("Song"), t_("Rhymes"), rhymes);
	AddItem(t_("Release"), t_("Social Media"), some_chk);
	
	InitListItems();
}

void Editor::Init() {
	LoadLast();
	tablist.SetCursor(page);
	SetView(page);
	Data();
	app.SetBar(); // requires Data();
}

void Editor::SetView(int i) {
	for (const ListItem& it : items)
		it.ctrl->Hide();
	
	parts.Enable();
	
	WhenStopUpdating();
	
	if (i >= 0 && i < items.GetCount())
		items[i].ctrl->Show();
	
	page = i;
	DataPage();
	
	app.SetBar();
}

void Editor::DataPage() {
	StoreLast();
	
	try {
		if (page >= 0 && page < items.GetCount())
			items[page].ctrl->Data();
	}
	catch (NoPointerExc e) {
		LOG("error: " << e);
	}
}

void Editor::ToolMenu(Bar& bar) {
	if (page >= 0 && page < items.GetCount())
		items[page].ctrl->ToolMenu(bar);
}

String Editor::GetStatusText() {
	if (page >= 0 && page < items.GetCount())
		return items[page].ctrl->GetStatusText();
	else
		return String();
}

void Editor::MoveTab(int d) {
	if (tablist.IsCursor()) {
		int c = tablist.GetCursor();
		c += d;
		if (c >= 0 && c < tablist.GetCount())
			tablist.SetCursor(c);
	}
}

void Editor::MovePart(int d) {
	if (parts.IsCursor()) {
		int c = parts.GetCursor();
		c += d;
		if (c >= 0 && c < parts.GetCount())
			parts.SetCursor(c);
	}
}

void Editor::LoadLast() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	p.artist = 0;
	p.release = 0;
	p.song = 0;
	p.part = 0;
	for (Artist& a : db.artists) {
		if (a.native_name == app.last_artist) {
			p.artist = &a;
			for (Release& r : a.releases) {
				if (r.native_title == app.last_release) {
					p.release = &r;
					for (Song& s : r.songs) {
						if (s.native_title == app.last_song) {
							p.song = &s;
							for (StaticPart& part : s.parts) {
								if (part.name == app.last_part) {
									p.part = &part;
									break;
								}
							}
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}
}

void Editor::StoreLast() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	app.last_artist = p.artist ? p.artist->native_name : String();
	app.last_release = p.release ? p.release->native_title : String();
	app.last_song = p.song ? p.song->native_title : String();
	app.last_part = p.part ? p.part->name : String();
	app.Store();
}

void Editor::UpdateView() {
	SetView(tablist.GetCursor());
	DataPage();
}

void Editor::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		artists.Set(i, 0, a.native_name);
	}
	artists.SetCount(db.artists.GetCount());
	
	int cursor = max(0, p.GetActiveArtistIndex());
	if (cursor >= 0 && cursor < artists.GetCount())
		artists.SetCursor(cursor);
	
	DataArtist();
}

void Editor::DataArtist() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!artists.IsCursor()) {
		p.artist = 0;
		p.release = 0;
		p.song = 0;
		p.part = 0;
		DataPage();
		return;
	}
	
	p.artist = &db.artists[artists.GetCursor()];
	Artist& a = *p.artist;
	
	for(int i = 0; i < a.releases.GetCount(); i++) {
		Release& r = a.releases[i];
		releases.Set(i, 0, r.native_title);
		releases.Set(i, 1, r.date);
	}
	releases.SetCount(a.releases.GetCount());
	
	int cursor = max(0, p.GetActiveReleaseIndex());
	if (cursor >= 0 && cursor < releases.GetCount())
		releases.SetCursor(cursor);
	
	DataRelease();
}

void Editor::DataRelease() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!releases.IsCursor() || !p.artist) {
		p.release = 0;
		p.song = 0;
		p.part = 0;
		DataPage();
		return;
	}
	
	p.release = &p.artist->releases[releases.GetCursor()];
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	for(int i = 0; i < r.songs.GetCount(); i++) {
		Song& s = r.songs[i];
		songs.Set(i, 0, s.native_title);
		songs.Set(i, 1, s.prj_name);
		songs.Set(i, 2, s.artist);
	}
	songs.SetCount(r.songs.GetCount());
	
	int cursor = max(0, p.GetActiveSongIndex());
	if (cursor >= 0 && cursor < songs.GetCount())
		songs.SetCursor(cursor);
	
	DataSong();
}

void Editor::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!songs.IsCursor() || !p.artist || !p.release) {
		p.song = 0;
		DataPage();
		return;
	}
	
	p.song = &p.release->songs[songs.GetCursor()];
	Artist& a = *p.artist;
	Release& r = *p.release;
	Song& s = *p.song;
	
	
	#if 0
	if (!s.pipe) {
		DataPage();
		return;
	}
	Pipe& e = *s.pipe;
	
	for(int i = 0; i < e.parts.GetCount()+1; i++) {
		String k;
		int c = 0;
		if (i == 0) {
			k = t_("Whole song");
		}
		else {
			int j = i-1;
			Part& p = e.parts[j];
			k = p.name;
			c = p.lines.GetCount();
		}
		parts.Set(i, 0, k);
		parts.Set(i, 1, c);
	}
	parts.SetCount(e.parts.GetCount());
	
	int cursor = max(0, e.p.GetActivePartIndex());
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	#else
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		String k;
		int c = 0;
		Color clr = White();
		StaticPart& p = s.parts[i];
		k = p.name;
		clr = GetSongPartPaperColor(p.type);
		c = p.rhymes.GetCount();
		parts.Set(i, 0, AttrText(k).NormalPaper(clr));
		parts.Set(i, 1, c);
	}
	parts.SetCount(s.parts.GetCount());
	
	int cursor = max(0, p.GetActivePartIndex());
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	
	#endif
	
	DataPart();
}

void Editor::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!parts.IsCursor() || !p.artist || !p.release || !p.song /*|| !p.song->pipe*/) {
		DataPage();
		return;
	}
	
	// OLD!!
	/*
	Pipe& e = *p.song->pipe;
	PipePtrs& pp = p.song->pipe->p;
	
	int cursor = parts.GetCursor();
	if (!cursor) {
		db.ctx.active_wholesong = true;
		pp.part = 0;
	}
	else {
		db.ctx.active_wholesong = false;
		pp.part = &e.parts[cursor-1];
	}
	
	int part_i = pp.GetActivePartIndex();
	if (part_i >= 0 && part_i < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(1+part_i);
	*/
	
	Song& song = *p.song;
	int cursor = parts.GetCursor();
	
	db.ctx.active_wholesong = false;
	p.part = &song.parts[cursor];
	
	int part_i = p.GetActivePartIndex();
	if (part_i >= 0 && part_i < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(part_i);
	
	
	DataPage();
}

void Editor::ArtistMenu(Bar& bar) {
	bar.Add(t_("Add Artist"), THISBACK(AddArtist));
	
	if (artists.IsCursor()) {
		bar.Add(t_("Rename Artist"), THISBACK(RenameArtist));
		bar.Add(t_("Delete Artist"), THISBACK(RemoveArtist));
	}
}

void Editor::ReleaseMenu(Bar& bar) {
	bar.Add(t_("Add Release"), THISBACK(AddRelease));
	
	if (releases.IsCursor()) {
		bar.Add(t_("Rename Release"), THISBACK(RenameRelease));
		bar.Add(t_("Delete Release"), THISBACK(RemoveRelease));
	}
}

void Editor::SongMenu(Bar& bar) {
	bar.Add(t_("Add Song"), THISBACK(AddSong));
	
	if (songs.IsCursor()) {
		bar.Add(t_("Rename Song"), THISBACK(RenameSong));
		bar.Add(t_("Delete Song"), THISBACK(RemoveSong));
	}
}

void Editor::PartMenu(Bar& bar) {
	
}

void Editor::AddArtist() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add Artist"),
		t_("Artist's English name"),
		0
	);
	if (!b) return;
	
	int artist_i = -1;
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		if (a.english_name == name) {
			artist_i = i;
			break;
		}
	}
	if (artist_i >= 0) {
		/*if (!PromptYesNo(DeQtf(Format(t_("Do you want to replace file for artist '%s'"), name)))
			return;*/
		PromptOK(DeQtf(t_("Artist exist already")));
		return;
	}
	
	Artist& a = db.artists.Add();
	a.file_title = MakeTitle(name);
	a.english_name = name;
	p.artist = &a;
	
	Data();
}

void Editor::RenameArtist() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist)
		return;
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Rename Artist"),
		t_("Artist's English name"),
		0
	);
	if (!b) return;
	
	p.artist->english_name = name;
	
	Data();
}

void Editor::RemoveArtist() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist)
		return;
	int idx = p.GetActiveArtistIndex();
	if (idx < 0) return;
	db.artists.Remove(idx);
	Data();
}

void Editor::AddRelease() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist)
		return;
	Artist& a = *p.artist;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Add Release"),
		t_("Release's English title"),
		0
	);
	if (!b) return;
	
	int rel_i = -1;
	for(int i = 0; i < a.releases.GetCount(); i++) {
		Release& r = a.releases[i];
		if (r.english_title == title) {
			rel_i = i;
			break;
		}
	}
	if (rel_i >= 0) {
		PromptOK(DeQtf(t_("Release exist already")));
		return;
	}
	
	Release& r = a.releases.Add();
	r.file_title = MakeTitle(title);
	r.english_title = title;
	p.release = &r;
	
	DataArtist();
}

void Editor::RenameRelease() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.release)
		return;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Rename Release"),
		t_("Release's English title"),
		0
	);
	if (!b) return;
	
	p.release->english_title = title;
	
	DataArtist();
}

void Editor::RemoveRelease() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist || !p.release)
		return;
	int idx = p.GetActiveReleaseIndex();
	if (idx < 0) return;
	p.artist->releases.Remove(idx);
	DataArtist();
}

void Editor::AddSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.artist)
		return;
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Add Song"),
		t_("Song's English title"),
		0
	);
	if (!b) return;
	
	int rel_i = -1;
	for(int i = 0; i < r.songs.GetCount(); i++) {
		Song& s = r.songs[i];
		if (s.english_title == title) {
			rel_i = i;
			break;
		}
	}
	if (rel_i >= 0) {
		PromptOK(DeQtf(t_("Song exist already")));
		return;
	}
	
	Song& s = r.songs.Add();
	s.file_title = MakeTitle(title);
	s.english_title = title;
	p.song = &s;
	
	DataArtist();
}

void Editor::RenameSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Rename Song"),
		t_("Song's English title"),
		0
	);
	if (!b) return;
	
	p.song->english_title = title.ToString();
	
	DataRelease();
}

void Editor::RemoveSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release)
		return;
	int idx = p.GetActiveSongIndex();
	if (idx < 0) return;
	p.release->songs.Remove(idx);
	p.song = 0;
	DataRelease();
}
