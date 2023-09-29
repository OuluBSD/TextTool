#include "SimplifiedCtrl.h"



MusicVideoImport::MusicVideoImport() {
	have_third_view = true;
	have_group_bad_better = true;
	audience_mode = AUDIENCE_MODE_SCREENPLAY;
	native_ref_screenplay = "NATIVE_REFERENCE_SCREENPLAY";
	english_autotrans_ref_screenplay = "NATIVE_TO_ENGLISH_REFERENCE_SCREENPLAY";
	english_auto_screen_struct = "AUTOMATIC_ENGLISH_SCREENSTRUCT";
	
	main_natural_english_key = english_autotrans_ref_screenplay;
	audience_evaluation_key = english_autotrans_ref_screenplay + "_AUDIENCE";
	main_key = native_ref_screenplay;
	other_key = english_autotrans_ref_screenplay;
	third_key = english_auto_screen_struct;
}

void MusicVideoImport::Init() {
	EditorCtrl::Init();
	
}

void MusicVideoImport::ToolMenu(Bar& bar) {
	bar.Add(t_("Translate to english"), AppImg::Part(), THISBACK(TranslateToEnglish)).Key(K_F5);
	bar.Add(t_("Convert to structure"), AppImg::Part(), THISBACK(ConvertToStructure)).Key(K_F6);
	bar.Add(t_("Evaluate the english text with an AI audience"), AppImg::Part(), THISBACK(EvaluateAudience)).Key(K_F7);
}

void MusicVideoImport::TranslateToEnglish() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || native_ref_screenplay.IsEmpty() || english_autotrans_ref_screenplay.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String txt = p.song->data.Get(native_ref_screenplay, "");
	txt.Replace("\r", "");
	txt.Replace("\n \n", "\n\n");
	Vector<String> parts = Split(txt, "\n\n");
	
	translate_waiting.Clear();
	translate_results.Clear();
	translate_results.SetCount(parts.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		translate_waiting.Add(i);
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		const String& part = parts[i];
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		
		TaskMgr& m = *p.song->pipe;
		m.Translate(orig_lng, part, trans_lng, THISBACK1(OnTranslateReady, i));
	}
}

void MusicVideoImport::OnTranslateReady(String s, int i) {
	if (i < 0 || i >= translate_results.GetCount())
		return;
	
	lock.Enter();
	translate_results[i] = s;
	translate_waiting.RemoveKey(i);
	bool finish = translate_waiting.IsEmpty();
	lock.Leave();
	
	if (finish)
		PostCallback(THISBACK(OnTranslateAllReady));
}

void MusicVideoImport::OnTranslateAllReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || english_autotrans_ref_screenplay.IsEmpty())
		return;
	
	String fully_translated = Join(translate_results, "\n\n", false);
	p.song->data.GetAdd(english_autotrans_ref_screenplay) = fully_translated;
	other.SetData(fully_translated);
}

void MusicVideoImport::ConvertToStructure() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || english_autotrans_ref_screenplay.IsEmpty())
		return;
	
	p.RealizePipe();
	
	String txt = p.song->data.Get(english_autotrans_ref_screenplay, "");
	txt.Replace("\r", "");
	txt.Replace("\n \n", "\n\n");
	Vector<String> parts = Split(txt, "\n\n");
	
	convert_waiting.Clear();
	convert_results.Clear();
	convert_results.SetCount(parts.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		convert_waiting.Add(i);
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		const String& part = parts[i];
		TaskMgr& m = *p.song->pipe;
		m.ConvertScreenplayToStructure(part, THISBACK1(OnStructureReady, i));
	}
}

void MusicVideoImport::OnStructureReady(String s, int i) {
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

void MusicVideoImport::OnConvertAllReady() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || english_auto_screen_struct.IsEmpty())
		return;
	
	String fully_converted = Join(convert_results, "\n\n", false);
	p.song->data.GetAdd(english_auto_screen_struct) = fully_converted;
	third.SetData(fully_converted);
}

void MusicVideoImport::EvaluateAudience() {
	EditorCtrl::EvaluateAudience();
}
