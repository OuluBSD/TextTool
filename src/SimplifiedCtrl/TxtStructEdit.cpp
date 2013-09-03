#include "SimplifiedCtrl.h"



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
	bar.Separator();
	bar.Add(t_("Import english text as idea"), AppImg::RedRing(), THISBACK(ImportIdea)).Key(K_CTRL_Q);
	bar.Add(t_("Import english text as attributes"), AppImg::RedRing(), THISBACK(ImportAttributes)).Key(K_CTRL_W);
	
}

String TxtStructEdit::GetStatusText() {
	return t_("Deconstructed, auto-english, auto-native");
}

void TxtStructEdit::ImportIdea() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = GetSong();
	
	String text = other.GetData();
	text.Replace("\r", "");
	
	Vector<String> not_found;
	
	Vector<String> parts = Split(text, "\n\n");
	Vector<StaticPart*> part_ptrs;
	Vector<String> lyrics, all_lyrics;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		
		String title = lines[0];
		if (title.Right(1) == ":")
			title = title.Left(title.GetCount()-1);
		title = TrimBoth(title);
		
		bool found = false;
		for(int j = 0; j < song.parts.GetCount(); j++) {
			StaticPart& part = song.parts[j];
			if (ToLower(part.name) == ToLower(title)) {
				part_ptrs << &part;
				found = true;
				break;
			}
		}
		if (!found)
			not_found << title;
		
		lines.Remove(0);
		lyrics << Join(lines, "\n");
		
		all_lyrics.Append(lines);
	}
	
	if (not_found.GetCount()) {
		PromptOK("Parts not found in structure: " + Join(not_found, ", "));
		return;
	}
	
	
	p.RealizePipe();
	TaskMgr& m = *p.song->pipe;
		
	ASSERT(parts.GetCount() == part_ptrs.GetCount());
	ASSERT(parts.GetCount() == lyrics.GetCount());
	for(int i = 0; i < parts.GetCount(); i++) {
		m.GetIdeaFromLyrics(lyrics[i], THISBACK2(OnIdeaFromLyrics, (Song*)0, part_ptrs[i]));
	}
	m.GetIdeaFromLyrics(Join(all_lyrics, "\n"), THISBACK2(OnIdeaFromLyrics, &song, (StaticPart*)0));
}

void TxtStructEdit::ImportAttributes() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = GetSong();
	
	String text = other.GetData();
	text.Replace("\r", "");
	
	Vector<String> parts = Split(text, "\n\n");
	Vector<String> all_lyrics;
	for(int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		lines.Remove(0);
		all_lyrics.Append(lines);
	}
	
	p.RealizePipe();
	
	TaskMgr& m = *p.song->pipe;
	m.GetAttributesFromLyrics(Join(all_lyrics, "\n"), THISBACK1(OnAttributesFromLyrics, &song));
}

void TxtStructEdit::OnIdeaFromLyrics(String result, Song* song, StaticPart* part) {
	Vector<String> lines = Split(result, "\n");
	
	if (lines.GetCount() < 7+1) {
		PromptOK("Invalid result");
		return;
	}
	
	String text[IDEAPATH_COUNT];
	Color clr[IDEAPATH_COUNT];
	
	for(int i = 0; i < IDEAPATH_COUNT; i++)
		ParseTextColor(lines[i], text[i], clr[i]);
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		String& s = text[i];
		int a = s.Find(":");
		if (a >= 0) s = TrimBoth(s.Mid(a+1));
	}
	
	if (song) {
		for(int i = 0; i < IDEAPATH_COUNT; i++) {
			song->active_idea[i] = text[i];
			song->active_idea_clr[i] = clr[i];
		}
	}
	if (part) {
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++) {
			part->active_idea[i] = text[i + IDEAPATH_PARTBEGIN];
			part->active_idea_clr[i] = clr[i + IDEAPATH_PARTBEGIN];
		}
	}
}

void TxtStructEdit::OnAttributesFromLyrics(String result, Song* song) {
	Vector<String> lines = Split(result, "\n");
	
	Vector<String> keys, values;
	for (String& l : lines) {
		l = TrimBoth(l.Mid(1));
		int a = l.Find(".");
		if (a >= 0)
			l = TrimBoth(l.Mid(a+1));
		
		a = l.Find(":");
		if (a < 0) {
			values << l;
			keys.Add();
		}
		else {
			String k = ToLower(TrimBoth(l.Left(a)));
			String v = ToLower(TrimBoth(l.Mid(a+1)));
			int b = v.Find(".");
			if (b >= 0)
				v = TrimBoth(v.Mid(b+1));
			
			values << v;
			keys << k;
		}
	}
	
	Vector<String> E, G, I0, I1;
	Index<String> not_found;
	#define ATTR_ITEM(e, g, i0, i1) not_found << #e; E << #e; G << ToLower(g); I0 << ToLower(i0); I1 << ToLower(i1);
	ATTR_LIST
	#undef ATTR_ITEM
	
	for(int j = 0; j < G.GetCount(); j++) {
		const String& e = E[j];
		const String& g = G[j];
		const String& i0 = I0[j];
		const String& i1 = I1[j];
		
		for (int i = 0; i < keys.GetCount(); i++) {
			//const String& k = keys[i];
			//if (k != g) continue;
			const String& v = values[i];
			if (v == i0 /*|| v == "positive"*/) {
				song->data.GetAdd(e) = "1";
				not_found.RemoveKey(e);
				break;
			} else if (v == i1 /*|| v == "negative"*/) {
				song->data.GetAdd(e) = "-1";
				not_found.RemoveKey(e);
				break;
			} /*else {
				song->data.RemoveKey(e);
			}
			break;*/
		}
	}
	
	for(int i = 0; i < not_found.GetCount(); i++) {
		String e = not_found[i];
		song->data.RemoveKey(e);
	}
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
	
	PostCallback(THISBACK(EnableAll));
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
