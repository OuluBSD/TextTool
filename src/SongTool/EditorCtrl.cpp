#include "SongTool.h"



EditorCtrl::EditorCtrl() {
	
}

void EditorCtrl::Init() {
	editortabsize = 4;
	indent_spaces = 0;
	indent_amount = 4;
	show_tabs = 0;
	no_parenthesis_indent = 0;
	hilite_scope = 2;
	hilite_bracket = 1;
	hilite_ifdef = 1;
	barline = 1;
	hilite_if_endif = 1;
	thousands_separator = 1;
	hline = 1;
	line_numbers = 1;
	auto_enclose = 1;
	mark_lines = 0;
	bordercolumn = 1;
	bordercolor = 1;
	persistent_find_replace = 1;
	find_replace_restore_pos = 0;
	font_height = 15;
	
	Add(editor_bottom.VSizePos(30,0).HSizePos());
	Add(top_bar.TopPos(0,30).HSizePos());
	
	editor_bottom.Vert();
	editor_bottom << hsplit_code.Horz() << bottom;
	editor_bottom.SetPos(8000);
	bottom.SetFrame(btabs);
	bottom.Add(console.SizePos().SetFrame(NullFrame()));
	bottom.Add(error.SizePos().SetFrame(NullFrame()));
	bottom.Add(ffound.SizePos().SetFrame(NullFrame()));
	bottom.Add(calc.SizePos().SetFrame(NullFrame()));
	btabs <<= THISBACK(SyncBottom);
	BTabs();
	SwapBottom();
	
	SetupError(error, "Message");
	error.AddIndex("NOTES");
	error.WhenBar = THISBACK(ErrorMenu);
	
	SetupError(ffound, "Source");
	ffound.ColumnWidths("207 41 834");
	ffound.ColumnAt(0).SetDisplay(Single<FoundFileDisplay>());
	ffound.ColumnAt(2).SetDisplay(Single<FoundDisplay>());
	ffound.WhenBar = THISBACK(FFoundMenu);
	
	error.WhenSel = THISBACK(SelError);
	error.WhenLeftClick = THISBACK(ShowError);
	ffound.WhenSel = ffound.WhenLeftClick = THISBACK(ShowFound);
	
	hsplit_code << main;
	InitEditor(main);
	//InitEditor(right);
	
	main.WhenAction << THISBACK(OnMainChange);
	
	other.SetFont(Monospace(12));
	
	if (have_third_view) {
		hsplit_code << right_split;
		right_split.Vert();
		right_split << other << third;
		third.SetFont(Monospace(12));
	}
	else {
		hsplit_code << other;
	}
	
	has_init = true;
}

void EditorCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!has_init)
		Init();
	
	String main_txt = song.data.Get(main_key, "");
	main.SetData(main_txt);
}

void EditorCtrl::OnMainChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	if (main_key.IsEmpty())
		return;
	
	String txt = main.GetData();
	song.data.GetAdd(main_key) = txt;
}

void EditorCtrl::SetTabText(int i, String key, int line, int col) {
	TODO
	/*String& active_path = i ? right_path : left_path;
	CodeEditor& edit = i ? right : left;
	
	if (active_path != path) {
		if (active_path.GetCount()) {
			String content = edit.GetData();
			FileOut fout(active_path);
			fout << content;
		}
		String content = LoadFile(path);
		active_path = path;
		edit.SetData(content);
	}
	if (line >= 0)
		edit.SetCursor(edit.GetPos(line, col));*/
}

void EditorCtrl::InitEditor(CodeEditor& codeedit) {
	String highlight = "cpp";
	
	codeedit.DefaultHlStyles();
	codeedit.Highlight(highlight);
	
	codeedit.TabSize(editortabsize);
	codeedit.IndentSpaces(indent_spaces);
	codeedit.IndentAmount(indent_amount);
	codeedit.ShowTabs(show_tabs);
	codeedit.ShowLineEndings(show_tabs);
	codeedit.NoParenthesisIndent(no_parenthesis_indent);
	codeedit.HiliteScope(hilite_scope);
	codeedit.HiliteBracket(hilite_bracket);
	codeedit.HiliteIfDef(hilite_ifdef);
	codeedit.BarLine(barline);
	codeedit.HiliteIfEndif(hilite_if_endif);
	codeedit.ThousandsSeparator(thousands_separator);
	codeedit.ShowCurrentLine(hline ? HighlightSetup::GetHlStyle(HighlightSetup::SHOW_LINE).color : (Color)Null);
	codeedit.LineNumbers(line_numbers);
	codeedit.AutoEnclose(auto_enclose);
	codeedit.MarkLines(mark_lines);
	//codeedit.BorderColumn(bordercolumn, bordercolor);
	codeedit.PersistentFindReplace(persistent_find_replace);
	codeedit.FindReplaceRestorePos(find_replace_restore_pos);
	
	codeedit.SetFont(Monospace(font_height));
	
	//codeedit.WhenLeftDown = THISBACK(WhenTextCursor);
	//codeedit.WhenAnyKey = THISBACK(WhenTextCursor);
	/*codeedit.WhenAction = THISBACK(WhenTextCursor);
	codeedit.WhenState = THISBACK(WhenTextCursor);
	codeedit.WhenUpdate = THISBACK(WhenTextCursor);
	codeedit.WhenAnnotationMove = THISBACK(WhenTextCursor);*/
	
	//codeedit.WhenCtrlClick = THISBACK(CtrlClick);
	//codeedit.WhenTip = THISBACK(EditorTip);
}

