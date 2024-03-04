#include "SocialCtrl.h"
#include <SongTool/SongTool.h>

SongInfoCtrl::SongInfoCtrl() {
	CtrlLayout(*this);
	
	song_artist <<= THISBACK(OnValueChange);
	song_prj_name <<= THISBACK(OnValueChange);
	reference <<= THISBACK(OnValueChange);
	origins <<= THISBACK(OnValueChange);
	
	typecasts.AddColumn(t_("Typecast"));
	typecasts.AddColumn(t_("Count"));
	typecasts.AddIndex("IDX");
	typecasts.ColumnWidths("3 1");
	archetypes.AddColumn(t_("Archetype"));
	archetypes.AddColumn(t_("Count"));
	archetypes.AddIndex("IDX");
	archetypes.ColumnWidths("3 1");
	lyrics.AddColumn(t_("Lyrics"));
	lyrics.AddIndex("IDX");
	
	typecasts.WhenCursor << THISBACK(DataTypecast);
	archetypes.WhenCursor << THISBACK(DataArchetype);
	lyrics.WhenCursor << THISBACK(DataLyrics);
	
	set.WhenAction << THISBACK(SetLyrics);
	
}

void SongInfoCtrl::Clear() {
	this->song_artist				.Clear();
	this->song_prj_name				.Clear();
	
}

void SongInfoCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	Clear();
	
	if (p.song) {
		Song& s = *p.song;
		
		song_artist.SetData(s.artist);
		song_prj_name.SetData(s.prj_name);
		reference.SetData(s.reference);
		origins.SetData(s.origins);
	}
	
	
	if (!p.artist) {
		typecasts.Clear();
		archetypes.Clear();
		lyrics.Clear();
		return;
	}
	
	Artist& a = *p.artist;
	if (p.song)
		a.FindSong(focus_tc, focus_arch, focus_lyr, p.song->lyrics_file_title);
	
	if (focus_tc < 0) {
		focus_tc = p.GetActiveTypecastIndex();
		focus_arch = p.GetActiveArchetypeIndex();
		focus_lyr = p.GetActiveLyricsIndex();
	}
	
	const auto& tcs = GetTypecasts();
	for(int i = 0; i < a.typecasts.GetCount(); i++) {
		const auto& t = tcs[i];
		const auto& tc = a.typecasts[i];
		typecasts.Set(i, "IDX", i);
		typecasts.Set(i, 0, t);
		typecasts.Set(i, 1, a.typecasts[i].GetLyricsCount());
	}
	INHIBIT_CURSOR(typecasts);
	typecasts.SetSortColumn(1, true);
	
	int cursor = max(0, focus_tc);
	if (cursor >= 0 && cursor < typecasts.GetCount())
		SetIndexCursor(typecasts, cursor);

	DataTypecast();
}

void SongInfoCtrl::DataTypecast() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.artist || !typecasts.IsCursor()) {
		archetypes.Clear();
		lyrics.Clear();
		return;
	}
	
	Artist& a = *p.artist;
	Typecast& t = a.typecasts[typecasts.Get("IDX")];
	const auto& cons = GetContrasts();
	for(int i = 0; i < t.archetypes.GetCount(); i++) {
		const auto& con = cons[i];
		const auto& at = t.archetypes[i];
		archetypes.Set(i, "IDX", i);
		archetypes.Set(i, 0, con.key);
		archetypes.Set(i, 1, at.lyrics.GetCount());
	}
	INHIBIT_CURSOR(archetypes);
	archetypes.SetSortColumn(1, true);
	
	int cursor = max(0, focus_arch);
	if (cursor >= 0 && cursor < archetypes.GetCount())
		SetIndexCursor(archetypes, cursor);

	DataArchetype();
}

void SongInfoCtrl::DataArchetype() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.artist || !typecasts.IsCursor() || !archetypes.IsCursor()) {
		lyrics.Clear();
		return;
	}
	
	Artist& a = *p.artist;
	Typecast& t = a.typecasts[typecasts.Get("IDX")];
	Archetype& at = t.archetypes[archetypes.Get("IDX")];
	
	for(int i = 0; i < at.lyrics.GetCount(); i++) {
		const Lyrics& lyr = at.lyrics[i];
		lyrics.Set(i, "IDX", i);
		lyrics.Set(i, 0, lyr.GetAnyTitle());
	}
	INHIBIT_CURSOR(lyrics);
	
	int cursor = max(0, focus_lyr);
	if (cursor >= 0 && cursor < lyrics.GetCount())
		SetIndexCursor(lyrics, cursor);

	DataLyrics();
}

void SongInfoCtrl::DataLyrics() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.artist || !typecasts.IsCursor() || !archetypes.IsCursor() || !lyrics.IsCursor()) {
		lyrics_text.Clear();
		return;
	}
	
	Artist& a = *p.artist;
	Typecast& t = a.typecasts[typecasts.Get("IDX")];
	Archetype& at = t.archetypes[archetypes.Get("IDX")];
	int lyr_i = lyrics.Get("IDX");
	if (lyr_i >= at.lyrics.GetCount()) {
		lyrics_text.SetData("<invalid IDX>");
		return;
	}
	Lyrics& lyr = at.lyrics[lyr_i];
	
	if (lyr.text.GetCount())
		lyrics_text.SetData(lyr.text);
	else
		lyrics_text.SetData("<no lyrics>");
}

void SongInfoCtrl::OnValueChange() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	
	if (p.song && editor->songs.IsCursor()) {
		Song& s = *p.song;
		
		s.artist = song_artist.GetData();
		s.prj_name = song_prj_name.GetData();
		s.reference = reference.GetData();
		s.origins = origins.GetData();
		
		int c = editor->songs.GetCursor();
		editor->songs.Set(c, 0, s.artist);
		editor->songs.Set(c, 1, s.prj_name);
	}
}

void SongInfoCtrl::SetLyrics() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	Song& s = *p.song;
	
	if (!p.artist || !p.song || !typecasts.IsCursor() || !archetypes.IsCursor() || !lyrics.IsCursor()) {
		return;
	}
	
	int tc_i = typecasts.Get("IDX");
	int at_i = archetypes.Get("IDX");
	int l_i = lyrics.Get("IDX");
	
	Lyrics& l = p.artist->typecasts[tc_i].archetypes[at_i].lyrics[l_i];
	
	s.lyrics_file_title = l.file_title;
}
