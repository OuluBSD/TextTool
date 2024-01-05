#include "SongLib.h"

namespace SongLib {

TaskManager::TaskManager() {
	
}

void TaskManager::Clear() {
	bool was_running = running;
	if (was_running) Stop();
	lock.EnterWrite();
	task_list.Clear();
	lock.LeaveWrite();
	if (was_running) Start();
}

void TaskManager::DoTokens(int ds_i, int fn) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_TOKENS;
	t.cb = THISBACK1(GetTokenData, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAmbiguousWordPairs(int ds_i, int fn) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_AMBIGUOUS_WORD_PAIRS;
	t.cb = THISBACK1(GetAmbiguousWordPairs, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoVirtualPhrases(int ds_i, int fn) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_VIRTUAL_PHRASES;
	t.cb = THISBACK1(GetVirtualPhrases, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionlistCache(int ds_i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Task task;
	task.ds_i = ds_i;
	
	String dir = ConfigFile("share") + DIR_SEPS + "task_actionlist" + DIR_SEPS +
		sda.datasets.GetKey(ds_i);
	if (!DirectoryExists(dir))
		return;
	
	FindFile ff(dir + DIR_SEPS + "*.txt");
	do {
		String title = GetFileTitle(ff.GetName());
		if (title == "." ||title == "..") continue;
		String content = LoadFile(ff.GetPath());
		if (content.Find("RGB(") >= 0)
			OnActionlistColors(content, &task);
		else
			OnActionlistAttrs(content, &task);
	}
	while (ff.Next());
}

void TaskManager::DoActionlist(int ds_i, int fn) {
	TODO
	#if 0
	if (IsInTaskList(TASK_ACTIONLIST))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (uniq_acts.IsEmpty()) {
		for (const ActionPhrase& ap : da.action_phrases) {
			for (const auto& a : ap.actions)
				uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
		}
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTIONLIST;
	t.cb = THISBACK1(GetActionlist, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
	#endif
}

void TaskManager::DoActionParallel(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_PARALLELS))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_PARALLELS;
	t.cb = THISBACK1(GetActionParallels, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionTransition(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_TRANSITIONS))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_TRANSITIONS;
	t.cb = THISBACK1(GetActionTransitions, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

bool TaskManager::IsInTaskList(TaskType type) const {
	for (const Task& t : task_list)
		if (t.type == type)
			return true;
	return false;
}

void TaskManager::Start() {
	if (running) return;
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void TaskManager::Stop() {
	running = false;
	while (!stopped) Sleep(10);
}

void TaskManager::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		
		if (task_list.IsEmpty()) {
			Sleep(10);
			continue;
		}
		
		lock.EnterRead();
		Task* tp = 0;
		for (Task& t : task_list) {
			if (!t.running) {
				tp = &t;
				break;
			}
		}
		lock.LeaveRead();
		
		if (!tp) {
			Sleep(10);
			continue;
		}
		
		tp->running = true;
		tp->cb();
	}
	
	running = false;
	stopped = true;
}

void TaskManager::RemoveTask(Task& t) {
	lock.EnterWrite();
	int pos = -1;
	for(int i = 0; i < task_list.GetCount(); i++) {
		if (&task_list[i] == &t) {
			pos = i;
			break;
		}
	}
	ASSERT(pos >= 0);
	if (pos >= 0)
		task_list.Remove(pos);
	lock.LeaveWrite();
}

void TaskManager::RealizePipe() {
	Database& db = Database::Single();
	if (!pipe) {
		pipe = &db.pipes.Add();
		pipe->song = 0;
	}
}

void TaskManager::GetTokenData(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TokenArgs& args = token_args;
	args.fn = 0;
	args.words.Clear();
	
	int per_action_task = 100;
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.tokens.GetCount());
	int count = end - begin;
	if (count <= 0) {
		RemoveTask(*t);
		return;
	}
	
	for(int i = begin; i < end; i++) {
		const String& tk = da.tokens.GetKey(i);
		args.words << tk;
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 0)
		m.GetTokenData(args, THISBACK1(OnTokenData, t));
	
}

void TaskManager::OnTokenData(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. suppote: verb | noun
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	int offset = 3+1;
	
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		/*int line_i = ScanInt(line);
		line_i -= offset;
		if (line_i < 0 || line_i >= args.words.GetCount())
			continue;
		
		const String& orig_word = args.words[line_i];*/
		
		int a = line.Find(".");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		
		a = line.Find(":");
		if (a == 0) {
			// Rare case of ":" being asked
			line = ":" + line;
			a = 1;
		}
		else if (a < 0)
			continue;
		
		//int orig_word_i = ;
		
		String result_word = TrimBoth(line.Left(a));
		/*ExportWord& wrd =
			orig_word_i >= 0 ?
				da.words[orig_word_i] :
				da.words.GetAdd(result_word, orig_word_i);*/
		int orig_word_i = -1;
		ExportWord& wrd = da.words.GetAdd(result_word, orig_word_i);
		
		//TODO // token to word
		
		line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, "|");
		for (String& p : parts) {
			p = TrimBoth(p);
			int wc_i = da.word_classes.FindAdd(p);
			if (wrd.class_count < wrd.MAX_CLASS_COUNT)
				FixedIndexFindAdd(wrd.classes, wrd.MAX_CLASS_COUNT, wrd.class_count, wc_i);
		}
		
	}
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::GetAmbiguousWordPairs(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TokenArgs& args = token_args;
	args.fn = 1;
	args.words.Clear();
	
	int per_action_task = 100;
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.ambiguous_word_pairs.GetCount());
	int count = end - begin;
	if (count <= 0) {
		RemoveTask(*t);
		return;
	}
	
	for(int i = begin; i < end; i++) {
		const auto& wp = da.ambiguous_word_pairs[i];
		if (wp.from >= 0 && wp.to >= 0) {
			const String& from = da.words.GetKey(wp.from);
			const String& to = da.words.GetKey(wp.to);
			args.words << (from + " " + to);
		}
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 1)
		m.GetTokenData(args, THISBACK1(OnAmbiguousWordPairs, t));
	
}

void TaskManager::OnAmbiguousWordPairs(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. is something : verb, noun
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	int offset = 1+1;
	
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		
		a = line.ReverseFind(":");
		if (a < 0)
			continue;
		
		Vector<String> result_words = Split(TrimBoth(line.Left(a)), " ");
		if (result_words.GetCount() != 2)
			continue;
		
		int w_i0 = da.words.Find(result_words[0]);
		int w_i1 = da.words.Find(result_words[1]);
		CombineHash ch;
		ch.Do(w_i0).Put(1).Do(w_i1);
		hash_t h = ch;
		
		//ExportWord& wrd0 = da.words[w_i0];
		//ExportWord& wrd1 = da.words[w_i1];
		WordPairType& wp = da.ambiguous_word_pairs.GetAdd(h);
		
		line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() != 2)
			continue;
		int wc_i_list[2];
		for(int i = 0; i < parts.GetCount(); i++) {
			String& p = parts[i];
			p = TrimBoth(p);
			wc_i_list[i] = da.word_classes.FindAdd(p);
		}
		
		wp.from_type = wc_i_list[0];
		wp.to_type = wc_i_list[1];
	}
	
