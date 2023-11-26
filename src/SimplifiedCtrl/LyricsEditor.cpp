#include "SimplifiedCtrl.h"


LyricsEditor::LyricsEditor() {
	Add(attr.HSizePos().TopPos(0,30));
	Add(vsplit.HSizePos().VSizePos(30));
	CtrlLayout(attr);
	
	// Top bar values
	attr.syllables.SetData(0);
	attr.syllable_range.SetData(2);
	
	attr.clr.Add(t_("All"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		attr.clr.Add(
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White())
		);
	}
	attr.clr.SetIndex(0);
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++)
		attr_groups.FindAdd(Attr::AttrKeys[i][1]);
	attr.attr.Add(t_("All"));
	for(int i = 0; i < attr_groups.GetCount(); i++)
		attr.attr.Add(attr_groups[i]);
	attr.attr.SetIndex(0);
	attr.attr.WhenAction << THISBACK(OnAttribute);
	attr.action.WhenAction << THISBACK(OnAction);
	
	attr.end_rhyming.Set(true);
	
	
	// Main gui
	vsplit.Vert() << pages << hsplit;
	vsplit.SetPos(8000);
	
	hsplit << parts << lines;
	
	parts.AddColumn(t_("Part"));
	//parts.AddColumn(t_("Lines"));
	//parts.ColumnWidths("3 1");
	parts <<= THISBACK(DataPart);
	
	
	pages.Add(line_split.SizePos(), t_("Line picker"));
	pages.Add(idea_hsplit.SizePos(), t_("Coherent idea"));
	pages.Add(part_hsplit.SizePos(), t_("Part idea"));
	
	
	// Line picker
	line_split.Horz() << line_actions << line_templates << line_phrases;
	
	line_actions.AddColumn(t_("Actions"));
	
	line_templates.AddColumn(t_("Template"));
	line_templates.AddColumn(t_("Match"));
	line_templates.ColumnWidths("4 1");
	
	line_phrases.AddColumn(t_("Phrase"));
	line_phrases.AddColumn(t_("Pronounciation"));
	line_phrases.AddColumn(t_("Distance"));
	line_phrases.ColumnWidths("3 3 1");
	
	
	// Coherent idea
	idea_hsplit.Horz() << idea_vsplit0 << idea_vsplit1;
	idea_vsplit0.Vert();
	idea_vsplit1.Vert();
	for(int i = 0; i < SCORE_COUNT; i++) {
		(i % 2 == 0 ? idea_vsplit0 : idea_vsplit1) << idea_lists[i];
		idea_lists[i].AddColumn(ScoreTitles[i]);
	}
	
	
	// Part idea
	part_hsplit.Horz() << part_vsplit0 << part_vsplit1;
	part_vsplit0.Vert();
	part_vsplit1.Vert();
	for(int i = 0; i < SCORE_COUNT; i++) {
		(i % 2 == 0 ? part_vsplit0 : part_vsplit1) << part_lists[i];
		part_lists[i].AddColumn(ScoreTitles[i]);
	}
	
}

void LyricsEditor::OnAttribute() {
	attr.attr_value.Clear();
	int idx = attr.attr.GetIndex();
	if (idx) {
		idx--;
		String group = attr_groups[idx];
		for(int i = 0; i < Attr::ATTR_COUNT; i++) {
			if (Attr::AttrKeys[i][1] == group) {
				attr.attr_value.Add(Attr::AttrKeys[i][2]);
				attr.attr_value.Add(Attr::AttrKeys[i][3]);
			}
		}
		if (attr.attr_value.GetCount())
			attr.attr_value.SetIndex(0);
	}
}

void LyricsEditor::UpdateActionList() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	for (const ActionPhrase& ap : da.action_phrases) {
		for (const auto& a : ap.actions)
			uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
	}
	struct Sorter {
		bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
			return a.GetCount() > b.GetCount();
		}
	};
	SortByValue(uniq_acts, Sorter());
	for (auto& v : uniq_acts.GetValues())
		SortByValue(v, StdGreater<int>());
	
	attr.action.Clear();
	attr.action.Add(t_("All"));
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		String key = uniq_acts.GetKey(i);
		attr.action.Add(key);
	}
	attr.action.SetIndex(0);
	
	OnAction();
}

