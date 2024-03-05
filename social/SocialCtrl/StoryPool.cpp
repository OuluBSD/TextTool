#include "SocialCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


StoryPool::StoryPool() {
	Add(list.SizePos());
	
	list.AddColumn("Phrase 1");
	list.AddColumn("Phrase 2");
	list.AddColumn("Phrase 3");
	list.AddColumn("Phrase 4");
	list.AddColumn("Score total");
	
}

void StoryPool::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::RedRing(), THISBACK(StopProcess)).Key(K_F6);
	
}

void StoryPool::StartProcess() {
	SocialLib::StoryGenerator& ls = SocialLib::StoryGenerator::Get(GetCompany(), GetStory());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void StoryPool::StopProcess() {
	SocialLib::StoryGenerator& ls = SocialLib::StoryGenerator::Get(GetCompany(), GetStory());
	ls.Stop();
}

void StoryPool::Data() {
	
}


END_SOCIALLIB_NAMESPACE
