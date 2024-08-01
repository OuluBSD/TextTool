#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ArrayMap<hash_t, ScriptGenerator>& __ScriptGenerators() {
	static ArrayMap<hash_t, ScriptGenerator> map;
	return map;
}

ScriptGenerator::ScriptGenerator() {
	
}

ScriptGenerator& ScriptGenerator::Get(int appmode, Entity& a, Script& l) {
	String t = IntStr(appmode) + " - " + a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, ScriptGenerator>& map = __ScriptGenerators();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptGenerator& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.script = &l;
	return ls;
}

int ScriptGenerator::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptGenerator::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_COLOR:	return script->parts.GetCount();
		case PHASE_ATTR:	return (script->simple_attrs.GetCount() + per_attr_batch - 1) / per_attr_batch;
		case PHASE_ACTION:	return 1;
		default: return 1;
	}
}

int ScriptGenerator::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_COLOR:	return 1;
		case PHASE_ATTR:	return 1;
		case PHASE_ACTION:	return 1;
		default: return 1;
	}
}

void ScriptGenerator::DoPhase() {
	EnterAppMode(appmode);
	
	if (phase == PHASE_COLOR) {
		ProcessColor();
	}
	else if (phase == PHASE_ATTR) {
		ProcessAttr();
	}
	else if (phase == PHASE_ACTION) {
		ProcessAction();
	}
	else if (phase == PHASE_MAKE_SOURCE_POOL) {
		ProcessSourcePool();
	}
	else if (phase == PHASE_TRANSLATE) {
		ProcessTranslate();
	}
	else if (phase == PHASE_MAKE_PHRASE_PAIRS) {
		ProcessPairPhrases();
	}
	else if (phase == PHASE_MAKE_RHYMES) {
		ProcessRhymes();
	}
	else if (phase == PHASE_GET_AI_SCORES) {
		ProcessScores();
	}
	else if (phase == PHASE_COLLECT) {
		ProcessCollect();
	}
	
	LeaveAppMode();
}

