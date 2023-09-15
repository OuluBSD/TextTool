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
	bottom.Add(audience.SizePos().SetFrame(NullFrame()));
	bottom.Add(ffound.SizePos().SetFrame(NullFrame()));
	bottom.Add(calc.SizePos().SetFrame(NullFrame()));
	btabs <<= THISBACK(SyncBottom);
	BTabs();
	SwapBottom();
	
	SetupError(error, t_("Message"));
	error.AddIndex("NOTES");
	error.WhenBar = THISBACK(ErrorMenu);
	
	SetupAudience(audience);
	
	SetupError(ffound, t_("Source"));
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
	
	if (other_key.GetCount()) other.WhenAction << THISBACK(OnOtherChange);
	if (third_key.GetCount()) third.WhenAction << THISBACK(OnThirdChange);
	
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
	
	error.Clear();
	main.Errors(Vector<Point>());
	other.Errors(Vector<Point>());
	third.Errors(Vector<Point>());
	
	audience.Clear();
	
	if (other_key.GetCount())
		other.SetData(song.data.Get(other_key, ""));
	else
		other.Clear();
	
	if (third_key.GetCount())
		third.SetData(song.data.Get(third_key, ""));
	else
		third.Clear();
	
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

void EditorCtrl::OnOtherChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	if (other_key.IsEmpty())
		return;
	
	String txt = other.GetData();
	song.data.GetAdd(other_key) = txt;
}

void EditorCtrl::OnThirdChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	if (third_key.IsEmpty())
		return;
	
	String txt = third.GetData();
	song.data.GetAdd(third_key) = txt;
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
	String highlight = highligh_struct ? "cpp" : "";
	
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
			CodeEditor& edit = have_errors_in_other ? other : main;
			
			int64 cur = edit.GetPos(ei.lineno-1, ei.linepos-1);
			if (cur > 0)
				edit.SetCursor(cur);
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

void EditorCtrl::OnErrorsRecv() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || error_result_key.IsEmpty())
		return;
	
	String results = p.song->pipe->data.Get(error_result_key, "");
	//LOG(results);
	
	results.Replace("\223", "\"");
	results.Replace("\224", "\"");
	
	{
		results.Replace("\r","");
		Vector<String> lines = Split(results, "\n", false);
		int i = -1;
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			l = TrimBoth(l);
			
			if (l.IsEmpty() && (i == 0 || lines[i-1].IsEmpty())) {
				lines.Remove(i--);
				continue;
			}
			
			// Ensure that there is at least 1 empty line between parts
			if (i > 0 && IsDigit(l[0]) && !lines[i-1].IsEmpty()) {
				lines.Insert(i);
				continue;
			}
		}
		results = Join(lines, "\n", false);
	}
	
	Vector<String> parts = Split(results, "\n\n");
	
	Vector<String> beginnings;
	beginnings << "Lines with ";
	beginnings << "Lines that are ";
	beginnings << "Lines that ";
	beginnings << "Lines ";
	beginnings << " ";
	
	Vector<Point> errors;
	
	TextMatchFinder finder(
		(have_errors_in_other ? other : main)
			.GetData());
	
	Array<ProcMsg> msgs;
	for(String& part : parts) {
		int a = -1;
		for (String& b : beginnings) {
			a = part.Find(b);
			if (a >= 0) {
				a += b.GetCount();
				break;
			}
		}
		if (a < 0) {
			LOG("error: unexpected result");
			continue;
		}
		
		int b = part.Find(" (and improvements)");
		if (b < 0) {
			b = part.Find(" in ");
			if (b < 0) {
				b = part.Find(":");
				if (b < 0) {
					LOG("error: unexpected result");
					continue;
				}
			}
		}
		String key = part.Mid(a, b-a);
		//LOG(key); LOG(part);
		
		bool all_has_lines = true;
		{
			Vector<String> part_lines = Split(part, "\n");
			for(int i = 1; i < part_lines.GetCount(); i++) {
				String& l = part_lines[i];
				if (l.GetCount() && l[0] != '-') {
					all_has_lines = false;
					break;
				}
			}
		}
		
		Vector<String> part_results;
		if (!all_has_lines) {
			part_results = Split(part, "\n-");
			for (String& part_result : part_results) {
				part_result.Replace("\n", " ");
			}
		}
		else {
			part_results = Split(part, "\n");
		}
		if (part_results.IsEmpty())
			continue;
		part_results.Remove(0);
		//DUMPC(part_results);
		
		for (String& part_result : part_results) {
		
			if (part_result.Left(1) == "-")
				part_result = TrimBoth(part_result.Mid(1)); // remove "-"
			
			String bad, better;
			int a = part_result.Find("\"");
			int b = part_result.Find("\" -> \"");
			if (b >= 0) {
				bad = TrimBoth(part_result.Mid(a, b+1-a));
				bad = bad.Mid(1, bad.GetCount()-2);
				better = TrimBoth(part_result.Mid(b+5));
				better = better.Mid(1, better.GetCount()-2);
				
				ProcMsg& err = msgs.Add();
				err.severity = PROCMSG_ERROR;
				err.parts[0] = key;
				err.parts[1] = bad;
				err.parts[2] = better;
				
				Point pt;
				if (finder.Find(bad, pt)) {
					errors << pt;
					err.line = pt.y+1;
					err.col = pt.x+1;
				}
				
			}
			else {
				bad = TrimBoth(part_result.Mid(a));
				if (a == 0)
					bad = bad.Mid(1, bad.GetCount()-2);
				
				if (bad == "None." || bad == "None" || bad == "N/A"|| bad == "etc" ||
					bad == "other better 1" || bad == "other better 2")
					continue;
				
				ProcMsg& err = msgs.Add();
				err.severity = PROCMSG_ERROR;
				err.parts[0] = key;
				err.parts[1] = bad;
				err.parts[2] = "";
				
				Point pt;
				if (finder.Find(bad, pt)) {
					errors << pt;
					err.line = pt.y+1;
					err.col = pt.x+1;
				}
				
			}
		}
	}
	
	this->error.Clear();
	Sort(msgs, ProcMsg());
	for (ProcMsg& m : msgs)
		OnMessage(m);
	
	(have_errors_in_other ? other : main)
		.Errors(std::move(errors));
	
	
	SetBottom(BERRORS);
}


