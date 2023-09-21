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

void TxtStructEdit::EnableAll() {
	all_disabled = false;
	main.Enable();
	other.Enable();
	third.Enable();
}

void TxtStructEdit::DisableAll() {
	all_disabled = true;
	this->SetFocus();
	main.Disable();
	other.Disable();
	third.Disable();
}

void TxtStructEdit::Init() {
	EditorCtrl::Init();
	
}

void TxtStructEdit::ToolMenu(Bar& bar) {
	bar.Add(t_("Copy the struct of the reference song"), AppImg::Part(), THISBACK(ImportReferenceStruct)).Key(K_F5);
	bar.Add(t_("Check for problems in the song structure"), AppImg::Part(), THISBACK(CheckErrors)).Key(K_F6);
	bar.Add(t_("Convert the structure to a english text"), AppImg::Part(), THISBACK(ConvertToEnglish)).Key(K_F7);
	bar.Add(t_("Convert the english text to a native text"), AppImg::Part(), THISBACK(ConvertToNative)).Key(K_F8);
	bar.Add(t_("Evaluate the english text with an AI audience"), AppImg::Part(), THISBACK(EvaluateAudience)).Key(K_F9);
}

String TxtStructEdit::GetStatusText() {
	return t_("Deconstructed, auto-english, auto-native");
}

void TxtStructEdit::ImportReferenceStruct() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	if (all_disabled)
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
	if (all_disabled)
		return;
	
	p.RealizePipe();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.CheckSongStructureErrors(main_key, error_result_key, THISBACK(PostOnErrorsRecv));
	}
}

void TxtStructEdit::ConvertToEnglish() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty() || main_natural_english_key.IsEmpty())
		return;
	
	if (all_disabled)
		return;
	DisableAll();
	
	p.RealizePipe();
	
	Song& song = *p.song;
	song.data.GetAdd(main_natural_english_key).Clear();
	song.data.GetAdd(main_natural_native_key).Clear();
	
	String content = song.data.Get(main_key, "");
	Vector<String> parts = GetStructureParts(content);
	Vector<String> summed_parts;
	String cur;
	for (const String& part : parts) {
		cur << part << "\n";
		Vector<String> lines = Split(cur, "\n");
		if (lines.GetCount() > 100) {
			summed_parts << cur;
			cur.Clear();
		}
	}
	if (cur.GetCount())
		summed_parts << cur;
	
	part_remaining.Clear();
	for(int i = 0; i < summed_parts.GetCount(); i++)
		part_remaining.Add(i);
	
	for(int i = 0; i < summed_parts.GetCount(); i++) {
		const String& part = summed_parts[i];
		
		TaskMgr& m = *p.song->pipe;
		m.ConvertSongStructureToEnglish(part, THISBACK1(OnStructureResult, i));
	}
}

void TxtStructEdit::OnStructureResult(String res, int part_i) {
	lock.Enter();
	part_results.Add(part_i, res);
	part_remaining.RemoveKey(part_i);
	bool ready = part_remaining.IsEmpty();
	lock.Leave();
	
	if (ready)
		PostCallback(THISBACK(ProcessStructureResult));
}

void TxtStructEdit::ProcessStructureResult() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	SortByKey(part_results, StdLess<int>());
	String output = Join(part_results.GetValues(), "\n");
	
	other.SetData(output);
	
	if (p.song)
		p.song->data.GetAdd(main_natural_english_key) = output;
	
	EnableAll();
}

void TxtStructEdit::ConvertToNative() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_english_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	if (all_disabled)
		return;
	
	p.RealizePipe();
	
	{
		String orig_lng = GetCurrentLanguageString().Left(5);
		String trans_lng = "EN-US";
		
		TaskMgr& m = *p.song->pipe;
		m.TranslateSongData(trans_lng, main_natural_english_key, orig_lng, main_natural_native_key, THISBACK1(PostUpdateExportData, 1));
	}
	
}

void TxtStructEdit::UpdateExportData(int i) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_natural_english_key.IsEmpty() || main_natural_native_key.IsEmpty())
		return;
	
	if (i == 0)
		other.SetData(p.song->data.Get(main_natural_english_key, ""));
	else if (i == 1)
		third.SetData(p.song->data.Get(main_natural_native_key, ""));
}
