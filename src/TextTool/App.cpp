#include "TextTool.h"

#define IMAGECLASS TextToolImg
#define IMAGEFILE <TextTool/TextTool.iml>
#include <Draw/iml_source.h>

BEGIN_TEXTLIB_NAMESPACE


TextTool::TextTool() : ed(this), lead(this) {
	skip_data = true;
	
	Title("TextTool");
	MaximizeBox().MinimizeBox().Sizeable();
	Icon(TextToolImg::icon());
	
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
	
	Add(lead.SizePos());
	Add(org.SizePos());
	Add(ed.SizePos());
	Add(ai.SizePos());
	Add(svc.SizePos());
	Add(mc.SizePos());
	
	AddFrame(menu);
	AddFrame(toolbar);
	AddFrame(statusbar);
	menu.Set(THISBACK(MainMenu));
	
	PostInit();
}

TextTool::~TextTool() {
	// Stop task manager
	TaskMgrConfig::Single().running = false;
	
	SaveWindowPos();
	Store();
	StoreLast();
}

void TextTool::StoreLast() {
	if (!active) return;
	MetaPtrs& mp = MetaPtrs::Single();
	
	TextDatabase& db = ed.GetDatabase();
	last_owner = mp.owner ? mp.owner->name : String();
	last_profile = mp.profile ? mp.profile->name : String();
	
	EditorPtrs& p = ed.GetPointers();
	last_scripts = p.script ? p.script->file_title : String();
	last_part = p.part ? p.part->name : String();
	last_entity = p.entity ? p.entity->file_title : String();
	last_snapshot = p.release ? p.release->file_title : String();
	last_component = p.component ? p.component->file_title : String();
	
	Store();
}

void TextTool::Init() {
	lead.Init();
	org.Init();
	svc.Init();
	mc.Init();
	
	ed.Init();
	ed.Data(); // sets active artist, song, etc.
	
	LoadWindowPos();
	SetView(page);
	
	skip_data = false;
	//PostCallback(THISBACK(Data));
}

void TextTool::FastExit() {
	ed.SetFastExit(1);
	TopWindow::Close();
}

void TextTool::SaveDatabase() {
	int appmode = ed.GetAppMode();
	EnterAppMode(appmode);
	TextDatabase& db = GetEditor().GetDatabase();
	db.Store();
	LeaveAppMode();
	MetaDatabase::Single().Store();
	ProfileData::StoreAll();
}

void TextTool::MainMenu(Bar& bar) {
	bar.Sub(t_("App"), [this](Bar& bar) {
		bar.Add(t_("Save"), THISBACK(SaveDatabase)).Key(K_CTRL_S);
		bar.Separator();
		bar.Add(t_("Set OpenAI token"), THISBACK(SetOpenAIToken));
		bar.Separator();
		//bar.Add(t_("Save song data analysis"), callback(&GetDatabase().src_data, &SourceData::Store));
		if (!ed.save_data)
			bar.Add(t_("Save song data analysis on exit"), THISBACK1(SetSaveSongdata, 1));
		else
			bar.Add(t_("Do not save song data analysis on exit"), THISBACK1(SetSaveSongdata, 0));
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
		bar.Add(t_("Move to higher part"), THISBACK1(MovePart, -1)).Key(K_SHIFT_1);
		bar.Add(t_("Move to lower part"), THISBACK1(MovePart, +1)).Key(K_SHIFT_2);
	});
	bar.Sub(t_("Tools"), [this](Bar& bar) {
		bar.Add(t_("Show orphaned files"), THISBACK(ShowOrphanedFiles));
	});
}

void TextTool::SetOpenAIToken() {
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

void TextTool::LoadWindowPos() {
	if (last_window.left > 0 && last_window.top > 0) {
		PostCallback([this](){
			SetRect(last_window);
		});
	}
	if (is_maximized) {
		PostCallback([this](){
			Maximize();
		});
	}
	started = true;
}

void TextTool::SaveWindowPos() {
	if (!started)
		return;
	is_maximized = IsMaximized();
	if (!is_maximized)
		last_window = GetRect();
}

void TextTool::Data() {
	switch (page) {
		case 0: lead.Data(); break;
		case 1: org.Data(); break;
		case 2: ed.Data(); break;
		case 3: ai.Data(); break;
		case 4: svc.Data(); break;
		case 5: mc.Data(); break;
	}
}

void TextTool::SetBar() {
	toolbar.Set(THISBACK(MainBar));
	
	String s;
	switch (page) {
		case 0: break;
		case 1: break;
		case 2: s = ed.GetStatusText(); break;
		case 3: break;
		case 4: break;
	}
	statusbar.SetData(s);
}

void TextTool::MainBar(Bar& bar) {
	switch (page) {
		case 0: lead.ToolMenu(bar); break;
		case 1: org.ToolMenu(bar); break;
		case 2: ed.ToolMenu(bar); break;
		case 3: ai.ToolMenu(bar); break;
		case 4: svc.ToolMenu(bar); break;
		case 5: mc.ToolMenu(bar); break;
	}
}

void TextTool::MovePageGroup(int i) {
	switch (page) {
		//case 0: fp.MovePageGroup(i); break;
		//case 1: cal.MovePageGroup(i); break;
		case 2: ed.MovePageGroup(i); break;
		//case 3: ai.MovePageGroup(i); break;
	}
}

void TextTool::MovePage(int i) {
	switch (page) {
		//case 0: fp.MovePage(i); break;
		//case 1: cal.MovePage(i); break;
		case 2: ed.MovePage(i); break;
		//case 3: ai.MovePage(i); break;
	}
}

void TextTool::MovePart(int i) {
	switch (page) {
		//case 0: fp.MovePart(i); break;
		//case 1: cal.MovePart(i); break;
		case 2: ed.MovePart(i); break;
		//case 3: ai.MovePart(i); break;
	}
}

void TextTool::SetView(int i) {
	lead.Hide();
	org.Hide();
	ed.Hide();
	ai.Hide();
	svc.Hide();
	mc.Hide();
	
	tc.Kill();
	active = 0;
	switch (i) {
		default: i = 0;
		case 0: lead.Show(); active = &lead; lead.FixMenuBar(); break;
		case 1: org.Show(); break;
		case 2: ed.Show(); ed.FixMenuBar(); active = &ed; break;
		case 3: ai.Show(); PostCallback(THISBACK(StartUpdating)); break;
		case 4: svc.Show(); break;
		case 5: mc.Show(); break;
	}
	
	page = i;
	Store();
	
	Data();
}

void TextTool::StartUpdating() {
	tc.Set(-500, THISBACK(Data));
}

/*void TextTool::MakeTasks() {
	SetView(3);
	ed.importer.MakeTasks();
}*/

void TextTool::ShowOrphanedFiles() {
	TopWindow tw;
	DocEdit doc;
	tw.Add(doc.SizePos());
	tw.SetRect(RectC(0,0,800,600));
	doc.SetData("TODO");
	tw.Open();
}


END_TEXTLIB_NAMESPACE

