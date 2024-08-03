#include "TextLib.h"
#include <TextDatabase/TextDatabase.h>


BEGIN_TEXTLIB_NAMESPACE


ArrayMap<hash_t, ScriptSolver>& __ScriptSolvers() {
	static ArrayMap<hash_t, ScriptSolver> map;
	return map;
}


ScriptSolver::ScriptSolver() {
	
}

ScriptSolver& ScriptSolver::Get(int appmode, Entity& a, Script& l) {
	String t = IntStr(appmode) + " - " + a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, ScriptSolver>& map = __ScriptSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptSolver& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.script = &l;
	return ls;
}

int ScriptSolver::GetPhaseCount() const {
	return LS_COUNT;
}

void ScriptSolver::DoPhase() {
	EnterAppMode(appmode);
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	sa = &da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if (phase == LS_FILL_LINES) {
		ProcessFillLines();
	}
	else if (phase == LS_COMPARISON) {
		ProcessComparison();
	}
	#if 0
	else if (phase == LS_MATCH_REFERENCE) {
		ProcessReference();
	}
	else if (phase == LS_SCORE_MATCH) {
		ProcessScoreMatch();
	}
	else if (phase == LS_FILL_REFERENCE_MATCH) {
		ProcessFillReferenceMatch();
	}
	else if (phase == LS_SMOOTH_REFERENCE_MATCH) {
		ProcessSmoothReferenceMatch();
	}
	#endif
	else if (phase == LS_TITLE) {
		ProcessTitle();
	}
	
	LeaveAppMode();
}

void ScriptSolver::ClearScript() {
	for(int i = 0; i < script->parts.GetCount(); i++) {
		DynPart& sp = script->parts[i];
		for(int j = 0; j < sp.sub.GetCount(); j++) {
			DynSub& ds = sp.sub[j];
			for(int k = 0; k < ds.lines.GetCount(); k++) {
				DynLine& dl = ds.lines[k];
				dl.pp_i = -1;
				dl.end_pp_i = -1;
			}
		}
		sp.phrase_parts.Clear();
	}
}

void ScriptSolver::ProcessFillLines() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if (/*!skip_ready &&*/ batch == 0 && sub_batch == 0) {
		added_phrases.Clear();
		sa.script_suggs.Clear();
	}
	
	if (sa.script_suggs.GetCount() >= sugg_limit) {
		NextPhase();
		return;
	}
	
	// Realize suggestion and minimum data
	ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
	{
		sugg.parts.SetCount(song.parts.GetCount());
		for(int i = 0; i < song.parts.GetCount(); i++) {
			auto& line = sugg.parts[i];
			DynPart& sp = song.parts[i];
			line.name = sp.GetName(appmode);
			if (!sp.person.IsEmpty())
				line.name += " by the singer '" + sp.person + "'";
		}
	}
	
	
	ScriptSolverArgs args; // 10
	args.fn = 10;
	args.lng_i = song.lng_i;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	
	MakeBelief(song, args, 1);
	
	// Add existing scripts
	active_part = -1;
	for(int i = 0; i < sugg.parts.GetCount(); i++) {
		auto& l = sugg.parts[i];
		const auto& lines = l.lines;
		DynPart& sp = song.parts[i];
		args.elements.Add(sp.element);
		int len = sp.GetExpectedLineCount();
		if (active_part == -1 && lines.GetCount() < len) {
			active_part = i; // Set active part to get new lines for
		}
		if (lines.IsEmpty()) continue;
		
		String s = sugg.parts[i].name + " (" + sp.element + ")";
		args.song.Add(s) = Join(lines, "\n");
	}
	
	if (active_part == -1) {
		NextBatch();
		return;
	}
	
	
	args.part = sugg.parts[active_part].name;
	DynPart& part = song.parts[active_part];
	
	// Get elements of generated rhyme
	{
		auto& spart = sugg.parts[active_part];
		int cur_len = spart.lines.GetCount();
		int expected_len = part.GetExpectedLineCount();
		args.rhyme_element = part.GetLineElementString(cur_len);
	}
	
	int per_part = 20;
	int min_per_part = 15;
	bool fail = false;
	int begin = batch * per_part;
	int end = begin + per_part;
	this->phrases.Clear();
	int con_type = part.GetContrastIndex();
	
	// Prefer con_type but use all if no phrases for some reason
	for(int i = -1; i < ContentType::PART_COUNT; i++)
	{
		if (i == con_type) continue;
		int idx = i < 0 ? con_type : i;
		
		const auto& map = song.phrase_parts[idx];
		
		// Save offsets for reading
		args.offsets << args.phrases.GetCount();
		
		// Add phrases
		int end0 = min(map.GetCount(), end);
		int count = end0 - begin;
		/*if (count < min_per_part) {
			fail = true;
			continue;
		}*/
		for(int j = begin; j < end0; j++) {
			int pp_i = map[j];
			if (collect_token_texts) {
				const PhrasePart& pp = sa.phrase_parts[idx][pp_i];
				String s = da.GetWordString(pp.words);
				args.phrases << s;
				this->phrases << s;
			}
			else {
				const TranslatedPhrasePart& tpp = sa.trans_phrase_combs[song.lng_i][idx][pp_i];
				args.phrases << tpp.phrase;
				this->phrases << tpp.phrase;
			}
		}
		
		if (args.phrases.GetCount() >= min_per_part)
			break;
	}
	if (args.phrases.IsEmpty())
		fail = true;
	
	if (fail) {
		NextPhase();
		return;
	}
	
	
	args.vision = song.content_vision;
	
	
	 
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessFillLines));
}

