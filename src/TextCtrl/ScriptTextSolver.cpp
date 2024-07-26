#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptTextSolverCtrl::ScriptTextSolverCtrl() {
	
}

void ScriptTextSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void ScriptTextSolverCtrl::Do(int fn) {
	TODO
}

void ScriptTextSolverCtrl::Data() {
	
}


END_TEXTLIB_NAMESPACE
