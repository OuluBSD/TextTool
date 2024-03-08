#include "SongLib.h"


BEGIN_SONGLIB_NAMESPACE


TaskManager::TaskManager() {
	
}

void TaskManager::Clear() {
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_CLEAR;
	t.cb = THISBACK1(DoClear, &t);
	lock.LeaveWrite();
}

void TaskManager::DoClear(Task* tp) {
	TimeStop ts;
	while (1) {
		bool any_running = false;
		for (Task& t : task_list) {
			if (&t == tp)
				continue;
			if (t.running)
				any_running = true;
		}
		if (any_running) {
			if (ts.Seconds() >= 30.0)
				return;
			Sleep(10);
		}
		else
			break;
	}
	lock.EnterWrite();
	task_list.Clear();
	lock.LeaveWrite();
}

bool TaskManager::IsInTaskList(TaskType type) const {
	for (const Task& t : task_list)
		if (t.type == type)
			return true;
	return false;
}

void TaskManager::Start() {
	//ImportPromptAnswers();
	
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
				class_i = ew.classes[0];
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

void TaskManager::ImportPromptAnswers() {
	String dir = ConfigFile("import");
	FindFile ff(AppendFileName(dir, "*.txt"));
	int i = 0;
	do {
		String path = ff.GetPath();
		String content = LoadFileBOM(path);
		
		Task t;
		t.ds_i = 0;
		if (content.Find("RGB(") >= 0) {
			OnDetails(content, &t);
		}
		else {
			OnSyllables(content, &t);
		}
		i++;
	}
	while (ff.Next());
	
}


END_SONGLIB_NAMESPACE

