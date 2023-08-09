#include "SongTool.h"


Editor::Editor() {
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
	tablist.ColumnWidths("1 3");
	tablist.Add(t_("All"), t_("Info"));
	tablist.Add(t_("All"), t_("Attributes"));
	tablist.Add(t_("Release"), t_("Recruitment"));
	tablist.Add(t_("Release"), t_("Social media campaign"));
	tablist.Add(t_("Release"), t_("Calendar"));
	tablist.Add(t_("Release"), t_("Tasks"));
	tablist.Add(t_("Song"), t_("Importer"));
	tablist.Add(t_("Song"), t_("Structure"));
	tablist.Add(t_("Song"), t_("Analysis"));
	tablist.Add(t_("Song"), t_("Story"));
	tablist.Add(t_("Song"), t_("Impact"));
	tablist.Add(t_("Song"), t_("Pattern mask"));
	tablist.Add(t_("Song"), t_("Pattern"));
	tablist.Add(t_("Song"), t_("Attribute scoring"));
	tablist.Add(t_("Song"), t_("Impact scoring"));
	tablist.Add(t_("Song"), t_("Pattern mask scoring"));
	tablist.Add(t_("Song"), t_("Pattern scoring"));
	tablist.Add(t_("Song"), t_("Reverse: make impacts"));
	tablist.Add(t_("Song"), t_("Reverse: make common mask"));
	tablist.Add(t_("Song"), t_("Reverse: make separate mask"));
	tablist.Add(t_("Song"), t_("Reverse: make pattern"));
	tablist.Add(t_("Song"), t_("Reversed Pattern"));
	tablist.Add(t_("Song"), t_("Reverse: make lyrics"));
	tablist.Add(t_("Song"), t_("Composition"));
	tablist.Add(t_("Song"), t_("Production"));
	tablist.Add(t_("Song"), t_("Rhymes"));
	tablist.SetCursor(0);
	
	artists.AddColumn(t_("Artist"));
	artists <<= THISBACK(DataArtist);
	
	releases.AddColumn(t_("Title"));
	releases.AddColumn(t_("Date"));
	releases.ColumnWidths("3 1");
	releases <<= THISBACK(DataRelease);
	
	songs.AddColumn(t_("Artist"));
	songs.AddColumn(t_("Title"));
	songs.AddColumn(t_("Project name"));
	songs <<= THISBACK(DataSong);
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Lines"));
	parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);
	
	info.editor = this;
	
	base.Add(info.SizePos());
	base.Add(attr.SizePos());
	base.Add(recru.SizePos());
	base.Add(social.SizePos());
	base.Add(cal.SizePos());
	base.Add(task.SizePos());
	base.Add(importer.SizePos());
	base.Add(patmask.SizePos());
	base.Add(pattern.SizePos());
	base.Add(attrscore.SizePos());
	base.Add(scoring.SizePos());
	base.Add(composition.SizePos());
	base.Add(analysis.SizePos());
	base.Add(impact.SizePos());
	base.Add(production.SizePos());
	base.Add(rhymes.SizePos());
	base.Add(reverse[0].SizePos());
	base.Add(reverse[1].SizePos());
	base.Add(reverse[2].SizePos());
	base.Add(lyrics.SizePos());
	base.Add(reverse_impact.SizePos());
	base.Add(rev_pattern.SizePos());
	base.Add(story.SizePos());
	base.Add(structure.SizePos());
	base.Add(impact_scoring.SizePos());
	base.Add(mask_scoring.SizePos());
	
	for(int i = 0; i < 3; i++)
		reverse[i].SetSource(i);
	
	rev_pattern.UseRev();
	
	PostCallback(THISBACK(Data)); // sets active artist, song, etc.
}

void Editor::Init() {
	tablist.SetCursor(page);
	SetView(page);
}

