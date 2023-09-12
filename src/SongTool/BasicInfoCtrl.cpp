#include "SongTool.h"

BasicInfoCtrl::BasicInfoCtrl() {
	CtrlLayout(*this);
	
	native_name <<= THISBACK(OnValueChange);
	english_name <<= THISBACK(OnValueChange);
	year_of_birth <<= THISBACK(OnValueChange);
	year_of_career_begin <<= THISBACK(OnValueChange);
	biography <<= THISBACK(OnValueChange);
	musical_style <<= THISBACK(OnValueChange);
	vibe_of_voice <<= THISBACK(OnValueChange);
	acoustic_instruments <<= THISBACK(OnValueChange);
	electronic_instruments <<= THISBACK(OnValueChange);
	native_album_title <<= THISBACK(OnValueChange);
	english_album_title <<= THISBACK(OnValueChange);
	album_date <<= THISBACK(OnValueChange);
	song_artist <<= THISBACK(OnValueChange);
	native_song_title <<= THISBACK(OnValueChange);
	english_song_title <<= THISBACK(OnValueChange);
	song_prj_name <<= THISBACK(OnValueChange);
	vocalist_visual <<= THISBACK(OnValueChange);
	
}

void BasicInfoCtrl::Clear() {
	this->native_name				.Clear();
	this->english_name				.Clear();
	this->year_of_birth				.Clear();
	this->year_of_career_begin		.Clear();
	this->biography					.Clear();
	this->musical_style				.Clear();
	this->vibe_of_voice				.Clear();
	this->acoustic_instruments		.Clear();
	this->electronic_instruments	.Clear();
	this->vocalist_visual			.Clear();
	this->native_album_title		.Clear();
	this->english_album_title		.Clear();
	this->album_date				.Clear();
	this->song_artist				.Clear();
	this->native_song_title			.Clear();
	this->english_song_title		.Clear();
	this->song_prj_name				.Clear();
}

void BasicInfoCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	Clear();
	
	if (p.artist) {
		Artist& a = *p.artist;
			
		this->native_name				.SetData(a.native_name);
		this->english_name				.SetData(a.english_name);
		this->year_of_birth				.SetData(a.year_of_birth);
		this->year_of_career_begin		.SetData(a.year_of_career_begin);
		this->biography					.SetData(a.biography);
		this->musical_style				.SetData(a.musical_style);
		this->vibe_of_voice				.SetData(a.vibe_of_voice);
		this->acoustic_instruments		.SetData(a.acoustic_instruments);
		this->electronic_instruments	.SetData(a.electronic_instruments);
		this->vocalist_visual			.SetData(a.vocalist_visual);
	}
	
	if (p.release) {
		Release& r = *p.release;
		
		native_album_title.SetData(r.native_title);
		english_album_title.SetData(r.english_title);
		album_date.SetData(r.date);
	}
	
	if (p.song) {
		Song& s = *p.song;
		
		song_artist.SetData(s.artist);
		native_song_title.SetData(s.native_title);
		english_song_title.SetData(s.english_title);
		song_prj_name.SetData(s.prj_name);
	}
}

void BasicInfoCtrl::OnValueChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (p.artist && editor->artists.IsCursor()) {
		Artist& o = *p.artist;
		o.native_name				= this->native_name.GetData();
		o.english_name				= this->english_name.GetData();
		o.year_of_birth				= this->year_of_birth.GetData();
		o.year_of_career_begin		= this->year_of_career_begin.GetData();
		o.biography					= this->biography.GetData();
		o.musical_style				= this->musical_style.GetData();
		o.vibe_of_voice				= this->vibe_of_voice.GetData();
		o.acoustic_instruments		= this->acoustic_instruments.GetData();
		o.electronic_instruments	= this->electronic_instruments.GetData();
		o.vocalist_visual			= this->vocalist_visual.GetData();
		
		int c = editor->artists.GetCursor();
		editor->artists.Set(c, 0, o.native_name);
	}
	
	if (p.release && editor->releases.IsCursor()) {
		Release& r = *p.release;
		
		r.native_title = native_album_title.GetData();
		r.english_title = english_album_title.GetData();
		r.date = album_date.GetData();
		
		int c = editor->releases.GetCursor();
		editor->releases.Set(c, 0, r.native_title);
	}
	
	if (p.song && editor->songs.IsCursor()) {
		Song& s = *p.song;
		
		s.artist = song_artist.GetData();
		s.native_title = native_song_title.GetData();
		s.english_title = english_song_title.GetData();
		s.prj_name = song_prj_name.GetData();
		
		int c = editor->songs.GetCursor();
		editor->songs.Set(c, 0, s.artist);
		editor->songs.Set(c, 1, s.native_title);
		editor->songs.Set(c, 2, s.prj_name);
	}
}
