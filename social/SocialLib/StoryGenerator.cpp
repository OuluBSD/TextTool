#include "SocialLib.h"


BEGIN_SOCIALLIB_NAMESPACE


StoryGenerator::StoryGenerator() {
	
}

StoryGenerator& StoryGenerator::Get(Company& a, Story& l) {
	String t = a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, StoryGenerator> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	StoryGenerator& ls = map.Add(h);
	ls.company = &a;
	ls.story = &l;
	return ls;
}

void StoryGenerator::RealizePipe() {
	SocialDatabase& db = SocialDatabase::Single();
	if (!pipe) {
		TaskManager::Single().RealizePipe();
		pipe = TaskManager::Single().GetPipe();
		ASSERT(pipe);
	}
}

void StoryGenerator::Process() {
	
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
			
			// Start StorySolver
			ASSERT(company && story);
			StorySolver& ls = StorySolver::Get(*company,*story);
			ls.Start();
			
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void StoryGenerator::ProcessSourcePool() {
	TimeStop ts;
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
	int program_tc = program.role;
	int program_con_base = program.generic; // generic/contrast has name mix-up problem
	//int program_arch = ScanInt(program.data.Get("ATTR_ARCHETYPE", "0"));
	
	Color no_clr(0,0,0);
	
	if (skip_ready && sa.source_pool[0].GetCount()) {
		NextPhase();
		return;
	}
	
	for(int j = 0; j < ContrastType::PART_COUNT; j++)
		sa.source_pool[j].Clear();
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		
		// Role
		// - get program's role
		// - check that phrase matches to role
		{
			bool found = false;
			for (int tc : pp.roles)
				if (tc == program_tc)
					{found = true; break;}
			if (!found)
				continue;
		}
		
		// Contrast type
		bool found_contrast[ContrastType::PART_COUNT] = {false,false,false};
		{
			bool found = false;
			for (int con : pp.generics) {
				int con_base = con / ContrastType::PART_COUNT;
				int con_mod = con % ContrastType::PART_COUNT;
				if (con_base == program_con_base) {
					found = true;
					found_contrast[con_mod] = true;
				}
			}
			if (!found)
				continue;
		}
		
		// Generic
		// - get program's generic
		// - check that phrases matches to the generic
		// NOTE skip this since data is not usualle fetched
		/*if (0) {
			bool found = false;
			for (int arch : pp.generics)
				if (arch == program_arch)
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
				bool program_enabled = program.simple_attrs[ea->simple_attr];
				if (!program_enabled)
					continue;
			}
			else continue;
		}
		
		// Check clr
		if (pp.clr != no_clr) {
			int clr_group = GetColorGroup(pp.clr);
			bool part_enabled = VectorFind(program.clr_list, clr_group) >= 0;
			if (!part_enabled)
				continue;
		}
		else continue;
		
		
		for(int j = 0; j < ContrastType::PART_COUNT; j++) {
			if (found_contrast[j])
				sa.source_pool[j].FindAdd(i);
		}
	}
	
	LOG("StoryGenerator::ProcessSourcePool: took " << ts.ToString());
	for(int j = 0; j < ContrastType::PART_COUNT; j++) {
		LOG("StoryGenerator::ProcessSourcePool: in pool #" << j << ": " << sa.source_pool[j].GetCount() << " phrases");
	}
	
	NextPhase();
	
}

void StoryGenerator::ProcessPairPhrases() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrases.SetCount(0);
		phrases.SetCount(ContrastType::PART_COUNT);
		
		for(int i = 0; i < ContrastType::PART_COUNT; i++) {
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
	
	if (batch >= ContrastType::PART_COUNT) {
		NextPhase();
		return;
	}
	if (skip_ready && sa.phrase_combs[batch].GetCount() >= pair_limit) {
		NextBatch();
		return;
	}
	
	StorySolverArgs args;
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
	
	
	int program_tc = program.role;
	bool is_unsafe = program.is_unsafe;
	bool is_female = company->is_female;
	const Vector<String>& companies = GetRoleCompanys(is_unsafe, is_female)[program_tc];
	args.parts <<= companies;
	
	SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetStorySolver(args, THISBACK(OnProcessPairPhrases));
	
}

void StoryGenerator::OnProcessPairPhrases(String res) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
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

void StoryGenerator::ProcessRhymes() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
	
	if (batch >= ContrastType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	if (skip_ready && sa.phrase_parts[batch].GetCount() >= phrase_limit) {
		NextBatch();
		return;
	}
	
	StorySolverArgs args;
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
	
	
	int program_tc = program.role;
	bool is_unsafe = program.is_unsafe;
	bool is_female = company->is_female;
	const Vector<String>& companies = GetRoleCompanys(is_unsafe, is_female)[program_tc];
	args.parts <<= companies;
	
	SetWaiting(1);
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetStorySolver(args, THISBACK(OnProcessRhymes));
	
}

void StoryGenerator::OnProcessRhymes(String res) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
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

void StoryGenerator::ProcessScores() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
	if (sub_batch == 0)
		iter = 0;
	
	if (batch >= ContrastType::PART_COUNT) {
		NextPhase();
		return;
	}
	
	StorySolverArgs args;
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
	RealizePipe();
	TaskMgr& m = *pipe;
	m.GetStorySolver(args, THISBACK(OnProcessScores));
	
}

void StoryGenerator::OnProcessScores(String res) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Story& program = *this->story;
	
	
	ProgramAnalysis& sa = da.GetProgramAnalysis(company->native_name + " - " + program.native_title);
	
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


END_SOCIALLIB_NAMESPACE
