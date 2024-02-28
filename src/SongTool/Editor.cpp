#include "SongTool.h"


Editor::Editor(SongTool* app) : app(*app) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << artists << releases << songs;
	artists.WhenBar << THISBACK(ArtistMenu);
	releases.WhenBar << THISBACK(ReleaseMenu);
	songs.WhenBar << THISBACK(SongMenu);
	parts.WhenBar << THISBACK(PartMenu);
	//importer.WhenStructureChange << THISBACK(DataSong);
	
	page_group_list.AddColumn(t_("Page group"));
	page_group_list <<= THISBACK(ViewPageGroup);
	
	page_list.AddColumn(t_("Page"));
	page_list <<= THISBACK(ViewPage);
	
	artists.AddColumn(t_("Artist"));
	artists <<= THISBACK(DataArtist);
	
	releases.AddColumn(t_("Release"));
	//releases.AddColumn(t_("Date"));
	releases.ColumnWidths("3 2");
	releases <<= THISBACK(DataRelease);
	
	songs.AddColumn(t_("Song"));
	//songs.AddColumn(t_("Project name"));
	//songs.AddColumn(t_("Artist"));
	//songs.ColumnWidths("2 2 1");
	songs <<= THISBACK(DataSong);
	
	/*parts.AddColumn(t_("Part"));
	//parts.AddColumn(t_("Lines"));
	//parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);*/
	
	info.editor = this;
	//song_struct.editor = this;
	
	//for(int i = 0; i < 3; i++)
	//	reverse[i].SetSource(i);
	
	//rev_pattern.UseRev();
	
}

void Editor::AddItem(String g, String i, SongToolCtrl& c) {
	ListItem& it = items.GetAdd(g).Add();
	it.item = i;
	it.ctrl = &c;
}

void Editor::InitListItems() {
	for(int i = 0; i < items.GetCount(); i++) {
		String group = items.GetKey(i);
		page_group_list.Add(group);
	}
	INHIBIT_ACTION(page_group_list);
	if (page_group_list.GetCount() && !page_group_list.IsCursor())
		page_group_list.SetCursor(0);
	PostCallback(THISBACK(ViewPageGroup));
}

void Editor::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	
	AddItem(t_("Database"), t_("Songs"), song_data);
	AddItem(t_("Database"), t_("Tokens"), tokens_data);
	AddItem(t_("Database"), t_("Token phrases"), token_phrases);
	AddItem(t_("Database"), t_("Words"), song_words);
	AddItem(t_("Database"), t_("Ambiguous word pairs"), ambiguous_word_pairs);
	AddItem(t_("Database"), t_("Virtual phrases"), virtual_phrases);
	AddItem(t_("Database"), t_("Virtual phrase parts"), virtual_phrase_parts);
	AddItem(t_("Database"), t_("Virtual phrase structs"), virtual_phrase_structs);
	AddItem(t_("Database"), t_("Phrase parts"), phrase_parts);
	AddItem(t_("Database"), t_("Phrase part analysis 1"), phrase_part_analysis);
	AddItem(t_("Database"), t_("Phrase part analysis 2"), phrase_part_analysis2);
	AddItem(t_("Database"), t_("Rhyme container"), rhyme_container);
	AddItem(t_("Database"), t_("Action attributes"), action_attrs);
	AddItem(t_("Database"), t_("Action parallels"), action_parallels);
	AddItem(t_("Database"), t_("Action transitions"), action_transitions);
	AddItem(t_("Database"), t_("Wordnet"), song_wordnet);
	AddItem(t_("Database"), t_("Attributes"), db_attrs);
	AddItem(t_("Database"), t_("Diagnostics"), db_diagnostics);
	/*AddItem(t_("Database"), t_("Analysis"), song_analysis);
	AddItem(t_("Database"), t_("Phrases"), song_phrases);
	//AddItem(t_("Database"), t_("Search phrases"), song_searchphrases);
	AddItem(t_("Database"), t_("Phrase templates"), song_tmpl);
	AddItem(t_("Database"), t_("Template scoring"), song_tmplscoring);
	AddItem(t_("Database"), t_("Wordnet scoring"), song_wnscoring);
	AddItem(t_("Database"), t_("Line actions"), song_lineactions);
	AddItem(t_("Database"), t_("Template actions"), song_tmplactions);
	AddItem(t_("Database"), t_("Line types"), line_types);
	AddItem(t_("Database"), t_("Export"), export_page);*/
	
	AddItem(t_("Company"), t_("Info"), info);
	AddItem(t_("Company"), t_("Calendar"), cal);
	AddItem(t_("Company"), t_("Press Release"), pressrel);
	AddItem(t_("Company"), t_("Public Relations"), pubrel);
	AddItem(t_("Company"), t_("Marketing"), marketing);
	//AddItem(t_("Company"), t_("Vocabulary"), vocabulary);
	
	AddItem(t_("Product"), t_("Briefing"), album_briefing);
	AddItem(t_("Product"), t_("Idea notepad"), album_ideas);
	AddItem(t_("Product"), t_("Cover Image"), cover_image);
	AddItem(t_("Product"), t_("Pitching"), pitching);
	
	AddItem(t_("Song"), t_("Briefing"), song_briefing); // initial ideas, notes, etc.
	
	AddItem(t_("Lyrics"), t_("Structure"), song_struct);
	AddItem(t_("Lyrics"), t_("Nana editor"), nana_editor);
	AddItem(t_("Lyrics"), t_("Song pool"), song_pool);
	AddItem(t_("Lyrics"), t_("Lyrics solver"), lyrics_solver);
	//AddItem(t_("Song"), t_("Fine Structure"), fine_struct);
	//AddItem(t_("Song"), t_("Rhyme Structure"), rhyme_struct);
	
	//AddItem(t_("Song lyrics"), t_("Production idea"), prod_idea);
	/*AddItem(t_("Song lyrics"), t_("Attributes"), ctx_attrs);
	AddItem(t_("Song lyrics"), t_("Line picker"), line_picker);
	AddItem(t_("Song lyrics"), t_("Line setter"), line_setter);
	//AddItem(t_("Song lyrics"), t_("Secondary setter"), sec_setter);
	AddItem(t_("Song lyrics"), t_("Native editor"), nat_editor);
	AddItem(t_("Song lyrics"), t_("English editor"), eng_edit);*/
	
	
	AddItem(t_("Checklist"), t_("Song"), checklist_composition);
	AddItem(t_("Checklist"), t_("Production"), checklist_production);
	AddItem(t_("Checklist"), t_("Music Video Production"), video_checklist);
	AddItem(t_("Checklist"), t_("Release"), release_checklist);
	
	
	InitListItems();
}

