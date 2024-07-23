#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptRangeSolverCtrl::ScriptRangeSolverCtrl() {
	
}

void ScriptRangeSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void ScriptRangeSolverCtrl::Data() {
	
}


END_TEXTLIB_NAMESPACE