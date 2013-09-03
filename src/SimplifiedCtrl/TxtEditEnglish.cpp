#include "SimplifiedCtrl.h"



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
	
}

void TxtEditEnglish::ToolMenu(Bar& bar) {
	bar.Add(t_("Import the direct translation of the native text"), AppImg::Part(), THISBACK(ImportNativeDirectTranslation)).Key(K_F5);
	
	Size sz = bar.GetStdSize();
	sz.cx = 200;
	for (String artist : CommonArtists())
		artist_style.Add(artist);
	artist_style.SetIndex(0);
	bar.Add(artist_style, sz);
	
	bar.Add(t_("Make poetic english with the given style"), AppImg::Part(), THISBACK(MakePoetic)).Key(K_F6);
	bar.Add(t_("Check for problems in the english translation"), AppImg::Part(), THISBACK(CheckErrors)).Key(K_F7);
	bar.Add(t_("Evaluate the english text with an AI audience"), AppImg::Part(), THISBACK(EvaluateAudience)).Key(K_F8);
}

String TxtEditEnglish::GetStatusText() {
	return t_("Poetic english, non-poetic-english, auto-poetic-english");
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



