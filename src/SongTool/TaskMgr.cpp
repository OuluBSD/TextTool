#include "SongTool.h"


void TaskMgr::Store() {
	for (AI_Task& t : tasks)
		t.Store();
}
