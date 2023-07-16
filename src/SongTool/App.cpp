#include "SongTool.h"



SongTool::SongTool() {
	Title("SongTool");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(AppImg::icon());
	
	last_window = RectC(0,0, 800, 600);
	is_maximized = false;
	Load();
	
	ed.WhenStartUpdating << [this](){tc.Set(-500, THISBACK(Data));};
	ed.WhenStopUpdating << [this](){tc.Kill();};
	ed.PostInit();
	
	Add(fp.SizePos());
	Add(cal.SizePos());
	Add(ed.SizePos());
	Add(seq.SizePos());
	Add(ai.SizePos());
	SetView(page);
	
	AddFrame(menu);
	menu.Set(THISBACK(MainMenu));
	PostCallback(THISBACK(LoadWindowPos));
	PostCallback(THISBACK(Data));
	
}

SongTool::~SongTool() {
	// Stop task manager
	TaskMgr::Single().running = false;
	
	SaveWindowPos();
	Store();
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
	});
	bar.Sub(t_("Tools"), [this](Bar& bar) {
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
	
	TaskMgr& m = TaskMgr::Single();
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
		case 3: ai.Show(); tc.Set(-500, THISBACK(Data)); break;
		case 4: seq.Show(); break;
	}
	
	page = i;
	Store();
	
	Data();
}

void SongTool::MakeTasks() {
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
