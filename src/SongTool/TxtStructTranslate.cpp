#include "SongTool.h"



TranslatorToolCtrl::TranslatorToolCtrl() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(translate.BottomPos(1,28).HCenterPos(120));
	
	hsplit.Horz() << orig << trans;
	
	translate.SetLabel(t_("Translate"));
	translate.WhenAction << THISBACK(Translate);
	
	orig.SetFont(Monospace(15));
	trans.SetFont(Monospace(15));
	
	orig.WhenAction << THISBACK(OnOriginalChange);
	trans.WhenAction << THISBACK(OnTranslatedChange);
	
}

void TranslatorToolCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	orig.SetData(GetOriginalText());
	trans.SetData(GetTranslatedText());
	
}

void TranslatorToolCtrl::DoMainAction(int i) {
	if (i == 0) Translate();
}

void TranslatorToolCtrl::Translate() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || key.IsEmpty() || trans_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String orig_lng = GetCurrentLanguageString().Left(5);
	String trans_lng = "EN-US";
	
	TaskMgr& m = *p.song->pipe;
	m.TranslateSongData(orig_lng, key, trans_lng, trans_key, THISBACK(OnTranslatedRecv));
}

String TranslatorToolCtrl::GetOriginalText() const {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (key.GetCount()) {
		if (artist && p.artist) {
			return p.artist->data.GetAdd(key);
		}
		else if (release && p.release) {
			return p.release->data.GetAdd(key);
		}
		else if (song && p.song) {
			return p.song->data.GetAdd(key);
		}
	}
	return String();
}

String TranslatorToolCtrl::GetTranslatedText() const {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (trans_key.GetCount()) {
		if (artist && p.artist) {
			return p.artist->data.GetAdd(trans_key);
		}
		else if (release && p.release) {
			return p.release->data.GetAdd(trans_key);
		}
		else if (song && p.song) {
			return p.song->data.GetAdd(trans_key);
		}
	}
	return String();
}

void TranslatorToolCtrl::OnOriginalChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	String txt = orig.GetData();
	if (key.GetCount()) {
		if (artist && p.artist) {
			p.artist->data.GetAdd(key) = txt;
		}
		else if (release && p.release) {
			p.release->data.GetAdd(key) = txt;
		}
		else if (song && p.song) {
			p.song->data.GetAdd(key) = txt;
		}
	}
}

void TranslatorToolCtrl::OnTranslatedChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	String txt = trans.GetData();
	if (trans_key.GetCount()) {
		if (artist && p.artist) {
			p.artist->data.GetAdd(trans_key) = txt;
		}
		else if (release && p.release) {
			p.release->data.GetAdd(trans_key) = txt;
		}
		else if (song && p.song) {
			p.song->data.GetAdd(trans_key) = txt;
		}
	}
}

void TranslatorToolCtrl::OnTranslatedRecv() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	trans.SetData(GetTranslatedText());
}






TxtStructTranslate::TxtStructTranslate() {
	key = "REFERENCE_SONG__NATIVE_TEXT";
	trans_key = "REFERENCE_SONG__ENGLISH_TEXT";
	song = true;
}
