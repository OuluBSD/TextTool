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

void ScriptSolver::ClearTasks() {
	for (ScriptSolver& g : __ScriptSolvers().GetValues())
		g.SetNotRunning();
}

void ScriptSolver::RestartTasks() {
	for (ScriptSolver& g : __ScriptSolvers().GetValues())
		g.Start();
}

void ScriptSolver::Process() {
	
	EnterAppMode(appmode);
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	sa = &da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
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
		else if (phase == LS_COLLECT) {
			ProcessCollect();
		}
		else if (phase == LS_FILL_LINES) {
			ProcessFillLines();
		}
		else if (phase == LS_COMPARISON) {
			ProcessComparison();
		}
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
		else if (phase == LS_TITLE) {
			ProcessTitle();
		}
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LS_BEGIN;
			
			
			if (start_post_solver) {
				// Start ScriptPostSolver
				ScriptPostSolver& ls = ScriptPostSolver::Get(*artist->profile, *artist, *script, appmode);
				ls.SetSkipReady(false);
				ls.Start();
			}
			
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	LeaveAppMode();
	
	running = false;
	stopped = true;
}

void ScriptSolver::ClearScript() {
	for(int i = 0; i < script->parts.GetCount(); i++) {
		StaticPart& sp = script->parts[i];
		auto& lines = sp.text.Get();
		for(int j = 0; j < lines.GetCount(); j++) {
			auto& line = lines[j];
			line.pp_i = -1;
			line.end_pp_i = -1;
		}
		sp.phrase_parts.Clear();
	}
	//script->picked_phrase_parts.Clear();
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
	
	ScriptSolverArgs args; // 0
	args.fn = 0;
	
	
	// Entity information
	args.artist.Add("year of birth", IntStr(artist->year_of_birth));
	args.artist.Add("year of beginning of career", IntStr(artist->year_of_career_begin));
	args.artist.Add("biography", artist->biography);
	args.artist.Add(RemUscore(GetAppModeLabel(appmode, AML_FIELD_VIBE_OF_TEXT)), artist->text_style);
	args.artist.Add(RemUscore(GetAppModeLabel(appmode, AML_FIELD_PERSON_VISUALLY)), artist->speaker_visually);
	
	// Snapshot information
	/*args.release.Add("title of release", release->english_title);
	args.release.Add("year of content", IntStr(release->year_of_content));*/
	
	// Song information
	if (song.native_title.GetCount())
		args.song.Add("title of " + __comp, song.native_title);
	args.song.Add(__entity + "'s " + __content + " vision", song.content_vision);
	args.song.Add(__typeclass, GetTypeclasses(appmode)[song.typeclass]);
	args.song.Add(__content, GetContents(appmode)[song.content].key);
	
	// Parts
	for(int i = 0; i < song.parts.GetCount(); i++)
		args.parts << song.parts[i].name;
	//args.part = sp.name; // active part
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessColor));
	
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
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	
	ScriptSolverArgs args; // 1
	args.fn = 1;
	
	
	// Entity information
	args.artist.Add("year of birth", IntStr(artist->year_of_birth));
	args.artist.Add("year of beginning of career", IntStr(artist->year_of_career_begin));
	args.artist.Add("biography", artist->biography);
	args.artist.Add(RemUscore(GetAppModeLabel(appmode, AML_FIELD_TEXT_STYLE)), artist->text_style);
	args.artist.Add(RemUscore(GetAppModeLabel(appmode, AML_FIELD_PERSON_VISUALLY)), artist->speaker_visually);
	
	// Snapshot information
	/*args.release.Add("title of release", release->english_title);
	args.release.Add("year of content", IntStr(release->year_of_content));*/
	
	// Song information
	if (scripts->native_title.GetCount())
		args.song.Add("title of " + __comp, scripts->native_title);
	args.song.Add(__entity + "'s content vision", scripts->content_vision);
	args.song.Add(__typeclass, GetTypeclasses(appmode)[scripts->typeclass]);
	args.song.Add(__content, GetContents(appmode)[scripts->content].key);
	
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
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessAttr));
	
}

void ScriptGenerator::OnProcessAttr(String result) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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
		
		if (pos < scripts->simple_attrs.GetCount())
			scripts->simple_attrs[pos] = negative;
	}
	
	NextBatch();
	SetWaiting(0);
}

void ScriptGenerator::ProcessAction() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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
				if (eat->simple_attr >= scripts->simple_attrs.GetCount()) {
					scripts->simple_attrs.SetCount(eat->simple_attr+1,false);
				}
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