void ScriptSolver::OnProcessFillLines(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	#if 0
	DynPart* sp = song.FindPartByName(appmode, active_part);
	ASSERT(sp);
	ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
	res = "line #" + res;
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	String phrase;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		int a = l.Find("#");
		if (a < 0) continue;
		int line_i = ScanInt(TrimLeft(l.Mid(a+1)));
		if (line_i < 0 || line_i >= this->phrases.GetCount()) {
			a = l.Find(":");
			if (a >= 0) {
				phrase = TrimBoth(l.Mid(a+1));
				break;
			}
			continue;
		}
		phrase = this->phrases[line_i];
		
		// Don't add the same phrase multiple
		if (added_phrases.Find(phrase) < 0)
			break;
	}
	added_phrases.FindAdd(phrase);
	
	Vector<String> parts = Split(phrase, "/");
	if (parts.GetCount() == 1)
		parts = Split(phrase, ",");
	int part_c = parts.GetCount();
	if (part_c > 2)
		parts.Remove(0, part_c - 2);
	for (String& part : parts)
		sugg.lines.GetAdd(active_part).Add(TrimBoth(part));
	
	#else
	CombineHash ch;
	bool fail = false;
	
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	
	Vector<int> part_is;
	
	res = TrimBoth(res);
	
	/*if (res.Find("Normal 1") >= 0) {
		LOG(res);
	}*/
	
	int result_line = 0;
	if (res.GetCount() && IsDigit(res[0])) {
		int result_line = ScanInt(res);
		
		ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
		
		DynPart& sp = song.parts[active_part];
		if (result_line >= 0 && result_line < this->phrases.GetCount()) {
			String phrase = this->phrases[result_line];
			sugg.parts[active_part].lines.Add(phrase);
		}
	}
	else if (res.GetCount()) {
		int a = res.Find("):");
		if (a >= 0) {
			res = TrimBoth(res.Mid(a+2));
		}
		
		a = res.Find("\"");
		if (a > 0)
			res = res.Left(a);
		else
			RemoveQuotes(res);
		
		ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
		
		DynPart& sp = song.parts[active_part];
		Vector<String> lines = Split(res, "/");
		if (lines.GetCount() == 1)
			lines = Split(res, "\n");
		#if 0
		if (lines.GetCount() == 1) {
			lines.Clear();
			int mid = res.GetCount() / 2;
			int a = res.Find(",", max(0, mid-5));
			if (a >= 0)
				mid = a+1;
			else {
				a = res.Find(" ", max(0, mid-5));
				if (a >= 0)
					mid = a;
			}
			lines.Add(TrimBoth(res.Left(mid)));
			lines.Add(TrimBoth(res.Mid(mid)));
		}
		else if (lines.GetCount() == 3) {
			int c0 = lines[0].GetCount();
			int c1 = lines[1].GetCount();
			int c2 = lines[2].GetCount();
			double a_bal = fabs( (c0 + c1) / (double)c2 - 1 );
			double b_bal = fabs( (c1 + c2) / (double)c0 - 1 );
			if (a_bal <= b_bal) {
				lines[0] += lines[1];
				lines.Remove(1);
			}
			else {
				lines[1] += lines[2];
				lines.Remove(2);
			}
		}
		#else
		if (lines.GetCount() > 2)
			lines.SetCount(2);
		#endif
		
		for (auto& l : lines) {
			if (l.Left(6) == "line #") {
				int a = l.Find(":");
				if (a >= 0)
					l = TrimBoth(l.Mid(a+1));
			}
		}
		auto& dst = sugg.parts[active_part].lines;
		for (String& line : lines)
			dst.Add(TrimBoth(line));
	}
	#endif
	
	
	SetWaiting(0);
	NextSubBatch();
}