void EditorCtrl::Compile() {
	if (state != IDLE) return;
	
}

void EditorCtrl::ProcessCompiling() {
	{
		GuiLock __;
		error.Clear();
		btabs.SetCursor(2);
		SyncBottom();
	}
	SaveFile();
}

void EditorCtrl::OnMessage(const ProcMsg& e) {
	lock.Enter();
	errors.Add(e);
	lock.Leave();
	
	GuiLock __;
	SyncBottom();
	
	Color bg = White();
	if (e.severity == PROCMSG_ERROR)			bg = Color(255, 182, 176);
	else if (e.severity == PROCMSG_WARNING)		bg = Color(255, 255, 180);
	
	ErrorInfo ei;
	ei.file = e.file;
	ei.lineno = e.line;
	ei.linepos = e.col;
	ei.kind = e.severity;
	ei.message = e.msg;
	for(int i = 0; i < 3; i++)
		ei.parts[i] = e.parts[i];
	
	if (have_group_bad_better)
		error.Add(e.file, e.line,
			AttrText(e.GetSeverityString() + ": " + e.parts[0]).NormalPaper(bg),
			AttrText(e.parts[1]).NormalPaper(bg),
			AttrText(e.parts[2]).NormalPaper(bg),
			RawToValue(ei));
	else
		error.Add(e.file, e.line, AttrText(e.GetSeverityString() + ": " + e.msg).NormalPaper(bg), RawToValue(ei));
}

void EditorCtrl::FocusLine(ArrayCtrl* list) {
	if (list->IsCursor()) {
		int c = list->GetCursor();
		int acol = have_group_bad_better ? 5 : 3;
		
		ErrorInfo ei = list->Get(acol).Get<ErrorInfo>();
		if (ei.lineno >= 0 && ei.linepos >= 0) {
			int64 cur = main.GetPos(ei.lineno-1, ei.linepos-1);
			if (cur > 0)
				main.SetCursor(cur);
		}
	}
}

void EditorCtrl::SaveFile() {
	TODO
	/*for(int i = 0; i < 2; i++) {
		String& active_path = i ? right_path : left_path;
		CodeEditor& edit = i ? right : left;
		
		if (active_path.GetCount()) {
			String content = edit.GetData();
			FileOut fout(active_path);
			fout << content;
		}
	}*/
}

void EditorCtrl::Execute() {
	state = COMPILING;
	ProcessCompiling();
	
}

void EditorCtrl::DebugFile() {
	state = COMPILING;
	ProcessCompiling();
	
}

void EditorCtrl::StopDebug() {
	
}

void EditorCtrl::Continue() {
	
}

bool EditorCtrl::IsRunning() {
	return state == DEBUGGING || state == RUNNING;
}

bool EditorCtrl::IsPaused() {
	return state == PAUSED;
}

void EditorCtrl::FindInFiles(bool b) {
	
}


bool EditorCtrl::HotKey(dword key) {
	//if (key == (K_CTRL | K_S)) {PostCallback(THISBACK(SaveFile)); return true;}
	if (key == K_ESCAPE) {SwapBottom(); return true;}
	/*if (key == K_F5) {
		
		if (!IsRunning()) {
			DebugFile(); return true;
		}
		else if (IsPaused()) {
			Continue(); return true;
		}
		else {
			StopDebug(); return true;
		}
	}
	if (key == (K_CTRL | K_F5) ) {Execute(); return true;}
	if (key == (K_CTRL | K_SHIFT | K_F)) {FindInFiles(0); return true;}
	if (key == (K_CTRL | K_SHIFT | K_R)) {FindInFiles(1); return true;}*/
	return false;
}