	t->running = false;
	t->batch_i++;
}

bool GetTypePhrase(Vector<int>& types, const DatasetAnalysis& da, int next_w_i, int w_i, int prev_w_i) {
	if (w_i < 0) {
		return false;
	}
	else {
		const ExportWord& ew = da.words[w_i];
		if (!ew.class_count)
			return false;
		
		int class_i = -1;
		if (ew.class_count > 1) {
			bool found = false;
			
			if (w_i >= 0 && prev_w_i >= 0) {
				CombineHash ch;
				ch.Do(prev_w_i).Put(1).Do(w_i);
				hash_t h = ch;
				int i = da.ambiguous_word_pairs.Find(h);
				if (i >= 0) {
					const WordPairType& wp0 = da.ambiguous_word_pairs[i];
					if (wp0.to_type >= 0) {
						class_i = wp0.to_type;
						found = true;
					}
				}
			}
			if (!found && w_i >= 0 && next_w_i >= 0) {
				CombineHash ch;
				ch.Do(w_i).Put(1).Do(next_w_i);
				hash_t h = ch;
				int i = da.ambiguous_word_pairs.Find(h);
				if (i >= 0) {
					const WordPairType& wp0 = da.ambiguous_word_pairs[i];
					if (wp0.from_type >= 0) {
						class_i = wp0.from_type;
						found = true;
					}
				}
			}
			
			if (!found)
				return false;
		}
		else {
			class_i = ew.classes[0];
		}
		
		String wc = da.word_classes[class_i];
		if (wc.Find("contraction") == 0 && wc.Find("(") >= 0) {
			int a = wc.Find("(");
			if (a < 0)
				return false;
			a++;
			int b = wc.Find(")", a);
			if (b < 0)
				return false;
			String arg = wc.Mid(a,b-a);
			
			a = arg.Find("/");
			if (a >= 0)
				arg = TrimBoth(arg.Left(a));
			
			a = arg.Find(";");
			if (a >= 0)
				arg = TrimBoth(arg.Left(a));
			
			const char* split_str = " ";
			arg.Replace("+", " ");
			Vector<String> words = Split(arg, " ");
			
			int prev_w_j = prev_w_i;
			Vector<int> w_js;
			for (String& w : words) {
				w = TrimBoth(w);
				int w_j = da.words.Find(w);
				if (w_j < 0)
					return false;
				w_js << w_j;
			}
			int c = w_js.GetCount();
			for(int j = 0; j < c; j++) {
				int w_j = w_js[j];
				int next_w_j = j < c-1 ? w_js[j+1] : -1;
				bool succ = GetTypePhrase(types, da, next_w_j, w_j, prev_w_j);
				if (!succ)
					return false;
				prev_w_j = w_j;
			}
		}
		else {
			if (class_i < 0)
				return false;
			
			types << class_i;
		}
		
	}
	return true;
}

