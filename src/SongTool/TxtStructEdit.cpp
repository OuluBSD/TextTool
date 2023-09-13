#include "SongTool.h"



TxtStructEdit::TxtStructEdit() {
	have_third_view = true;
}

void TxtStructEdit::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_reference_struct.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(convert_to_native.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,w).VSizePos(1,1));
	
	import_reference_struct.SetLabel(t_("Copy the struct of the reference song"));
	import_reference_struct << THISBACK(ImportReferenceStruct);
	
	check_errors.SetLabel(t_("Check for problems in the song structure"));
	check_errors << THISBACK(CheckErrors);
	
	convert_to_native.SetLabel(t_("Convert the structure to native text"));
	convert_to_native << THISBACK(ConvertToNative);
	
	evaluate_audience.SetLabel(t_("Evaluate english text with audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
}

void TxtStructEdit::ImportReferenceStruct() {
	
}

void TxtStructEdit::CheckErrors() {
	
}

void TxtStructEdit::ConvertToNative() {
	
}

void TxtStructEdit::EvaluateAudience() {
	
}

void TxtStructEdit::DoMainAction(int i) {
	if (i == 0) ImportReferenceStruct();
	if (i == 1) CheckErrors();
	if (i == 2) ConvertToNative();
	if (i == 3) EvaluateAudience();
}