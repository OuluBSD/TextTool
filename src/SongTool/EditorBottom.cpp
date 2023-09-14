#include "SongTool.h"


void EditorCtrl::BTabs()
{
	btabs.Clear();
	btabs.Add(AppImg::close, t_("Close (Esc)"));
	btabs.Add(AppImg::console, t_("Console"));
	btabs.Add(AppImg::errors, t_("Errors"));
	btabs.Add(AppImg::audience, t_("Audience"));
	btabs.Add(AppImg::console2, t_("Find in files"));
	btabs.Add(AppImg::calc, t_("Calculator"));
	if(bottomctrl)
		btabs.Add(AppImg::debug, t_("Debug"));
}

void EditorCtrl::SyncBottom()
{
	int q = btabs.GetCursor();
	if(q == BCLOSE) {
		HideBottom();
		return;
	}
	if(editor_bottom.GetZoom() >= 0)
		editor_bottom.NoZoom();
	console.Show(q == BCONSOLE);
	error.Show(q == BERRORS);
	ffound.Show(q == BFINDINFILES);
	audience.Show(q == BAUDIENCE);
	calc.Show(q == BCALC);
	if(bottomctrl)
		bottomctrl->Show(q == BDEBUG);
	//calc.LoadHlStyles(editor.StoreHlStyles());
	//calc.SetFont(editorfont);
	//SetBar();
	if(q == BCALC)
		GetTopWindow()->ActiveFocus(calc);
}

void EditorCtrl::SetBottom(int i)
{
	btabs.SetCursor(i);
	SyncBottom();
}

void EditorCtrl::ShowBottom(int i)
{
	if(i == btabs.GetCursor() && IsBottomShown())
		return;
	SetBottom(i);
}

void EditorCtrl::ToggleBottom(int i)
{
	if(i == btabs.GetCursor() && IsBottomShown())
		HideBottom();
	else
		SetBottom(i);
}

bool EditorCtrl::IsBottomShown() const
{
	return editor_bottom.GetZoom() < 0;
}

void EditorCtrl::SwapBottom()
{
	if(IsBottomShown())
		HideBottom();
	else {
		if(btabs.GetCursor() <= 0)
			btabs.SetCursor(1);
		SyncBottom();
	}
}

void EditorCtrl::HideBottom() {
	editor_bottom.Zoom(0);
	//files.SetWantFocus();
	//SetBar();
}

void EditorCtrl::IdeSetBottom(Ctrl& ctrl)
{
	bottom.Add(ctrl.SizePos());
	bottomctrl = &ctrl;
	BTabs();
	SetBottom(BDEBUG);
}

void EditorCtrl::IdeRemoveBottom(Ctrl& ctrl)
{
	ctrl.Remove();
	bottomctrl = NULL;
	BTabs();
	HideBottom();
}

void EditorCtrl::IdeActivateBottom()
{
	SetBottom(BDEBUG);
}

void EditorCtrl::SetupError(ArrayCtrl& error, const char *s)
{
	error.AddColumn(t_("File")).SetDisplay(Single<EditorCtrl::FoundFileDisplay>());
	error.AddColumn(t_("Line")).SetDisplay(Single<EditorCtrl::TopAlignedDisplay>());
	if (have_group_bad_better) {
		error.AddColumn(t_("Category"));
		error.AddColumn(t_("Bad"));
		error.AddColumn(t_("Better"));
		error.ColumnWidths("20 10 40 100 100");
	}
	else {
		error.AddColumn(s);
		error.ColumnWidths("184 44 298");
	}
	error.AddIndex("INFO");
	error.NoWantFocus();
	error.WhenAction = THISBACK1(FocusLine, &error);
}

void EditorCtrl::SetupAudience(ArrayCtrl& audience)
{
	audience.AddColumn(t_("Name"));
	audience.AddColumn(t_("Birthyear (age)"));
	audience.AddColumn(t_("Favourite Genre"));
	audience.AddColumn(t_("Reaction"));
	audience.AddColumn(t_("Comment"));
	audience.NoWantFocus();
	audience.ColumnWidths("2 2 2 4 20");
}

void EditorCtrl::ErrorMenu(Bar& bar)
{
	bar.Add(error.IsCursor(), t_("Copy"), THISBACK1(CopyError, false));
	bar.Add(t_("Copy all"), THISBACK1(CopyError, true));
}