#if 0
void ScriptSolver::ProcessPrimary() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if ((skip_ready && sa.script_suggs.GetCount() >= sugg_limit)) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args; // 6
	args.fn = 6;
	
	int per_part = 15;
	int min_per_part = 5;
	bool fail = false;
	int begin = batch * per_part;
	int end = begin + per_part;
	this->phrases.Clear();
	for(int i = 0; i < ContentType::PART_COUNT; i++) {
		const auto& map = song.phrase_parts[i];
		
		// Save offsets for reading
		args.offsets << args.phrases.GetCount();
		
		// Add phrases
		int end0 = min(map.GetCount(), end);
		int count = end0 - begin;
		if (count < min_per_part) {
			fail = true;
			break;
		}
		for(int j = begin; j < end0; j++) {
			int pp_i = map[j];
			if (collect_token_texts) {
				const PhrasePart& pp = sa.phrase_parts[i][pp_i];
				String s = da.GetWordString(pp.words);
				args.phrases << s;
				this->phrases << s;
			}
			else {
				const TranslatedPhrasePart& tpp = sa.trans_phrase_combs[song.lng_i][i][pp_i];
				args.phrases << tpp.phrase;
				this->phrases << tpp.phrase;
			}
		}
	}
	if (args.phrases.IsEmpty())
		fail = true;
	
	if (fail) {
		NextPhase();
		return;
	}
	
	// Parts
	TODO
	#if 0
	part_sizes.Clear();
	for(int i = 0; i < song.parts.GetCount(); i++) {
		const DynPart& sp = song.parts[i];
		args.parts << sp.name;
		
		int len = 2;
		
		if (sp.name.Find(__Verse) == 0)
			len = song.verse_length;
		
		if (sp.name.Find(__PreChorus) == 0)
			len = song.prechorus_length;
		
		if (sp.name.Find(__Chorus) == 0)
			len = song.chorus_length;
		
		if (sp.name.Find(__Bridge) == 0)
			len = song.bridge_length;
		
		args.counts << len;
		part_sizes.Add(sp.name, len);
	}
	#endif
	
	args.part = song.content_vision;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessPrimary));
}

void ScriptSolver::OnProcessPrimary(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = "- " + res;
	
	CombineHash ch;
	VectorMap<String,Vector<String>> sugg;
	bool fail = false;
	
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	
	Vector<int> part_is;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find(":");
		if (a < 0) continue;
		String part = TrimBoth(line.Left(a));
		ch.Do(part);
		line = line.Mid(a+1);
		a = line.Find(":");
		if (a < 0) continue;
		line = line.Mid(a+1);
		
		
		part_is.SetCount(0);
		Vector<String> parts = Split(line, ",");
		for (String& part : parts) {
			part = TrimBoth(part);
			part_is << (ScanInt(part)-1);
		}
		int len = 0;
		int j = part_sizes.Find(part);
		if (j >= 0)
			len = part_sizes[j];
		else {
			int a = part.ReverseFind(" ");
			if (a >= 0) {
				j = part_sizes.Find(part.Left(a));
				if (j >= 0)
					len = part_sizes[j];
				else a = -1; // fail
			}
			else {
				j = part_sizes.Find(part + " 1");
				if (j >= 0)
					len = part_sizes[j];
			}
			//if (a < 0) // if fail
			//	len = 16; // give up on limiting
		}
		auto& v = sugg.Add(part);
		
		for(int i = 0; i < part_is.GetCount(); i++) {
			int part_i = part_is[i];
			if (part_i < 0 || part_i >= this->phrases.GetCount()) {
				fail = true;
				break;
			}
			Vector<String> phrase_lines = Split(this->phrases[part_i], "/");
			for (String& phrase_line : phrase_lines) {
				if (v.GetCount() >= len) break;
				phrase_line = TrimBoth(phrase_line);
				v.Add(phrase_line);
				ch.Do(phrase_line);
			}
			if (v.GetCount() >= len) break;
		}
	}
	
	//DUMPM(sugg);
	
	hash_t h = ch;
	ScriptSuggestion& ls = sa.script_suggs.GetAdd(h);
	if (ls.lines.IsEmpty())
		Swap(ls.lines, sugg);
	
	SetWaiting(0);
	NextBatch();
}

