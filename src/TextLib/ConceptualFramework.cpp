#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ConceptualFrameworkProcess::ConceptualFrameworkProcess() {
	
}

int ConceptualFrameworkProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ConceptualFrameworkProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_MAKE_ALL_CONCEPTS:		return max(1, tasks.GetCount());
		case PHASE_MAKE_TOPIC_CONCEPTS:		return max(1, snap->data.AllCategories().GetCount());
		case PHASE_IMPROVE_STORY:			return max(1, cf->stories.GetCount());
		case PHASE_SCORE_CONCEPTS:			return max(1, cf->stories.GetCount());
		default: return 1;
	}
}

int ConceptualFrameworkProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_MAKE_ALL_CONCEPTS:		return 1;
		case PHASE_MAKE_TOPIC_CONCEPTS:		return 1;
		case PHASE_IMPROVE_STORY:			return 1;
		case PHASE_SCORE_CONCEPTS:			return 1;
		default: return 1;
	}
}

void ConceptualFrameworkProcess::DoPhase() {
	switch (phase) {
		case PHASE_MAKE_ALL_CONCEPTS:		MakeAllConcepts(); return;
		case PHASE_MAKE_TOPIC_CONCEPTS:		MakeTopicConcepts(); return;
		case PHASE_IMPROVE_STORY:			ImproveStory(); return;
		case PHASE_SCORE_CONCEPTS:			ScoreConcepts(); return;
		default: return;
	}
}

ConceptualFrameworkProcess& ConceptualFrameworkProcess::Get(int appmode, Entity& e, Concept& cf, BiographySnapshot& snap) {
	static ArrayMap<String, ConceptualFrameworkProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ", PROFILE(" + e.profile->name + "), CF(" + cf.name + "), REVISION(" + IntStr(snap.revision) + ")";
	ConceptualFrameworkProcess& ts = arr.GetAdd(key);
	ts.owner = e.profile->owner;
	ts.entity = &e;
	ts.profile = e.profile;
	ts.snap = &snap;
	ts.cf = &cf;
	ts.appmode = appmode;
	ASSERT(ts.owner);
	return ts;
}