void TaskManager::GetVirtualPhrases(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	if (t->fn == 0) {
		Vector<int> word_is, type_is;
		for(int i = 0; i < da.token_texts.GetCount(); i++) {
			TokenText& txt = da.token_texts[i];
			
			bool succ = true;
			word_is.SetCount(0);
			type_is.SetCount(0);
			for(int tk_i : txt.tokens) {
				const Token& tk = da.tokens[tk_i];
				int w_i = tk.word_;
				if (w_i < 0) {
					String key = ToLower(da.tokens.GetKey(tk_i));
					w_i = da.words.Find(key);
					tk.word_ = w_i;
				}
				word_is << w_i;
			}
			
			int prev_w_i = -1;
			for(int j = 0; j < word_is.GetCount(); j++) {
				int w_i = word_is[j];
				int next_w_i = j+1 < word_is.GetCount() ? word_is[j] : -1;
				succ = succ && GetTypePhrase(type_is, da, next_w_i, w_i, prev_w_i);
				prev_w_i = w_i;
			}
			
			if (type_is.IsEmpty())
				succ = false;
			
			if (succ) {
				CombineHash ch;
				for (int type_i : type_is)
					ch.Do(type_i);
				hash_t h = ch;
				
				int vp_i = -1;
				VirtualPhrase& vp = da.virtual_phrases.GetAdd(h, vp_i);
				Swap(type_is, vp.types);
				
				txt.virtual_phrase = vp_i;
			}
		}
		
		int punctuation_mark_i = da.word_classes.FindAdd("punctuation mark");
		int punctuation_i = da.word_classes.FindAdd("punctuation");
		
		for(int i = 0; i < da.virtual_phrases.GetCount(); i++) {
			const VirtualPhrase& vp = da.virtual_phrases[i];
			Vector<Vector<int>> tmps;
			Vector<int> tmp;
			
			for (int type : vp.types) {
				if (type == punctuation_mark_i || type == punctuation_i) {
					if (tmp.GetCount()) {
						Swap(tmps.Add(), tmp);
						tmp.SetCount(0);
					}
				}
				else
					tmp << type;
			}
			if (tmp.GetCount()) {
				Swap(tmps.Add(), tmp);
				tmp.SetCount(0);
			}
			for (const Vector<int>& tmp : tmps) {
				CombineHash ch;
				for (int type : tmp)
					ch.Do(type).Put(1);
				hash_t h = ch;
				
				int vpp_i = -1;
				VirtualPhrasePart& vpp = da.virtual_phrase_parts.GetAdd(h, vpp_i);
				if (vpp.types.IsEmpty())
					vpp.types <<= tmp;
			}
		}
		LOG(da.virtual_phrase_parts.GetCount());
		LOG(da.virtual_phrase_parts.GetCount() * 100.0 / da.virtual_phrases.GetCount());
	}
	else if (t->fn == 1) {
		TokenArgs& args = token_args;
		args.fn = 2;
		args.words.Clear();
		
		int per_action_task = 75;
		int begin = t->batch_i * per_action_task;
		int end = begin + per_action_task;
		end = min(end, da.virtual_phrase_parts.GetCount());
		int count = end - begin;
		if (count <= 0) {
			RemoveTask(*t);
			return;
		}
		
		for(int i = begin; i < end; i++) {
			const VirtualPhrasePart& vpp = da.virtual_phrase_parts[i];
			String s;
			for(int j = 0; j < vpp.types.GetCount(); j++) {
				if (j) s << ",";
				int type = vpp.types[j];
				String type_str = da.word_classes[type];
				
				int a = type_str.Find("(");
				if (a >= 0) type_str = type_str.Left(a);
				a = type_str.Find(",");
				if (a >= 0) type_str = type_str.Left(a);
				
				s << type_str;
			}
			args.words << s;
		}
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetTokenData(args, THISBACK1(OnVirtualPhrases, t));
	}
}

