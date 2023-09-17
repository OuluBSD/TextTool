#include "SongTool.h"



SongTool::SongTool() : ed(this) {
	Title("SongTool");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(AppImg::icon());
	
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
	
	Add(fp.SizePos());
	Add(cal.SizePos());
	Add(ed.SizePos());
	Add(seq.SizePos());
	Add(ai.SizePos());
	
	AddFrame(menu);
	AddFrame(toolbar);
	AddFrame(statusbar);
	menu.Set(THISBACK(MainMenu));
	
	PostInit();
}

SongTool::~SongTool() {
	// Stop task manager
	TaskMgrConfig::Single().running = false;
	
	SaveWindowPos();
	Store();
}

void SongTool::Init() {
	ed.Init();
	ed.Data(); // sets active artist, song, etc.
	
	LoadWindowPos();
	SetView(page);
}

void SongTool::MainMenu(Bar& bar) {
	bar.Sub(t_("App"), [this](Bar& bar) {
		bar.Add(t_("Save"), callback(&Database::Single(), &Database::Store)).Key(K_CTRL_S);
		bar.Separator();
		bar.Add(t_("Set OpenAI token"), THISBACK(SetOpenAIToken));
		bar.Separator();
		bar.Add(t_("Exit"), callback(this, &TopWindow::Close));
	});
	bar.Sub(t_("View"), [this](Bar& bar) {
		bar.Add(t_("View Front Page"), THISBACK1(SetView, 0)).Key(K_ALT_1);
		bar.Add(t_("View Calendar"), THISBACK1(SetView, 1)).Key(K_ALT_2);
		bar.Add(t_("View Editor"), THISBACK1(SetView, 2)).Key(K_ALT_3);
		bar.Add(t_("View AI Tasks"), THISBACK1(SetView, 3)).Key(K_ALT_4);
		bar.Add(t_("View Sequencer"), THISBACK1(SetView, 4)).Key(K_ALT_5);
		bar.Add(t_("View AI Image Generator"), THISBACK1(SetView, 5)).Key(K_ALT_6);
		bar.Separator();
		bar.Add(t_("Move to higher tab"), THISBACK1(MoveTab, -1)).Key(K_CTRL_1);
		bar.Add(t_("Move to lower tab"), THISBACK1(MoveTab, +1)).Key(K_CTRL_2);
	});
	bar.Sub(t_("Tools"), [this](Bar& bar) {
		bar.Add(t_("Do the 1st action"), THISBACK1(DoMainAction, 0)).Key(K_F5);
		bar.Add(t_("Do the 2nd action"), THISBACK1(DoMainAction, 1)).Key(K_F6);
		bar.Add(t_("Do the 3rd action"), THISBACK1(DoMainAction, 2)).Key(K_F7);
		bar.Add(t_("Do the 4th action"), THISBACK1(DoMainAction, 3)).Key(K_F8);
		bar.Add(t_("Do the 5th action"), THISBACK1(DoMainAction, 4)).Key(K_F9);
		bar.Add(t_("Do the 6th action"), THISBACK1(DoMainAction, 5)).Key(K_F10);
		bar.Add(t_("Do the 7th action"), THISBACK1(DoMainAction, 6)).Key(K_F11);
		bar.Add(t_("Do the 8th action"), THISBACK1(DoMainAction, 7)).Key(K_F12);
		bar.Separator();
		bar.Add(t_("Show orphaned files"), THISBACK(ShowOrphanedFiles));
		bar.Add(t_("Make Tasks"), THISBACK(MakeTasks)).Key(K_CTRL_R);
	});
}

void SongTool::SetOpenAIToken() {
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

void SongTool::LoadWindowPos() {
	if (last_window.left > 0 && last_window.top > 0)
		SetRect(last_window);
	if (is_maximized)
		Maximize();
	started = true;
}

void SongTool::SaveWindowPos() {
	if (!started)
		return;
	is_maximized = IsMaximized();
	if (!is_maximized)
		last_window = GetRect();
}

void SongTool::Data() {
	switch (page) {
		case 0: fp.Data(); break;
		case 1: cal.Data(); break;
		case 2: ed.Data(); break;
		case 3: ai.Data(); break;
		case 4: seq.Data(); break;
	}
}

void SongTool::SetBar() {
	toolbar.Set(THISBACK(MainBar));
}

void SongTool::MainBar(Bar& bar) {
	switch (page) {
		case 0: fp.ToolMenu(bar); break;
		case 1: cal.ToolMenu(bar); break;
		case 2: ed.ToolMenu(bar); break;
		case 3: ai.ToolMenu(bar); break;
		case 4: seq.ToolMenu(bar); break;
	}
}

void SongTool::DoMainAction(int i) {
	switch (page) {
		//case 0: fp.DoMainAction(i); break;
		//case 1: cal.DoMainAction(i); break;
		case 2: ed.DoMainAction(i); break;
		//case 3: ai.DoMainAction(i); break;
		//case 4: seq.DoMainAction(i); break;
	}
}

void SongTool::MoveTab(int i) {
	switch (page) {
		//case 0: fp.MoveTab(i); break;
		//case 1: cal.MoveTab(i); break;
		case 2: ed.MoveTab(i); break;
		//case 3: ai.MoveTab(i); break;
		//case 4: seq.MoveTab(i); break;
	}
}

void SongTool::SetView(int i) {
	fp.Hide();
	cal.Hide();
	ed.Hide();
	ai.Hide();
	seq.Hide();
	
	tc.Kill();
	
	switch (i) {
		default: i = 0;
		case 0: fp.Show(); break;
		case 1: cal.Show(); break;
		case 2: ed.Show(); break;
		case 3: ai.Show(); PostCallback(THISBACK(StartUpdating)); break;
		case 4: seq.Show(); break;
	}
	
	page = i;
	Store();
	
	Data();
}

void SongTool::StartUpdating() {
	tc.Set(-500, THISBACK(Data));
}

void SongTool::MakeTasks() {
	SetView(3);
	ed.importer.MakeTasks();
}

void SongTool::ShowOrphanedFiles() {
	TopWindow tw;
	DocEdit doc;
	tw.Add(doc.SizePos());
	tw.SetRect(RectC(0,0,800,600));
	doc.SetData("TODO");
	tw.Open();
}
