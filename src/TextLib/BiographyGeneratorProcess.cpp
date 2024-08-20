#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyGeneratorProcess::BiographyGeneratorProcess() {
	
}

int BiographyGeneratorProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographyGeneratorProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_GENERATE:			return tasks.GetCount();
		default: return 1;
	}
}

int BiographyGeneratorProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		default: return 1;
	}
}

void BiographyGeneratorProcess::DoPhase() {
	switch (phase) {
		case PHASE_GENERATE:			Generate(); return;
		case PHASE_GENERATE_DETAILS:	GenerateDetails(); return;
		default: return;
	}
}

BiographyGeneratorProcess& BiographyGeneratorProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographyGeneratorProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographyGeneratorProcess& ts = arr.GetAdd(key);
	ts.profile = &p;
	ts.owner = p.owner;
	ts.snap = &snap;
	return ts;
}

void BiographyGeneratorProcess::Generate() {
	Biography& data = snap->data;
	if (batch == 0 && sub_batch == 0) {
		tasks.Clear();
		Time now = GetSysTime();
		if (!skip_ready)
			snap->data.ClearAll();
		for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
			if (skip_ready && data.Find(*owner, i))
				continue;
			Task& t = tasks.Add();
			t.category = i;
		}
	}
	
	if (batch >= tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	Task& t = tasks[batch];
	
	BiographyGeneratorArgs args;
	args.fn = 0;
	args.name = profile->name;
	args.birth_year = owner->year_of_birth;
	args.biography = profile->biography;
	args.preferred_genres = profile->preferred_genres;
	args.category = t.category;
	
	
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiographyGenerator(args, [this](String result) {
		Biography& data = snap->data;
		Task& t = tasks[batch];
		BiographyCategory& bcat = data.GetAdd(*owner, t.category);
		
		RemoveEmptyLines3(result);
		Vector<String> lines = Split(result, "\n");
		
		Time now = GetSysTime();
		snap->last_modified = now;
		
		for (String& line : lines) {
			int a = line.Find("(");
			if (a < 0) continue;
			int year = ScanInt(ToLower(TrimBoth(line.Left(a))));
			a = line.Find(":");
			if (a < 0) continue;
			String value = TrimBoth(line.Mid(a+1));
			
			if (year < owner->year_of_birth || year > now.year)
				continue;
			
			BioYear& by = bcat.GetAdd(year);
			
			by.text = value;
		}
		
		NextBatch();
		SetWaiting(0);
	});
}

void BiographyGeneratorProcess::GenerateDetails() {
	Biography& data = snap->data;
	
	if (batch == 0 && sub_batch == 0) {
		tasks.Clear();
		
		for(int i = 0; i < data.AllCategories().GetCount(); i++) {
			auto& cat = data.AllCategories()[i];
			for(int j = 0; j < cat.years.GetCount(); j++) {
				BioYear& by = cat.years[j];
				if (by.text.IsEmpty())
					continue;
				
				// Multiline texts has details already
				if (by.text.Find("\n") >= 0)
					continue;
				
				Task& t = tasks.Add();
				t.category = i;
				t.year_i = j;
				t.year = by.year;
			}
		}
	}
	
	if (batch >= tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	Task& t = tasks[batch];
	
	BiographyGeneratorArgs args;
	args.fn = 1;
	args.name = profile->name;
	args.birth_year = owner->year_of_birth;
	args.biography = profile->biography;
	args.preferred_genres = profile->preferred_genres;
	args.category = t.category;
	
	BioYear& by = data.AllCategories()[t.category].years[t.year_i];
	args.text = by.text;
	args.year = by.year;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiographyGenerator(args, [this](String result) {
		Biography& data = snap->data;
		Task& t = tasks[batch];
		BioYear& by = data.AllCategories()[t.category].years[t.year_i];
		
		result = TrimBoth(result);
		
		by.text += "\n\n" + result;
		
		NextBatch();
		SetWaiting(0);
	});
}

END_TEXTLIB_NAMESPACE

