#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ArrayMap<hash_t, DemandSolver>& __DemandSolvers() {
	static ArrayMap<hash_t, DemandSolver> map;
	return map;
}



DemandSolver::DemandSolver() {
	
}

void DemandSolver::Process() {
	
}

DemandSolver& DemandSolver::Get(MetaEntity& e) {
	String t = e.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, DemandSolver>& map = __DemandSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	DemandSolver& ls = map.Add(h);
	ls.entity = &e;
	return ls;
}

void DemandSolver::ClearTasks() {
	
}

void DemandSolver::RestartTasks() {
	
}



END_TEXTLIB_NAMESPACE
