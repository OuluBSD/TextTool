#include "SimplifiedCtrl.h"


TextIdeaCtrl::TextIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << texts;
	
	list.AddColumn(t_("Part #"));
	list.AddColumn(t_("Part"));
	list.AddColumn(t_("Text"));
	LineListCtrl::Init();
	list.ColumnWidths("1 2 6");
	list.SetLineCy(15*8);
	list.WhenCursor << THISBACK(DataPart);
	
	texts.AddColumn(t_("Text"));
	texts.ColumnWidths("1");
	texts.SetLineCy(15*8);
	
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
		list.Set(i, 2, Join(part.active_source_text, "\n"));
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
	}
	texts.SetCount(part.source_text_suggestions.GetCount());
	
}

void TextIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update part"), AppImg::BlueRing(), THISBACK2(GetPartTexts, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all parts"), AppImg::RedRing(), THISBACK2(GetPartTexts, 0, true)).Key(K_F5);
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
	
}

