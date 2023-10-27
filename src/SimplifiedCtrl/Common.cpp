#include "SimplifiedCtrl.h"



LineListCtrl::LineListCtrl() {
	
}

void LineListCtrl::Init() {
	list.AddIndex("PART_IDX");
	list.AddIndex("LINE_IDX");
	
}

Vector<String> LineListCtrl::GetLineWordSalads(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	Vector<String> out;
	if (line_i >= 0 && line_i < part.wordsalads.GetCount())
		out <<= part.wordsalads[line_i];
	return out;
}

Vector<Vector<String>> LineListCtrl::GetPartWordSalads(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	Vector<Vector<String>> out;
	out <<= part.wordsalads;
	return out;
}

Vector<Color> LineListCtrl::GetLineColorIdea(int row) {
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

Vector<String> LineListCtrl::GetPartData(int row, String key) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row < list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	StaticPart& part = s.parts[part_i];
	String txt = part.data.Get(key,"");
	return Split(txt, "\n");
}

Vector<String> LineListCtrl::FindLineData(int row, Vector<String> lines) {
	int line_i = list.Get(row, "LINE_IDX");
	if (lines.GetCount() == 0) {
		return lines;
	}
	else if (lines.GetCount() == 1) {
		RemoveLineNumber(lines[0]);
		return lines;
	}
	else if (lines.GetCount() <= 8) {
		if (line_i < lines.GetCount()) {
			lines.Remove(0, line_i);
			lines.Remove(1, lines.GetCount()-1);
				for (auto& s : lines)
					RemoveLineNumber(s);
		}
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

Vector<String> LineListCtrl::GetLineVisualIdea(int row) {
	return FindLineData(row, GetPartData(row, "VISUAL_IDEA_STORY"));
}

Vector<String> LineListCtrl::GetPartVisualIdea(int row) {
	return GetPartData(row, "VISUAL_IDEA_STORY");
}

Vector<String> LineListCtrl::GetLineCharactersIdea(int row) {
	Vector<String> v = GetPartData(row, "VISUAL_IDEA_CHARACTERS");
	for (auto& s : v)
		RemoveLineNumber(s);
	return v;
}

Vector<String> LineListCtrl::GetLineDialogueIdea1(int row) {
	return FindLineData(row, GetPartData(row, "DIALOGUE_IDEA_1"));
}

Vector<String> LineListCtrl::GetPartDialogueIdea1(int row) {
	return GetPartData(row, "DIALOGUE_IDEA_1");
}

Vector<String> LineListCtrl::GetLineDialogueIdea2(int row) {
	return FindLineData(row, GetPartData(row, "DIALOGUE_IDEA_2"));
}

Vector<String> LineListCtrl::GetPartDialogueIdea2(int row) {
	return GetPartData(row, "DIALOGUE_IDEA_2");
}

Vector<Vector<String>> LineListCtrl::GetLinePhraseIdea(int row, bool colors) {
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

Vector<Vector<String>> LineListCtrl::GetVocabulary(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row< list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	if (line_i >= part.vocabulary.GetCount()) {
		return Vector<Vector<String>>();
	}
	
	Vector<Vector<String>> out;
	out <<= part.vocabulary[line_i];
	return out;
}

Vector<Vector<Color>> LineListCtrl::GetColors(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row< list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	if (line_i >= part.vocabulary.GetCount()) {
		return Vector<Vector<Color>>();
	}
	Vector<Vector<Color>> out;
	for(int i = 0; i < part.colors.GetCount(); i++) {
		const auto& clr_vv = part.colors[i];
		auto& o = out.Add();
		if (line_i < clr_vv.GetCount() && i < clr_vv[line_i].GetCount()) {
			o <<= clr_vv[line_i];
		}
	}
	return out;
}

Vector<Color> LineListCtrl::GetListenerColors(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row< list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	Vector<Color> out;
	for(int k = 0; k < part.listener_colors.GetCount(); k++) {
		const auto& v = part.listener_colors[k];
		if (line_i < v.GetCount())
			out.Add(v[line_i]);
	}
	return out;
}

Vector<Vector<Color>> LineListCtrl::GetPreviousListenerColors(int row) {
	Song& s = GetSong();
	ASSERT(row >= 0 && row< list.GetCount());
	int part_i = list.Get(row, "PART_IDX");
	int line_i = list.Get(row, "LINE_IDX");
	StaticPart& part = s.parts[part_i];
	Vector<Vector<Color>> out;
	out.SetCount(LISTENERTYPE_COUNT);
	
	Index<String> possible_previous, possible_next;
	GetPossibleParts(s, part, possible_previous, possible_next);
	
	for (const String& type : possible_previous.GetKeys()) {
		for(int i = 0; i < s.parts.GetCount(); i++) {
			const auto& p = s.parts[i];
			if (p.type == type) {
				for(int k = 0; k < p.listener_colors.GetCount(); k++) {
					const auto& v = p.listener_colors[k];
					if (line_i < v.GetCount())
						out[k].Add(v[line_i]);
				}
			}
		}
	}
	return out;
}

void LineListCtrl::GetPossibleParts(Song& s, StaticPart& part, Index<String>& possible_previous, Index<String>& possible_next) {
	int c = s.active_struct.parts.GetCount();
	for(int i = 0; i < c; i++) {
		const String& type = s.active_struct.parts[i];
		if (type == part.type) {
			if (i > 0)
				possible_previous.FindAdd(s.active_struct.parts[i-1]);
			if (i+1 < c)
				possible_next.FindAdd(s.active_struct.parts[i+1]);
		}
	}
}
