#include "TextLib.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


void TaskManager::OnTokenData(String result, Task* t) {
	// MOVED
}

void TaskManager::OnAmbiguousWordPairs(String res, Task* t) {
	// MOVED
}

void TaskManager::OnVirtualPhrases(String res, Task* t) {
	// MOVED
}

void TaskManager::OnVirtualPhraseTypes(String res, Task* t) {
	// MOVED
}

void TaskManager::OnActionlistColors(String result, Task* t) {
	// MOVED
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	// MOVED
}

void TaskManager::OnLineActions(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.dataset;
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	Vector<String> txt_lines = Split(batch.txt, "\n");
	ASSERT(txt_lines.GetCount());
	
	// e.g. tone(desperate) + msg(distracting oneself) + bias(impulsiveness)
	Vector<ActionHeader> actions;
	
	if (lines.GetCount() == txt_lines.GetCount()) {
		int line_idx = -1;
		Vector<int> ap_is;
		for (String& l : lines) {
			line_idx++;
			l = TrimBoth(l);
			if (l.IsEmpty())
				continue;
			
			const String& txt = txt_lines[line_idx];
			actions.SetCount(0);
			
			Vector<String> parts = Split(l, "+");
			CombineHash ch;
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				s = TrimBoth(s);
				int a = s.Find("(");
				int b = s.Find(")");
				if (a < 0 || b < 0 || a > b) {
					parts.Remove(i--);
					continue;
				}
				String action = TrimBoth(s.Left(a));
				a++;
				String arg = TrimBoth(s.Mid(a,b-a));
				
				ActionHeader& aa = actions.Add();
				aa.action = action;
				aa.arg = arg;
				
			}
			/*Sort(actions, ActionHeader());
			for (ActionHeader& aa : actions) {
				ch.Do(aa.action);
				ch.Do(aa.arg);
			}
			hash_t h = ch;
			
			bool found = false;
			int i = 0;
			for (ExportDepActionPhrase& ap : da.action_phrases.GetValues()) {
				if (ap.hash == h) {
					found = true;
					ap_is << i;
					if (line_idx == 0 && batch.song_begins)
						ap.first_lines++;
				}
				i++;
			}
			if (!found) {*/
			int ap_i = -1;
			ExportDepActionPhrase& ap = da.action_phrases.GetAdd(txt, ap_i);
			ap_is << ap_i;
			if (ap.actions.IsEmpty()) {
				//ap.hash = h;
				//ap.txt = txt;
				ap.first_lines = line_idx == 0 && batch.song_begins ? 1 : 0;
				//Swap(ap.actions, actions);
				for (const ActionHeader& ah : actions) {
					ASSERT(ah.action.Find("\"") != 0 && ah.arg.Find("\"") != 0);
					da.actions.GetAdd(ah, ap.actions.Add());
				}
			}
		}
		for(int i = 1; i < ap_is.GetCount(); i++) {
			int ap_i0 = ap_is[i-1];
			int ap_i1 = ap_is[i];
			ExportDepActionPhrase& ap0 = da.action_phrases[ap_i0];
			VectorFindAdd(ap0.next_phrases, ap_i1);
		}
	}
	
	t->update(t->batch_i, t->batches.GetCount());
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnSyllables(String res, Task* t) {
	// MOVED
}

void TaskManager::OnDetails(String res, Task* t) {
	// MOVED
}

