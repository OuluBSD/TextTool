#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptSolver::ScriptSolver() {
	
}

ScriptSolver& ScriptSolver::Get(Entity& a, Script& l) {
	String t = a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, ScriptSolver> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptSolver& ls = map.Add(h);
	ls.artist = &a;
	ls.scripts = &l;
	return ls;
}

void ScriptSolver::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LS_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			NextPhase();
			ClearScript();
		}
		else if (phase == LS_FILTER) {
			ProcessFilter();
		}
		else if (phase == LS_PRIMARY) {
			ProcessPrimary();
		}
		else if (phase == LS_COMPARISON) {
			ProcessComparison();
		}
		/*else if (phase == LS_FINETUNING) {
			ProcessFineTuning();
		}*/
		/*else if (phase == LS_SECONDARY_WORD_CLASS) {
			ProcessSecondaryWordClass();
		}
		else if (phase == LS_SECONDARY_FILTER) {
			ProcessSecondaryFilter();
		}
		else if (phase == LS_SECONDARY) {
			ProcessSecondary();
		}*/
		
		
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LS_BEGIN;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void ScriptSolver::ClearScript() {
	for(int i = 0; i < scripts->parts.GetCount(); i++) {
		StaticPart& sp = scripts->parts[i];
		auto& lines = sp.nana.Get();
		for(int j = 0; j < lines.GetCount(); j++) {
			auto& line = lines[j];
			line.pp_i = -1;
			line.end_pp_i = -1;
		}
		sp.phrase_parts.Clear();
	}
	scripts->picked_phrase_parts.Clear();
}

void ScriptGenerator::ProcessColor() {
	if (batch >= scripts->parts.GetCount()) {
		NextPhase();
		return;
	}
	
	Script& song = *this->scripts;
	if (skip_ready && song.clr_list.GetCount()) {
		NextPhase();
		return;
	}
	
	/*StaticPart& sp = scripts->parts[batch];
	if (sp.part_type == StaticPart::SKIP ||
		sp.name.IsEmpty() ||
		(skip_ready && sp.clr_list.GetCount())) {
		NextBatch();
		return;
	}
	*/
	
	ScriptSolverArgs args;
	args.fn = 0;
	
	
	// Entity information
	args.artist.Add("year of birth", IntStr(artist->year_of_birth));
	args.artist.Add("year of beginning of career", IntStr(artist->year_of_career_begin));
	args.artist.Add("biography", artist->biography);
	args.artist.Add("musical style", artist->musical_style);
	args.artist.Add("vocalist visually", artist->vocalist_visual);
	
	// Snapshot information
	/*args.release.Add("title of release", release->english_title);
	args.release.Add("year of content", IntStr(release->year_of_content));*/
	
	// Song information
	if (song.english_title.GetCount())
		args.song.Add("title of song", song.english_title);
	args.song.Add("artist's content vision", song.content_vision);
	args.song.Add("typecast", GetTypeclasses(appmode)[song.typecast]);
	args.song.Add("archetype", GetContents(appmode)[song.archetype].key);
	
	// Parts
	for(int i = 0; i < song.parts.GetCount(); i++)
		args.parts << song.parts[i].name;
	//args.part = sp.name; // active part
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessColor));
	
}

void ScriptGenerator::OnProcessColor(String result) {
	//LOG(result);
	Script& song = *this->scripts;
	
	result = "- RGB(" + result;
	
	Index<int> no_clr_list;
	RemoveEmptyLines3(result);
	Vector<String> lines = Split(result, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find("RGB(");
		if (a < 0) continue;
		a += 4;
		int b = line.Find(")");
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		int clr_group = GetColorGroup(clr);
		no_clr_list.FindAdd(clr_group);
	}
	
	song.clr_list.Clear();
	int c = GetColorGroupCount();
	for(int i = 0; i < c; i++)
		if (no_clr_list.Find(i) < 0)
			song.clr_list.Add(i);
	
	NextPhase();
	SetWaiting(0);
}