void ScriptGenerator::ProcessColor() {
	if (batch >= script->parts.GetCount()) {
		NextPhase();
		return;
	}
	
	Script& song = *this->script;
	if (skip_ready && song.clr_list.GetCount()) {
		NextPhase();
		return;
	}
	
	/*StaticPart& sp = script->parts[batch];
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
		args.parts << song.parts[i].GetName(appmode);
	//args.part = sp.name; // active part
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessColor));
	
}

void ScriptGenerator::OnProcessColor(String result) {
	//LOG(result);
	Script& song = *this->script;
	
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
	if (script->native_title.GetCount())
		args.song.Add("title of " + __comp, script->native_title);
	args.song.Add(__entity + "'s content vision", script->content_vision);
	args.song.Add(__typeclass, GetTypeclasses(appmode)[script->typeclass]);
	args.song.Add(__content, GetContents(appmode)[script->content].key);
	
	// Parts
	for(int i = 0; i < script->parts.GetCount(); i++)
		args.parts << script->parts[i].GetName(appmode);
	
	
	int begin = batch * per_attr_batch;
	int end = begin + per_attr_batch;
	end = min(end, da.simple_attrs.GetCount());
	
	if (skip_ready && end < script->simple_attrs.GetCount()) {
		NextBatch();
		return;
	}
	if (skip_ready && end == script->simple_attrs.GetCount()) {
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
	batch_count = da.simple_attrs.GetCount() / per_attr_batch;
	
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
	
	int begin = batch * per_attr_batch;
	int end = begin + per_attr_batch;
	end = min(end, da.simple_attrs.GetCount());
	
	if (end > script->simple_attrs.GetCount())
		script->simple_attrs.SetCount(end, 0);
	
	RemoveEmptyLines3(result);
	Vector<String> lines = Split(result, "\n");
	int c = min(lines.GetCount(), per_attr_batch);
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
		
		if (pos < script->simple_attrs.GetCount())
			script->simple_attrs[pos] = negative;
	}
	
	NextBatch();
	SetWaiting(0);
}

void ScriptGenerator::ProcessAction() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& s = *this->script;
	
	s.actions_enabled.SetCount(0);
	s.actions_enabled.SetCount(da.actions.GetCount(), false);
	
	Color no_clr(0,0,0);
	for(int i = 0; i < da.actions.GetCount(); i++) {
		const ExportAction& ea = da.actions[i];
		bool& enabled = s.actions_enabled[i];
		
		// Filter by attribute
		if (ea.attr >= 0) {
			const ExportAttr* eat = &da.attrs[ea.attr];
			while (eat->link >= 0) {
				eat = &da.attrs[eat->link];
			}
			
			if (eat->simple_attr >= 0) {
				const ExportSimpleAttr& esa = da.simple_attrs[eat->simple_attr];
				if (eat->simple_attr >= script->simple_attrs.GetCount()) {
					script->simple_attrs.SetCount(eat->simple_attr+1,false);
				}
				bool script_is_positive = script->simple_attrs[eat->simple_attr];
				bool attr_is_positive = eat->positive;
				enabled = script_is_positive == attr_is_positive;
				continue;
			}
		}
		// Filter by color
		if (ea.clr != no_clr) {
			int clr_group = GetColorGroup(ea.clr);
			enabled = VectorFind(script->clr_list, clr_group) >= 0;
			continue;
		}
		
		enabled = false;
	}
	
	NextPhase();
}

void ScriptGenerator::ProcessSourcePool() {
	TimeStop ts;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	int song_tc = song.typeclass;
	int song_con_base = song.content; // content/archetype/contrast has name mix-up problem
	//int song_arch = ScanInt(song.data.Get("ATTR_ARCHETYPE", "0"));
	
	int tc_count = ::GetTypeclassCount(appmode);
	ASSERT(song_tc >= 0 && song_tc < tc_count);
	
	Color no_clr(0,0,0);
	
	if (skip_ready && sa.source_pool[0].GetCount()) {
		NextPhase();
		return;
	}
	
	for(int j = 0; j < ContentType::PART_COUNT; j++)
		sa.source_pool[j].Clear();
	
	int max_tries = 0x10;
	
	for (int tries = 0; tries < max_tries; tries++) {
		bool skip_filter_attr = tries & (1 << 0);
		bool skip_filter_content = tries & (1 << 1);
		bool skip_filter_clr = tries & (1 << 2);
		bool skip_filter_typeclass = tries & (1 << 3);
		
		int filtered_by_typeclass = 0;
		int filtered_by_content = 0;
		int filtered_by_attr = 0;
		int filtered_by_clr = 0;
		
		for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
			const PhrasePart& pp = da.phrase_parts[i];
			
			// Filter by typeclass
			// - get text's typeclass
			// - check that phrase matches to typeclass
			if (!skip_filter_typeclass) {
				bool found = false;
				for (int tc : pp.typecasts)
					if (tc == song_tc)
						{found = true; break;}
				if (!found) {
					filtered_by_typeclass++;
					continue;
				}
			}
			
			// Filter by content type
			bool found_contrast[ContentType::PART_COUNT] = {false,false,false};
			if (!skip_filter_content) {
				bool found = false;
				for (int con : pp.contrasts) {
					int con_base = con / ContentType::PART_COUNT;
					int con_mod = con % ContentType::PART_COUNT;
					if (con_base == song_con_base) {
						found = true;
						found_contrast[con_mod] = true;
					}
				}
				if (!found) {
					filtered_by_content++;
					continue;
				}
			}
			
			// Filter by attr
			if (!skip_filter_attr && pp.attr >= 0) {
				const ExportAttr* ea = &da.attrs[pp.attr];
				if (ea->link >= 0)
					ea = &da.attrs[ea->link];
				if (ea->simple_attr >= 0) {
					if (ea->simple_attr >= song.simple_attrs.GetCount()) {
						song.simple_attrs.SetCount(ea->simple_attr+1, false);
					}
					bool song_enabled = song.simple_attrs[ea->simple_attr];
					if (!song_enabled) {
						filtered_by_attr++;
						continue;
					}
				}
				else {
					filtered_by_attr++;
					continue;
				}
			}
			
			// Filter by color
			if (!skip_filter_clr) {
				if (pp.clr != no_clr) {
					int clr_group = GetColorGroup(pp.clr);
					bool part_enabled = VectorFind(song.clr_list, clr_group) >= 0;
					if (!part_enabled) {
						filtered_by_clr++;
						continue;
					}
				}
				else {
					filtered_by_clr++;
					continue;
				}
			}
			
			// Here you have filtered all the phrases, which matches to the params of the text
			// Collect them to the ComponentAnalysis::source_pool
			for(int j = 0; j < ContentType::PART_COUNT; j++) {
				if (found_contrast[j])
					sa.source_pool[j].FindAdd(i);
			}
		}
		
		bool fail = false;
		LOG("ScriptGenerator::ProcessSourcePool: took " << ts.ToString());
		for(int j = 0; j < ContentType::PART_COUNT; j++) {
			LOG("ScriptGenerator::ProcessSourcePool: in pool #" << j << ": " << sa.source_pool[j].GetCount() << " phrases");
			if (sa.source_pool[j].GetCount() < (phrase_limit*2/3))
				fail = true;
		}
		
		DUMP(filtered_by_typeclass);
		DUMP(filtered_by_content);
		DUMP(filtered_by_attr);
		DUMP(filtered_by_clr);
		
		if (fail) {
			if (tries == max_tries-1) {
				LOG("ScriptGenerator::ProcessSourcePool: error: not enough phrases found");
				SetNotRunning();
				return;
			}
		}
		else break;
	}
	
	NextPhase();
	
}

void ScriptGenerator::ProcessTranslate() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	if (song.lng_i == 0) {
		NextPhase();
		return;
	}
	
	auto& translations = da.phrase_translations[song.lng_i];
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrases.SetCount(0);
		phrases.SetCount(ContentType::PART_COUNT);
		
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& p = phrases[i];
			
			int sep_i = 0; // add some separating value to get same sorting every time (equal values breaks sorting)
			for (int pp_i : sa.source_pool[i].GetKeys()) {
				
				// Get score sum
				const PhrasePart& pp = da.phrase_parts[pp_i];
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += pp.scores[j];
				
				// so this actually fixes cache misses
				double separator = sep_i++ * 0.001;
				score += separator;
				
				p.Add(pp_i, score);
			}
			// Sort phrases
			SortByValue(p, StdGreater<double>());
			
			// Clear old phrases
			if (!skip_ready)
				sa.phrase_combs[i].Clear();
		}
	}
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args; // 11
	args.fn = 11;
	args.lng_i = song.lng_i;
	
	per_sub_batch =  20;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrases[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	tmp.Clear();
	for(int i = begin; i < end; i++) {
		int pp_i = phrases[batch].GetKey(i);
		hash_t hash = da.phrase_parts.GetKey(pp_i);
		if (translations.Find(hash) >= 0)
			continue;
		
		const PhrasePart& pp = da.phrase_parts[pp_i];
		args.phrases << da.GetWordString(pp.words);
		tmp << pp_i;
	}
	
	if (args.phrases.IsEmpty()) {
		NextBatch();
		return;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessTranslate));
}

void ScriptGenerator::OnProcessTranslate(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	ASSERT(song.lng_i > 0 && song.lng_i < LNG_COUNT);
	auto& translations = da.phrase_translations[song.lng_i];
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	int begin = sub_batch * per_sub_batch;
	ASSERT(begin >= 0);
	
	res = TrimBoth(res);
	if (res.Left(2) != "1.")
		res = "1. " + res;
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		line = TrimBoth(line);
		if (line.IsEmpty())
			continue;
		
		if (i >= tmp.GetCount())
			break;
		
		int pp_i = tmp[i];
		const PhrasePart& pp = da.phrase_parts[pp_i];
		hash_t hash = da.phrase_parts.GetKey(pp_i);
		
		translations.GetAdd(hash) = line;
	}
	
	SetWaiting(0);
	
	NextSubBatch();
}

void ScriptGenerator::ProcessPairPhrases() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrases.SetCount(0);
		phrases.SetCount(ContentType::PART_COUNT);
		
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& p = phrases[i];
			
			int sep_i = 0; // add some separating value to get same sorting every time (equal values breaks sorting)
			for (int pp_i : sa.source_pool[i].GetKeys()) {
				
				// Get score sum
				const PhrasePart& pp = da.phrase_parts[pp_i];
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += pp.scores[j];
				
				// so this actually fixes cache misses
				double separator = sep_i++ * 0.001;
				score += separator;
				
				p.Add(pp_i, score);
			}
			// Sort phrases
			SortByValue(p, StdGreater<double>());
			
			// Clear old phrases
			if (!skip_ready)
				sa.phrase_combs[i].Clear();
		}
	}
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	if (skip_ready && sa.phrase_combs[batch].GetCount() >= pair_limit) {
		NextBatch();
		return;
	}
	
	ScriptSolverArgs args; // 3
	args.fn = 3;
	
	MakeBelief(song, args, 0);
	
	per_sub_batch =  50;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrases[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	const auto& translations = da.phrase_translations[song.lng_i];
	for(int i = begin; i < end; i++) {
		int pp_i = phrases[batch].GetKey(i);
		if (collect_token_texts) {
			const PhrasePart& pp = da.phrase_parts[pp_i];
			args.phrases << da.GetWordString(pp.words);
		}
		else {
			hash_t hash = da.phrase_parts.GetKey(pp_i);
			int j = translations.Find(hash);
			if (j < 0)
				continue;
			String trans_phrase = translations[j];
			args.phrases << trans_phrase;
		}
	}
	
	
	int song_tc = song.typeclass;
	bool is_unsafe = song.is_unsafe;
	bool is_female = artist->is_female;
	const Vector<String>& entities = GetTypeclassEntities(appmode, is_unsafe, is_female)[song_tc];
	args.parts <<= entities;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessPairPhrases));
}

void ScriptGenerator::OnProcessPairPhrases(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = "3. " + res;
	
	int begin = sub_batch * per_sub_batch;
	ASSERT(begin >= 0);
	MapFile<hash_t,PhraseComb>& p = sa.phrase_combs[batch];
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find("(");
		if (a >= 0) {
			line = TrimBoth(line.Left(a));
		}
		a = line.Find(":");
		if (a >= 0)
			line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() > 0) {
			Vector<int> pp_is;
			CombineHash ch;
			bool fail = false;
			for (String& part : parts) {
				part = TrimBoth(part);
				if (part.IsEmpty() || !IsDigit(part[0]))
					break;
				int j = ScanInt(part) - 1;
				if (j <= 0 || j >= per_sub_batch) {fail = true; break;}
				ASSERT(j >= 0);
				int k = begin + j;
				if (k >= this->phrases[batch].GetCount()) {fail = true; break;}
				int pp_i = this->phrases[batch].GetKey(k);
				ASSERT(pp_i >= 0 && pp_i < 100000000);
				pp_is.Add(pp_i);
				ch.Do(pp_i);
				if (pp_is.GetCount() == 3) break; // limit
			}
			if (!fail && pp_is.GetCount()) {
				PhraseComb& pc = p.GetAdd(ch);
				if (pc.phrase_parts.IsEmpty())
					Swap(pp_is, pc.phrase_parts);
			}
		}
	}
	
	
	SetWaiting(0);
	
	if (p.GetCount() >= pair_limit)
		NextBatch();
	else
		NextSubBatch();
}

void ScriptGenerator::ProcessRhymes() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	if (collect_token_texts) {
		if (skip_ready && sa.phrase_parts[batch].GetCount() >= phrase_limit) {
			NextBatch();
			return;
		}
	}
	else {
		if (skip_ready && sa.trans_phrase_combs[song.lng_i][batch].GetCount() >= phrase_limit) {
			NextBatch();
			return;
		}
	}
	
	ScriptSolverArgs args; // 4
	args.fn = 4;
	args.lng_i = song.lng_i;
	
	MakeBelief(song, args, 0);
	
	per_sub_batch =  15;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, sa.phrase_combs[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	const auto& translations = da.phrase_translations[song.lng_i];
	for(int i = begin; i < end; i++) {
		const PhraseComb& pc = sa.phrase_combs[batch][i];
		
		String p;
		for (int pp_i : pc.phrase_parts) {
			if (collect_token_texts) {
				const PhrasePart& pp = da.phrase_parts[pp_i];
				if (!p.IsEmpty()) p << ", ";
				p << '\''<< da.GetWordString(pp.words) << '\'';
			}
			else {
				hash_t hash = da.phrase_parts.GetKey(pp_i);
				int j = translations.Find(hash);
				if (j < 0)
					continue;
				if (!p.IsEmpty()) p << ", ";
				String trans_phrase = translations[j];
				p << '\''<< trans_phrase << '\'';
			}
		}
		args.phrases << p;
	}
	
	
	int song_tc = song.typeclass;
	bool is_unsafe = song.is_unsafe;
	bool is_female = artist->is_female;
	const Vector<String>& entities = GetTypeclassEntities(appmode, is_unsafe, is_female)[song_tc];
	args.parts <<= entities;
	args.is_story = song.is_story;
	args.is_unsafe = song.is_unsafe;
	args.is_self_centered = song.is_self_centered;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessRhymes));
}

void ScriptGenerator::OnProcessRhymes(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = TrimBoth(res);
	if (res.Left(2) != "3.")
		res = "3. '','': \"" + res;
	
	int begin = sub_batch * per_sub_batch;
	ASSERT(begin >= 0);
	MapFile<hash_t,PhraseComb>& p = sa.phrase_combs[batch];
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	Vector<int> token_is, word_is;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		//LOG(line);
		
		int a = line.Find(":");
		if (a < 0) continue;
		
		line = TrimBoth(line.Mid(a+1));
		RemoveQuotes(line);
		RemoveQuotes(line); // twice
		
		if (line.IsEmpty()) continue;
		
		
		if (collect_token_texts) {
			// Parse text
			NaturalTokenizer tk;
			
			String str = line;
			
			if (!tk.Parse(str)) {
				continue;
			}
			
			//LOG("Lines: " << tk.GetLines().GetCount());
			for (const auto& line : tk.GetLines()) {
				token_is.SetCount(0);
				CombineHash ch;
				for (const WString& line : line) {
					String s = line.ToString();
					int tk_i = -1;
					Token& tk = da.tokens.GetAdd(s, tk_i);
					ch.Do(tk_i);
					token_is << tk_i;
				}
				hash_t h = ch;
				
				int tt_i = -1;
				TokenText& tt = da.token_texts.GetAdd(h, tt_i);
				if (tt.tokens.IsEmpty()) {
					Swap(tt.tokens, token_is);
				}
				
				word_is.SetCount(0);
				for (int tk_i : tt.tokens) {
					const Token& tk = da.tokens[tk_i];
					if (tk.word_ < 0) {
						String key = ToLower(da.tokens.GetKey(tk_i));
						tk.word_ = da.words.FindAdd(key);
					}
					int w_i = tk.word_;
					word_is << w_i;
				}
				
				if (word_is.GetCount() == 0)
					continue;
				
				if (0) {
					LOG("Original: " << str);
					LOG("Token string: " << da.GetTokenTextString(tt));
					LOG("Word string: " << da.GetWordString(word_is));
				}
				
				hash_t w_h;
				{
					CombineHash ch;
					for (int w_i : word_is)
						ch.Do(w_i).Put(1);
					w_h = ch;
				}
				
				PhrasePart& pp = sa.phrase_parts[batch].GetAdd(w_h);
				pp.tt_i = tt_i;
				if (pp.words.IsEmpty())
					Swap(pp.words, word_is);
			}
		}
		// Don't collect texts for other languages than native dataset language
		else {
			ASSERT(song.lng_i > 0);
			hash_t hash = line.GetHashValue();
			int tpp_i = -1;
			TranslatedPhrasePart& tpp = sa.trans_phrase_combs[song.lng_i][batch].GetAdd(hash, tpp_i);
			tpp.phrase = line;
		}
	}
	
	
	SetWaiting(0);
	if (sa.phrase_parts[batch].GetCount() >= phrase_limit)
		NextBatch();
	else
		NextSubBatch();
	
}

void ScriptGenerator::ProcessScores() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	
	if (sub_batch == 0)
		iter = 0;
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args; // 5
	args.fn = 5;
	
	MakeBelief(song, args, 0);
	
	per_sub_batch =  15;
	
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	if (collect_token_texts) {
		const auto& v = sa.phrase_parts[batch];
		pp_is.Clear();
		while(iter < v.GetCount()) {
			int pp_i = iter++;
			const PhrasePart& pp = v[pp_i];
			
			// Check if score is fetched already
			int total_score = 0;
			for(int i = 0; i < SCORE_COUNT; i++)
				total_score += pp.scores[i];
			if (total_score > 0)
				continue;
			
			// Fetch scores for the phrase
			String phrase = da.GetWordString(pp.words);
			phrase = phrase.Left(360);
			args.phrases << phrase;
			pp_is.Add(phrase, pp_i);
			if (args.phrases.GetCount() >= per_sub_batch)
				break;
		}
		
		if (args.phrases.IsEmpty()) {
			NextBatch();
			return;
		}
	}
	else {
		// Use temporary and non-persistent phrases from the previous phase
		// This may cause duplicate queries to the AI
		// TODO optimize: cache
		ASSERT(song.lng_i > 0);
		const auto& v = sa.trans_phrase_combs[song.lng_i][batch];
		pp_is.Clear();
		while(iter < v.GetCount()) {
			int tpp_i = iter++;
			const TranslatedPhrasePart& tpp = v[tpp_i];
			
			// Check if score is fetched already
			int total_score = 0;
			for(int i = 0; i < SCORE_COUNT; i++)
				total_score += tpp.scores[i];
			if (total_score > 0)
				continue;
			
			// Fetch scores for the phrase
			args.phrases << tpp.phrase;
			pp_is.Add(tpp.phrase, tpp_i);
			if (args.phrases.GetCount() >= per_sub_batch)
				break;
		}
		
		if (args.phrases.IsEmpty()) {
			NextBatch();
			return;
		}
	}
	
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessScores));
}

void ScriptGenerator::OnProcessScores(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = "2. \"" + res;
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	int scores[SCORE_COUNT];
	bool use_tmp = lines.GetCount() == pp_is.GetCount();
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		
		int a = line.Find(":");
		if (a < 0) continue;
		
		int pp_i = -1;
		
		if (use_tmp)
			pp_i = pp_is[i];
		else {
			String phrase = line.Left(a);
			RemoveQuotes(phrase);
			pp_i = pp_is.Get(phrase, -1);
			if (pp_i < 0)
				continue;
		}
		
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
		
		if (collect_token_texts) {
			PhrasePart& pp = sa.phrase_parts[batch][pp_i];
			for(int j = 0; j < SCORE_COUNT; j++) {
				pp.scores[j] = scores[j];
			}
		}
		else {
			TranslatedPhrasePart& tpp = sa.trans_phrase_combs[song.lng_i][batch][pp_i];
			for(int j = 0; j < SCORE_COUNT; j++) {
				tpp.scores[j] = scores[j];
			}
		}
	}
	
	SetWaiting(0);
	NextSubBatch();
}

void ScriptGenerator::ProcessCollect() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	VectorMap<int,double> m;
	
	if (collect_token_texts) {
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			m.Clear();
			ASSERT(sa.phrase_parts[i].GetCount());
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
			
			auto& pp = song.phrase_parts[i];
			pp.Clear();
			for(int i = 0; i < max_phrase_parts && i < m.GetCount(); i++)
				pp << m.GetKey(i);
		}
	}
	else {
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			m.Clear();
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
			
			auto& pp = song.phrase_parts[i];
			pp.Clear();
			for(int i = 0; i < max_phrase_parts && i < m.GetCount(); i++)
				pp << m.GetKey(i);
		}
	}
	
	NextPhase();
}

END_TEXTLIB_NAMESPACE