void Editor::Init() {
	INHIBIT_ACTION_(page_group_list, 0);
	INHIBIT_ACTION_(page_list, 1);
	LoadLast();
	page_group_list.SetCursor(page_group);
	int page = this->page.GetAdd(page_group, 0);
	page_list.SetCursor(page);
	SetView(page_group, page);
	Data();
	app.SetBar(); // requires Data();
}

void Editor::SetView(int i, int j) {
	for (const auto& v : items)
		for (const ListItem& it : v)
			it.ctrl->Hide();
	
	parts.Enable();
	
	WhenStopUpdating();
	
	if (i >= 0 && i < items.GetCount() && j >= 0 && j < items[i].GetCount())
		items[i][j].ctrl->Show();
	
	// Save 'cookie' about last viewed page
	page_group = i;
	page.GetAdd(i) = j;
	
	DataPage();
	
	app.SetBar();
}

void Editor::DataPage() {
	if (app.skip_data) return;
	
	StoreLast();
	
	int page = this->page.GetAdd(page_group, 0);
	try {
		if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
			items[page_group][page].ctrl->Data();
	}
	catch (NoPointerExc e) {
		LOG("error: " << e);
	}
}

void Editor::ToolMenu(Bar& bar) {
	int page = this->page.GetAdd(page_group, 0);
	if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
		items[page_group][page].ctrl->ToolMenu(bar);
}

String Editor::GetStatusText() {
	int page = this->page.GetAdd(page_group, 0);
	if (page_group >= 0 && page_group < items.GetCount() && page >= 0 && page < items[page_group].GetCount())
		return items[page_group][page].ctrl->GetStatusText();
	else
		return String();
}

void Editor::MovePageGroup(int d) {
	if (page_group_list.IsCursor()) {
		int c = page_group_list.GetCursor();
		c += d;
		if (c >= 0 && c < page_group_list.GetCount())
			page_group_list.SetCursor(c);
	}
}

void Editor::MovePage(int d) {
	if (page_list.IsCursor()) {
		int c = page_list.GetCursor();
		c += d;
		if (c >= 0 && c < page_list.GetCount())
			page_list.SetCursor(c);
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
						/*if (s.native_title == app.last_song) {
							p.song = &s;
							for (StaticPart& part : s.parts) {
								if (part.name == app.last_part) {
									p.part = &part;
									break;
								}
							}
							break;
						}*/
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
	TODO //app.last_song = p.song ? p.song->native_title : String();
	app.last_part = p.part ? p.part->name : String();
	app.Store();
}

void Editor::ViewPageGroup() {
	int page_group = page_group_list.GetCursor();
	int page = this->page.GetAdd(page_group, 0);
	
	if (!page_group_list.IsCursor() || page_group < 0 || page_group >= items.GetCount()) {
		return;
	}
	
	const auto& v = items[page_group];
	for(int j = 0; j < v.GetCount(); j++) {
		const ListItem& it = v[j];
		page_list.Set(j, 0, it.item);
		base.Add(it.ctrl->SizePos());
	}
	INHIBIT_ACTION(page_list);
	page_list.SetCount(v.GetCount());
	page_list.SetCursor(page);
	
	if (page >= v.GetCount())
		page = v.GetCount()-1;
	
	SetView(page_group, page);
	DataPage();
}

void Editor::ViewPage() {
	SetView(page_group_list.GetCursor(), page_list.GetCursor());
	//DataPage(); // Duplicate
}

void Editor::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		artists.Set(i, 0, a.native_name);
	}
	INHIBIT_ACTION(artists);
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
	INHIBIT_ACTION(releases);
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
		TODO /*songs.Set(i, 0, s.native_title);
		songs.Set(i, 1, s.prj_name);
		songs.Set(i, 2, s.artist);*/
	}
	INHIBIT_ACTION(songs);
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
	TaskMgr& e = *s.pipe;
	
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
	INHIBIT_ACTION(parts);
	parts.SetCount(e.parts.GetCount());
	
	int cursor = max(0, e.p.GetActivePartIndex());
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	#else
	
	TODO
	/*
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
	INHIBIT_ACTION(parts);
	parts.SetCount(s.parts.GetCount());*/
	
	int cursor = 0;
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	
	#endif
	
	DataPage();
}

#if 0
void Editor::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!parts.IsCursor() || !p.artist || !p.release || !p.song /*|| !p.song->pipe*/) {
		DataPage();
		return;
	}
	
	// OLD!!
	/*
	TaskMgr& e = *p.song->pipe;
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
#endif

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
	
	TODO
	/*
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
	*/
	DataArtist();
}

void Editor::RenameSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song)
		return;
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Rename Song"),
		t_("Song's English title"),
		0
	);
	if (!b) return;
	
	
	TODO //p.song->english_title = title.ToString();
	
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