void ScriptSolver::ProcessMakeHoles() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	if (batch >= sa->script_suggs.GetCount()) {
		NextPhase();
		return;
	}
	
	ScriptSuggestion& sugg = sa->script_suggs[batch];
	
	ScriptSolverArgs args; // 9
	args.fn = 9;
	
	phrase_src.Clear();
	for(int i = 0; i < sugg.lines.GetCount(); i++) {
		const String& part = sugg.lines.GetKey(i);
		const auto& lines = sugg.lines[i];
		for(int j = 0; j < lines.GetCount(); j+=2) {
			String s0 = lines[j];
			String s1 = j+1 < lines.GetCount() ? lines[j+1] : String();
			String rhyme = s0 + " / " + s1;
			args.phrases << rhyme;
			int line_i = i * 1000 + j;
			phrase_src << line_i;
		}
	}
	
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessMakeHoles));
}

void ScriptSolver::OnProcessMakeHoles(String res) {
	LOG("RESULT:");
	LOG(res);
	
	ScriptSuggestion& sugg = sa->script_suggs[batch];
	
	res = "1. 1 to 2:" + res;
	RemoveEmptyLines2(res);
	
	// 1. 1 to 2: 6, 6
	// 2. 2 to 3: 7, 8
	Vector<String> lines = Split(res, "\n");
	
	sugg.transfers.Clear();
	int score_sum[2] = {0,0};
	int score_count = 0;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		line = TrimBoth(line);
		
		if (line.Find("n/a") >= 0)
			continue;
		
		int a = line.Find(":");
		if (a < 0) continue;
		
		String h = line.Left(a);
		String v = TrimBoth(line.Mid(a+1));
		a = h.Find(" to ");
		if (a < 0) continue;
		
		int from = StrInt(h.Left(a));
		int to = StrInt(h.Mid(a+4));
		int src = from-1;
		if (src < 0 || src >= phrase_src.GetCount())
			continue;
		
		Vector<String> parts = Split(v, ",");
		if (parts.GetCount() < 2) continue;
		
		Vector<int> scores;
		scores.SetCount(2);
		for(int j = 0; j < 2; j++) {
			String& p = parts[j];
			p = TrimBoth(p);
			int s = ScanInt(p);
			scores[j] = s;
			score_sum[j] += s;
		}
		score_count++;
		
		int code = phrase_src[src];
		int part_i = code / 1000;
		int line_i = code % 1000;
		
		if (sugg.transfers.GetCount() <= part_i)
			sugg.transfers.SetCount(part_i+1);
		auto& trans = sugg.transfers[part_i];
		if (line_i+1 >= trans.GetCount())
			trans.SetCount(line_i+2);
		trans[line_i  ] <<= scores;
		trans[line_i+1] <<= scores;
	}
	for(int i = 0; i < 2; i++)
		sugg.scores[i] = score_sum[i] * 100 / score_count;
	
	LOG("SCORE: " << sugg.scores[0] << ", " << sugg.scores[1]);
	
	NextBatch();
	SetWaiting(0);
}

#endif

void ScriptSolver::ProcessComparison() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	ScriptSolverArgs args; // 7
	args.fn = 7;
	
	MakeBelief(song, args, 0);
	
	if (batch == 0 && sub_batch == 0) {
		// Clear 'visited' vector, which stores visited suggestion comparisons
		visited.Clear();
		
		// Clear output already in case of errors or break of processing
		song.__text.Clear();
		
		// Find average length of suggestions
		Vector<int> lengths;
		for(int i = 0; i < sa.script_suggs.GetCount(); i++)
			lengths.Add(sa.script_suggs[i].GetText().GetCount());
		Sort(lengths, StdLess<int>());
		int mode = lengths[lengths.GetCount()/2];
		
		
		// Add all proper suggestions to remaining-vector
		matches.Clear();
		remaining.Clear();
		for(int i = 0; i < sa.script_suggs.GetCount(); i++) {
			// If the suggestion is very short, then skip it
			String sugg = sa.script_suggs[i].GetText();
			double diff = fabs((sugg.GetCount() / (double)mode) - 1.0);
			if (diff >= 0.66667)
				continue;
			
			
			remaining.Add(i);
		}
	}
	
	if (remaining.GetCount() <= 1) {
		NextPhase();
		return;
	}
	
	for(int i = 0; i < 2; i++) {
		int sugg_i = remaining[i];
		const ScriptSuggestion& ls = sa.script_suggs[sugg_i];
		String content = ls.GetText();
		
		if (TrimBoth(content).IsEmpty()) {
			OnProcessComparisonFail(i);
			return;
		}
		
		args.phrases << content;
	}
	
	args.part = song.content_vision;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessComparison));
}