void EditorCtrl::FFoundMenu(Bar& bar)
{
	bar.Add(t_("Copy text"), THISBACK1(CopyFound, false));
	bar.Add(t_("Copy all"), THISBACK1(CopyFound, true));
}

void EditorCtrl::SelError()
{
	if(removing_notes)
		return;
	if(error.IsCursor()) {
		Value v = error.Get("NOTES");
		if(v != "0") {
			int sc = error.GetScroll();
			removing_notes = true;
			for(int i = error.GetCount() - 1; i >= 0; i--)
				if(error.Get(i, "NOTES") == "0")
					error.Remove(i);
			removing_notes = false;
			error.ScrollTo(sc);
			error.ScrollIntoCursor();
			ValueArray n = v;
			int ii = error.GetCursor();
			for(int i = 0; i < n.GetCount(); i++) {
				const ErrorInfo& f = ValueTo<ErrorInfo>(n[i]);
				error.Insert(++ii);
				error.Set(ii, 0, f.file);
				error.Set(ii, 1, f.lineno);
				int linecy;
				if (have_group_bad_better) {
					if(f.error_pos.GetCount()) {
						error.Set(ii, 2, FormatErrorLineEP(f.message, f.error_pos, linecy));
						error.SetDisplay(ii, 2, Single<ElepDisplay>());
					}
					else
						error.Set(ii, 2, FormatErrorLine(f.message, linecy));
				}
				else {
					error.Set(ii, 2, f.parts[0]);
					error.Set(ii, 3, f.parts[1]);
					error.Set(ii, 4, f.parts[2]);
				}
				error.Set(ii, "INFO", n[i]);
				error.Set(ii, "NOTES", "0");
				error.SetLineCy(ii, linecy);
			}
		}
		GoToError(error);
	}
}

void EditorCtrl::ShowError()
{
	if(error.IsCursor())
		GoToError(error);
}

void EditorCtrl::ShowFound()
{
	if(ffound.IsCursor())
		GoToError(ffound);
}

void EditorCtrl::CopyError(bool all)
{
	int c = error.GetCursor();
	if(!all && c < 0)
		return;
	String txt;
	int h = all ? error.GetCount() : error.GetCursor() + 1;
	for(int i = all ? 0 : c; i < h; i++)
		txt << error.Get(i, 0) << " (" << error.Get(i, 1) << "): " << error.Get(i, 2) << "\r\n";
	WriteClipboardText(txt);
}

void EditorCtrl::CopyFound(bool all)
{
	String txt;
	for(int i = 0; i < ffound.GetCount(); i++) {
		if(all)
			txt << ffound.Get(i, 0) << " (" << ffound.Get(i, 1) << "): ";
		String h = ffound.Get(i, 2);
		if(*h == '\1')
			h = Split(~h + 1, '\1', false).Top();
		txt << h << "\r\n";
	}
	WriteClipboardText(txt);
}

void EditorCtrl::GoToError(const ErrorInfo& f) {
	if(IsNull(f.file))
		return;
	
	SetTabText(main.HasFocus(), f.file, f.lineno-1, f.linepos-1);
}

void EditorCtrl::GoToError(ArrayCtrl& a) {
	Value v = a.Get("INFO");
	if(v.Is<ErrorInfo>())
		GoToError(ValueTo<ErrorInfo>(v));
}


bool IsDarkMismatch() {
	return IsDark(SColorPaper()) != IsDark(HighlightSetup::GetHlStyle(HighlightSetup::PAPER_NORMAL).color);
}


