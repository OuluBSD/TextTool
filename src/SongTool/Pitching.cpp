#include "SongTool.h"




Pitching::Pitching() {
	int w = 300;
	top_bar.Add(translate.HCenterPos(w,-w).VSizePos(1,1));
	top_bar.Add(make_feedback.HCenterPos(w,0).VSizePos(1,1));
	top_bar.Add(make_pitch.HCenterPos(w,+w).VSizePos(1,1));
	
	translate.SetLabel(t_("Translate native artist's process to english"));
	translate << THISBACK(ImportReferenceStruct);
	
	make_feedback.SetLabel(t_("Make feedback based on all release data"));
	make_feedback << THISBACK(CheckErrors);
	
	make_pitch.SetLabel(t_("Make pitch based on all release data"));
	make_pitch << THISBACK(ConvertToNative);
	
	Add(top_bar.TopPos(0,30).HSizePos());
	Add(hsplit.VSizePos(30,0).HSizePos());
	
	hsplit.Horz() << vsplit0.Vert() << vsplit1.Vert();
	
	vsplit0 << english << native;
	vsplit1 << feedback << pitching;
	
	
	
}

void Pitching::Data() {
	
}

void Pitching::DoMainAction(int i) {
	if (i == 0) ImportReferenceStruct();
	if (i == 1) CheckErrors();
	if (i == 2) ConvertToNative();
}

void Pitching::ImportReferenceStruct() {
/*	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || user_natural_english_key.IsEmpty())
		return;
	
	String txt = p.song->data.Get(user_natural_english_key, "");
	
	if (txt.GetCount()) {
		if (list.GetCount()) {
			if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the list?"))))
				return;
		}
		
	}*/
}

void Pitching::CheckErrors() {
	
}

void Pitching::ConvertToNative() {
	
}
