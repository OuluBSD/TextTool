#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


SourceAnalysisProcess::SourceAnalysisProcess() {
	
}

int SourceAnalysisProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int SourceAnalysisProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_ANALYZE_ARTISTS:			return GetDatabase().src_data.entities.GetCount();
		case PHASE_ANALYZE_ELEMENTS:		return GetDatabase().src_data.a.dataset.scripts.GetCount();
		case PHASE_SUMMARIZE_CONTENT:		TODO;
		default: TODO; return 1;
	}
}

int SourceAnalysisProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void SourceAnalysisProcess::DoPhase() {
	switch (phase) {
		case PHASE_ANALYZE_ARTISTS:			AnalyzeArtists(); return;
		case PHASE_ANALYZE_ELEMENTS:		AnalyzeElements(); return;
		case PHASE_SUMMARIZE_CONTENT:		SummarizeContent(); return;
		default: TODO;
	}
}

void SourceAnalysisProcess::AnalyzeArtists() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	ASSERT(appmode == DB_SONG);
	
	if (batch >= sd.entities.GetCount()) {
		NextPhase();
		return;
	}
	
	EntityDataset& ent = sd.entities[batch];
	if (ent.genres.GetCount()) {
		NextBatch();
		return;
	}
	args.fn = 1;
	args.artist = ent.name;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, [this](String result) {
		SourceDataAnalysisArgs& args = this->args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		
		RemoveEmptyLines3(result);
		RemoveEmptyLines2(result);
		//LOG(result);
		
		Vector<String> genres = Split(result, "\n");
		for (String& genre : genres) {
			genre = ToLower(TrimBoth(genre));
			int i = genre.Find(":");
			if (i >= 0)
				genre = TrimBoth(genre.Mid(i+1));
		}
		EntityDataset& ent = sd.entities[batch];
		ent.genres <<= genres;
		
		NextBatch();
		SetWaiting(false);
	});
}

void SourceAnalysisProcess::AnalyzeElements() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	DatasetAnalysis& da = sd.a.dataset;
	Vector<EntityDataset>& entities = sd.entities;
	
	if (batch >= da.scripts.GetCount()) {
		NextPhase();
		return;
	}
	ScriptStruct& ss = da.scripts[batch];
	if (ss.parts.GetCount() && ss.parts[0].cls >= 0) {
		NextBatch();
		return;
	}
	
	args.fn = 0;
	args.text = TrimBoth(da.GetScriptDump(batch));
	Vector<String> all_sections = Split(args.text, "[");
	if (args.text.IsEmpty() || all_sections.GetCount() >= 50) {
		NextBatch();
		return;
	}
	
	// Another hotfix
	if (args.text.Find("http://") >= 0 || args.text.Find("https://") >= 0) {
		NextBatch();
		return;
	}
	
	bool keep_going = true;
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	if (m.keep_going_counter >= 50) {
		SetNotRunning();
		return;
	}
	
	m.GetSourceDataAnalysis(appmode, args, [this](String result) {
		SourceDataAnalysisArgs& args = this->args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = sd.a;
		DatasetAnalysis& da = sda.dataset;
		ScriptStruct& ss = da.scripts[batch];
		
		RemoveEmptyLines3(result);
		//LOG(result);
		
		Vector<String> lines = Split(result, "\n");
		VectorMap<String,String> section_values;
		for (String& l : lines) {
			int a = l.Find("[");
			if (a < 0) continue;
			a++;
			int b = l.Find("]", a);
			if (b < 0) continue;
			String key = l.Mid(a,b-a);
			a = l.Find(":", b);
			if (a < 0) continue;
			a++;
			String value = ToLower(TrimBoth(l.Mid(a)));
			RemoveQuotes(value);
			for(int i = 0; i < key.GetCount(); i++) {
				int chr = key[i];
				if (chr == '.' || IsDigit(chr))
					continue;
				key = key.Left(i);
				break;
			}
			if (key.IsEmpty() || value.IsEmpty())
				continue;
			section_values.GetAdd(key, value);
		}
		for(int i = 0; i < ss.parts.GetCount(); i++) {
			auto& p = ss.parts[i];
			String key;
			key << i;
			int l = section_values.Find(key);
			if (l >= 0) {
				String& val = section_values[l];
				int el_i = da.element_keys.FindAdd(val);
				p.cls = el_i;
			}
			
			for(int j = 0; j < p.sub.GetCount(); j++) {
				auto& s = p.sub[j];
				String key;
				key << i << "." << j;
				int l = section_values.Find(key);
				if (l >= 0) {
					String& val = section_values[l];
					int el_i = da.element_keys.FindAdd(val);
					s.cls = el_i;
				}
				
				for(int k = 0; k < s.sub.GetCount(); k++) {
					auto& ss = s.sub[k];
					String key;
					key << i << "." << j << "." << k;
					int l = section_values.Find(key);
					if (l >= 0) {
						String& val = section_values[l];
						int el_i = da.element_keys.FindAdd(val);
						ss.cls = el_i;
					}
				}
			}
		}
		
		NextBatch();
		SetWaiting(false);
	}, keep_going);
	
	
}

void SourceAnalysisProcess::SummarizeContent() {
	
}

SourceAnalysisProcess& SourceAnalysisProcess::Get(int appmode) {
	static ArrayMap<String, SourceAnalysisProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	SourceAnalysisProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

