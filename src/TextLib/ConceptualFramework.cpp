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
		case PHASE_MAKE_TOPIC_CONCEPTS:		return max(1, tasks.GetCount());
		case PHASE_SCORE_CONCEPTS:			return max(1, tasks.GetCount());
		default: return 1;
	}
}

int ConceptualFrameworkProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_MAKE_ALL_CONCEPTS:		return 1;
		case PHASE_MAKE_TOPIC_CONCEPTS:		return 1;
		case PHASE_SCORE_CONCEPTS:			return 1;
		default: return 1;
	}
}

void ConceptualFrameworkProcess::DoPhase() {
	switch (phase) {
		case PHASE_MAKE_ALL_CONCEPTS:		MakeAllConcepts();
		case PHASE_MAKE_TOPIC_CONCEPTS:		MakeTopicConcepts();
		case PHASE_SCORE_CONCEPTS:			ScoreConcepts();
		default: return;
	}
}

ConceptualFrameworkProcess& ConceptualFrameworkProcess::Get(Entity& e, Concept& cf, BiographySnapshot& snap) {
	static ArrayMap<String, ConceptualFrameworkProcess> arr;
	
	String key = "PROFILE(" + e.profile->name + "), CF(" + cf.name + "), REVISION(" + IntStr(snap.revision) + ")";
	ConceptualFrameworkProcess& ts = arr.GetAdd(key);
	ts.owner = e.profile->owner;
	ts.entity = &e;
	ts.profile = e.profile;
	ts.snap = &snap;
	ts.cf = &cf;
	ASSERT(ts.owner);
	return ts;
}

void ConceptualFrameworkProcess::MakeAllConcepts() {
	Biography& data = snap->data;
	
	if (batch == 0 && sub_batch == 0) {
		items.Clear();
		tasks.Clear();
		
		// Get best performing scores
		
		for(int i = 0; i < data.AllCategories().GetCount(); i++) {
			BiographyCategory& bcat = data.AllCategories()[i];
			
			for(int j = 0; j < bcat.years.GetCount(); j++) {
				BioYear& by = bcat.years[j];
				
				for(int k = 0; k < by.elements.GetCount(); k++) {
					BioYear::Element& el = by.elements[k];
					double av = el.GetAverageScore();
					if (av > 10)
						continue; // error
					Item& it = items.Add();
					it.cat_i = i;
					it.year_i = j;
					it.el_i = k;
					it.av = av;
				}
			}
		}
		Sort(items, Item());
		
		
		int concepts = min(max_concepts, (items.GetCount() + elements_per_concept_prompt - 1) / this->elements_per_concept_prompt);
		
		for(int i = 0; i < concepts; i++) {
			Task& t = tasks.Add();
			t.begin = i * elements_per_concept_prompt;
			t.end = min(items.GetCount(), t.begin + elements_per_concept_prompt);
			ASSERT(t.begin < t.end);
			ASSERT(t.begin < items.GetCount());
		}
	}
	if (batch >= tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	Task& task = tasks[batch];
	
	ConceptualFrameworkArgs args;
	args.fn = 0;
	
	TaskMgr& m = TaskMgr::Single();
	SetWaiting(1);
	m.GetConceptualFramework(args, [this](String result) {
		RemoveEmptyLines3(result);
		Vector<String> lines = Split(result, "\n");
		
		
		
		NextSubBatch();
		SetWaiting(0);
	});
	
}

void ConceptualFrameworkProcess::MakeTopicConcepts() {
	
	if (batch == 0 && sub_batch == 0) {
		tasks.Clear();
		
		
	}
	
}

void ConceptualFrameworkProcess::ScoreConcepts() {
	
}


END_TEXTLIB_NAMESPACE
