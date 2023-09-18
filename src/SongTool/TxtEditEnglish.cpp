#include "SongTool.h"



TxtEditEnglish::TxtEditEnglish() {
	have_third_view = true;
	have_group_bad_better = true;
	user_natural_english_key = "NATURAL_ENGLISH_OF_AUTHOR";
	user_natural_english_trans_key = "ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	auto_poetic_english_key = "POETIC_ENGLISH_FROM_NATURAL_ENGLISH_OF_AUTHOR";
	
	error_result_key = "ERRORS_IN_NATURAL_ENGLISH_OF_AUTHOR";
	main_natural_english_key = user_natural_english_key;
	audience_evaluation_key = "NATURAL_ENGLISH_OF_AUTHOR_AUDIENCE";
	main_key = user_natural_english_key;
	other_key = user_natural_english_trans_key;
	third_key = auto_poetic_english_key;
}

void TxtEditEnglish::Init() {
	EditorCtrl::Init();
	
	int w = 200;
	top_bar.Add(import_native_direct_trans.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(artist_style.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(make_poetic.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,w).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,2*w).VSizePos(1,1));
	top_bar.Add(lbl.HCenterPos(w,3*w).VSizePos(1,1));
	
	artist_style.Add("Red Hot Chili Peppers");
	artist_style.Add("Beastie Boys");
	artist_style.Add("The Jimi Hendrix Experience");
	artist_style.Add("Grandmaster Flash and the Furious Five");
	artist_style.Add("Bob Marley");
	artist_style.Add("Bob Dylan");
	artist_style.Add("The Beatles");
	artist_style.Add("The Doors");
	artist_style.Add("Nirvana");
	artist_style.Add("Tupac Shakur");
	artist_style.Add("Marvin Gaye");
	artist_style.Add("N.W.A");
	artist_style.Add("Run-DMC");
	artist_style.Add("Led Zeppelin");
	artist_style.Add("Jefferson Airplane");
	artist_style.Add("Madonna");
	artist_style.Add("Buffalo Springfield");
	artist_style.Add("Elton John");
	artist_style.Add("R.E.M.");
	artist_style.Add("AC/DC");
	artist_style.Add("ZZ Top");
	artist_style.Add("Aerosmith");
	artist_style.Add("David Bowie");
	artist_style.Add("The Yardbirds");
	artist_style.Add("Johnny Cash ");
	artist_style.Add("The Grateful Dead");
	artist_style.Add("The Who");
	artist_style.Add("The Byrds");
	artist_style.Add("Simon & Garfunkel");
	artist_style.Add("Roy Orbison");
	artist_style.Add("The Beach Boys");
	artist_style.Add("Ray Charles");
	artist_style.Add("James Brown");
	artist_style.Add("Limp Bizkit");
	artist_style.Add("Public Enemy");
	artist_style.Add("Eminem");
	artist_style.Add("Elvis Presley");
	artist_style.Add("ABBA");
	artist_style.Add("Green Day");
	artist_style.Add("Jay-Z");
	artist_style.Add("Rage Against the Machine");
	artist_style.Add("Metallica");
	artist_style.Add("Pink Floyd");
	artist_style.Add("Stevie Wonder");
	artist_style.Add("Bon Jovi");
	artist_style.Add("Guns N' Roses");
	artist_style.Add("The Supremes");
	artist_style.Add("Drake");
	artist_style.SetIndex(0);
	
	import_native_direct_trans.SetLabel(t_("Import the direct translation of the native text"));
	import_native_direct_trans << THISBACK(ImportNativeDirectTranslation);
	
	make_poetic.SetLabel(t_("Make poetic english with the given style"));
	make_poetic << THISBACK(MakePoetic);
	
	check_errors.SetLabel(t_("Check for problems in the english translation"));
	check_errors << THISBACK(CheckErrors);
	
	evaluate_audience.SetLabel(t_("Evaluate the english text with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
	lbl.SetLabel(t_("Poetic english, non-poetic-english, auto-poetic-english"));
	
}

void TxtEditEnglish::DoMainAction(int i) {
	if (i == 0) ImportNativeDirectTranslation();
	if (i == 1) MakePoetic();
	if (i == 2) CheckErrors();
	if (i == 3) EvaluateAudience();
}

void TxtEditEnglish::ImportNativeDirectTranslation() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!song.data.Get(main_key, "").IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Do you want to overwrite existing text? This cannot be undone."))))
			return;
	}
	String txt = song.data.Get(user_natural_english_trans_key, "");
	song.data.GetAdd(main_key) = txt;
	
	Data();
}

void TxtEditEnglish::MakePoetic() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || auto_poetic_english_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		String style = artist_style.GetValue();
		
		TaskMgr& m = *p.song->pipe;
		m.MakePoetic(style, main_key, auto_poetic_english_key, THISBACK(PostOnPoeticRecv));
	}
}

void TxtEditEnglish::CheckErrors() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || user_natural_english_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.CheckSongNaturalErrors(user_natural_english_key, error_result_key, THISBACK(PostOnErrorsRecv));
	}
}

void TxtEditEnglish::OnPoeticRecv() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || auto_poetic_english_key.IsEmpty())
		return;
	
	third.SetData(p.song->data.Get(auto_poetic_english_key, ""));
}