void ScriptSolver::OnProcessComparison(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	int loser = 0;
	
	
	res = "entry #1: S0:" + res;
	
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	Vector<double> total_scores;
	for (String& line : lines) {
		int a = line.Find(":");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		Vector<String> parts = Split(line, ",");
		int score_sum = 0, score_count = 0;
		for (String& p : parts) {
			p = TrimBoth(p);
			a = p.Find(":");
			if (a < 0) continue;
			p = p.Mid(a+1);
			int score = ScanInt(TrimLeft(p));
			score_sum += score;
			score_count++;
		}
		double score = (double)score_sum / score_count;
		total_scores << score;
	}
	if (total_scores.GetCount() >= 2) {
		double s0 = total_scores[0];
		double s1 = total_scores[1];
		
		// Give second chance, if score is same
		if (s0 == s1) {
			int sugg0 = remaining[0];
			int sugg1 = remaining[1];
			hash_t hash = sugg0 * 1000 + sugg1; // easy
			if (visited.FindAdd(hash) < 0) {
				// Move first suggestion to last
				remaining.Remove(0);
				remaining.Add(sugg0);
				NextSubBatch(); // just to avoid clearing in the "batch==0&&sub_batch==0" case
				return;
			}
		}
		
		loser = s0 > s1 ? 1:0;
	}
	else
		loser = 0; // error
	
	
	OnProcessComparisonFail(loser);
}

void ScriptSolver::OnProcessComparisonFail(int loser) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	
	int loser_sugg_i = remaining[loser];
	ScriptSuggestion& ls = sa.script_suggs[loser_sugg_i];
	ls.rank = remaining.GetCount()-1; // if remaining one, then rank is 0
	String& output = song.__suggestions.GetAdd(ls.rank);
	output = ls.GetText();
	FixOffensiveWords(output);
	remaining.Remove(loser);
	
	if (remaining.GetCount() == 1) {
		int sugg_i = remaining[0];
		ScriptSuggestion& ls = sa.script_suggs[sugg_i];
		ls.rank = 0;
		
		String content = ls.GetText();
		FixOffensiveWords(content);
		song.__suggestions.GetAdd(ls.rank) = content;
		SortByKey(song.__suggestions, StdLess<int>());
		
		
		LOG("Winner scripts:");
		LOG(content);
		
		song.__text = content;
		
		for(int i = 0; i < ls.parts.GetCount(); i++) {
			DynPart& sp = song.parts[i];
			
			/*
			sp->generated.Get().Clear();
			const auto& from = ls.lines[i];
			for(int j = 0; j < from.GetCount(); j++)
				sp->generated.Get().Add().text = from[j];
			*/
		}
	}
	
	SetWaiting(0);
	NextBatch();
}

#if 0
void ScriptSolver::ProcessReference() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if (batch == 0 && sub_batch == 0) {
		conv_tasks.Clear();
		
		ScriptSuggestion& sugg = sa.script_suggs[0];
		for(int i = 0; i < sugg.lines.GetCount(); i++) {
			String part_name = sugg.lines.GetKey(i);
			DynPart* sp = song.FindPartByName(appmode, part_name);
			ASSERT(sp);
			sp->coarse_text.Clear();
			sp->conv.Clear();
			auto& sugg_lines = sugg.lines[i];
			auto& ref_lines = sp->reference.Get();
			for(int j = 0; j < sugg_lines.GetCount(); j++) {
				if (TrimBoth(sugg_lines[j]).IsEmpty())
					sugg_lines.Remove(j--);
			}
			int sugg_line_count = sugg_lines.GetCount();
			int ref_line_count = ref_lines.GetCount();
			int line_count = min(sugg_line_count, ref_line_count);
			for(int j = 0; j < line_count;) {
				
				// Combine lines to rhyming blocks
				int comb_line_count = min(2, line_count-j);
				if (line_count-j == 3)
					comb_line_count = 3;
				
				ConvTask& task = conv_tasks.Add();
				for(int k = 0; k < comb_line_count; k++) {
					const auto& sugg_line = sugg_lines[j+k];
					const auto& ref_line = ref_lines[j+k];
					task.part = part_name;
					task.from << sugg_line;
					task.ref << ref_line.AsText();
				}
				
				j += comb_line_count;
			}
		}
	}
	
	if (batch >= conv_tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args; // 15
	args.fn = 15;
	
	MakeBelief(song, args, 0);
	
	ConvTask& task = conv_tasks[batch];
	args.phrases <<= task.from;
	args.attrs <<= task.ref;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessReference));
}

