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


END_TEXTLIB_NAMESPACE

