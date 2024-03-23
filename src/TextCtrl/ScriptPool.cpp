#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptPool::ScriptPool() {
	Add(list.SizePos());
	
	list.AddColumn("Phrase 1");
	list.AddColumn("Phrase 2");
	list.AddColumn("Phrase 3");
	list.AddColumn("Phrase 4");
	list.AddColumn("Score total");
	
}

void ScriptPool::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::RedRing(), THISBACK(StopProcess)).Key(K_F6);
	
}

void ScriptPool::StartProcess() {
	TextLib::ScriptGenerator& ls = TextLib::ScriptGenerator::Get(GetAppMode(), GetEntity(), GetScript());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void ScriptPool::StopProcess() {
	TextLib::ScriptGenerator& ls = TextLib::ScriptGenerator::Get(GetAppMode(),GetEntity(), GetScript());
	ls.Stop();
}

void ScriptPool::Data() {
	
}


END_TEXTLIB_NAMESPACE