void ScriptSolver::ProcessCollect() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	this->phrase_parts.Clear();
	this->phrase_parts.SetCount(ContentType::PART_COUNT);
	
	if (collect_token_texts) {
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& m = this->phrase_parts[i];
			for(int j = 0; j < sa.phrase_parts[i].GetCount(); j++) {
				const PhrasePart& pp = sa.phrase_parts[i][j];
				
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += pp.scores[j];
				
				// so this actually fixes cache misses
				double separator = j * 0.001;
				score += separator;
				
				m.Add(j, score);
			}
			SortByValue(m, StdGreater<double>());
		}
	}
	else {
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& m = this->phrase_parts[i];
			const auto& v = sa.trans_phrase_combs[song.lng_i][i];
			for(int j = 0; j < v.GetCount(); j++) {
				const TranslatedPhrasePart& tpp = v[j];
				
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += tpp.scores[j];
				
				// so this actually fixes cache misses
				double separator = j * 0.001;
				score += separator;
				
				m.Add(j, score);
			}
			SortByValue(m, StdGreater<double>());
		}
	}
	
	NextPhase();
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
		for(int i = 0; i < song.parts.GetCount(); i++) {
			StaticPart& sp = song.parts[i];
			String name = sp.name;
			if (!sp.singer.IsEmpty())
				name += " by the singer '" + sp.singer + "'";
			if (name.GetCount())
				sugg.lines.GetAdd(name);
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
	active_part.Clear();
	for(int i = 0; i < sugg.lines.GetCount(); i++) {
		String key = sugg.lines.GetKey(i);
		if (key.IsEmpty())
			continue;
		const auto& lines = sugg.lines[i];
		StaticPart* sp = song.FindPartByName(key);
		if (!sp)
			continue;
		int len = sp->GetExpectedLineCount(song);
		if (active_part.IsEmpty() && lines.GetCount() < len) {
			active_part = key; // Set active part to get new lines for
		}
		if (lines.IsEmpty()) continue;
		
		args.song.Add(key) = Join(lines, "\n");
	}
	args.part = active_part;
	
	if (active_part.IsEmpty()) {
		NextBatch();
		return;
	}
	
	
	StaticPart* part = song.FindPartByName(active_part);
	ASSERT(part);
	if (!part) part = &song.parts[0];
	
	int per_part = 20;
	int min_per_part = 15;
	bool fail = false;
	int begin = batch * per_part;
	int end = begin + per_part;
	this->phrases.Clear();
	int con_type = part->GetContrastIndex(song);
	
	// Prefer con_type but use all if no phrases for some reason
	for(int i = -1; i < ContentType::PART_COUNT; i++)
	{
		if (i == con_type) continue;
		int idx = i < 0 ? con_type : i;
		
		const auto& map = this->phrase_parts[idx];
		
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
			int pp_i = map.GetKey(j);
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
	
	#if 1
	StaticPart* sp = song.FindPartByName(active_part);
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
	int result_line = 0;
	if (res.GetCount() && IsDigit(res[0])) {
		int result_line = ScanInt(res);
		
		ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
		
		StaticPart* sp = song.FindPartByName(active_part);
		if (sp) {
			if (result_line >= 0 && result_line < this->phrases.GetCount()) {
				String phrase = this->phrases[result_line];
				sugg.lines.GetAdd(active_part).Add(phrase);
			}
		}
	}
	else if (res.GetCount()) {
		int a = res.Find("\"");
		if (a >= 0)
			res = res.Left(a);
		
		ScriptSuggestion& sugg = sa.script_suggs.GetAdd(batch);
		
		StaticPart* sp = song.FindPartByName(active_part);
		if (sp) {
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
			auto& dst = sugg.lines.GetAdd(active_part);
			for (String& line : lines)
				dst.Add(TrimBoth(line));
		}
	}
	#endif
	
	SetWaiting(0);
	NextSubBatch();
}

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
		const StaticPart& sp = song.parts[i];
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
		
		for(int i = 0; i < ls.lines.GetCount(); i++) {
			String key = ls.lines.GetKey(i);
			StaticPart* sp = song.FindPartByName(key);
			if (!sp)
				continue;
			sp->generated.Get().Clear();
			const auto& from = ls.lines[i];
			for(int j = 0; j < from.GetCount(); j++)
				sp->generated.Get().Add().text = from[j];
		}
	}
	
	SetWaiting(0);
	NextBatch();
}

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
			StaticPart* sp = song.FindPartByName(part_name);
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
	StaticPart& sp = *song.FindPartByName(task.part);
	
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
	StaticPart& sp = song.parts[batch];
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
	StaticPart& sp = song.parts[batch];
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
	StaticPart& sp = song.parts[batch];
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
	
	for(int i = 0; i < song.active_struct.parts.GetCount(); i++) {
		String part = song.active_struct.parts[i];
		StaticPart* sp1 = song.FindPartByName(part);
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
}

void ScriptSolver::OnProcessFillReferenceMatch(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	StaticPart& sp = song.parts[batch];
	
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
		StaticPart* sp = song.FindPartByName(name);
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



END_TEXTLIB_NAMESPACE

