#include "SimplifiedCtrl.h"


TextIdeaCtrl::TextIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << texts;
	hsplit.SetPos(6666);
	
	list.AddColumn(t_("Part #"));
	list.AddColumn(t_("Part"));
	list.AddColumn(t_("Active word Salad"));
	list.AddColumn(t_("Text"));
	LineListCtrl::Init();
	list.ColumnWidths("1 2 6 6");
	list.SetLineCy(20*8);
	list.WhenCursor << THISBACK(DataPart);
	
	texts.AddColumn(t_("Word Salad"));
	texts.AddIndex("TEXT_IDX");
	texts.ColumnWidths("1");
	texts.SetLineCy(20*8);
	
}

void TextIdeaCtrl::DisableAll() {
	
}

void TextIdeaCtrl::EnableAll() {
	
}

void TextIdeaCtrl::Data() {
	Song& s = GetSong();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		
		list.Set(i, 0, i);
		list.Set(i, 1, part.name);
		list.Set(i, 2, Join(part.active_source_wordsalad, "\n"));
		list.Set(i, 3, Join(part.active_source_text, "\n"));
		list.Set(i, "PART_IDX", i);
		list.Set(i, "LINE_IDX", -1);
	}
	list.SetCount(s.parts.GetCount());
	
	if (list.GetCount() && !list.IsCursor())
		list.SetCursor(0);
	
	DataPart();
}

void TextIdeaCtrl::DataPart() {
	Song& s = GetSong();
	
	if (!list.IsCursor())
		return;
	int cur = list.GetCursor();
	int part_i = list.Get(cur, "PART_IDX");
	StaticPart& part = s.parts[part_i];
	
	for(int i = 0; i < part.source_text_suggestions.GetCount(); i++) {
		const auto& v = part.source_text_suggestions[i];
		texts.Set(i, 0, Join(v, "\n"));
		texts.Set(i, "TEXT_IDX", i);
	}
	texts.SetCount(part.source_text_suggestions.GetCount());
	
}

void TextIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update part"), AppImg::BlueRing(), THISBACK2(GetPartTexts, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all parts"), AppImg::RedRing(), THISBACK2(GetPartTexts, 0, true)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Set active part"), AppImg::RedRing(), THISBACK(SetActivePart)).Key(K_CTRL_W);
	bar.Separator();
	bar.Add(t_("Update natural text"), AppImg::BlueRing(), THISBACK2(GetNaturalText, -1, false)).Key(K_CTRL_E);
	bar.Add(t_("Update all natural texts"), AppImg::RedRing(), THISBACK2(GetNaturalText, 0, true)).Key(K_F6);
}

void TextIdeaCtrl::SetActivePart() {
	if (!list.IsCursor() || !texts.IsCursor())
		return;
	Song& s = GetSong();
	int cur = list.GetCursor();
	int part_i = list.Get(cur, "PART_IDX");
	int text_i = texts.Get("TEXT_IDX");
	StaticPart& part = s.parts[part_i];
	const Vector<String>& sug = part.source_text_suggestions[text_i];
	part.active_source_wordsalad <<= sug;
	
	PostCallback(THISBACK(Data));
}

void TextIdeaCtrl::GetPartTexts(int row, bool start_next) {
	if (row < 0) {
		if (!list.IsCursor())
			return;
		row = list.GetCursor();
	}
	
	Song& s = GetSong();
	
	WordSaladIdeaArgs args;
	args.fn = 1;
	args.vocabulary = GetPartWordSalads(row);
	args.visual = GetPartVisualIdea(row);
	args.characters = GetLineCharactersIdea(row);
	args.dialogue1 = GetPartDialogueIdea1(row);
	args.dialogue2 = GetPartDialogueIdea2(row);
	
	for (auto& v : args.vocabulary)
		v.SetCount(10);
	
	s.RealizePipe();
	
	TaskMgr& m = *s.pipe;
	m.GetWordSaladIdea(args, [this, &s, row, start_next](String result) {PostCallback(THISBACK4(OnPartTexts, result, &s, row, start_next));});
}

void TextIdeaCtrl::OnPartTexts(String result, Song* song, int list_i, bool start_next) {
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	StaticPart& part = song->parts[part_i];
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	part.source_text_suggestions.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		
		int a = l.Find(":");
		if (a >= 0)
			l = TrimBoth(l.Mid(a+1));
		
		Vector<String> parts = Split(l, " ");
		if (parts.GetCount() == 1) {
			for(int j = 1; j < l.GetCount(); j++) {
				if (IsAlpha(l[j]) && IsDigit(l[j-1])) {
					l.Insert(j, " ");
					j++;
				}
			}
			parts = Split(l, " ");
		}
		
		Vector<int> codes;
		for(int j = 0; j < parts.GetCount(); j++) {
			String& part = parts[j];
			for(int k = 0; k < part.GetCount(); k++) {
				int chr = part[k];
				if (IsDigit(chr)) {
					part = part.Mid(k);
					break;
				}
			}
			int code = ScanInt(part);
			codes << code;
		}
		
		Vector<String> sent;
		bool fail = false;
		int j = -1;
		for (int code : codes) {
			j++;
			if (j < 0 || j >= part.wordsalads.GetCount()) {
				fail = true;
				break;
			}
			const auto& salad_send = part.wordsalads[j];
			if (code < 0 || code >= salad_send.GetCount()) {
				fail = true;
				break;
			}
			const String& wrd = salad_send[code];
			sent << wrd;
		}
		
		if (fail) {
			LOG("warning: bad wordsalad code");
			continue;
		}
		
		part.source_text_suggestions.Add() <<= sent;
	}
	
	PostCallback(THISBACK(DataPart));
	
	
	if (start_next && list_i+1 < list.GetCount())
		PostCallback(THISBACK2(GetPartTexts, list_i+1, true));
}

void TextIdeaCtrl::GetNaturalText(int row, bool start_next) {
	if (row < 0) {
		if (!list.IsCursor())
			return;
		row = list.GetCursor();
	}
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	
	Song& s = GetSong();
	StaticPart& part = s.parts[part_i];
	if (part.active_source_wordsalad.IsEmpty()) {
		PromptOK("Empty active source text");
		return;
	}
	
	
	WordSaladIdeaArgs args;
	args.fn = 2;
	args.vocabulary.Add() <<= part.active_source_wordsalad;
	args.visual = GetPartVisualIdea(row);
	args.characters = GetLineCharactersIdea(row);
	args.dialogue1 = GetPartDialogueIdea1(row);
	args.dialogue2 = GetPartDialogueIdea2(row);
	
	s.RealizePipe();
	
	TaskMgr& m = *s.pipe;
	m.GetWordSaladIdea(args, [this, &s, row, start_next](String result) {PostCallback(THISBACK4(OnNaturalText, result, &s, row, start_next));});
}

void TextIdeaCtrl::OnNaturalText(String result, Song* song, int list_i, bool start_next) {
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	StaticPart& part = song->parts[part_i];
	
	part.active_source_text.Clear();
	
	if (result.Find("1/10: a") >= 0)
		result = result.Mid(7);
	
	result.Replace("\r", "");
	if (result.Find("\n") < 0)
		result.Replace(". ", ".\n");
	
	Vector<String> lines = Split(result, "\n");
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		RemoveLineNumber(l);
		if (l.IsEmpty()) continue;
		part.active_source_text << l;
	}
	
	PostCallback(THISBACK(Data));
	
	if (start_next && list_i+1 < list.GetCount())
		PostCallback(THISBACK2(GetNaturalText, list_i+1, true));
}
