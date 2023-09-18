#include "SongTool.h"



TxtStructEdit::TxtStructEdit() {
	highligh_struct = true;
	have_third_view = true;
	have_group_bad_better = true;
	unpacked_struct_key = "ENGLISH_UNPACKED_STRUCTURE";
	main_natural_native_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL_NATIVE";
	reference_unpacked_english = "REFERENCE_SONG__ENGLISH_UNPACKED_STRUCTURE";
	
	error_result_key = "ENGLISH_UNPACKED_STRUCTURE_ERRORS";
	main_natural_english_key = "ENGLISH_UNPACKED_STRUCTURE_TO_NATURAL";
	audience_evaluation_key = "ENGLISH_UNPACKED_STRUCTURE_AUDIENCE";
	main_key = unpacked_struct_key;
	other_key = main_natural_english_key;
	third_key = main_natural_native_key;
}

void TxtStructEdit::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_reference_struct.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(convert_to_native.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,w).VSizePos(1,1));
	top_bar.Add(lbl.HCenterPos(w,2*w).VSizePos(1,1));
	
	import_reference_struct.SetLabel(t_("Copy the struct of the reference song"));
	import_reference_struct << THISBACK(ImportReferenceStruct);
	
	check_errors.SetLabel(t_("Check for problems in the song structure"));
	check_errors << THISBACK(CheckErrors);
	
	convert_to_native.SetLabel(t_("Convert the structure to a native text"));
	convert_to_native << THISBACK(ConvertToNative);
	
	evaluate_audience.SetLabel(t_("Evaluate the english text with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
	lbl.SetLabel(t_("Deconstructed, auto-english, auto-native"));
	
}

void TxtStructEdit::ImportReferenceStruct() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!song.data.Get(main_key, "").IsEmpty()) {
		if (!PromptYesNo(DeQtf(t_("Do you want to overwrite existing text? This cannot be undone."))))
			return;
	}
	String txt = song.data.Get(reference_unpacked_english, "");
	song.data.GetAdd(main_key) = txt;
	
	Data();
}

void TxtStructEdit::CheckErrors() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.CheckSongStructureErrors(main_key, error_result_key, THISBACK(PostOnErrorsRecv));
	}
}

void TxtStructEdit::ConvertToNative() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || main_natural_english_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	p.song->data.GetAdd(main_natural_english_key).Clear();
	p.song->data.GetAdd(main_natural_native_key).Clear();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.ConvertSongStructureToEnglish(main_key, main_natural_english_key, THISBACK(OnNaturalExportReady));
	}
}

void TxtStructEdit::OnNaturalExportReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_english_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		
		TaskMgr& m = *p.song->pipe;
		m.TranslateSongData(trans_lng, main_natural_english_key, orig_lng, main_natural_native_key, THISBACK(OnNaturalNativeExportReady));
	}
	
	PostCallback(THISBACK(UpdateExportData));
}

void TxtStructEdit::OnNaturalNativeExportReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_native_key.IsEmpty())
		return;
	
	PostCallback(THISBACK(UpdateExportData));
}

void TxtStructEdit::UpdateExportData() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_english_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	
	other.SetData(p.song->data.Get(main_natural_english_key, ""));
	third.SetData(p.song->data.Get(main_natural_native_key, ""));
}

void TxtStructEdit::DoMainAction(int i) {
	if (i == 0) ImportReferenceStruct();
	if (i == 1) CheckErrors();
	if (i == 2) ConvertToNative();
	if (i == 3) EvaluateAudience();
}
