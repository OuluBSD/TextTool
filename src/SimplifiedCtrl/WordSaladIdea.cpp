#include "SimplifiedCtrl.h"


WordSaladIdeaCtrl::WordSaladIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << vocab << sentences;
	hsplit.SetPos(5000,0);
	hsplit.SetPos(6500,1);
	
	list.AddColumn(t_("Part #"));
	list.AddColumn(t_("Part"));
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Color sequence"));
	list.AddColumn(t_("Listener colors"));
	LineListCtrl::Init();
	list.ColumnWidths("1 2 1 6 2");
	list.WhenCursor << THISBACK(OnList);
	
	vocab.AddColumn(t_("#"));
	vocab.AddColumn(t_("Phrase"));
	vocab.ColumnWidths("1 4");
	
	sentences.AddColumn(t_("Sentence"));
	
	
}
	
void WordSaladIdeaCtrl::DisableAll() {
	
}
	
void WordSaladIdeaCtrl::EnableAll() {
	
}
	
void WordSaladIdeaCtrl::Data() {
	Song& s = GetSong();
	
	int row = 0;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		
		part.colors.SetCount(4);
		int part_rows = max(1, part.colors[0].GetCount());
		
		for(int j = 0; j < part_rows; j++) {
			list.Set(row, "PART_IDX", i);
			list.Set(row, "LINE_IDX", j);
			list.Set(row, 0, i);
			list.Set(row, 1, part.name);
			list.Set(row, 2, j);
			
			// Get color vectors for: main, attack, decay, release
			for(int k = 0; k < 1; k++) {
				int col = 3 + k;
				if (k > 0) col++; // jump over listener column (if not main)
				const auto& vv = part.colors[k];
				if (j >= vv.GetCount()) {
					list.Set(row, col, Value());
					continue;
				}
				const auto& v = vv[j];
				ValueArray va;
				for (const auto& c : v)
					va.Add(c);
				list.Set(row, col, va);
			}
			
			// Color vector for listeners
			{
				ValueArray va;
				for(int k = 0; k < part.listener_colors.GetCount(); k++) {
					const auto& v = part.listener_colors[k];
					if (j < v.GetCount())
						va.Add(v[j]);
				}
				list.Set(row, 4, va);
			}
			
			row++;
		}
	}
	list.SetCount(row);
	list.SetColumnDisplay(3, Single<ColorRowDisplay>());
	list.SetColumnDisplay(4, Single<ColorRowDisplay>());
	
	if (!list.IsCursor() && list.GetCount())
		list.SetCursor(0);
	
	DataVocabulary();
	DataSentences();
}

void WordSaladIdeaCtrl::OnList() {
	DataVocabulary();
	DataSentences();
}

void WordSaladIdeaCtrl::DataVocabulary() {
	Song& s = GetSong();
	if (!list.IsCursor()) return;
	int list_i = list.GetCursor();
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	int line_i = list.Get(list_i, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	if (line_i >= part.vocabulary.GetCount()) {
		vocab.Clear();
		return;
	}
	
	const auto& clr_vv = part.colors[0];
	const auto& vv = part.vocabulary[line_i];
	int row = 0;
	for(int i = 0; i < vv.GetCount(); i++) {
		const auto& v = vv[i];
		for(int j = 0; j < v.GetCount(); j++) {
			const String& s = v[j];
			vocab.Set(row, 0, i);
			
			if (line_i < clr_vv.GetCount() && i < clr_vv[line_i].GetCount()) {
				Color clr = clr_vv[line_i][i];
				vocab.Set(row, 1,
					AttrText(s)
						.NormalPaper(Blend(clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(clr, GrayColor(), 128)).Ink(White())
					);
			}
			else {
				vocab.Set(row, 1, s);
			}
			row++;
		}
	}
	vocab.SetCount(row);
}

void WordSaladIdeaCtrl::DataSentences() {
	Song& s = GetSong();
	if (!list.IsCursor()) return;
	int list_i = list.GetCursor();
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	int line_i = list.Get(list_i, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	
	if (line_i >= part.wordsalads.GetCount()) {
		sentences.Clear();
		return;
	}
	
	const auto& v = part.wordsalads[line_i];
	const auto& cv = part.wordsalad_clrs[line_i];
	for(int i = 0; i < v.GetCount(); i++) {
		Color c = cv[i];
		sentences.Set(i, 0, AttrText(v[i])
			.NormalPaper(Blend(c, White(), 128+64)).NormalInk(Black())
			.Paper(Blend(c, GrayColor())).Ink(White()));
	}
	sentences.SetCount(v.GetCount());
}

void WordSaladIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update line sentence"), AppImg::BlueRing(), THISBACK2(GetLineSentence, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all sentences"), AppImg::RedRing(), THISBACK2(GetLineSentence, 0, true)).Key(K_F5);
}

void WordSaladIdeaCtrl::GetLineSentence(int row, bool start_next) {
	if (row < 0) {
		if (!list.IsCursor())
			return;
		row = list.GetCursor();
	}
	
	Song& s = GetSong();
	
	WordSaladIdeaArgs args;
	args.fn = 0;
	args.vocabulary = GetVocabulary(row);
	args.colors = GetColors(row);
	args.listener_colors = GetListenerColors(row);
	args.listener_colors_in_begin = GetPreviousListenerColors(row);
	args.visual = GetLineVisualIdea(row);
	args.dialogue1 = GetLineDialogueIdea1(row);
	args.dialogue2 = GetLineDialogueIdea2(row);
	
	
	s.RealizePipe();
	
	TaskMgr& m = *s.pipe;
	m.GetWordSaladIdea(args, [this, &s, row, start_next](String result) {PostCallback(THISBACK4(OnLineSentence, result, &s, row, start_next));});
}

void WordSaladIdeaCtrl::OnLineSentence(String result, Song* song, int list_i, bool start_next) {
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	int line_i = list.Get(list_i, "LINE_IDX");
	StaticPart& part = song->parts[part_i];
	
	if (line_i >= part.wordsalads.GetCount()) {
		part.wordsalads.SetCount(line_i+1);
		part.wordsalad_clrs.SetCount(line_i+1);
	}
	Vector<String>& out = part.wordsalads[line_i];
	Vector<Color>& out_clr = part.wordsalad_clrs[line_i];
	out.Clear();
	out_clr.Clear();
	
	result.Replace("\r","");
	result.Replace("(RGB","RGB");
	result.Replace("))",")");
	Vector<String> lines = Split(result, "\n");
	
	for (String& l : lines) {
		RemoveLineChar(l);
		
		String txt;
		Color clr;
		ParseTextColor(l, txt, clr);
		
		out << txt;
		out_clr << clr;
	}
	
	// Sort values based on the closest distance to the target listener's feeling (metaphorical color)
	Vector<Color> listener_colors = GetListenerColors(list_i);
	if (!listener_colors.IsEmpty()) {
		TextColorDistanceSorter sorter;
		sorter.cmp = listener_colors[0];
		sorter.str = &out;
		sorter.clr = &out_clr;
		sorter.Sort();
	}
	
	PostCallback(THISBACK(DataSentences));
	
	if (start_next && list_i+1 < list.GetCount())
		PostCallback(THISBACK2(GetLineSentence, list_i+1, true));
}
