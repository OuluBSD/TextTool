#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE

#if 0
ScriptRangeProcess::ScriptRangeProcess() {
	
}

int ScriptRangeProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

void ScriptRangeProcess::DoPhase() {
	EnterAppMode(appmode);
	
	if (phase == PHASE_COLOR) {
		ProcessColor();
	}
	else if (phase == PHASE_ATTR) {
		ProcessAttr();
	}
	else if (phase == PHASE_ACTION) {
		ProcessAction();
	}
	else if (phase == PHASE_COLLECT) {
		ProcessCollect();
	}
	else TODO
		
	LeaveAppMode();
}

ScriptRangeProcess& ScriptRangeProcess::Get(int appmode, Entity& a, Script& l) {
	String t = IntStr(appmode) + " - " + a.file_title + " - " + l.file_title;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, ScriptRangeProcess> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptRangeProcess& ls = map.Add(h);
	ls.appmode = appmode;
	ls.artist = &a;
	ls.script = &l;
	return ls;
}

#endif


END_TEXTLIB_NAMESPACE