void ScriptSolver::OnProcessReference(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	ConvTask& task = conv_tasks[batch];
	DynPart& sp = *song.FindPartByName(appmode, task.part);
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	//DUMPC(lines);
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	int line_n = task.from.GetCount();
	LineScore& ls = sp.conv.Add();
	ls.SetCount(lines.GetCount(), line_n);
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		Vector<String> phrases = Split(line, "/");
		for (String& p : phrases) {
			p = TrimBoth(p);
		}
		
		if (phrases.GetCount() > line_n)
			phrases.SetCount(line_n);
		
		int c = min(line_n, phrases.GetCount());
		for(int j = 0; j < c; j++) {
			ls.Set(i, j, phrases[j]);
		}
	}
	
	SetWaiting(0);
	NextBatch();
}

void ScriptSolver::ProcessScoreMatch() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	if (batch >= song.parts.GetCount()) {
		NextPhase();
		return;
	}
	DynPart& sp = song.parts[batch];
	if (sub_batch >= sp.conv.GetCount()) {
		NextBatch();
		return;
	}
	LineScore& ls = sp.conv[sub_batch];
	
	ScriptSolverArgs args; // 5
	args.fn = 5;
	
	MakeBelief(song, args, 0);
	
	int c = ls.GetCount();
	for(int i = 0; i < c; i++) {
		String s;
		for(int j = 0; j < ls.line_n; j++) {
			if (j) s << " / ";
			s << ls.Get(i, j);
		}
		args.phrases << s;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessScoreMatch));
}

void ScriptSolver::OnProcessScoreMatch(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	if (batch >= song.parts.GetCount()) {
		NextPhase();
		return;
	}
	DynPart& sp = song.parts[batch];
	if (sub_batch >= sp.conv.GetCount()) {
		NextBatch();
		return;
	}
	LineScore& ls = sp.conv[sub_batch];
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = "2. \"" + res;
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	int scores[SCORE_COUNT];
	bool use_tmp = lines.GetCount() == ls.GetCount();
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find(":");
		if (a < 0) continue;
		
		line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() != SCORE_COUNT)
			continue;
		
		bool fail = false;
		
		for(int j = 0; j < parts.GetCount(); j++) {
			String& part = parts[j];
			a = part.Find(":");
			if (a < 0) {
				fail = true;
				break;
			}
			a++;
			part = TrimBoth(part.Mid(a));
			if (part.IsEmpty() || !IsDigit(part[0])) {
				fail = true;
				break;
			}
			int score = ScanInt(part);
			scores[j] = score;
		}
		if (fail)
			continue;
		
		for(int j = 0; j < SCORE_COUNT; j++) {
			ls.SetScore(i, j, scores[j]);
		}
	}
	
	SetWaiting(0);
	NextSubBatch();
}

void ScriptSolver::ProcessFillReferenceMatch() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	if (batch == 0 && sub_batch == 0) {
		added_phrases.Clear();
		for(int i = 0; i < song.parts.GetCount(); i++)
			song.parts[i].coarse_text.Clear();
	}
	
	if (batch >= song.parts.GetCount()) {
		NextPhase();
		return;
	}
	DynPart& sp = song.parts[batch];
	if (sub_batch >= sp.conv.GetCount()) {
		NextBatch();
		return;
	}
	LineScore& ls = sp.conv[sub_batch];
	
	int c = ls.GetCount();
	
	// Calculate score sums
	VectorMap<int,int> score_sums;
	for(int i = 0; i < c; i++) {
		int sum = 0;
		for(int j = 0; j < SCORE_COUNT; j++) {
			int sc = ls.GetScore(i, j);
			sum += sc;
		}
		score_sums.GetAdd(i, sum);
	}
	
	// Sort score sums by greater and trim the worst 50% off
	SortByValue(score_sums, StdGreater<int>());
	int c_2 = max(1, c / 2);
	while (score_sums.GetCount() > c_2) score_sums.Remove(c_2);
	
	ScriptSolverArgs args; // 10
	args.fn = 10;
	args.lng_i = song.lng_i;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	args.part = sp.name;
	args.vision = song.content_vision;
	
	MakeBelief(song, args, 1);
	
	TODO
	#if 0
	for(int i = 0; i < song.active_struct.parts.GetCount(); i++) {
		String part = song.active_struct.parts[i];
		DynPart* sp1 = song.FindPartByName(appmode, part);
		if (!sp1) continue;
		String& dst = args.song.Add(sp1->name);
		for(int j = 0; j < sp1->coarse_text.Get().GetCount(); j++) {
			const auto& line = sp1->coarse_text.Get()[j];
			if (j) dst << "\n";
			dst << line.AsText();
		}
		if (&sp == sp1)
			break;
	}
	
	
	this->phrases.Clear();
	for(int i = 0; i < score_sums.GetCount(); i++) {
		int j = score_sums.GetKey(i);
		String phrase = ls.lines[j];
		//LOG(i << ": " << phrase);
		args.phrases << phrase;
		this->phrases << phrase;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessFillReferenceMatch));
	#endif
}