void EditorCtrl::EvaluateAudience() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || audience_evaluation_key.IsEmpty() || main_natural_english_key.IsEmpty())
		return;
	
	audience.Clear();
	audience_data.Clear();
	
	p.RealizePipe();
	
	p.song->data.GetAdd(audience_evaluation_key).Clear();
	
	{
		TaskMgr& m = *p.song->pipe;
		m.EvaluateSongAudience(main_natural_english_key, audience_evaluation_key, audience_mode, THISBACK(OnAudienceEvaluationReady));
	}
}

void EditorCtrl::OnAudienceEvaluationReady() {
	audience_data.Clear();
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || audience_evaluation_key.IsEmpty())
		return;
	
	String results = p.song->data.Get(audience_evaluation_key, "");
	//LOG(results);
	
	{
		results.Replace("\r","");
		Vector<String> lines = Split(results, "\n", false);
		int i = -1;
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			l = TrimBoth(l);
			
			if (l.IsEmpty() && (i == 0 || lines[i-1].IsEmpty())) {
				lines.Remove(i--);
				continue;
			}
			
			// Ensure that there is at least 1 empty line between parts
			if (i > 0 && IsDigit(l[0]) && !lines[i-1].IsEmpty()) {
				lines.Insert(i);
				continue;
			}
		}
		results = Join(lines, "\n", false);
	}
	
	Time now = GetSysTime();
	
	Vector<String> parts = Split(results, "\n\n");
	
	for (String& part : parts) {
		Vector<String> lines = Split(part, "\n");
		
		AudiencePerson& person = audience_data.Add();
		
		if (lines.GetCount() == 2) {
			String header = lines[0];
			String output = lines[1];
			
			{
				int a, b;
				a = header.Find("What ");
				if (a < 0) continue;
				a += 5;
				b = header.Find("(", a);
				if (b < 0) continue;
				person.name = header.Mid(a, b-a);
				
				a=b;
				a = header.Find("born ", a);
				if (a < 0) continue;
				a += 5;
				b = header.Find(",", a);
				if (b < 0) continue;
				person.born = StrInt(header.Mid(a, b-a));
				person.age = now.year - person.born;
				
				a=b;
				a = header.Find("likes ", a);
				if (a < 0) continue;
				a += 6;
				b = header.Find(")", a);
				if (b < 0) continue;
				person.likes = header.Mid(a, b-a);
			}
			
			{
				if (output.Left(1) == "-") output = TrimBoth(output.Mid(1));
				
				int punct = 0;
				WString line = output.ToWString();
				
				if (line.GetCount() >= 2 && line[0] == '\"' && line[line.GetCount()-1] == '\"')
					line = line.Mid(1,line.GetCount()-2);
				
				for(int i = 1; i < line.GetCount(); i++) {
					int chr = line[i];
					if (chr == '!' || chr == '.' || chr == '?') {
						punct = i+1;
						break;
					}
				}
				
				person.reaction = line.Left(punct).ToString();
				person.comment = TrimBoth(line.Mid(punct).ToString());
			}
		}
	}
	
	PostCallback(THISBACK(OnAudienceDataReady));
}

void EditorCtrl::OnAudienceDataReady() {
	for(int i = 0; i < audience_data.GetCount(); i++) {
		AudiencePerson& person = audience_data[i];
		
		audience.Set(i, 0, person.name);
		audience.Set(i, 1, Format(t_("%d (age %d)"), person.born, person.age));
		audience.Set(i, 2, person.likes);
		audience.Set(i, 3, person.reaction);
		audience.Set(i, 4, person.comment);
	}
	audience.SetCount(audience_data.GetCount());
	
	SetBottom(BAUDIENCE);
}
