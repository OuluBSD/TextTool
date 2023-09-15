#include "SongTool.h"



MusicVideoEdit::MusicVideoEdit() {
	have_third_view = true;
	have_group_bad_better = true;
	highligh_struct = true;
	audience_mode = AUDIENCE_MODE_SCREENPLAY;
	english_screenplay_struct = "ENGLISH_SCREENPLAY_STRUCTURED";
	english_screenplay_auto = "ENGLISH_SCREENPLAY_AUTOMATIC";
	english_production_plan_auto = "ENGLISH_PRODUCTIONPLAN_AUTOMATIC";
	english_auto_screen_struct = "AUTOMATIC_ENGLISH_SCREENSTRUCT";
	
	main_natural_english_key = english_screenplay_auto;
	main_key = english_screenplay_struct;
	other_key = english_screenplay_auto;
	third_key = english_production_plan_auto;
	audience_evaluation_key = other_key + "_AUDIENCE";
	error_result_key = main_key + "_ERRORS";
}

void MusicVideoEdit::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_automatic_structure.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(convert_to_screenplay.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(convert_to_plan.HCenterPos(w,w).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,2*w).VSizePos(1,1));
	
	import_automatic_structure.SetLabel(t_("Copy the struct of the reference screenplay"));
	import_automatic_structure << THISBACK(ImportReferenceStruct);
	
	check_errors.SetLabel(t_("Check for problems in the screenplay structure"));
	check_errors << THISBACK(CheckErrors);
	
	convert_to_screenplay.SetLabel(t_("Convert the structure to a screenplay"));
	convert_to_screenplay << THISBACK(ConvertToScreenplay);
	
	convert_to_plan.SetLabel(t_("Convert the structure to a production plan"));
	convert_to_plan << THISBACK(ConvertToPlan);
	
	evaluate_audience.SetLabel(t_("Evaluate the english screenplay with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
}

void MusicVideoEdit::DoMainAction(int i) {
	if (i == 0) ImportReferenceStruct();
	if (i == 1) CheckErrors();
	if (i == 2) ConvertToScreenplay();
	if (i == 3) ConvertToPlan();
	if (i == 4) EvaluateAudience();
}

void MusicVideoEdit::ImportReferenceStruct() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!song.data.Get(main_key, "").IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Do you want to overwrite existing text? This cannot be undone."))))
			return;
	}
	String txt = song.data.Get(english_auto_screen_struct, "");
	song.data.GetAdd(main_key) = txt;
	
	Data();
}

void MusicVideoEdit::CheckErrors() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String txt = p.song->data.Get(main_key, "");
	txt.Replace("\r", "");
	Vector<String> parts = Split(txt, "\n}\n");
	
	error_waiting.Clear();
	error_results.Clear();
	error_results.SetCount(parts.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		error_waiting.Add(i);
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		String& part = parts[i];
		if (i < parts.GetCount()-1)
			part << "\n}";
		TaskMgr& m = *p.song->pipe;
		m.CheckScreenplayStructureErrors(part, THISBACK1(OnErrorReady, i));
	}
}

void MusicVideoEdit::OnErrorReady(String s, int i) {
	if (i < 0 || i >= error_results.GetCount())
		return;
	
	lock.Enter();
	error_results[i] = s;
	error_waiting.RemoveKey(i);
	bool finish = error_waiting.IsEmpty();
	lock.Leave();
	
	if (finish)
		PostCallback(THISBACK(OnErrorAllReady));
}

void MusicVideoEdit::OnErrorAllReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || error_result_key.IsEmpty())
		return;
	
	String full = Join(error_results, "\n\n", false);
	p.song->data.GetAdd(error_result_key) = full;
	
	EditorCtrl::OnErrorsRecv();
}

void MusicVideoEdit::ConvertToScreenplay() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String txt = p.song->data.Get(main_key, "");
	txt.Replace("\r", "");
	Vector<String> parts = Split(txt, "\n}\n");
	
	convert_waiting.Clear();
	convert_results.Clear();
	convert_results.SetCount(parts.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		convert_waiting.Add(i);
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		String& part = parts[i];
		if (i < parts.GetCount()-1)
			part << "\n}";
		TaskMgr& m = *p.song->pipe;
		m.ConvertStructureToScreenplay(part, THISBACK1(OnScreenplayReady, i));
	}
}

void MusicVideoEdit::OnScreenplayReady(String s, int i) {
	if (i < 0 || i >= convert_results.GetCount())
		return;
	
	lock.Enter();
	convert_results[i] = s;
	convert_waiting.RemoveKey(i);
	bool finish = convert_waiting.IsEmpty();
	lock.Leave();
	
	if (finish)
		PostCallback(THISBACK(OnConvertAllReady));
}

void MusicVideoEdit::OnConvertAllReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || english_screenplay_auto.IsEmpty())
		return;
	
	String fully_converted = Join(convert_results, "\n\n", false);
	p.song->data.GetAdd(english_screenplay_auto) = fully_converted;
	other.SetData(fully_converted);
}

void MusicVideoEdit::ConvertToPlan() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || other_key.IsEmpty() || third_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.ConvertScreenplayToPlan(other_key, third_key, THISBACK(PostOnPlanConverted));
	}
}

void MusicVideoEdit::OnPlanConverted() {
	
}

void MusicVideoEdit::EvaluateAudience() {
	EditorCtrl::EvaluateAudience();
}
