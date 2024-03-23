#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptGenerator::ScriptGenerator() {
	
}

ScriptGenerator& ScriptGenerator::Get(int appmode, Entity& a, Script& l) {
	String t = a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, ScriptGenerator> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptGenerator& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.scripts = &l;
	return ls;
}

void ScriptGenerator::Process() {
	
	EnterAppMode(appmode);
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		
		if (phase == LG_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			NextPhase();
		}
		else if (phase == LG_COLOR) {
			ProcessColor();
		}
		else if (phase == LG_ATTR) {
			ProcessAttr();
		}
		else if (phase == LG_ACTION) {
			ProcessAction();
		}
		else if (phase == LG_MAKE_SOURCE_POOL) {
			ProcessSourcePool();
		}
		else if (phase == LG_MAKE_PHRASE_PAIRS) {
			ProcessPairPhrases();
		}
		else if (phase == LG_MAKE_RHYMES) {
			ProcessRhymes();
		}
		else if (phase == LG_GET_AI_SCORES) {
			ProcessScores();
		}
		
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LG_BEGIN;
			
			// Start ScriptSolver
			ASSERT(artist && scripts);
			ScriptSolver& ls = ScriptSolver::Get(appmode, *artist,*scripts);
			ls.Start();
			
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	LeaveAppMode();
	
	running = false;
	stopped = true;
}

void ScriptGenerator::ProcessSourcePool() {
	TimeStop ts;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	int song_tc = song.typeclass;
	int song_con_base = song.content; // archetype/contrast has name mix-up problem
	//int song_arch = ScanInt(song.data.Get("ATTR_ARCHETYPE", "0"));
	
	Color no_clr(0,0,0);
	
	if (skip_ready && sa.source_pool[0].GetCount()) {
		NextPhase();
		return;
	}
	
	for(int j = 0; j < ContentType::PART_COUNT; j++)
		sa.source_pool[j].Clear();
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		
		// Typeclass
		// - get song's typecast
		// - check that phrase matches to typecast
		{
			bool found = false;
			for (int tc : pp.typecasts)
				if (tc == song_tc)
					{found = true; break;}
			if (!found)
				continue;
		}
		
		// Contrast type
		bool found_contrast[ContentType::PART_COUNT] = {false,false,false};
		{
			bool found = false;
			for (int con : pp.contrasts) {
				int con_base = con / ContentType::PART_COUNT;
				int con_mod = con % ContentType::PART_COUNT;
				if (con_base == song_con_base) {
					found = true;
					found_contrast[con_mod] = true;
				}
			}
			if (!found)
				continue;
		}
		
		// Content
		// - get song's archetype
		// - check that phrases matches to the archetype
		// NOTE skip this since data is not usualle fetched
		/*if (0) {
			bool found = false;
			for (int arch : pp.contents)
				if (arch == song_arch)
					{found = true; break;}
			if (!found)
				continue;
		}*/
		
		// Check attr
		if (pp.attr >= 0) {
			const ExportAttr* ea = &da.attrs[pp.attr];
			if (ea->link >= 0)
				ea = &da.attrs[ea->link];
			if (ea->simple_attr >= 0) {
				bool song_enabled = song.simple_attrs[ea->simple_attr];
				if (!song_enabled)
					continue;
			}
			else continue;
		}
		
		// Check clr
		if (pp.clr != no_clr) {
			int clr_group = GetColorGroup(pp.clr);
			bool part_enabled = VectorFind(song.clr_list, clr_group) >= 0;
			if (!part_enabled)
				continue;
		}
		else continue;
		
		
		for(int j = 0; j < ContentType::PART_COUNT; j++) {
			if (found_contrast[j])
				sa.source_pool[j].FindAdd(i);
		}
	}
	
	LOG("ScriptGenerator::ProcessSourcePool: took " << ts.ToString());
	for(int j = 0; j < ContentType::PART_COUNT; j++) {
		LOG("ScriptGenerator::ProcessSourcePool: in pool #" << j << ": " << sa.source_pool[j].GetCount() << " phrases");
	}
	
	NextPhase();
	
}

void ScriptGenerator::ProcessPairPhrases() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrases.SetCount(0);
		phrases.SetCount(ContentType::PART_COUNT);
		
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& p = phrases[i];
			
			for (int pp_i : sa.source_pool[i].GetKeys()) {
				
				// Get score sum
				const PhrasePart& pp = da.phrase_parts[pp_i];
				double score = 0;
				for(int j = 0; j < SCORE_COUNT; j++)
					score += pp.scores[j];
				
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
	
	ScriptSolverArgs args;
	args.fn = 3;
	
	per_sub_batch =  50;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrases[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	for(int i = begin; i < end; i++) {
		int pp_i = phrases[batch].GetKey(i);
		const PhrasePart& pp = da.phrase_parts[pp_i];
		args.phrases << da.GetWordString(pp.words);
	}
	
	
	int song_tc = song.typeclass;
	bool is_unsafe = song.is_unsafe;
	bool is_female = artist->is_female;
	const Vector<String>& entities = GetTypeclassEntities(appmode, is_unsafe, is_female)[song_tc];
	args.parts <<= entities;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessPairPhrases));
	
}

void ScriptGenerator::OnProcessPairPhrases(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
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
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	if (skip_ready && sa.phrase_parts[batch].GetCount() >= phrase_limit) {
		NextBatch();
		return;
	}
	
	ScriptSolverArgs args;
	args.fn = 4;
	
	per_sub_batch =  15;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, sa.phrase_combs[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	for(int i = begin; i < end; i++) {
		const PhraseComb& pc = sa.phrase_combs[batch][i];
		
		String p;
		for (int pp_i : pc.phrase_parts) {
			const PhrasePart& pp = da.phrase_parts[pp_i];
			if (!p.IsEmpty()) p << ", ";
			p << '\''<< da.GetWordString(pp.words) << '\'';
		}
		args.phrases << p;
	}
	
	
	int song_tc = song.typeclass;
	bool is_unsafe = song.is_unsafe;
	bool is_female = artist->is_female;
	const Vector<String>& entities = GetTypeclassEntities(appmode, is_unsafe, is_female)[song_tc];
	args.parts <<= entities;
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessRhymes));
	
}

void ScriptGenerator::OnProcessRhymes(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
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
		
		if (line.IsEmpty()) continue;
		
		
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
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	if (sub_batch == 0)
		iter = 0;
	
	if (batch >= ContentType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	ScriptSolverArgs args;
	args.fn = 5;
	
	per_sub_batch =  15;
	
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
		args.phrases << phrase;
		pp_is.Add(phrase, pp_i);
		
		if (args.phrases.GetCount() >= per_sub_batch)
			break;
	}
	
	if (args.phrases.IsEmpty()) {
		NextBatch();
		return;
	}
	
	SetWaiting(1);
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(args, THISBACK(OnProcessScores));
	
}

void ScriptGenerator::OnProcessScores(String res) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Script& song = *this->scripts;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	res = "2. \"" + res;
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	int scores[SCORE_COUNT];
	bool use_tmp = lines.GetCount() == pp_is.GetCount();
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
		
		PhrasePart& pp = sa.phrase_parts[batch][pp_i];
		
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
			pp.scores[j] = scores[j];
		}
	}
	
	SetWaiting(0);
	NextSubBatch();
	
}


END_TEXTLIB_NAMESPACE