void LyricsEditor::OnAction() {
	int idx = attr.action.GetIndex();
	attr.action_value.Clear();
	if (idx) {
		idx--;
		String group = uniq_acts.GetKey(idx);
		auto& v = uniq_acts[idx];
		for(int i = 0; i < v.GetCount(); i++) {
			String val = v.GetKey(i);
			attr.action_value.Add(val);
		}
		if (attr.action_value.GetCount())
			attr.action_value.SetIndex(0);
	}
}

void LyricsEditor::Data() {
	Song& s = GetSong();
	
	if (attr.action.GetCount() == 0)
		UpdateActionList();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& p = s.parts[i];
		SetColoredListValue(parts, i, 0, p.name, GetSongPartPaperColor(p.type), false);
	}
	parts.SetCount(s.parts.GetCount());
	
	int cursor = 0;
	if (cursor >= 0 && cursor < parts.GetCount() && !parts.IsCursor())
		parts.SetCursor(cursor);
	
	DataPart();
}

void LyricsEditor::DataPart() {
	if (!parts.IsCursor()) {
		lines.Clear();
		return;
	}
	int part_i = parts.GetCursor();
	Song& s = GetSong();
	StaticPart& part = s.parts[part_i];
	
	for(int i = 0; i < part.phrases.GetCount(); i++) {
		StaticPhrase& sp = part.phrases[i];
		SetColoredListValue(lines, i, 0, sp.txt, sp.clr);
	}
	lines.SetCount(part.phrases.GetCount());
	
	
	
	DataLine();
}

void LyricsEditor::DataLine() {
	int tab = pages.Get();
	switch (tab) {
		case 0: DataLinePicker(); break;
		case 1: DataIdea(); break;
		case 2: DataPartIdea(); break;
		default: break;
	}
}

void LyricsEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

void LyricsEditor::DataLinePicker() {
	if (!parts.IsCursor()) {
		line_actions.Clear();
		line_templates.Clear();
		line_phrases.Clear();
		return;
	}
	
	int part_i = parts.GetCursor();
	Song& s = GetSong();
	StaticPart& part = s.parts[part_i];
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	if (sda.datasets.IsEmpty()) return;
	DatasetAnalysis& da = sda.datasets[0];
	
	
	UpdateActions(s, part, da);
	UpdateTemplates(da);
	UpdatePhrases(da);
	
	
	for(int i = 0; i < action_results.GetCount(); i++) {
		int ap_i = action_results[i].ap_i;
		const ActionPhrase& ap = da.action_phrases[ap_i];
		line_actions.Set(i, 0, ap.GetActionText());
	}
	line_actions.SetCount(action_results.GetCount());
	
	for(int i = 0; i < ta_matches.GetCount(); i++) {
		int tp_i = ta_matches.GetKey(i);
		const TemplatePhrase& tp = da.tmpl_phrases[tp_i];
		double match = ta_matches[i];
		SetColoredListValue(line_templates, i, 0, tp.GetText(), tp.clr);
		line_templates.Set(i, 1, match);
	}
	line_templates.SetCount(ta_matches.GetCount());
	
	EnglishPronounciation ep;
	for(int i = 0; i < sents.GetCount(); i++) {
		const String& sent = sents[i];
		line_phrases.Set(i, 0, sent);
		if (ep.Parse(sent)) {
			line_phrases.Set(i, 1, ep.GetPronounciation());
			line_phrases.Set(i, 2, ep.GetInternalRhyming());
		}
		else {
			line_phrases.Set(i, 1, Value());
			line_phrases.Set(i, 2, DBL_MAX);
		}
	}
	line_phrases.SetCount(sents.GetCount());
	line_phrases.SetSortColumn(2, false);
	
}

void LyricsEditor::UpdateActions(Song& song, StaticPart& part, DatasetAnalysis& da) {
	
	action_results.Clear();
	
	String first_part;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& p = song.parts[i];
		if (p.part_type != StaticPart::SKIP) {
			first_part = p.name;
			break;
		}
	}
	
	
	// Case: first line
	if (part.phrases.GetCount() == 0) {
		if (part.name != first_part)
			return;
		int idx = 0;
		for (const ActionPhrase& ap : da.action_phrases) {
			if (ap.first_lines > 0 && ap.actions.GetCount()) {
				auto& r = action_results.Add();
				r.ap_i = idx;
			}
			idx++;
		}
	}
	// Case: continue line
	else {
		PromptOK("TODO");
		
		
	}
}