void TaskManager::OnLineChangeScores(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.dataset;
	
	res.Replace("\r", "");
	res = "1. Stop line 1 & start line 2: S0:" + res;
	res.Replace("not applicable", "0");
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	
	Vector<int>& ap_is = t->tmp;
	
	for (String& l : lines) {
		int a = l.Find("line");
		if (a < 0) continue;
		int b = l.Find("&", a);
		if (b < 0) continue;
		a += 4;
		int line0 = StrInt(TrimBoth(l.Mid(a,b-a)));
		a = l.Find("line",b);
		if (a < 0) continue;
		b = l.Find(":", a);
		if (b < 0) continue;
		a += 4;
		int line1 = StrInt(TrimBoth(l.Mid(a,b-a)));
		
		Vector<String> parts = Split(l.Mid(b+1), ",");
		if (parts.GetCount() != SCORE_COUNT)
			continue;
		int score[SCORE_COUNT] = {0,0,0,0,0};
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			int a = s.Find(":");
			if (a < 0) continue;
			score[i] = StrInt(TrimBoth(s.Mid(a+1)));
		}
		
		line0--;
		line1--;
		if (line0 >= ap_is.GetCount() || line1 >= ap_is.GetCount()) continue;
		
		int ap_i0 = ap_is[line0];
		int ap_i1 = ap_is[line1];
		if (ap_i0 < 0 || ap_i1 < 0)
			continue;
		
		ExportDepActionPhrase& ap0 = da.action_phrases[ap_i0];
		
		int j = VectorFind(ap0.next_phrases, ap_i1);
		if (j >= 0) {
			if (ap0.next_scores.GetCount() <= j)
				ap0.next_scores.SetCount(j+1);
			Vector<int>& tgt = ap0.next_scores[j];
			tgt.SetCount(SCORE_COUNT);
			for(int j = 0; j < SCORE_COUNT; j++)
				tgt[j] = score[j];
		}
	}
	
	int a = 0;
	for(const ExportDepActionPhrase& p : da.action_phrases.GetValues())
		if (p.next_phrases.GetCount())
			a++;
	da.diagnostics.GetAdd("line change scores: actual") =  IntStr(a);
	da.diagnostics.GetAdd("line change scores: total") = IntStr(da.action_phrases.GetCount());
	da.diagnostics.GetAdd("line change scores: percentage") =  DblStr((double)a / (double)da.action_phrases.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnColorAlternatives(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	VectorMap<String, Color>& word_clr = t->word_clr;
	
	res = t->tmp_str + res;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	VectorMap<int,Vector<String>> iter_phrase_parts;
	VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
	
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		
		int a = l.Find(":");
		if (a < 0) continue;
		String main_class = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find("->");
		if (a < 0) continue;
		String src_word = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+2));
		
		a = l.Find(":");
		if (a < 0) continue;
		String dst_clr_str = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		String dst_word = l;
		
		
		dst_clr_str = dst_clr_str.Mid(dst_clr_str.Find("(") + 1);
		dst_clr_str = dst_clr_str.Left(dst_clr_str.GetCount()-1);
		
		int clr_i[3];
		Vector<String> clr_parts = Split(dst_clr_str, ",");
		if (clr_parts.GetCount() != 3)
			continue;
		for(int i = 0; i < clr_parts.GetCount(); i++)
			clr_i[i] = StrInt(clr_parts[i]);
		Color dst_clr(clr_i[0], clr_i[1], clr_i[2]);
		
		Color src_clr = word_clr.Get(src_word, Black());
		
		
		
		
		CombineHash ch;
		ch.Do(main_class);
		ch.Do(src_word);
		ch.Do(src_clr);
		hash_t h = ch;
		
		break; // difficult problem: hash is calculated differently & matching is done differently
		#if 0
		DatasetAnalysis& ds = sda.dataset;
		bool found = false;
		for (ColorWordnet& wn : ds.clr_wordnets) {
			if (wn.hash == h) {
				int j = VectorFindAdd(wn.words, dst_word);
				if (j == wn.colors.GetCount())
					wn.colors.Add(dst_clr);
				found = true;
			}
		}
		if (!found) {
			ColorWordnet& wn = ds.clr_wordnets.Add();
			wn.hash = h;
			wn.words << dst_word;
			wn.colors << dst_clr;
			wn.main_class = main_class;
			wn.src_word = src_word;
			wn.clr = src_clr;
			wn.clr_group = GetColorGroup(src_clr);
		}
		#endif
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnWordData(String res, Task* t) {
	
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnSubPicked(String result, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Component& song = *t->component;
	
	TODO
	#if 0
	
	result = TrimBoth(result);
	if (result.GetCount()) {
		int sub_i = -1;
		if (IsDigit(result[0]))
			sub_i = ScanInt(result) - 1;
		else
			sub_i = t->tmp_words.Find(result);
		
		if (sub_i >= 0) {
			ASSERT(t->part_i >= 0 && t->line_i >= 0);
			StaticPart& sp = song.parts[t->part_i];
			auto& line = sp.text.Get()[t->line_i];
			
			if (sub_i >= 0 && sub_i < line.sub_pp_i.GetCount()) {
				int pp_i = line.sub_pp_i[sub_i];
				line.end_pp_i = pp_i;
			}
			else {
				line.end_pp_i = -1;
			}
		}
	}
	
	t->on_ready();
	
	RemoveTask(*t);
	#endif
}

void TaskManager::OnNanaFit(String result, Task* t) {
	
	
	
	
	t->on_ready();
	
	RemoveTask(*t);
}

void TaskManager::OnAttributes(String res, Task* t) {
	// MOVED
}

void TaskManager::OnAttributePolars(String res, Task* t) {
	// MOVED
}

void TaskManager::OnAttributeJoins(String res, Task* t) {
	// MOVED
}


END_TEXTLIB_NAMESPACE
#endif
