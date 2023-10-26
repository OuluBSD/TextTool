#include "SimplifiedCtrl.h"


VocabularyIdeaCtrl::VocabularyIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << local << vocab;
	hsplit.SetPos(9000, 0);
	hsplit.SetPos(8000, 1);
	
	list.AddColumn(t_("Part #"));
	list.AddColumn(t_("Part"));
	list.AddColumn(t_("Line #"));
	list.AddColumn(t_("Visual idea"));
	list.AddColumn(t_("Dialogue idea 1"));
	list.AddColumn(t_("Dialogue idea 2"));
	list.AddColumn(t_("Color idea"));
	list.AddIndex("PART_IDX");
	list.AddIndex("LINE_IDX");
	list.ColumnWidths("1 2 1 4 4 4 2 1 1");
	list.WhenCursor << THISBACK(OnList);
	
	local.AddColumn(t_("#"));
	local.AddColumn(t_("Type"));
	local.AddColumn(t_("Local"));
	local.ColumnWidths("2 3 7");
	
	vocab.AddColumn(t_("Seq #"));
	vocab.AddColumn(t_("Word"));
	vocab.ColumnWidths("1 4");
	
}
	
void VocabularyIdeaCtrl::DisableAll() {
	
}
	
void VocabularyIdeaCtrl::EnableAll() {
	
}
	
void VocabularyIdeaCtrl::Data() {
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
				int col = 6 + k;
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
			
			list.Set(row, 3, Join(GetLineVisualIdea(row), " "));
			list.Set(row, 4, Join(GetLineDialogueIdea1(row), " "));
			list.Set(row, 5, Join(GetLineDialogueIdea2(row), " "));
			
			row++;
		}
	}
	list.SetCount(row);
	list.SetColumnDisplay(6, Single<ColorRowDisplay>());
	
	if (!list.IsCursor() && list.GetCount())
		list.SetCursor(0);
	
	
	DataLocal();
	DataResult();
}

void VocabularyIdeaCtrl::OnList() {
	
	
	DataLocal();
	DataResult();
}

void VocabularyIdeaCtrl::DataLocal() {
	Song& s = GetSong();
	if (!list.IsCursor()) return;
	int list_i = list.GetCursor();
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	int line_i = list.Get(list_i, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	const auto& colorvec = part.colors[0][line_i];
	int row = 0;
	for(int i = 0; i < colorvec.GetCount(); i++) {
		Color clr0 = colorvec[i];
		for (int t = 0; t < VOCABULARYTYPE_COUNT; t++) {
			int min_dist = INT_MAX;
			Vector<int> min_j;
			for(int j = 0; j < SIGNIFICANT_PHRASE_COUNT; j++) {
				Color cmp_clr = SignificantPhrases[t][j].GetColor();
				int clr_distance = GetColorDistance(clr0, cmp_clr);
				if (clr_distance < min_dist) {
					min_dist = clr_distance;
					min_j.Clear();
				}
				if (clr_distance <= min_dist) {
					min_j << j;
				}
			}
			for(int j = 0; j < min_j.GetCount(); j++) {
				int k = min_j[j];
				Color clr1 = SignificantPhrases[t][k].GetColor();
				local.Set(row, 0, i);
				local.Set(row, 1, VocabularyTypeString[t]);
				local.Set(row, 2,
					AttrText(SignificantPhrases[t][k].txt)
						.NormalPaper(Blend(clr1, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(clr1, Black(), 128)).Ink(White())
						);
				
				row++;
			}
		}
	}
	local.SetCount(row);
}

void VocabularyIdeaCtrl::DataResult() {
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

void VocabularyIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update line idea"), AppImg::BlueRing(), THISBACK2(GetLineIdea, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all lines"), AppImg::RedRing(), THISBACK2(GetLineIdea, 0, true)).Key(K_F5);
}

void VocabularyIdeaCtrl::GetLineIdea(int row, bool start_next) {
	if (row < 0) {
		if (!list.IsCursor())
			return;
		row = list.GetCursor();
	}
	
	Song& s = GetSong();
	
	VocabularyIdeaArgs args;
	args.fn = 0;
	
	// Set visual idea
	args.visual = GetLineVisualIdea(row);
	
	// Set characters idea
	args.characters = GetLineCharactersIdea(row);
	
	// Set dialogue idea
	args.dialogue1 = GetLineDialogueIdea1(row);
	args.dialogue2 = GetLineDialogueIdea2(row);
	
	// Set color idea
	args.colors = GetLineColorIdea(row);
	
	// Set matching phrases
	if (0) {
		args.phrases = GetLinePhraseIdea(row, true);
		for (auto& v : args.phrases)
			if (v.GetCount() > phrase_limit)
				v.SetCount(phrase_limit);
	}
	
	s.RealizePipe();
	
	TaskMgr& m = *s.pipe;
	m.GetVocabularyIdea(args, [this, &s, row, start_next](String result) {PostCallback(THISBACK4(OnIdeaResult, result, &s, row, start_next));});
}