void ScriptGenerator::ProcessAttr() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	ScriptSolverArgs args;
	args.fn = 1;
	
	
	// Entity information
	args.artist.Add("year of birth", IntStr(artist->year_of_birth));
	args.artist.Add("year of beginning of career", IntStr(artist->year_of_career_begin));
	args.artist.Add("biography", artist->biography);
	args.artist.Add("musical style", artist->musical_style);
	args.artist.Add("vocalist visually", artist->vocalist_visual);
	
	// Snapshot information
	/*args.release.Add("title of release", release->english_title);
	args.release.Add("year of content", IntStr(release->year_of_content));*/
	
	// Song information
	if (scripts->english_title.GetCount())
		args.song.Add("title of song", scripts->english_title);
	args.song.Add("artist's content vision", scripts->content_vision);
	args.song.Add("typecast", GetTypeclasses(appmode)[scripts->typecast]);
	args.song.Add("archetype", GetContents(appmode)[scripts->archetype].key);
	
	// Parts
	for(int i = 0; i < scripts->parts.GetCount(); i++)
		args.parts << scripts->parts[i].name;
	
	
	per_batch = 50;
	int begin = batch * per_batch;
	int end = begin + per_batch;
	end = min(end, da.simple_attrs.GetCount());
	
	if (skip_ready && end < scripts->simple_attrs.GetCount()) {
		NextBatch();
		return;
	}
	if (skip_ready && end == scripts->simple_attrs.GetCount()) {
		NextPhase();
		return;
	}
	
	
	for(int i = begin; i < end; i++) {
		const ExportSimpleAttr& t = da.simple_attrs[i];
		String group = da.simple_attrs.GetKey(i);
		String attr_s0 = da.attrs.GetKey(t.attr_i0).value;
		String attr_s1 = da.attrs.GetKey(t.attr_i1).value;
		args.attrs << (group + ": +" + attr_s0 + "/-" + attr_s1);
	}
	batch_count = da.simple_attrs.GetCount() / per_batch;
	
	if (args.attrs.IsEmpty()) {
		NextPhase();
		return;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessAttr));
	
}

void ScriptGenerator::OnProcessAttr(String result) {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int begin = batch * per_batch;
	int end = begin + per_batch;
	end = min(end, da.simple_attrs.GetCount());
	
	if (end > scripts->simple_attrs.GetCount())
		scripts->simple_attrs.SetCount(end, 0);
	
	RemoveEmptyLines3(result);
	Vector<String> lines = Split(result, "\n");
	int c = min(lines.GetCount(), per_batch);
	for(int i = 0; i < c; i++) {
		String& line = lines[i];
		int pos = begin + i;
		
		int p = line.Find("+");
		int n = line.Find("-");
		int s = line.Find("/");
		
		bool positive = true;
		if (p >= 0 && n >= 0)
			positive = p < n;
		else if (s >= 0 && p >= 0)
			positive = p < s;
		else if (s >= 0 && n >= 0)
			positive = s < n;
		else if (n >= 0)
			positive = false;
		bool negative = !positive;
		
		scripts->simple_attrs[pos] = negative;
	}
	
	NextBatch();
	SetWaiting(0);
}

void ScriptGenerator::ProcessAction() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (batch >= scripts->parts.GetCount()) {
		NextPhase();
		return;
	}
	
	StaticPart& sp = scripts->parts[batch];
	if ((skip_ready && sp.actions_enabled.GetCount() == da.actions.GetCount()) ||
		sp.part_type == StaticPart::SKIP) {
		NextBatch();
		return;
	}
	
	sp.actions_enabled.SetCount(da.actions.GetCount(), false);
	
	Color no_clr(0,0,0);
	for(int i = 0; i < da.actions.GetCount(); i++) {
		const ExportAction& ea = da.actions[i];
		bool& enabled = sp.actions_enabled[i];
		
		// Filter by attribute
		if (ea.attr >= 0) {
			const ExportAttr* eat = &da.attrs[ea.attr];
			while (eat->link >= 0) {
				eat = &da.attrs[eat->link];
			}
			
			if (eat->simple_attr >= 0) {
				const ExportSimpleAttr& esa = da.simple_attrs[eat->simple_attr];
				bool song_positive = scripts->simple_attrs[eat->simple_attr];
				bool attr_positive = eat->positive;
				enabled = song_positive == attr_positive;
				continue;
			}
		}
		// Filter by color
		if (ea.clr != no_clr) {
			int clr_group = GetColorGroup(ea.clr);
			enabled = VectorFind(scripts->clr_list, clr_group) >= 0;
			continue;
		}
		
		enabled = false;
	}
	
	NextBatch();
}

void ScriptSolver::ProcessFilter() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(artist->native_name + " - " + song.native_title);
	
	this->phrase_parts.Clear();
	this->phrase_parts.SetCount(ContentType::PART_COUNT);
	for(int i = 0; i < ContentType::PART_COUNT; i++) {
		auto& m = this->phrase_parts[i];
		for(int j = 0; j < sa.phrase_parts[i].GetCount(); j++) {
			const PhrasePart& pp = sa.phrase_parts[i][j];
			
			double score = 0;
			for(int j = 0; j < SCORE_COUNT; j++)
				score += pp.scores[j];
			m.Add(j, score);
		}
		SortByValue(m, StdGreater<double>());
	}
	
	NextPhase();
}

