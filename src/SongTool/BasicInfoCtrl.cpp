#include "SongTool.h"

BasicInfoCtrl::BasicInfoCtrl() {
	CtrlLayout(*this);
	
	name <<= THISBACK(OnValueChange);
	year_of_birth <<= THISBACK(OnValueChange);
	year_of_career_begin <<= THISBACK(OnValueChange);
	biography <<= THISBACK(OnValueChange);
	musical_style <<= THISBACK(OnValueChange);
	vibe_of_voice <<= THISBACK(OnValueChange);
	acoustic_instruments <<= THISBACK(OnValueChange);
	electronic_instruments <<= THISBACK(OnValueChange);
	album_title <<= THISBACK(OnValueChange);
	album_date <<= THISBACK(OnValueChange);
	song_artist <<= THISBACK(OnValueChange);
	song_title <<= THISBACK(OnValueChange);
	song_prj_name <<= THISBACK(OnValueChange);
	vocalist_visual <<= THISBACK(OnValueChange);
	
}

void BasicInfoCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	
	if (p.artist) {
		Artist& a = *p.artist;
			
		this->name						.SetData(a.name);
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
		
		album_title.SetData(r.title);
		album_date.SetData(r.date);
	}
	
	if (p.song) {
		Song& s = *p.song;
		
		song_artist.SetData(s.artist);
		song_title.SetData(s.title);
		song_prj_name.SetData(s.prj_name);
	}
}

void BasicInfoCtrl::OnValueChange() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	
	if (p.artist) {
		Artist& o = *p.artist;
		o.name						= this->name.GetData();
		o.year_of_birth				= this->year_of_birth.GetData();
		o.year_of_career_begin		= this->year_of_career_begin.GetData();
		o.biography					= this->biography.GetData();
		o.musical_style				= this->musical_style.GetData();
		o.vibe_of_voice				= this->vibe_of_voice.GetData();
		o.acoustic_instruments		= this->acoustic_instruments.GetData();
		o.electronic_instruments	= this->electronic_instruments.GetData();
		o.vocalist_visual			= this->vocalist_visual.GetData();
		
		int c = editor->artists.GetCursor();
		editor->artists.Set(c, 0, o.name);
	}
	
	if (p.release) {
		Release& r = *p.release;
		
		r.title = album_title.GetData();
		r.date = album_date.GetData();
		
		int c = editor->releases.GetCursor();
		editor->releases.Set(c, 0, r.title);
	}
	
	if (p.song) {
		Song& s = *p.song;
		
		s.artist = song_artist.GetData();
		s.title = song_title.GetData();
		s.prj_name = song_prj_name.GetData();
		
		int c = editor->songs.GetCursor();
		editor->songs.Set(c, 0, s.artist);
		editor->songs.Set(c, 1, s.title);
		editor->songs.Set(c, 2, s.prj_name);
	}
}
