#include "SongDataCtrl.h"


RhymeContainerPage::RhymeContainerPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos());
	
	hsplit.Horz() << vsplit << data;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << syl_counts << colors << attrs << actions;
	
	
	syl_counts.AddColumn(t_("Syllable count"));
	
	colors.AddColumn(t_("Color"));
	
	attrs.AddColumn(t_("Attributes"));
	
	actions.AddColumn(t_("Actions"));
	
	data.AddColumn(t_("Data"));
	
	prog.Set(0,1);
	
}

RhymeContainerPage::~RhymeContainerPage() {
	Stop();
}

void RhymeContainerPage::EnableAll() {
	
}

void RhymeContainerPage::DisableAll() {
	
}

void RhymeContainerPage::Data() {
	
}

void RhymeContainerPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Process data"), AppImg::BlueRing(), THISBACK(ProcessData)).Key(K_F5);
	
}

void RhymeContainerPage::ProcessData() {
	Database& db = Database::Single();
	
	
	
}

void RhymeContainerPage::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void RhymeContainerPage::Stop() {
	running = false;
	while (!stopped)
		Sleep(10);
}

void RhymeContainerPage::Process() {
	
	while (running) {
		
		// Make sentences from templates
		
		
		
		// Make extra sentences using wordnets
		
		
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}