void ScriptSolver::ProcessPrimary() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(artist->native_name + " - " + song.native_title);
	
	if ((skip_ready && sa.scripts_suggs.GetCount() >= sugg_limit)) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args;
	args.fn = 6;
	
	int per_part = 15;
	int min_per_part = 5;
	bool fail = false;
	int begin = batch * per_part;
	int end = begin + per_part;
	this->phrases.Clear();
	for(int i = 0; i < ContentType::PART_COUNT; i++) {
		const auto& map = this->phrase_parts[i];
		
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
			int pp_i = map.GetKey(j);
			const PhrasePart& pp = sa.phrase_parts[i][pp_i];
			String s = da.GetWordString(pp.words);
			args.phrases << s;
			this->phrases << s;
		}
	}
	if (args.phrases.IsEmpty())
		fail = true;
	
	if (fail) {
		NextPhase();
		return;
	}
	
	// Parts
	part_sizes.Clear();
	for(int i = 0; i < song.parts.GetCount(); i++) {
		const StaticPart& sp = song.parts[i];
		args.parts << sp.name;
		
		int len = 2;
		
		if (sp.name.Find("Verse") == 0)
			len = song.verse_length;
		
		if (sp.name.Find("Prechorus") == 0)
			len = song.prechorus_length;
		
		if (sp.name.Find("Chorus") == 0)
			len = song.chorus_length;
		
		if (sp.name.Find("Bridge") == 0)
			len = song.bridge_length;
		
		args.counts << len;
		part_sizes.Add(sp.name, len);
	}
	
	args.part = song.content_vision;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessPrimary));
}

void ScriptSolver::OnProcessPrimary(String res) {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(artist->native_name + " - " + song.native_title);
	
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
	ScriptSuggestions& ls = sa.scripts_suggs.GetAdd(h);
	if (ls.lines.IsEmpty())
		Swap(ls.lines, sugg);
	
	SetWaiting(0);
	NextBatch();
}

void ScriptSolver::ProcessComparison() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(artist->native_name + " - " + song.native_title);
	
	ScriptSolverArgs args;
	args.fn = 7;
	
	if (batch == 0 && sub_batch == 0) {
		// Do matches until 1 remaining
		matches.Clear();
		remaining.Clear();
		for(int i = 0; i < sa.scripts_suggs.GetCount(); i++) {
			remaining.Add(i);
		}
	}
	
	if (remaining.GetCount() < 2) {
		NextPhase();
		return;
	}
	
	for(int i = 0; i < 2; i++) {
		int sugg_i = remaining[i];
		const ScriptSuggestions& ls = sa.scripts_suggs[sugg_i];
		String content;
		for(int j = 0; j < ls.lines.GetCount(); j++) {
			String part = ls.lines.GetKey(j);
			const auto& v = ls.lines[j];
			if (part.IsEmpty() || v.IsEmpty()) continue;
			content << part << ":\n";
			
			for(int k = 0; k < v.GetCount(); k++) {
				content << v[k] << "\n";
			}
			content << "\n";
		}
		args.phrases << content;
	}
	
	args.part = song.content_vision;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessComparison));
}

void ScriptSolver::OnProcessComparison(String res) {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(artist->native_name + " - " + song.native_title);
	
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
	if (total_scores.GetCount() >= 2)
		loser = total_scores[0] > total_scores[1] ? 1:0;
	else
		loser = 0; // error
	
	
	int loser_sugg_i = remaining[loser];
	ScriptSuggestions& ls = sa.scripts_suggs[loser_sugg_i];
	ls.rank = remaining.GetCount()-1;
	String& output = song.suggestions.GetAdd(ls.rank);
	output = ls.GetText();
	FixOffensiveWords(output);
	remaining.Remove(loser);
	
	if (remaining.GetCount() == 1) {
		int sugg_i = remaining[0];
		ScriptSuggestions& ls = sa.scripts_suggs[sugg_i];
		ls.rank = 0;
		String& content = song.suggestions.GetAdd(ls.rank);
		content = ls.GetText();
		FixOffensiveWords(content);
		
		LOG("Winner scripts:");
		LOG(content);
		
		song.text = content;
	}
	SortByKey(song.suggestions, StdLess<int>());
	
	SetWaiting(0);
	NextBatch();
}




END_TEXTLIB_NAMESPACE