void TaskManager::OnVirtualPhrases(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::GetActionlist(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	ActionAnalysisArgs args;
	args.fn = t->fn;
	
	int per_action_task = 0;
	if (t->fn == 0)	per_action_task = per_action_clrs;
	if (t->fn == 1)	per_action_task = per_action_attrs;
	
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	
	int iter = 0;
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		const VectorMap<String,int>& idx = uniq_acts[i];
		for(int j = 0; j < idx.GetCount(); j++) {
			if (iter >= begin && iter < end) {
				String s = uniq_acts.GetKey(i) + "(" + idx.GetKey(j) + ")";
				args.actions << s;
			}
			iter++;
		}
	}
	if (args.actions.IsEmpty()) {
		RemoveTask(*t);
		return; // ready
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 0)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistColors, t));
	else if (t->fn == 1)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistAttrs, t));
}

void TaskManager::OnActionlistColors(String result, Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		a = line.Find("RGB(", b);
		if (a < 0) continue;
		a += 4;
		b = line.Find(")", a);
		if (b < 0) continue;
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = ScanInt(TrimLeft(clr_parts[0]));
		int G = ScanInt(TrimLeft(clr_parts[1]));
		int B = ScanInt(TrimLeft(clr_parts[2]));
		Color clr(R,G,B);
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.clr = clr;
	}
	
	t->batch_i++;
	t->running = false;
	#endif
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-procedures(planning)" problem solving strategy / shortcut taking
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		
		a = b+1;
		b = line.Find("/", b);
		if (a < 0) continue;
		String attr_group = TrimBoth(line.Mid(a,b-a));
		a = b+1;
		String attr_value = TrimBoth(line.Mid(a));
		
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.group = attr_group;
		aa.value = attr_value;
	}
	
	t->batch_i++;
	t->running = false;
	#endif
}

void TaskManager::GetActionParallels(Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	da.action_parallel.Clear();
	
	// Loop from previous to 'current' to get score at the 'current'
	// (Instead of 'current' to next)
	
	for(const ActionPhrase& prev : da.action_phrases) {
		for(int k = 0; k < prev.next_phrases.GetCount(); k++) {
			int next_i = prev.next_phrases[k];
			const ActionPhrase& ap = da.action_phrases[next_i];
			
			for(int i = 0; i < ap.actions.GetCount(); i++) {
				const auto& aa0 = ap.actions[i];
				for(int j = 0; j < ap.actions.GetCount(); j++) {
					if (i == j) continue;
					
					const auto& aa1 = ap.actions[j];
					// we have a transition
					ActionHeader ah0, ah1;
					ah0.action = aa0.action;
					ah0.arg = aa0.arg;
					ah1.action = aa1.action;
					ah1.arg = aa1.arg;
					ActionParallel& ap = da.action_parallel.GetAdd(ah0).GetAdd(ah1);
					ap.count++; // increase count
					
					if (k < prev.next_scores.GetCount()) {
						auto& sc = prev.next_scores[k];
						int total_score = 0;
						for(int j = 0; j < SCORE_MODE_COUNT; j++) {
							for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
								int v = sc.scores[j][k];
								total_score += v;
							}
						}
						ap.score_sum += max(0, min(
							SCORE_MODE_COUNT * SCORE_ATTR_COUNT,
							total_score));
					}
				}
			}
		}
	}
	
	LOG("TaskManager::GetActionParallels took " << ts.ToString());
	#endif
}

void TaskManager::GetActionTransitions(Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	da.action_trans.Clear();
	
	for(const ActionPhrase& ap : da.action_phrases) {
		int ap_i0 = 0;
		for(int next_i : ap.next_phrases) {
			if (ap_i0 < ap.next_scores.GetCount()) {
				const ActionPhrase& next = da.action_phrases[next_i];
				
				for (const auto& aa0 : ap.actions) {
					for (const auto& aa1 : next.actions) {
						if (aa0.action == aa1.action) {
							// we have a transition
							ActionHeader ah0, ah1;
							ah0.action = aa0.action;
							ah0.arg = aa0.arg;
							ah1.action = aa1.action;
							ah1.arg = aa1.arg;
							ActionTransition& at = da.action_trans.GetAdd(ah0).GetAdd(ah1);
							at.count++; // increase count
							
							auto& sc = ap.next_scores[ap_i0];
							int total_score = 0;
							for(int j = 0; j < SCORE_MODE_COUNT; j++) {
								for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
									int v = sc.scores[j][k];
									total_score += v;
								}
							}
							at.score_sum += max(0, min(
								SCORE_MODE_COUNT * SCORE_ATTR_COUNT,
								total_score));
						}
					}
				}
			}
			ap_i0++;
		}
	}
	
	LOG("TaskManager::GetActionTransitions took " << ts.ToString());
	#endif
}

}
