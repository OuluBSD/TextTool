#include "SongTool.h"


Editor::Editor() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << tablist << artists << releases << songs;
	artists.WhenBar << THISBACK(ArtistMenu);
	releases.WhenBar << THISBACK(ReleaseMenu);
	songs.WhenBar << THISBACK(SongMenu);
	
	tablist.NoHeader();
	tablist.AddColumn("");
	tablist.AddColumn("");
	tablist.ColumnWidths("1 3");
	tablist.Add(t_("Release"), t_("Recruitment"));
	tablist.Add(t_("Release"), t_("Social media campaign"));
	tablist.Add(t_("Release"), t_("Calendar"));
	tablist.Add(t_("Release"), t_("Tasks"));
	tablist.Add(t_("Song"), t_("Story"));
	tablist.Add(t_("Song"), t_("Pattern mask"));
	tablist.Add(t_("Song"), t_("Pattern"));
	tablist.Add(t_("Song"), t_("Composition"));
	tablist.Add(t_("Song"), t_("Analysis"));
	tablist.Add(t_("Song"), t_("Production"));
	tablist.Add(t_("Song"), t_("Rhymes"));
	
	artists.AddColumn("");
	artists.NoHeader();
	
	releases.AddColumn("");
	releases.NoHeader();
	
	songs.AddColumn("");
	songs.AddColumn("");
	songs.NoHeader();
	
	base.Add(recru.SizePos());
	base.Add(social.SizePos());
	base.Add(cal.SizePos());
	base.Add(task.SizePos());
	base.Add(story.SizePos());
	base.Add(patmask.SizePos());
	base.Add(pattern.SizePos());
	base.Add(composition.SizePos());
	base.Add(analysis.SizePos());
	base.Add(production.SizePos());
	base.Add(rhymes.SizePos());
	
	SetView(0);
}

void Editor::SetView(int i) {
	recru.Hide();
	social.Hide();
	cal.Hide();
	task.Hide();
	story.Hide();
	patmask.Hide();
	pattern.Hide();
	composition.Hide();
	analysis.Hide();
	production.Hide();
	rhymes.Hide();
	
	page = i;
	
	switch (i) {
		default: i = 0;
		case 0: recru.Show(); break;
		case 1: social.Show(); break;
		case 2: cal.Show(); break;
		case 3: task.Show(); break;
		case 4: story.Show(); break;
		case 5: patmask.Show(); break;
		case 6: pattern.Show(); break;
		case 7: composition.Show(); break;
		case 8: analysis.Show(); break;
		case 9: production.Show(); break;
		case 10: rhymes.Show(); break;
	}
	page = i;
}

void Editor::Data() {
	Database& db = Database::Single();
	
	for(int i = 0; i < db.artists.GetCount(); i++) {
		Artist& a = db.artists[i];
		artists.Set(i, 0, a.name);
	}
	artists.SetCount(db.artists.GetCount());
	
	int cursor = db.GetActiveArtistIndex();
	if (cursor >= 0 && cursor < artists.GetCount())
		artists.SetCursor(cursor);
	
	DataArtist();
}

void Editor::DataArtist() {
	Database& db = Database::Single();
	if (!artists.IsCursor())
		return;
	
	db.active_artist = &db.artists[artists.GetCursor()];
	Artist& a = *db.active_artist;
	
	for(int i = 0; i < a.releases.GetCount(); i++) {
		Release& r = a.releases[i];
		releases.Set(i, 0, r.title);
	}
	releases.SetCount(a.releases.GetCount());
	
	int cursor = db.GetActiveReleaseIndex();
	if (cursor >= 0 && cursor < releases.GetCount())
		releases.SetCursor(cursor);
	
	DataRelease();
}

void Editor::DataRelease() {
	Database& db = Database::Single();
	if (!releases.IsCursor() || !db.active_artist)
		return;
	
	db.active_release = &db.active_artist->releases[releases.GetCursor()];
	Artist& a = *db.active_artist;
	Release& r = *db.active_release;
	
	for(int i = 0; i < r.songs.GetCount(); i++) {
		Song& s = r.songs[i];
		songs.Set(i, 0, s.artist);
		songs.Set(i, 1, s.title);
	}
	songs.SetCount(r.songs.GetCount());
	
	int cursor = db.GetActiveSongIndex();
	if (cursor >= 0 && cursor < songs.GetCount())
		songs.SetCursor(cursor);
	
	DataSong();
}

void Editor::DataSong() {
	Database& db = Database::Single();
	if (!releases.IsCursor() || !db.active_artist || !db.active_release)
		return;
	
	db.active_song = &db.active_release->songs[releases.GetCursor()];
	Artist& a = *db.active_artist;
	Release& r = *db.active_release;
	Song& s = *db.active_song;
	
	int cursor = db.GetActiveSongIndex();
	if (cursor >= 0 && cursor < songs.GetCount() && !songs.IsCursor())
		songs.SetCursor(cursor);
	
	DataPage();
}

void Editor::DataPage() {
	switch (page) {
		case 0: recru.Data(); break;
		case 1: social.Data(); break;
		case 2: cal.Data(); break;
		case 3: task.Data(); break;
		case 4: story.Data(); break;
		case 5: patmask.Data(); break;
		case 6: pattern.Data(); break;
		case 7: composition.Data(); break;
		case 8: analysis.Data(); break;
		case 9: production.Data(); break;
		case 10: rhymes.Data(); break;
		default: break;
	}
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

void Editor::AddArtist() {
	Database& db = Database::Single();
	
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
	a.file_title = ToLower(name);
	a.file_title.Replace(" ", "_");
	a.name = name;
	db.active_artist = &a;
	
	Data();
}

void Editor::RenameArtist() {
	Database& db = Database::Single();
	if (!db.active_artist)
		return;
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Rename Artist"),
		t_("Artist's name"),
		0
	);
	if (!b) return;
	
	db.active_artist->name = name;
	
	Data();
}

void Editor::RemoveArtist() {
	Database& db = Database::Single();
	if (!db.active_artist)
		return;
	int idx = db.GetActiveArtistIndex();
	if (idx < 0) return;
	db.artists.Remove(idx);
	Data();
}

void Editor::AddRelease() {
	
	
}

void Editor::RenameRelease() {
	
	
}

void Editor::RemoveRelease() {
	
	
}

void Editor::AddSong() {
	
	
}

void Editor::RenameSong() {
	
	
}

void Editor::RemoveSong() {
	
	
}
