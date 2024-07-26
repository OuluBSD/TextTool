#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptTextProcess::ScriptTextProcess() {
	
}

int ScriptTextProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptTextProcess::GetBatchCount(int phase) const {
	return 1;
}

int ScriptTextProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ScriptTextProcess::DoPhase() {
	if (phase == PHASE_MAKE_SOURCE_POOL) {
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
	else if (phase == PHASE_FILL_LINES) {
		ProcessFillLines();
	}
	else if (phase == PHASE_COMPARISON) {
		ProcessComparison();
	}
	else TODO
}

ScriptTextProcess& ScriptTextProcess::Get(int appmode, Entity& a, Script& l) {
	String t = IntStr(appmode) + " - " + a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, ScriptTextProcess> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptTextProcess& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.script = &l;
	return ls;
}

void ScriptTextProcess::ProcessSourcePool() {
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
		LOG("ScriptTextProcess::ProcessSourcePool: took " << ts.ToString());
		for(int j = 0; j < ContentType::PART_COUNT; j++) {
			LOG("ScriptTextProcess::ProcessSourcePool: in pool #" << j << ": " << sa.source_pool[j].GetCount() << " phrases");
			if (sa.source_pool[j].GetCount() < (phrase_limit*2/3))
				fail = true;
		}
		
		DUMP(filtered_by_typeclass);
		DUMP(filtered_by_content);
		DUMP(filtered_by_attr);
		DUMP(filtered_by_clr);
		
		if (fail) {
			if (tries == max_tries-1) {
				LOG("ScriptTextProcess::ProcessSourcePool: error: not enough phrases found");
				SetNotRunning();
				return;
			}
		}
		else break;
	}
	
	NextPhase();
	
}

void ScriptTextProcess::ProcessTranslate() {
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
		phrase_ints.SetCount(0);
		phrase_ints.SetCount(ContentType::PART_COUNT);
		
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& p = phrase_ints[i];
			
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
	
	ScriptTextProcessArgs args; // new 0
	args.fn = 0;
	args.lng_i = song.lng_i;
	
	per_sub_batch =  20;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrase_ints[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	tmp.Clear();
	for(int i = begin; i < end; i++) {
		int pp_i = phrase_ints[batch].GetKey(i);
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessTranslate));
}

void ScriptTextProcess::OnProcessTranslate(String res) {
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

void ScriptTextProcess::ProcessPairPhrases() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	// Prepare process
	if (batch == 0 && sub_batch == 0) {
		phrase_ints.SetCount(0);
		phrase_ints.SetCount(ContentType::PART_COUNT);
		
		for(int i = 0; i < ContentType::PART_COUNT; i++) {
			auto& p = phrase_ints[i];
			
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
	
	ScriptTextProcessArgs args; // 3
	args.fn = 3;
	
	MakeBelief(song, args, 0);
	
	per_sub_batch =  50;
	int begin = sub_batch * per_sub_batch;
	int end = min(begin + per_sub_batch, phrase_ints[batch].GetCount());
	if (end <= begin) {
		NextBatch();
		return;
	}
	
	bool collect_token_texts = song.lng_i == LNG_NATIVE;
	const auto& translations = da.phrase_translations[song.lng_i];
	for(int i = begin; i < end; i++) {
		int pp_i = phrase_ints[batch].GetKey(i);
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessPairPhrases));
}

void ScriptTextProcess::OnProcessPairPhrases(String res) {
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
				if (k >= this->phrase_ints[batch].GetCount()) {fail = true; break;}
				int pp_i = this->phrase_ints[batch].GetKey(k);
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

void ScriptTextProcess::ProcessRhymes() {
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
	
	ScriptTextProcessArgs args; // 4
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessRhymes));
}

void ScriptTextProcess::OnProcessRhymes(String res) {
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

void ScriptTextProcess::ProcessScores() {
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
	
	ScriptTextProcessArgs args; // 5
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessScores));
}

void ScriptTextProcess::OnProcessScores(String res) {
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

void ScriptTextProcess::ProcessCollect() {
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

void ScriptTextProcess::ProcessFillLines() {
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
	
	
	ScriptTextProcessArgs args; // 10
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
	this->phrase_strs.Clear();
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
				this->phrase_strs << s;
			}
			else {
				const TranslatedPhrasePart& tpp = sa.trans_phrase_combs[song.lng_i][idx][pp_i];
				args.phrases << tpp.phrase;
				this->phrase_strs << tpp.phrase;
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessFillLines));
}

void ScriptTextProcess::OnProcessFillLines(String res) {
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
		if (line_i < 0 || line_i >= this->phrase_strs.GetCount()) {
			a = l.Find(":");
			if (a >= 0) {
				phrase = TrimBoth(l.Mid(a+1));
				break;
			}
			continue;
		}
		phrase = this->phrase_strs[line_i];
		
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
			if (result_line >= 0 && result_line < this->phrase_strs.GetCount()) {
				String phrase = this->phrase_strs[result_line];
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

void ScriptTextProcess::ProcessComparison() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& song = *this->script;
	
	
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, artist->file_title + " - " + song.file_title);
	
	ScriptTextProcessArgs args; // 7
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
	m.GetScriptTextProcess(appmode, args, THISBACK(OnProcessComparison));
}

void ScriptTextProcess::OnProcessComparison(String res) {
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

void ScriptTextProcess::OnProcessComparisonFail(int loser) {
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


END_TEXTLIB_NAMESPACE