void EditorCtrl::FoundDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	String s = q;
	if(*s == '\1') {
		Vector<String> h = Split(~s + 1, '\1', false);
		if(h.GetCount() < 4)
			return;
		One<EditorSyntax> es = EditorSyntax::Create(h[0]);
		es->IgnoreErrors();
		WString ln = h[3].ToWString();
		Vector<LineEdit::Highlight> hln;
		hln.SetCount(ln.GetCount() + 1);
		for(int i = 0; i < ln.GetCount(); i++) {
			LineEdit::Highlight& h = hln[i];
			h.paper = paper;
			h.ink = SColorText();
			h.chr = ln[i];
			h.font = StdFont();
		}
		HighlightOutput hl(hln);
		es->Highlight(ln.Begin(), ln.End(), hl, NULL, 0, 0);
		int fcy = GetStdFontCy();
		int y = r.top + (r.GetHeight() - fcy) / 2;
		int x = r.left;
		w.DrawRect(r, paper);
		int sl = Utf8Len(~ln, atoi(h[1]));
		int sh = Utf8Len(~ln + sl, atoi(h[2])) + sl;
		for(int i = 0; i < hln.GetCount(); i++) {
			Font fnt = StdFont();
			LineEdit::Highlight& h = hln[i];
			fnt.Bold(h.font.IsBold());
			fnt.Italic(h.font.IsItalic());
			fnt.Underline(h.font.IsUnderline());
			int cw = fnt[h.chr];
			if(h.chr == '\t')
				cw = 4 * fnt[' '];
			Color hpaper = HighlightSetup::GetHlStyle(HighlightSetup::PAPER_SELWORD).color;
			Color hink = h.ink;
			if(IsDarkMismatch()) {
				hpaper = paper;
				hink = ink;
			}
			if(i >= sl && i < sh && !(style & (CURSOR|SELECT|READONLY)))
				w.DrawRect(x, y, cw, fcy, hpaper);
			if(h.chr != '\t')
				w.DrawText(x, y, &h.chr, fnt, hink, 1);
			x += cw;
		}
	}
	else
		StdDisplay().Paint(w, r, q, ink, paper, style);
}

void EditorCtrl::TopAlignedDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	w.DrawText(r.left, r.top, AsString(q));
}

void PaintTeXt(Draw& w, int& x, int y, String text, Font font, Color ink)
{
	Size sz = GetTextSize(text, font);
	w.DrawText(x, y, text, font, ink);
	x += sz.cx;
}

int DrawFileName0(Draw& w, const Rect& r, String h, Color ink, int x)
{
	if(h.GetCount() == 0)
		return 0;
	int q = h.Find("\xff");
	String ns;
	String fn = h;
	if(q >= 0) {
		ns = h.Mid(0, q) + ' ';
		fn = h.Mid(q + 1);
	}
	String s = GetFileName(GetFileFolder(h)) + "/";
	x += r.left;
	if(ns.GetCount()) {
		PaintTeXt(w, x, r.top, ns, StdFont().Bold(), ink);
		PaintTeXt(w, x, r.top, "(", StdFont(), ink);
	}
	PaintTeXt(w, x, r.top, s, StdFont(), ink);
	s = GetFileName(h);
	PaintTeXt(w, x, r.top, s, StdFont().Bold(), ink);
	if(ns.GetCount())
		PaintTeXt(w, x, r.top, ")", StdFont(), ink);
	return x - r.left;
}

Size GetDrawFileNameSize(String h)
{
	NilDraw w;
	return Size(DrawFileName0(w, Size(999999, 999999), h, Null, 0), StdFont().Bold().GetCy());
}

void DrawFileName(Draw& w, const Rect& r, String h, Color ink)
{
	DrawFileName0(w, r, h, ink, min(r.GetWidth() - GetDrawFileNameSize(h).cx, 0));
}

void EditorCtrl::FoundFileDisplay::Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	w.DrawRect(r, paper);
	DrawFileName(w, r, q, ink);
}

Size EditorCtrl::FoundFileDisplay::GetStdSize(const Value& q) const
{
	return GetDrawFileNameSize(q);
}

WString EditorCtrl::FormatErrorLine(String text, int& linecy)
{
	WString txt;
	int cx = error.HeaderObject().GetTabWidth(2) - error.HeaderTab(2).GetMargin() * 2;
	int x = 0;
	Font fnt = StdFont();
	WString h = text.ToWString();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0)
		s++;
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.Cat('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		txt.Cat(*s);
		x += chcx;
		s++;
	}
	return txt;
}

WString EditorCtrl::FormatErrorLineEP(String text, const char *ep, int& linecy)
{
	WString txt;
	int cx = error.HeaderObject().GetTabWidth(2) - error.HeaderTab(2).GetMargin() * 2;
	int x = 0;
	Font fnt = StdFont();
	WString h = text.ToWString();
	linecy = fnt.GetCy();
	const wchar *s = h;
	while(findarg(*s, ' ', '\t') >= 0) {
		s++;
		if(*ep)
			ep++;
	}
	int lep = ' ';
	while(*s) {
		int chcx = fnt[*s];
		if(x + chcx > cx) {
			txt.Cat('\n');
			x = 0;
			linecy += fnt.GetCy();
		}
		if(lep != *ep) {
			txt.Cat(decode(*ep, '~', 2, '^', 3, 1));
			lep = *ep;
		}
		txt.Cat(*s);
		x += chcx;
		s++;
		if(*ep)
			ep++;
	}
	return txt;
}