void Editor::SetView(int i) {
	info.Hide();
	attr.Hide();
	recru.Hide();
	social.Hide();
	cal.Hide();
	task.Hide();
	importer.Hide();
	patmask.Hide();
	pattern.Hide();
	attrscore.Hide();
	scoring.Hide();
	reverse[0].Hide();
	reverse[1].Hide();
	reverse[2].Hide();
	reverse_impact.Hide();
	rev_pattern.Hide();
	composition.Hide();
	analysis.Hide();
	production.Hide();
	rhymes.Hide();
	impact.Hide();
	story.Hide();
	structure.Hide();
	impact_scoring.Hide();
	mask_scoring.Hide();
	lyrics.Hide();
	
	parts.Enable();
	
	WhenStopUpdating();
	
	switch (i) {
		default: i = 0;
		case 0: info.Show(); break;
		case 1: attr.Show(); break;
		case 2: recru.Show(); break;
		case 3: social.Show(); break;
		case 4: cal.Show(); break;
		case 5: task.Show(); break;
		case 6: importer.Show(); break;
		case 7: structure.Show(); break;
		case 8: analysis.Show(); break;
		case 9: story.Show(); break;
		case 10: impact.Show(); break;
		case 11: patmask.Show(); break;
		case 12: parts.Disable(); pattern.Show(); break;
		case 13: attrscore.Show(); break;
		case 14: impact_scoring.Show(); break;
		case 15: mask_scoring.Show(); break;
		case 16: scoring.Show(); break;
		case 17: WhenStartUpdating(); reverse_impact.Show(); break;
		case 18: WhenStartUpdating(); reverse[0].Show(); break;
		case 19: WhenStartUpdating(); reverse[1].Show(); break;
		case 20: WhenStartUpdating(); reverse[2].Show(); break;
		case 21: rev_pattern.Show(); break;
		case 22: lyrics.Show(); break;
		case 23: composition.Show(); break;
		case 24: production.Show(); break;
		case 25: rhymes.Show(); break;
	}
	page = i;
	DataPage();
}

void Editor::DataPage() {
	switch (page) {
		case 0: info.Data(); break;
		case 1: attr.Data(); break;
		case 2: recru.Data(); break;
		case 3: social.Data(); break;
		case 4: cal.Data(); break;
		case 5: task.Data(); break;
		case 6: importer.Data(); break;
		case 7: structure.Data(); break;
		case 8: analysis.Data(); break;
		case 9: story.Data(); break;
		case 10: impact.Data(); break;
		case 11: patmask.Data(); break;
		case 12: pattern.Data(); break;
		case 13: attrscore.Data(); break;
		case 14: impact_scoring.Data(); break;
		case 15: mask_scoring.Data(); break;
		case 16: scoring.Data(); break;
		case 17: reverse_impact.Data(); break;
		case 18: reverse[0].Data(); break;
		case 19: reverse[1].Data(); break;
		case 20: reverse[2].Data(); break;
		case 21: rev_pattern.Data(); break;
		case 22: lyrics.Data(); break;
		case 23: composition.Data(); break;
		case 24: production.Data(); break;
		case 25: rhymes.Data(); break;
		default: break;
	}
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
		artists.Set(i, 0, a.name);
	}
	artists.SetCount(db.artists.GetCount());
	
	int cursor = max(0, p.GetActiveArtistIndex());
	if (cursor >= 0 && cursor < artists.GetCount())
		artists.SetCursor(cursor);
	
	DataArtist();
}

void Editor::DataArtist() {
	Database& db = Database::Single();
	if (!artists.IsCursor())
		return;
	
	EditorPtrs& p = db.ctx.ed;
	p.artist = &db.artists[artists.GetCursor()];
	Artist& a = *p.artist;
	
	for(int i = 0; i < a.releases.GetCount(); i++) {
		Release& r = a.releases[i];
		releases.Set(i, 0, r.title);
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
	if (!releases.IsCursor() || !p.artist)
		return;
	
	p.release = &p.artist->releases[releases.GetCursor()];
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	for(int i = 0; i < r.songs.GetCount(); i++) {
		Song& s = r.songs[i];
		songs.Set(i, 0, s.artist);
		songs.Set(i, 1, s.title);
		songs.Set(i, 2, s.prj_name);
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
	if (!songs.IsCursor() || !p.artist || !p.release)
		return;
	
	p.song = &p.release->songs[songs.GetCursor()];
	Artist& a = *p.artist;
	Release& r = *p.release;
	Song& s = *p.song;
	
	if (!s.pipe)
		return;
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
	
	DataPart();
}

void Editor::DataPart() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!parts.IsCursor() || !p.artist || !p.release || !p.song || !p.song->pipe) {
		DataPage();
		return;
	}
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
		t_("Artist's name"),
		0
	);
	if (!b) return;
	
	int artist_i = -1;
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		if (a.name == name) {
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
	a.name = name;
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
		t_("Artist's name"),
		0
	);
	if (!b) return;
	
	p.artist->name = name;
	
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
		t_("Release's title"),
		0
	);
	if (!b) return;
	
	int rel_i = -1;
	for(int i = 0; i < a.releases.GetCount(); i++) {
		Release& r = a.releases[i];
		if (r.title == title) {
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
	r.title = title;
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
		t_("Release's title"),
		0
	);
	if (!b) return;
	
	p.release->title = title;
	
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
		t_("Song's title"),
		0
	);
	if (!b) return;
	
	int rel_i = -1;
	for(int i = 0; i < r.songs.GetCount(); i++) {
		Song& s = r.songs[i];
		if (s.title == title) {
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
	s.title = title;
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
		t_("Song's title"),
		0
	);
	if (!b) return;
	
	p.song->title = title.ToString();
	
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
