#include "SocialTool.h"


BEGIN_SOCIALLIB_NAMESPACE


SocialTool::SocialTool() : ed(this) {
	skip_data = true;
	
	Title("SocialTool");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(SocialToolImg::icon());
	
	last_window = RectC(0,0, 800, 600);
	is_maximized = false;
	Load();
	
	ed.WhenStartUpdating << [this](){
		#ifdef flagDEBUG
		tc.Set(-500, THISBACK(Data));
		#else
		tc.Set(-100, THISBACK(Data));
		#endif
	};
	ed.WhenStopUpdating << [this](){tc.Kill();};
	
	Add(ed.SizePos());
	Add(ai.SizePos());
	
	AddFrame(menu);
	AddFrame(toolbar);
	AddFrame(statusbar);
	menu.Set(THISBACK(MainMenu));
	
	PostInit();
}

SocialTool::~SocialTool() {
	// Stop task manager
	TaskMgrConfig::Single().running = false;
	
	SaveWindowPos();
	Store();
}

void SocialTool::Init() {
	ed.Init();
	ed.Data(); // sets active company, program, etc.
	
	LoadWindowPos();
	SetView(page);
	
	skip_data = false;
	//PostCallback(THISBACK(Data));
}

void SocialTool::FastExit() {
	ed.SetFastExit(1);
	TopWindow::Close();
}

void SocialTool::ClearSocialLibTasks() {
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	tm.Clear();
}

void SocialTool::MainMenu(Bar& bar) {
	bar.Sub(t_("App"), [this](Bar& bar) {
		bar.Add(t_("Save"), callback(&SocialDatabase::Single(), &SocialDatabase::Store)).Key(K_CTRL_S);
		bar.Separator();
		bar.Add(t_("Set OpenAI token"), THISBACK(SetOpenAIToken));
		bar.Separator();
		bar.Add(t_("Save program data analysis"), callback(&SocialDatabase::Single().program_data, &ProgramData::Store));
		if (!ed.save_programdata)
			bar.Add(t_("Save program data analysis on exit"), THISBACK1(SetSaveProgramData, 1));
		else
			bar.Add(t_("Do not save program data analysis on exit"), THISBACK1(SetSaveProgramData, 0));
		bar.Separator();
		bar.Add(t_("Exit"), callback(this, &TopWindow::Close));
		bar.Add(t_("Fast Exit"), THISBACK(FastExit)).Key(K_CTRL|K_SHIFT|K_Q);
	});
	bar.Sub(t_("View"), [this](Bar& bar) {
		bar.Add(t_("View Front Page"), THISBACK1(SetView, 0)).Key(K_ALT_1);
		bar.Add(t_("View Calendar"), THISBACK1(SetView, 1)).Key(K_ALT_2);
		bar.Add(t_("View Editor"), THISBACK1(SetView, 2)).Key(K_ALT_3);
		bar.Add(t_("View AI Tasks"), THISBACK1(SetView, 3)).Key(K_ALT_4);
		bar.Add(t_("View Sequencer"), THISBACK1(SetView, 4)).Key(K_ALT_5);
		bar.Add(t_("View AI Image Generator"), THISBACK1(SetView, 5)).Key(K_ALT_6);
		bar.Separator();
		bar.Add(t_("Move to higher page"), THISBACK1(MovePage, -1)).Key(K_CTRL_1);
		bar.Add(t_("Move to lower page"), THISBACK1(MovePage, +1)).Key(K_CTRL_2);
		bar.Add(t_("Move to higher page group"), THISBACK1(MovePageGroup, -1)).Key(K_CTRL_3);
		bar.Add(t_("Move to lower page group"), THISBACK1(MovePageGroup, +1)).Key(K_CTRL_4);
	});
	bar.Sub(t_("Tools"), [this](Bar& bar) {
		bar.Add(t_("Show orphaned files"), THISBACK(ShowOrphanedFiles));
		//bar.Add(t_("Make Tasks"), THISBACK(MakeTasks));//.Key(K_CTRL_R);
		bar.Add(t_("Clear SocialLib tasks"), THISBACK(ClearSocialLibTasks));
	});
}

void SocialTool::SetOpenAIToken() {
	String token;
	bool b = EditTextNotNull(
		token,
		t_("OpenAI token"),
		t_("OpenAI token"),
		0
	);
	if (!b) return;
	
	TaskMgrConfig& m = TaskMgrConfig::Single();
	m.openai_token = token;
	m.Store();
	
	PromptOK(t_("Restart is required for applying the new token"));
}

void SocialTool::LoadWindowPos() {
	if (last_window.left > 0 && last_window.top > 0)
		SetRect(last_window);
	if (is_maximized)
		Maximize();
	started = true;
}

void SocialTool::SaveWindowPos() {
	if (!started)
		return;
	is_maximized = IsMaximized();
	if (!is_maximized)
		last_window = GetRect();
}

void SocialTool::Data() {
	switch (page) {
		case 2: ed.Data(); break;
		case 3: ai.Data(); break;
	}
}

void SocialTool::SetBar() {
	toolbar.Set(THISBACK(MainBar));
	
	String s;
	switch (page) {
		case 0: break;
		case 1: break;
		case 2: s = ed.GetStatusText(); break;
		case 3: break;
	}
	statusbar.SetData(s);
}

void SocialTool::MainBar(Bar& bar) {
	switch (page) {
		case 2: ed.ToolMenu(bar); break;
		case 3: ai.ToolMenu(bar); break;
	}
}

void SocialTool::MovePageGroup(int i) {
	switch (page) {
		//case 0: fp.MovePageGroup(i); break;
		//case 1: cal.MovePageGroup(i); break;
		case 2: ed.MovePageGroup(i); break;
		//case 3: ai.MovePageGroup(i); break;
	}
}

void SocialTool::MovePage(int i) {
	switch (page) {
		//case 0: fp.MovePage(i); break;
		//case 1: cal.MovePage(i); break;
		case 2: ed.MovePage(i); break;
		//case 3: ai.MovePage(i); break;
	}
}

void SocialTool::SetView(int i) {
	ed.Hide();
	ai.Hide();
	
	tc.Kill();
	
	switch (i) {
		default: i = 0;
		case 2: ed.Show(); break;
		case 3: ai.Show(); PostCallback(THISBACK(StartUpdating)); break;
	}
	
	page = i;
	Store();
	
	Data();
}

void SocialTool::StartUpdating() {
	tc.Set(-500, THISBACK(Data));
}

/*void SocialTool::MakeTasks() {
	SetView(3);
	ed.importer.MakeTasks();
}*/

void SocialTool::ShowOrphanedFiles() {
	TopWindow tw;
	DocEdit doc;
	tw.Add(doc.SizePos());
	tw.SetRect(RectC(0,0,800,600));
	doc.SetData("TODO");
	tw.Open();
}


END_SOCIALLIB_NAMESPACE
