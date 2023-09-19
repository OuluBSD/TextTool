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
	
	for (String artist : CommonArtists())
		artist_style.Add(artist);
	
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