void ScriptSolver::OnProcessFillReferenceMatch(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	DynPart& sp = song.parts[batch];
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	#if 1
	res = "line #" + res;
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	String phrase;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		int a = l.Find("#");
		if (a < 0) continue;
		int line_i = ScanInt(TrimLeft(l.Mid(a+1)));
		if (line_i < 0 || line_i >= this->phrases.GetCount())
			continue;
		
		phrase = this->phrases[line_i];
		
		// Don't add the same phrase multiple
		if (added_phrases.Find(phrase) < 0)
			break;
	}
	Vector<String> parts = Split(phrase, "/");
	if (parts.GetCount() == 1)
		parts = Split(phrase, ",");
	int part_c = parts.GetCount();
	if (part_c > 2)
		parts.Remove(0, part_c - 2);
	for (String& part : parts)
		sp.coarse_text.Get().Add().text = TrimBoth(part);
	
	#else
	CombineHash ch;
	bool fail = false;
	
	RemoveEmptyLines3(res);
	Vector<String> lines = Split(res, "\n");
	
	Vector<int> part_is;
	
	res = TrimBoth(res);
	int result_line = 0;
	if (res.GetCount() && IsDigit(res[0])) {
		int result_line = ScanInt(res);
		if (result_line >= 0 && result_line < this->phrases.GetCount()) {
			String phrase = this->phrases[result_line];
			sp.coarse_text.Get().Add().text = phrase;
		}
	}
	else if (res.GetCount()) {
		int a = res.Find("\"");
		if (a >= 0)
			res = res.Left(a);
		
		Vector<String> lines = Split(res, "/");
		if (lines.GetCount() == 1)
			lines = Split(res, "\n");
		if (lines.GetCount() == 1) {
			lines.Clear();
			int mid = res.GetCount() / 2;
			int a = res.Find(",", max(0, mid-5));
			if (a >= 0)
				mid = a+1;
			else {
				a = res.Find(" ", max(0, mid-5));
				if (a >= 0)
					mid = a;
			}
			lines.Add(TrimBoth(res.Left(mid)));
			lines.Add(TrimBoth(res.Mid(mid)));
		}
		else if (lines.GetCount() == 3) {
			int c0 = lines[0].GetCount();
			int c1 = lines[1].GetCount();
			int c2 = lines[2].GetCount();
			double a_bal = fabs( (c0 + c1) / (double)c2 - 1 );
			double b_bal = fabs( (c1 + c2) / (double)c0 - 1 );
			if (a_bal <= b_bal) {
				lines[0] += lines[1];
				lines.Remove(1);
			}
			else {
				lines[1] += lines[2];
				lines.Remove(2);
			}
		}
		for (String& line : lines)
			sp.coarse_text.Get().Add().text = line;
	}
	#endif
	
	SetWaiting(0);
	NextSubBatch();
}

void ScriptSolver::ProcessSmoothReferenceMatch() {
	Script& song = *this->script;
	
	
	ScriptSolverArgs args; // 16
	args.fn = 16;
	args.lng_i = song.lng_i;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	args.vision = song.content_vision;
	
	MakeBelief(song, args, 1);
	
	args.part = song.GetTextStructure(1);
	
	TODO
	#if 0
	if (song.active_struct.structured_script_i >= 0) {
		TextDatabase& db = GetDatabase();
		StructuredScript& ss = db.structured_scripts[song.active_struct.structured_script_i];
		args.ref = ss.AsText();
		
		for(auto& part : ss.parts) {
			args.parts.Add(part.name + ": " + IntStr(part.lines.GetCount()) + " lines");
		}
	}
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessSmoothReferenceMatch));
	#endif
}

