#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


SourceAnalysisProcess::SourceAnalysisProcess() {
	
}

int SourceAnalysisProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int SourceAnalysisProcess::GetBatchCount(int phase) const {
	switch (phase) {
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
		case PHASE_ANALYZE_ELEMENTS:		AnalyzeElements(); return;
		case PHASE_SUMMARIZE_CONTENT:		SummarizeContent(); return;
		default: TODO;
	}
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
	
	args.fn = 0;
	args.text = TrimBoth(da.GetScriptDump(batch));
	if (args.text.IsEmpty()) {
		NextBatch();
		return;
	}
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, [this](String result) {
		SourceDataAnalysisArgs& args = this->args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = sd.a;
		DatasetAnalysis& da = sda.dataset;
		ScriptStruct& ss = da.scripts[batch];
		
		//LOG(result);
		
		NextBatch();
		SetWaiting(false);
	});
	
	
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