void ConceptualFrameworkProcess::MakeAllConcepts() {
	Biography& data = snap->data;
	
	if (batch == 0 && sub_batch == 0) {
		items.Clear();
		tasks.Clear();
		//cf->stories.Clear();
		
		// Get best performing scores
		Index<String> added;
		double adjust = 0.0001; // for determined sorting order
		for(int i = 0; i < data.AllCategories().GetCount(); i++) {
			BiographyCategory& bcat = data.AllCategories()[i];
			
			for(int j = 0; j < bcat.years.GetCount(); j++) {
				BioYear& by = bcat.years[j];
				
				for(int k = 0; k < by.elements.GetCount(); k++) {
					BioYear::Element& el = by.elements[k];
					
					String s = el.key + ": " + el.value;
					if (added.Find(s) >= 0)
						continue;
					added.Add(s);
					
					double av = el.GetAverageScore();
					if (av > 10)
						continue; // error
					Item& it = items.Add();
					it.cat_i = i;
					it.year_i = j;
					it.el_i = k;
					it.av = av + adjust;
					adjust += 0.0001;
				}
			}
		}
		Sort(items, Item());
		
		
		int task_count = (items.GetCount() + elements_per_story_prompt - 1) / this->elements_per_story_prompt;
		
		for(int i = 0; i < task_count; i++) {
			Task& t = tasks.Add();
			t.begin = i * elements_per_story_prompt;
			t.end = min(items.GetCount(), t.begin + elements_per_story_prompt);
			ASSERT(t.begin < t.end);
			ASSERT(t.begin < items.GetCount());
		}
	}
	int count = 0;
	for (const auto& s : cf->stories)
		if (s.src == 0)
			count++;
	if (skip_ready && count >= max_stories) {
		NextPhase();
		return;
	}
	if (batch >= tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	Task& task = tasks[batch];
	
	args.fn = 0;
	args.elements.Clear();
	args.scores.Clear();
	
	el_items.Clear();
	for(int i = task.begin; i < task.end; i++) {
		Item& it = items[i];
		BiographyCategory& bcat = data.AllCategories()[it.cat_i];
		BioYear& by = bcat.years[it.year_i];
		BioYear::Element& el = by.elements[it.el_i];
		args.elements.Add(el.key, el.value);
		el_items << i;
	}
	
	if (cf->belief_uniq != 0) {
		auto& mdb = MetaDatabase::Single();
		int bf_i = mdb.FindBelief(cf->belief_uniq);
		ASSERT(bf_i >= 0);
		if (bf_i >= 0) {
			MakeBelief(mdb.beliefs[bf_i], args, 0);
		}
	}
	
	TaskMgr& m = TaskMgr::Single();
	SetWaiting(1);
	m.GetConceptualFramework(appmode, args, THISBACK(OnMakeConcepts));
}

void ConceptualFrameworkProcess::MakeTopicConcepts() {
	Biography& data = snap->data;
	
	auto& all = data.AllCategories();
	if (batch >= all.GetCount()){
		NextPhase();
		return;
	}
	
	if (sub_batch == 0) {
		items.Clear();
		tasks.Clear();
		
		// Get best performing scores
		
		BiographyCategory& bcat = data.AllCategories()[batch];
		Index<String> added;
		
		double adjust = 0.0001; // for determined sorting order
		for(int j = 0; j < bcat.years.GetCount(); j++) {
			BioYear& by = bcat.years[j];
			
			for(int k = 0; k < by.elements.GetCount(); k++) {
				BioYear::Element& el = by.elements[k];
				
				String s = el.key + ": " + el.value;
				if (added.Find(s) >= 0)
					continue;
				added.Add(s);
				
				double av = el.GetAverageScore();
				if (av > 10)
					continue; // error
				Item& it = items.Add();
				it.cat_i = batch;
				it.year_i = j;
				it.el_i = k;
				it.av = av + adjust;
				adjust += 0.0001;
			}
		}
		Sort(items, Item());
		
		
		int expected_stories_per_task = 7;
		int max_tasks = max_stories / expected_stories_per_task;
		int task_count = min(max_tasks, (items.GetCount() + elements_per_story_prompt - 1) / this->elements_per_story_prompt);
		
		for(int i = 0; i < task_count; i++) {
			Task& t = tasks.Add();
			t.begin = i * elements_per_story_prompt;
			t.end = min(items.GetCount(), t.begin + elements_per_story_prompt);
			ASSERT(t.begin < t.end);
			ASSERT(t.begin < items.GetCount());
		}
	}
	if (sub_batch >= tasks.GetCount()) {
		NextBatch();
		return;
	}
	
	int src = 1 + batch;
	int count = 0;
	for (const auto& s : cf->stories)
		if (s.src == src)
			count++;
	if (skip_ready && count >= max_topic_stories) {
		NextBatch();
		return;
	}
	
	Task& task = tasks[sub_batch];
	
	args.fn = 0;
	args.elements.Clear();
	args.scores.Clear();
	
	el_items.Clear();
	for(int i = task.begin; i < task.end; i++) {
		Item& it = items[i];
		BiographyCategory& bcat = data.AllCategories()[it.cat_i];
		BioYear& by = bcat.years[it.year_i];
		BioYear::Element& el = by.elements[it.el_i];
		args.elements.Add(el.key, el.value);
		el_items << i;
	}
	
	if (cf->belief_uniq != 0) {
		auto& mdb = MetaDatabase::Single();
		int bf_i = mdb.FindBelief(cf->belief_uniq);
		ASSERT(bf_i >= 0);
		if (bf_i >= 0) {
			MakeBelief(mdb.beliefs[bf_i], args, 0);
		}
	}
	
	TaskMgr& m = TaskMgr::Single();
	SetWaiting(1);
	m.GetConceptualFramework(appmode, args, THISBACK(OnMakeConcepts));
}

void ConceptualFrameworkProcess::OnMakeConcepts(String result) {
	Biography& data = snap->data;
	RemoveEmptyLines2(result);
	Vector<String> lines = Split(result, "\n");
	Vector<int> item_indices;
	Vector<BioYear::Element*> el_ptrs;
	bool all = phase == PHASE_MAKE_ALL_CONCEPTS;
	
	int count = 0;
	int src = all ? 0 : 1 + batch;
	for (const auto& s : cf->stories)
		if (s.src == src)
			count++;

	for (String& l : lines) {
		
		int a = l.Find("(");
		if (a < 0) continue;
		String pre = l.Left(a);
		Vector<String> elements;
		if (a == 0) {
			elements = Split(l, ", (");
			a = l.Find(" - ");
			if (a < 0) a = l.Find(") (");
			if (a >= 0) a += 3;
		}
		else
			elements = Split(pre, ",");
		
		bool fail = false;
		hash_t h = 0;
		CombineHash ch;
		item_indices.SetCount(0);
		el_ptrs.SetCount(0);
		if (elements.GetCount() < 2) {
			continue;
		}
		else {
			for (String& el : elements) {
				int el_i = ScanInt(TrimLeft(el));
				el_i -= 1;
				if (el_i < 0 || el_i >= items.GetCount()) {
					fail = true;
					break;
				}
				item_indices << el_i;
				
				Item& it = items[el_i];
				BiographyCategory& bcat = data.AllCategories()[it.cat_i];
				BioYear& by = bcat.years[it.year_i];
				BioYear::Element& e = by.elements[it.el_i];
				el_ptrs << &e;
				ch.Do(e.key);
				ch.Do(e.value);
			}
			h = ch;
		}
		
		if (fail || item_indices.IsEmpty() || h == 0)
			continue;
		
		if (skip_ready && cf->FindStory(h) >= 0)
			continue;
		
		if ((all && count > max_stories) ||
			(!all && count >= max_topic_stories))
			break;
		count++;
		
		ConceptStory& story = cf->GetAddStory(h);
		
		if (all)
			story.src = 0;
		else
			story.src = src;
			
		if (a >= 0) {
			story.desc = TrimBoth(l.Mid(a));
			RemoveParenthesis(story.desc);
		}
		
		story.elements.Clear();
		for (BioYear::Element* el : el_ptrs) {
			auto& e = story.elements.Add();
			e.key = el->key;
			e.value = el->value;
		}
	}
	
	if (all)
		NextBatch();
	else
		NextSubBatch();
	
	SetWaiting(0);
}

void ConceptualFrameworkProcess::ImproveStory() {
	if (batch >= cf->stories.GetCount()) {
		NextPhase();
		return;
	}
	
	ConceptStory& story = cf->stories[batch];
	
	if (skip_ready && story.improved_elements.GetCount()) {
		NextBatch();
		return;
	}
	
	args.fn = 1;
	args.elements.Clear();
	args.scores.Clear();
	
	for(int i = 0; i < story.elements.GetCount(); i++) {
		const auto& el = story.elements[i];
		args.elements.Add(el.key, el.value);
	}
	
	if (cf->belief_uniq != 0) {
		auto& mdb = MetaDatabase::Single();
		int bf_i = mdb.FindBelief(cf->belief_uniq);
		ASSERT(bf_i >= 0);
		if (bf_i >= 0) {
			MakeBelief(mdb.beliefs[bf_i], args, 0);
		}
	}
	
	TaskMgr& m = TaskMgr::Single();
	SetWaiting(1);
	m.GetConceptualFramework(appmode, args, [this](String result) {
		ConceptStory& story = cf->stories[batch];
		Biography& data = snap->data;
		RemoveEmptyLines2(result);
		Vector<String> lines = Split(result, "\n");
		story.improved_elements.Clear();
		for (String& l : lines) {
			int a = l.Find(":");
			if (a < 0) continue;
			String key = TrimBoth(l.Left(a));
			String value = TrimBoth(l.Mid(a+1));
			auto& el = story.improved_elements.Add();
			el.key = key;
			el.value = value;
		}
		
		NextBatch();
		SetWaiting(0);
	});
}

void ConceptualFrameworkProcess::ScoreConcepts() {
	if (batch >= cf->stories.GetCount()) {
		NextPhase();
		return;
	}
	
	ConceptStory& story = cf->stories[batch];
	
	args.fn = 2;
	args.elements.Clear();
	args.scores.Clear();
	
	for(int i = 0; i < story.improved_elements.GetCount(); i++) {
		const auto& el = story.improved_elements[i];
		args.elements.Add(el.key, el.value);
	}
	if (args.elements.IsEmpty()) {
		NextBatch();
		return;
	}
	
	
	
	if (cf->belief_uniq != 0) {
		auto& mdb = MetaDatabase::Single();
		int bf_i = mdb.FindBelief(cf->belief_uniq);
		ASSERT(bf_i >= 0);
		if (bf_i >= 0) {
			MakeBelief(mdb.beliefs[bf_i], args, 0);
		}
	}
	
	TaskMgr& m = TaskMgr::Single();
	SetWaiting(1);
	m.GetConceptualFramework(appmode, args, [this](String result) {
		ConceptStory& story = cf->stories[batch];
		RemoveEmptyLines2(result);
		Vector<String> lines = Split(result, "\n");
		
		for (String& line : lines) {
			int a = line.Find(":");
			if (a < 0) continue;
			String key = ToLower(TrimBoth(line.Left(a)));
			String value = TrimBoth(line.Mid(a+1));
			int i = story.FindImprovedElement(key);
			if (i < 0)
				continue;
			auto& el = story.improved_elements[i];
			
			Vector<String> scores = Split(value, ",");
			int j = -1;
			el.ResetScore();
			for (String& s : scores) {
				j++;
				Vector<String> parts = Split(s, ":");
				if (parts.GetCount() != 2)
					continue;
				int sc = ScanInt(TrimLeft(parts[1]));
				sc = min(10, max(0, sc));
				el.scores[j] = sc;
			}
		}
		
		NextBatch();
		SetWaiting(0);
	});
}


END_TEXTLIB_NAMESPACE