void ScriptSolver::OnProcessSmoothReferenceMatch(String res) {
	Script& song = *this->script;
	//LOG(res);
	
	res.Replace("\r", "");
	
	Vector<String> parts = Split(res, "\n[");
	//DUMPC(parts);
	
	for (String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		String header = lines[0];
		lines.Remove(0);
		int a = header.Find("[");
		if (a >= 0)
			header = header.Mid(a+1);
		a = header.Find(":");
		String name, singer;
		if (a >= 0) {
			Vector<String> v = Split(header, ":");
			name = TrimBoth(v[0]);
			singer = TrimBoth(v[1]);
		}
		else {
			a = header.Find("]");
			if (a < 0)
				continue; // error
			name = TrimBoth(header.Left(a));
		}
		DynPart* sp = song.FindPartByName(appmode, name);
		if (!sp)
			continue;
		sp->text.Clear();
		for(int j = 0; j < lines.GetCount(); j++) {
			sp->text.Get().Add().text = TrimBoth(lines[j]);
		}
	}
	
	SetWaiting(0);
	NextPhase();
}
#endif

void ScriptSolver::ProcessTitle() {
	Script& song = *this->script;
	ScriptSolverArgs args; // 8
	args.fn = 8;
	
	args.part = song.__text;
	args.lng_i = song.lng_i;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, [this](String res) {
		res = TrimBoth(res);
		RemoveQuotes(res);
		
		TaskMgr& m = TaskMgr::Single();
		//script->english_title = res;
		//script->native_title.Clear();
		
		int lng = MetaDatabase::Single().GetLanguageIndex();
		/*if (lng != LNG_ENGLISH) {
			String code = GetLanguageCode(lng);
			m.Translate("EN-US", script->english_title, code, [this](String res) {
				res = TrimBoth(res);
				script->native_title = res;
				SetWaiting(0);
				NextPhase();
			});
		}
		else*/ {
			//script->native_title = script->english_title;
			script->native_title = res;
			SetWaiting(0);
			NextPhase();
		}
	});
}

void ScriptSolver::GetSuggestions(const DynPart& part, const DynSub& sub, const Vector<const DynLine*>& lines, Event<> WhenPartiallyReady) {
	tmp_part = const_cast<DynPart*>(&part);
	tmp_sub = const_cast<DynSub*>(&sub);
	tmp_lines <<= lines;
	this->WhenPartiallyReady = WhenPartiallyReady;
	
	Script& song = *this->script;
	ScriptSolverArgs args; // 18
	args.fn = 18;
	
	args.lng_i = song.lng_i;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		const DynLine& l = *lines[i];
		if (l.text.IsEmpty())
			break;
		args.phrases << l.text;
		args.phrases2 << l.edit_text;
	}
	
	Index<String> elements;
	if (part.element.GetCount()) elements.FindAdd(part.element);
	if (sub.element0.GetCount()) elements.FindAdd(sub.element0);
	if (sub.element1.GetCount()) elements.FindAdd(sub.element1);
	args.elements <<= elements.GetKeys();
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, [this](String res) {
		Vector<String> lines = Split(res, "\n");
		
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			l = TrimBoth(l);
			if (l.IsEmpty()) {
				lines.Remove(i--);
				continue;
			}
			if (!IsDigit(l[0]) && i > 0) {
				lines[i-1] << " / " << l;
				lines.Remove(i--);
				continue;
			}
		}
		
		for (String& s : lines) {
			if (IsDigit(s[0])) {
				int a = s.Find(".");
				if (a >= 0)
					s = TrimLeft(s.Mid(a+1));
			}
		}
		DUMPC(lines);
		
		for (const DynLine* l : tmp_lines) {
			DynLine& dl = const_cast<DynLine&>(*l);
			dl.suggs.Clear();
		}
		
		for (String& s : lines) {
			Vector<String> parts = Split(s, "/");
			for(int i = 0; i < parts.GetCount() && i < tmp_lines.GetCount(); i++) {
				DynLine& dl = const_cast<DynLine&>(*tmp_lines[i]);
				dl.suggs << TrimBoth(parts[i]);
			}
			for(int i = parts.GetCount(); i < tmp_lines.GetCount(); i++) {
				DynLine& dl = const_cast<DynLine&>(*tmp_lines[i]);
				dl.suggs.Clear();
			}
		}
		
		this->WhenPartiallyReady();
	});
}


END_TEXTLIB_NAMESPACE

