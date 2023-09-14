#include "SongTool.h"



TxtEditEnglish::TxtEditEnglish() {
	have_group_bad_better = true;
	
}

void TxtEditEnglish::Init() {
	EditorCtrl::Init();
	
	int w = 300;
	top_bar.Add(import_native_direct_trans.HCenterPos(w,-2*w).VSizePos(1,1));
	top_bar.Add(make_poetic.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(check_errors.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(evaluate_audience.HCenterPos(w,w).VSizePos(1,1));
	
	import_native_direct_trans.SetLabel(t_("Import the direct translation of the native text"));
	import_native_direct_trans << THISBACK(ImportNativeDirectTranslation);
	
	make_poetic.SetLabel(t_("Make poetic english from direct translation"));
	make_poetic << THISBACK(MakePoetic);
	
	check_errors.SetLabel(t_("Check for problems in the english translation"));
	check_errors << THISBACK(CheckErrors);
	
	evaluate_audience.SetLabel(t_("Evaluate the english text with an AI audience"));
	evaluate_audience << THISBACK(EvaluateAudience);
	
}

void TxtEditEnglish::DoMainAction(int i) {
	if (i == 0) ImportNativeDirectTranslation();
	if (i == 1) MakePoetic();
	if (i == 2) CheckErrors();
	if (i == 3) EvaluateAudience();
}

void TxtEditEnglish::ImportNativeDirectTranslation() {
	
}

void TxtEditEnglish::MakePoetic() {
	
}

void TxtEditEnglish::CheckErrors() {
	
}



