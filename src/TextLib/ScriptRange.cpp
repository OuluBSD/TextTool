#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptRangeProcess::ScriptRangeProcess() {
	
}

int ScriptRangeProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptRangeProcess::GetBatchCount(int phase) const {
	return 1;
}

int ScriptRangeProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ScriptRangeProcess::DoPhase() {
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
	else if (phase == LS_COLLECT) {
		ProcessCollect();
	}
	else TODO
		
	LeaveAppMode();
}

ScriptRangeProcess& ScriptRangeProcess::Get(int appmode, Entity& a, Script& l) {
	String t = IntStr(appmode) + " - " + a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, ScriptRangeProcess> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptRangeProcess& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.script = &l;
	return ls;
}

void ScriptRangeProcess::ProcessColor() {
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
		args.parts << song.parts[i].name;
	//args.part = sp.name; // active part
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessColor));
	
}

void ScriptRangeProcess::OnProcessColor(String result) {
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

void ScriptRangeProcess::ProcessAttr() {
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
		args.parts << script->parts[i].name;
	
	
	per_batch = 50;
	int begin = batch * per_batch;
	int end = begin + per_batch;
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
	batch_count = da.simple_attrs.GetCount() / per_batch;
	
	if (args.attrs.IsEmpty()) {
		NextPhase();
		return;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(appmode, args, THISBACK(OnProcessAttr));
	
}

void ScriptRangeProcess::OnProcessAttr(String result) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	int begin = batch * per_batch;
	int end = begin + per_batch;
	end = min(end, da.simple_attrs.GetCount());
	
	if (end > script->simple_attrs.GetCount())
		script->simple_attrs.SetCount(end, 0);
	
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
		
		if (pos < script->simple_attrs.GetCount())
			script->simple_attrs[pos] = negative;
	}
	
	NextBatch();
	SetWaiting(0);
}

void ScriptRangeProcess::ProcessAction() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	if (batch >= script->parts.GetCount()) {
		NextPhase();
		return;
	}
	
	StaticPart& sp = script->parts[batch];
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
				if (eat->simple_attr >= script->simple_attrs.GetCount()) {
					script->simple_attrs.SetCount(eat->simple_attr+1,false);
				}
				bool song_positive = script->simple_attrs[eat->simple_attr];
				bool attr_positive = eat->positive;
				enabled = song_positive == attr_positive;
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
	
	NextBatch();
}

void ScriptRangeProcess::ProcessCollect() {
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



END_TEXTLIB_NAMESPACE
