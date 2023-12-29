#include "EditorCtrl.h"




Pitching::Pitching() {
	Add(hsplit.VSizePos().HSizePos());
	
	hsplit.Horz() << vsplit0.Vert() << vsplit1.Vert();
	
	vsplit0 << english << native;
	vsplit1 << feedback << pitching;
	
	
	user_natural_english_key = "NATURAL_ENGLISH_OF_AUTHOR";
	native_pitching_key = "NATIVE_SONG_PITCHING";
	english_autotrans_pitching_key = "ENGLISH_AUTO_SONG_PITCHING";
	audience_pitching_key = "AUDIENCE_SONG_PITCHING";
	final_pitching_key = "FINAL_SONG_PITCHING";
	
	
	native.WhenAction << THISBACK2(OnText, &native, native_pitching_key);
	english.WhenAction << THISBACK2(OnText, &english, english_autotrans_pitching_key);
	feedback.WhenAction << THISBACK2(OnText, &feedback, audience_pitching_key);
	pitching.WhenAction << THISBACK2(OnText, &pitching, final_pitching_key);
}

void Pitching::ToolMenu(Bar& bar) {
	bar.Add(t_("Translate native artist's process to english"), AppImg::Part(), THISBACK(TranslateEnglishArtistProcess)).Key(K_F5);
	bar.Add(t_("Make feedback based on all release data"), AppImg::Part(), THISBACK(MakeFeedback)).Key(K_F6);
	bar.Add(t_("Make pitch based on all release data"), AppImg::Part(), THISBACK(MakePitch)).Key(K_F7);
}

void Pitching::Data() {
	SetText(native, native_pitching_key);
	SetText(english, english_autotrans_pitching_key);
	SetText(feedback, audience_pitching_key);
	SetText(pitching, final_pitching_key);
}

void Pitching::SetText(DocEdit& e, String data_key) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || data_key.IsEmpty())
		return;
	
	String txt = p.song->data.Get(data_key, "");
	e.SetData(txt);
}

void Pitching::OnText(DocEdit* e, String data_key) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || data_key.IsEmpty())
		return;
	
	String& txt = p.song->data.GetAdd(data_key);
	txt = e->GetData();
}

void Pitching::TranslateEnglishArtistProcess() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || native_pitching_key.IsEmpty())
		return;
	
	
	
	String txt = p.song->data.Get(native_pitching_key, "");
	
	if (txt.GetCount()) {
		String existing = english.GetData();
		if (existing.GetCount()) {
			if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the english editor?"))))
				return;
		}
		
		TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		m.Translate(orig_lng, txt, trans_lng, THISBACK(PostOnTranslateReady));
	}
}

void Pitching::OnTranslateReady(String res) {
	english.SetData(res);
	OnText(&english, english_autotrans_pitching_key);
}

void Pitching::MakeFeedback() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist ||!p.release)
		return;
	
	Release& rel = *p.release;
	
	
	
	String existing = feedback.GetData();
	if (existing.GetCount()) {
		if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the feedback editor?"))))
			return;
	}
	
	{
		const Song& song = *p.song;
		String lyrics = song.data.Get(user_natural_english_key, "");
		if (lyrics.IsEmpty())
			return;
		
		String title = !song.english_title.IsEmpty() ? song.english_title : song.native_title;
		
		String raw_prompt;
		raw_prompt << "Artist: " << p.artist->english_name << "\n";
		raw_prompt << "Album: " << p.release->english_title << "\n";
		raw_prompt << "Year: " << (int)p.release->date.year << "\n";
		raw_prompt << "Genre: " << p.artist->musical_style << "\n";
		raw_prompt << "Description of singer: " << p.artist->vocalist_visual << "\n";
		raw_prompt << "Count of songs: " << p.release->songs.GetCount() << "\n";
		raw_prompt << "\n\n";
		
		raw_prompt << "Song: " << title << "\n\n";
		raw_prompt << lyrics << "\n\n";
		
		raw_prompt << "The band has a male politically republican fan (age 80 years old), who has only following positive things to say about this band and song:\n";
		
		
		TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
		m.RawCompletion(raw_prompt, THISBACK(PostOnFeedback));
	}
}

void Pitching::OnFeedback(String result) {
	feedback.SetData(result);
	OnText(&feedback, audience_pitching_key);
}

void Pitching::MakePitch() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist ||!p.release)
		return;
	
	Release& rel = *p.release;
	
	
	
	String existing = pitching.GetData();
	if (existing.GetCount()) {
		if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the pitching editor?"))))
			return;
	}
	
	{
		const Song& song = *p.song;
		String lyrics = song.data.Get(user_natural_english_key, "");
		if (lyrics.IsEmpty())
			return;
		
		String title = !song.english_title.IsEmpty() ? song.english_title : song.native_title;
		
		String raw_prompt;
		raw_prompt << "Artist: " << p.artist->english_name << "\n";
		raw_prompt << "Album: " << p.release->english_title << "\n";
		raw_prompt << "Year: " << (int)p.release->date.year << "\n";
		raw_prompt << "Genre: " << p.artist->musical_style << "\n";
		raw_prompt << "Description of singer: " << p.artist->vocalist_visual << "\n";
		raw_prompt << "Count of songs: " << p.release->songs.GetCount() << "\n";
		raw_prompt << "\n\n";
		
		raw_prompt << "Song: " << title << "\n\n";
		raw_prompt << lyrics << "\n\n";
		
		raw_prompt <<	"Instructions for pitching a song is following:\n"
						"Be as specific as possible to help curators understand the creative influences behind your release...\n"
						"\n"
						"Artist will pitch this song \"" << title << "\" to a Spotify playlist with the following text:\n";
		
		TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
		m.RawCompletion(raw_prompt, THISBACK(PostOnPitch));
	}
}

void Pitching::OnPitch(String result) {
	pitching.SetData(result);
	OnText(&pitching, final_pitching_key);
}
