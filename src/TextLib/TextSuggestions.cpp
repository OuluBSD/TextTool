#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


TextSuggestions::TextSuggestions() {
	
}

int TextSuggestions::GetPhaseCount() const {
	return PHASE_COUNT;
}

void TextSuggestions::DoPhase() {
	StaticPart& sp = script->parts[part_i];
	auto& gen = sp.generated.Get();
	auto& ref = sp.reference.Get();
	auto& txt = sp.text.Get();
	
	if (phase == PHASE_COUNT_SYLLABLES) {
		if (batch == 0) {
			suggestions.Clear();
			
			rhyme.Clear();
			rhyme_i = (line_i % 2) == 0 ? line_i+1 : line_i-1;
			if (rhyme_i < txt.GetCount()) {
				rhyme = txt[rhyme_i].text;
				int a = rhyme.ReverseFind(" ", max(0, rhyme.GetCount() - 5));
				//int a = max(0, max(rhyme.ReverseFind(" ", max(0, rhyme.GetCount() - 5)), rhyme.ReverseFind(",", max(0, rhyme.GetCount() - 5))));
				if (a >= 0)
					rhyme = TrimBoth(rhyme.Mid(a));
			}
			if (line_i < ref.GetCount())
				this->ref = ref[line_i].text;
			if (line_i < gen.GetCount())
				this->line = gen[line_i].text;
			
			TextSolver& ts = TextSolver::Get(appmode, this->line);
			ts.WhenReady << [this,&ts]() {
				gen_syllables = ts.rc.GetSyllableCountPhonetic();
				NextBatch();
				SetWaiting(0);
			};
			SetWaiting(1);
			ts.Start();
		}
		else if (batch == 1) {
			TextSolver& ts = TextSolver::Get(appmode, this->ref);
			ts.WhenReady << [this,&ts]() {
				ref_syllables = ts.rc.GetSyllableCountPhonetic();
				NextPhase();
				SetWaiting(0);
			};
			SetWaiting(1);
			ts.Start();
		}
	}
	else if (phase == PHASE_GET_SUGGESTIONS) {
		Script& song = *this->script;
		
		/*
		 - more/less words
		 - with ref beginning
		 - with original last word
		 - with something rhyming to next
		*/
		if (batch >= 8) {
			NextPhase();
			return;
		}
		
		ScriptSolverArgs args; // 17
		args.fn = 17;
		args.lng_i = song.lng_i;
		args.is_story = song.is_story;
		args.is_unsafe = song.is_unsafe;
		args.is_self_centered = song.is_self_centered;
		args.sub_fn = batch;
		//args.factor = (double)ref_syllables / (double)gen_syllables - 1.0;
		args.factor = ref_syllables - gen_syllables;
		
		args.phrases << this->line;
		args.phrases << this->rhyme;
		
		// Last word of the generated phrase
		{
			int a = this->line.ReverseFind(" ");
			if (a < 0)
				args.phrases << "";
			else
				args.phrases << TrimBoth(this->line.Mid(a));
		}
		
		// First word of the reference phrase (at least 5 chars)
		{
			int a = this->line.Find(" ", 5);
			if (a < 0)
				args.phrases << "";
			else
				args.phrases << TrimBoth(this->line.Left(a));
		}
		
		MakeBelief(song, args, 1);
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptSolver(appmode, args, [this](String res) {
			RemoveEmptyLines3(res);
			
			Vector<String> lines = Split(res, "\n");
			for (String& l : lines)
				suggestions.Add().line = l;
			
			WhenPartiallyReady();
			SetWaiting(0);
			NextBatch();
		});
	}
	else if (phase == PHASE_SUGGESTION_RHYME_CONTAINER) {
		if (batch >= suggestions.GetCount()) {
			NextPhase();
			return;
		}
		
		TextSuggestion& ts = suggestions[batch];
		
		if (skip_ready && ts.rc.Get().GetCount()) {
			NextBatch();
			return;
		}
		
		TextSolver& s = TextSolver::Get(appmode, ts.line);
		SetWaiting(1);
		s.WhenReady = [this,&s]() {
			suggestions[batch].rc = s.rc;
			NextBatch();
			SetWaiting(0);
		};
		s.Start();
	}
	else if (phase == PHASE_TEXT_RHYME_CONTAINER) {
		if (rhyme_i >= txt.GetCount())
			txt.SetCount(rhyme_i+1);
		auto& l = txt[rhyme_i];
		if (skip_ready && l.words.GetCount()) {
			NextPhase();
			return;
		}
		
		TextSolver& s = TextSolver::Get(appmode, l.text);
		SetWaiting(1);
		s.WhenReady = [this,&s,&txt]() {
			txt[rhyme_i] = s.rc.Get()[0];
			NextPhase  ();
			SetWaiting(0);
		};
		s.Start();
	}
	else if (phase == PHASE_RHYMING_SCORE) {
		WString w0;
		if (txt[rhyme_i].words.GetCount()) {
			const auto& rhyme_word = txt[rhyme_i].words.Top();
			w0 = rhyme_word.AsWString();
		}
		else if (rhyme_i < gen.GetCount() && gen[rhyme_i].words.GetCount()) {
			const auto& rhyme_word = gen[rhyme_i].words.Top();
			w0 = rhyme_word.AsWString();
		}
		else if (rhyme_i < ref.GetCount() && ref[rhyme_i].words.GetCount()) {
			const auto& rhyme_word = ref[rhyme_i].words.Top();
			w0 = rhyme_word.AsWString();
		}
		if (w0.IsEmpty()) {
			NextPhase();
			return;
		}
		VectorMap<int,double> sugg_dists;
		for(int i = 0; i < suggestions.GetCount(); i++) {
			TextSuggestion& ts = suggestions[i];
			if (ts.rc.Get().IsEmpty())
				continue;
			double shortest_dist = DBL_MAX;
			for (const auto& cmp_word : ts.rc.Get()[0].words) {
				WString w1 = cmp_word.AsWString();
				double dist = GetSpellingRelativeDistance(w0, w1);
				if (dist < shortest_dist) {
					shortest_dist = dist;
					LOG(w0 << " < " << w1 << ": " << dist);
				}
			}
			ts.rhyming_dist = shortest_dist;
			sugg_dists.Add(i, shortest_dist);
		}
		SortByValue(sugg_dists, StdLess<double>());
		
		for(int i = 0; i < sugg_dists.GetCount(); i++)
			suggestions[sugg_dists.GetKey(i)].rhyming_idx = i;
		
		WhenPartiallyReady();
		NextPhase();
	}
	else if (phase == PHASE_LENGTH_SCORE) {
		VectorMap<int,double> sugg_dists;
		for(int i = 0; i < suggestions.GetCount(); i++) {
			TextSuggestion& ts = suggestions[i];
			if (ts.rc.Get().IsEmpty())
				continue;
			int sugg_syllables = ts.rc.GetSyllableCountPhonetic();
			ts.length_dist = abs(sugg_syllables - ref_syllables);
			sugg_dists.Add(i, ts.length_dist);
		}
		SortByValue(sugg_dists, StdLess<double>());
		
		for(int i = 0; i < sugg_dists.GetCount(); i++)
			suggestions[sugg_dists.GetKey(i)].length_idx = i;
		
		WhenPartiallyReady();
		NextPhase();
	}
	else TODO
	
}

TextSuggestions& TextSuggestions::Get(int appmode, Entity& e, Script& script, int part_i, int line_i) {
	String t;
	t += e.profile->owner->name + ": " + e.profile->name;
	t += ": " + script.GetAnyTitle();
	t += ": " + IntStr(appmode) + ";" + IntStr(part_i) + ";" + IntStr(line_i);
	static ArrayMap<String, TextSuggestions> a;
	int i = a.Find(t);
	if (i >= 0)
		return a[i];
	TextSuggestions& sugg = a.Add(t);
	sugg.appmode = appmode;
	sugg.part_i = part_i;
	sugg.line_i = line_i;
	sugg.script = &script;
	sugg.entity = &e;
	return sugg;
}



END_TEXTLIB_NAMESPACE
