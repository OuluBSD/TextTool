#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



ArrayMap<hash_t, LeadSolver>& __LeadSolvers() {
	static ArrayMap<hash_t, LeadSolver> map;
	return map;
}


LeadSolver::LeadSolver() {
	
}

LeadSolver& LeadSolver::Get(MetaEntity& e) {
	String t = e.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, LeadSolver>& map = __LeadSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	LeadSolver& ls = map.Add(h);
	ls.entity = &e;
	return ls;
}

void LeadSolver::ClearTasks() {
	for (LeadSolver& g : __LeadSolvers().GetValues())
		g.SetNotRunning();
}

void LeadSolver::RestartTasks() {
	for (LeadSolver& g : __LeadSolvers().GetValues())
		g.Start();
}

void LeadSolver::Process() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& sd = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	sa = &sda.GetLeadEntityAnalysis(entity->file_title);
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LS_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			NextPhase();
			ClearDB();
		}
		else if (phase == LS_DOWNLOAD_WEBSITES) {
			ProcessDownloadWebsites();
		}
		else if (phase == LS_PARSE_WEBSITES) {
			ProcessParseWebsites();
		}
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LS_BEGIN;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void LeadSolver::ClearDB() {
	/*for(int i = 0; i < script->parts.GetCount(); i++) {
		StaticPart& sp = script->parts[i];
		auto& lines = sp.nana.Get();
		for(int j = 0; j < lines.GetCount(); j++) {
			auto& line = lines[j];
			line.pp_i = -1;
			line.end_pp_i = -1;
		}
		sp.phrase_parts.Clear();
	}
	script->picked_phrase_parts.Clear();*/
}

void LeadSolver::ProcessDownloadWebsites() {
	
	
	SetWaiting(0);
	NextPhase();
}

void LeadSolver::ProcessParseWebsites() {
	
	
	
	SetWaiting(0);
	NextPhase();
}



END_TEXTLIB_NAMESPACE