void LyricsEditor::UpdateTemplates(DatasetAnalysis& da) {
	ta_matches.Clear();
	
	for(int i = 0; i < action_results.GetCount(); i++) {
		ActionResult& ar = action_results[i];
		const ActionPhrase& ap = da.action_phrases[ar.ap_i];
		
		ar.ta_matches.Clear();
		
		double mf_sum = 0;
		int j = 0;
		for (const ActionTemplate& at : da.action_tmpls) {
			const TemplatePhrase& tp = da.tmpl_phrases[at.tp_i];
			double match_factor = GetActionMatching(ap.actions, at.actions);
			if (match_factor > 0.25) {
				mf_sum += match_factor;
				ar.ta_matches.Add(j, match_factor);
			}
			j++;
		}
		ar.av_match = mf_sum / da.action_tmpls.GetCount();
		
		SortByValue(ar.ta_matches, StdGreater<double>());
		int c = min(ar.ta_matches.GetCount(), 10);
		for(int j = 0; j < c; j++) {
			int ta_i = ar.ta_matches.GetKey(j);
			double ta_match = ar.ta_matches[j];
			double& match = ta_matches.GetAdd(ta_i, 0);
			match = max(match, ta_match);
		}
	}
	SortByValue(ta_matches, StdGreater<double>());
	
}

void LyricsEditor::UpdatePhrases(DatasetAnalysis& da) {
	sents.SetCount(0);
	
	for(int i = 0; i < ta_matches.GetCount(); i++) {
		int at_i = ta_matches.GetKey(i);
		const ActionTemplate& at = da.action_tmpls[at_i];
		const TemplatePhrase& tp = da.tmpl_phrases[at.tp_i];
		
		GetSentences(tp, sents, 50);
	}
	
}

void LyricsEditor::GetSentences(const TemplatePhrase& tp, Vector<String>& sents, int limit) {
	thread_local static Vector<int> combs;
	thread_local static Vector<int> comb;
	thread_local static Vector<int> lengths;
	
	// Fast clear vector
	int wc = tp.words.GetCount();
	if (!wc) return;
	comb.SetCount(wc);
	lengths.SetCount(wc);
	combs.SetCount(0);
	combs.Reserve(1000);
	for (auto& i : comb) i = 0;
	for(int i = 0; i < wc; i++)
		lengths[i] = tp.words[i].GetCount();
	
	// Get all combinations
	while (1) {
		// Add combination
		int begin = combs.GetCount();
		combs.SetCount(begin + wc);
		int* dst = combs.Begin() + begin;
		const int* src = comb.Begin();
		int* end = dst + wc;
		while (dst != end)
			*dst++ = *src++;
		
		// Increase combination vector
		int* len_it = lengths.End()-1;
		int* it = comb.End()-1;
		end = it - wc;
		while (it != end) {
			*it += 1;
			if (*it >= *len_it) {
				*it = 0;
			}
			else break;
			len_it--;
			it--;
		}
		if (it == end)
			break;
	}
	
	int comb_count = combs.GetCount() / wc;
	
	comb_count = min(comb_count, limit);
	
	int* comb_it = combs.Begin();
	for(int i = 0; i < comb_count; i++) {
		String phrase;
		int wrd_i = 0;
		for(const String& part : tp.parts) {
			if (part.IsEmpty()) continue;
			if (part[0] == '{') {
				const String& wrd = tp.words[wrd_i][comb_it[wrd_i]];
				if (!phrase.IsEmpty()) phrase << " ";
				phrase << wrd;
				wrd_i++;
			}
			else {
				if (part.GetCount() == 1 && part == ",")
					phrase << ", ";
				else {
					if (!phrase.IsEmpty()) phrase << " ";
					phrase << part;
				}
			}
		}
		sents << phrase;
		comb_it += wc;
	}
	
}

void LyricsEditor::DataIdea() {
	if (!parts.IsCursor()) {
		for(int i = 0; i < SCORE_COUNT; i++)
			idea_lists[i].Clear();
		return;
	}
	
}

void LyricsEditor::DataPartIdea() {
	if (!parts.IsCursor()) {
		for(int i = 0; i < SCORE_COUNT; i++)
			part_lists[i].Clear();
		return;
	}
	
}