void VocabularyIdeaCtrl::OnIdeaResult(String result, Song* song, int list_i, bool start_next) {
	ASSERT(list_i >= 0 && list_i < list.GetCount());
	int part_i = list.Get(list_i, "PART_IDX");
	int line_i = list.Get(list_i, "LINE_IDX");
	StaticPart& part = song->parts[part_i];
	
	if (line_i >= part.vocabulary.GetCount())
		part.vocabulary.SetCount(line_i+1);
	
	Vector<Vector<String>>& vocab = part.vocabulary[line_i];
	
	result.Replace("\r", "");
	result.Replace("\n \n", "\n\n");
	result.Replace("\n  \n", "\n\n");
	result.Replace("\n   \n", "\n\n");
	result.Replace("\n\t\n", "\n\n");
	result.Replace("\n\t\t\n", "\n\n");
	
	Vector<String> parts = Split(result, "\n\n");
	vocab.SetCount(parts.GetCount());
	
	for(int i = 0; i < parts.GetCount(); i++) {
		auto& v = vocab[i];
		v = Split(parts[i], "\n");
		v.Remove(0);
		
		Index<String> uniq;
		for (String& s : v) {
			RemoveLineChar(s);
			if (s.GetCount())
				uniq.FindAdd(s);
		}
		
		v <<= uniq.GetKeys();
	}
	
	PostCallback(THISBACK(DataResult));
	
	if (start_next && list_i+1 < list.GetCount())
		PostCallback(THISBACK2(GetLineIdea, list_i+1, true));
}

Vector<Color> VocabularyIdeaCtrl::GetLineColorIdea(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	Vector<Color> o;
	const auto& vv = part.colors[0];
	if (line_i < vv.GetCount()) {
		const auto& v = vv[line_i];
		o <<= v;
	}
	return o;
}

Vector<String> VocabularyIdeaCtrl::GetPartData(int row, String key) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	String txt = part.data.Get(key,"");
	return Split(txt, "\n");
}

Vector<String> VocabularyIdeaCtrl::FindLineData(int row, Vector<String> lines) {
	int line_i = list.Get(row, "LINE_IDX");
	if (lines.GetCount() == 1) {
		RemoveLineNumber(lines[0]);
		return lines;
	}
	else if (lines.GetCount() == 8) {
		lines.Remove(0, line_i);
		lines.Remove(1, lines.GetCount()-1);
		RemoveLineNumber(lines[0]);
		return lines;
	}
	else if (lines.GetCount() == 16) {
		int begin = line_i * 2;
		lines.Remove(0, begin);
		lines.Remove(2, lines.GetCount()-2);
		RemoveLineNumber(lines[0]);
		RemoveLineNumber(lines[1]);
		return lines;
	}
	else if (lines.GetCount() == 9 || lines.GetCount() == 10) {
		if (line_i == 0) {
			int begin = lines.GetCount() - 7;
			lines.Remove(begin,lines.GetCount()-begin);
			for (auto& s : lines)
				RemoveLineNumber(s);
			return lines;
		}
		else {
			int begin = line_i+1;
			lines.Remove(0, begin);
			lines.Remove(1, lines.GetCount()-1);
			RemoveLineNumber(lines[0]);
			return lines;
		}
	}
	else if (lines.GetCount() == 18) {
		if (line_i == 0) {
			lines.Remove(4,18-4);
			RemoveLineNumber(lines[0]);
			RemoveLineNumber(lines[1]);
			RemoveLineNumber(lines[2]);
			RemoveLineNumber(lines[3]);
			return lines;
		}
		else {
			int begin = (line_i-1)*2+4;
			lines.Remove(0, begin);
			lines.Remove(2, lines.GetCount()-2);
			RemoveLineNumber(lines[0]);
			RemoveLineNumber(lines[1]);
			return lines;
		}
	}
	else TODO
	return lines;
}

Vector<String> VocabularyIdeaCtrl::GetLineVisualIdea(int row) {
	return FindLineData(row, GetPartData(row, "VISUAL_IDEA_STORY"));
}

Vector<String> VocabularyIdeaCtrl::GetLineCharactersIdea(int row) {
	Vector<String> v = GetPartData(row, "VISUAL_IDEA_CHARACTERS");
	for (auto& s : v)
		RemoveLineNumber(s);
	return v;
}

Vector<String> VocabularyIdeaCtrl::GetLineDialogueIdea1(int row) {
	return FindLineData(row, GetPartData(row, "DIALOGUE_IDEA_1"));
}

Vector<String> VocabularyIdeaCtrl::GetLineDialogueIdea2(int row) {
	return FindLineData(row, GetPartData(row, "DIALOGUE_IDEA_2"));
}

Vector<Vector<String>> VocabularyIdeaCtrl::GetLinePhraseIdea(int row, bool colors) {
	Vector<Vector<String>> o;
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	const auto& colorvec = part.colors[0][line_i];
	for(int i = 0; i < colorvec.GetCount(); i++) {
		auto& ov = o.Add();
		Color clr0 = colorvec[i];
		for (int t = 0; t < VOCABULARYTYPE_COUNT; t++) {
			int min_dist = INT_MAX;
			Vector<int> min_j;
			for(int j = 0; j < SIGNIFICANT_PHRASE_COUNT; j++) {
				Color cmp_clr = SignificantPhrases[t][j].GetColor();
				int clr_distance = GetColorDistance(clr0, cmp_clr);
				if (clr_distance < min_dist) {
					min_dist = clr_distance;
					min_j.Clear();
				}
				if (clr_distance <= min_dist) {
					min_j << j;
				}
			}
			for(int j = 0; j < min_j.GetCount(); j++) {
				int k = min_j[j];
				String s;
				s << SignificantPhrases[t][k].txt;
				if (colors) {
					Color clr = SignificantPhrases[t][k].GetColor();
					s << ", RGB(" << clr.GetR() << "," << clr.GetG() << "," << clr.GetB() << ")";
				}
				ov << s;
			}
		}
	}
	return o;
}
