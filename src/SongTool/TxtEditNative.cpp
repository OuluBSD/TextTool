#include "SongTool.h"



TxtEditNative::TxtEditNative() {
	have_third_view = true;
	have_group_bad_better = true;
	have_errors_in_other = true;
	main_natural_native_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL_NATIVE";
	user_natural_native_key = "NATURAL_NATIVE_OF_AUTHOR";
	user_natural_english_trans_key = "ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	auto_natural_english_trans_key = "AUTOMATIC_ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	
	error_result_key = "ERRORS_IN_ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR";
	main_natural_english_key = user_natural_english_trans_key;
	audience_evaluation_key = "ENGLISH_FROM_NATURAL_NATIVE_OF_AUTHOR_AUDIENCE";
	main_key = user_natural_native_key;
	other_key = user_natural_english_trans_key;
	third_key = auto_natural_english_trans_key;
}

void TxtEditNative::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_autotrans_native.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(translate_to_english.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,w).VSizePos(1,1));
	
	import_autotrans_native.SetLabel(t_("Import the native text converted from structure"));
	import_autotrans_native << THISBACK(ImportAutomaticNativeTranslation);
	
	translate_to_english.SetLabel(t_("Translate native text of the user to english"));
	translate_to_english << THISBACK(TranslateToEnglish);
	
	check_errors.SetLabel(t_("Check for problems in the english translation"));
	check_errors << THISBACK(CheckErrors);
	
	evaluate_audience.SetLabel(t_("Evaluate the english text with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
}

void TxtEditNative::DoMainAction(int i) {
	if (i == 0) ImportAutomaticNativeTranslation();
	if (i == 1) TranslateToEnglish();
	if (i == 2) CheckErrors();
	if (i == 3) EvaluateAudience();
}

void TxtEditNative::ImportAutomaticNativeTranslation() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!song.data.Get(main_key, "").IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Do you want to overwrite existing text? This cannot be undone."))))
			return;
	}
	String txt = song.data.Get(main_natural_native_key, "");
	song.data.GetAdd(main_key) = txt;
	
	Data();
}

void TxtEditNative::TranslateToEnglish() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || auto_natural_english_trans_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		
		TaskMgr& m = *p.song->pipe;
		m.TranslateSongData(orig_lng, main_key, trans_lng, auto_natural_english_trans_key, THISBACK(OnTranslatedRecv));
	}
}

void TxtEditNative::CheckErrors() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || user_natural_english_trans_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.CheckSongNaturalErrors(user_natural_english_trans_key, error_result_key, THISBACK(PostOnErrorsRecv));
	}
}

void TxtEditNative::OnTranslatedRecv() {
	PostCallback(THISBACK(UpdateTranslated));
}

void TxtEditNative::UpdateTranslated() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || auto_natural_english_trans_key.IsEmpty())
		return;
	
	third.SetData(p.song->data.Get(auto_natural_english_trans_key, ""));
}
