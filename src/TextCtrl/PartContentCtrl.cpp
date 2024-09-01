#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE

const int PartLineCtrl::indent = 30;


void PartLineCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	int fnt_h = 15;
	Font mono = Monospace(fnt_h);
	Font sans = SansSerif(fnt_h);
	bool focused = IsSelected();
	int off = 2;
	
	// Header
	Color part_bg_clr = White();
	Color sub_bg_clr = Color(225, 227, 255);
	Color line_bg_clr = Color(228, 255, 227);
	Color rest_bg_clr = GrayColor(256-16);
	if (focused) {
		int alpha = 128-64-32;
		part_bg_clr = Blend(part_bg_clr, LtRed(), alpha);
		sub_bg_clr = Blend(sub_bg_clr, LtRed(), alpha);
		line_bg_clr = Blend(line_bg_clr, LtRed(), alpha);
		rest_bg_clr = Blend(rest_bg_clr, LtRed(), alpha);
	}
	
	Rect part_bg_r = RectC(0,0,indent,sz.cy);
	Rect sub_bg_r = RectC(indent,0,indent,sz.cy);
	Rect line_bg_r = RectC(indent*2,0,indent,sz.cy);
	Rect rest_bg_r = RectC(indent*3,0,sz.cx-3*indent,sz.cy);
	d.DrawRect(part_bg_r, part_bg_clr);
	if (sub_i >= 0) d.DrawRect(sub_bg_r, sub_bg_clr);
	if (line_i >= 0) d.DrawRect(line_bg_r, line_bg_clr);
	
	
	
	// Line texts
	const EditorPtrs& p = o.o.GetPointers();
	Script& s = *p.script;
	int part_i = o.o.parts.GetCursor();
	if (part_i < 0)
		return;
	
	DynPart& dp = s.parts[part_i];
	LineElement* el = 0;
	int left = 0;
	if (sub_i < 0 && line_i < 0) {
		String text = "P";
		d.DrawText(off,off,text,mono,Black());
		left = indent*1;
		el = &dp.el;
		d.DrawRect(rest_bg_r, rest_bg_clr);
	}
	else if (sub_i >= 0 && line_i < 0) {
		String text = IntStr(sub_i);
		d.DrawText(indent+off,off,text,mono,Black());
		left = indent*2;
		DynSub& ds = dp.sub[sub_i];
		el = &ds.el;
		d.DrawRect(rest_bg_r, rest_bg_clr);
	}
	else if (sub_i >= 0 && line_i >= 0) {
		String text = IntStr(line_i);
		d.DrawText(indent*2+off,off,text,mono,Black());
		left = indent*3;
		DynSub& ds = dp.sub[sub_i];
		DynLine& dl = ds.lines[line_i];
		el = &dl.el;
		
		rest_bg_clr = Blend(
			el->clr_i >= 0 && el->clr_i < GetColorGroupCount() ? GetGroupColor(el->clr_i) : rest_bg_clr,
			rest_bg_clr,
			256-64);
		d.DrawRect(rest_bg_r, rest_bg_clr);
		
		int y_2 = sz.cy / 2;
		d.DrawText(left+off, y_2+off, dl.text, sans, Black());
	}
	
	
	
	if (el) {
		off = 5;
		int x = left;
		#define RAND_CLR Blend(White(), Color(Random(256),Random(256),Random(256)),128+64)
		Rect r;
		Color element_clr, attr_group_clr, attr_value_clr, clr_clr, act_action_clr, act_arg_clr, typeclass_clr, content_clr;
		element_clr = Color(255, 205, 175);
		attr_group_clr = Color(206, 229, 201);
		attr_value_clr = Color(217, 241, 211);
		clr_clr = el->clr_i >= 0 && el->clr_i < GetColorGroupCount() ? GetGroupColor(el->clr_i) : Color(231, 221, 231);
		act_action_clr = Color(204, 227, 235);
		act_arg_clr = Color(222, 242, 249);
		typeclass_clr = Color(28, 212, 150);
		content_clr = Color(198, 85, 150);
		int mode = DatabaseBrowser::FindMode(el->sorter);
		String tc  = el->typeclass_i >= 0 ? o.o.GetTypeclasses()[el->typeclass_i] : String();
		int con_i = el->con_i / 3;
		int con_mod = el->con_i % 3;
		String con = el->con_i >= 0 ?
			o.o.GetContents()[con_i].key + ": " + o.o.GetContents()[con_i].parts[con_mod] : String();
		if (mode < 0) {
			PaintTextBlock(d, x, off, r, element_clr, el->element, sans);
			PaintTextBlock(d, x, off, r, attr_group_clr, el->attr.group, sans);
			PaintTextBlock(d, x, off, r, attr_value_clr, el->attr.value, sans);
			PaintTextBlock(d, x, off, r, clr_clr, IntStr(el->clr_i), sans);
			PaintTextBlock(d, x, off, r, act_action_clr, el->act.action, sans);
			PaintTextBlock(d, x, off, r, act_arg_clr, el->act.arg, sans);
			PaintTextBlock(d, x, off, r, typeclass_clr, tc, sans);
			PaintTextBlock(d, x, off, r, content_clr, con, sans);
		}
		else {
			Vector<String> parts = Split(DatabaseBrowser::GetModeKey(mode), "_");
			for (const String& p : parts) {
				if (p == "ELEMENT")
					PaintTextBlock(d, x, off, r, element_clr, el->element, sans);
				else if (p == "ATTR") {
					PaintTextBlock(d, x, off, r, attr_group_clr, el->attr.group, sans);
					PaintTextBlock(d, x, off, r, attr_value_clr, el->attr.value, sans);
				}
				else if (p == "COLOR") {
					PaintTextBlock(d, x, off, r, clr_clr, IntStr(el->clr_i), sans);
				}
				else if (p == "ACTION") {
					PaintTextBlock(d, x, off, r, act_action_clr, el->act.action, sans);
					PaintTextBlock(d, x, off, r, act_arg_clr, el->act.arg, sans);
				}
				else if (p == "TYPECLASS") {
					PaintTextBlock(d, x, off, r, typeclass_clr, tc, sans);
				}
				else if (p == "CONTENT") {
					PaintTextBlock(d, x, off, r, content_clr, con, sans);
				}
				else TODO
			}
		}
	}
	
	
	// Focus highlight
	if (focused) {
		Color c = LtRed();
		int y = sz.cy-1;
		d.DrawLine(0,0,sz.cx-1,0,1,c);
		d.DrawLine(0,y,sz.cx-1,y,1,c);
	}
}

void PartLineCtrl::PaintTextBlock(Draw& d, int& x, int off, Rect& out, Color bg, const String& txt, const Font& fnt) {
	if (txt == "All" || txt == "-1" || txt.IsEmpty())
		return;
	
	Size sz = GetTextSize(txt, fnt);
	sz.cx += off * 2;
	sz.cx = max(sz.cx, 30);
	Rect r = RectC(x,0,sz.cx,sz.cy);
	d.DrawRect(r, bg);
	
	Color border = Blend(bg, Black());
	d.DrawLine(x,0, x+sz.cx-1,0, 1, border);
	d.DrawLine(x,sz.cy-1, x+sz.cx-1,sz.cy-1, 1, border);
	d.DrawLine(x,0, x,sz.cy-1, 1, border);
	d.DrawLine(x+sz.cx-1,0, x+sz.cx-1,sz.cy, 1, border);
	
	d.DrawText(x+off,0, txt, fnt, Black());
	x += sz.cx + 5;
}

void PartLineCtrl::LeftDown(Point p, dword keyflags) {
	Select();
	//Refresh();
	SetFocus();
}

void PartLineCtrl::Select() {
	o.Select(this);
	Refresh();
	o.WhenCursor();
}

void PartLineCtrl::GotFocus() {
	Select();
}

void PartLineCtrl::LostFocus() {
	Refresh();
}

bool PartLineCtrl::Key(dword key, int count) {
	
	if (key == K_UP) {
		o.MoveSelection(-1);
		return true;
	}
	if (key == K_DOWN) {
		o.MoveSelection(+1);
		return true;
	}
	
	return false;
}

bool PartLineCtrl::IsSelected() const {
	return o.GetCursor() == o.Find(this);
}

LineElement* PartLineCtrl::GetLineEl() const {
	const EditorPtrs& p = o.o.GetPointers();
	Script& s = *p.script;
	int part_i = o.o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	LineElement* el = 0;
	if (sub_i < 0 && line_i < 0) {
		String text = "P";
		el = &dp.el;
	}
	else if (sub_i >= 0 && line_i < 0) {
		DynSub& ds = dp.sub[sub_i];
		el = &ds.el;
	}
	else if (sub_i >= 0 && line_i >= 0) {
		DynSub& ds = dp.sub[sub_i];
		DynLine& dl = ds.lines[line_i];
		el = &dl.el;
	}
	return el;
}

DynLine* PartLineCtrl::GetDynLine() const {
	const EditorPtrs& p = o.o.GetPointers();
	Script& s = *p.script;
	int part_i = o.o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	if (sub_i >= 0 && line_i >= 0) {
		DynSub& ds = dp.sub[sub_i];
		DynLine& dl = ds.lines[line_i];
		return &dl;
	}
	return 0;
}

PartLineCtrl::PartLineCtrl(PartContentCtrl& o) : o(o) {
	NoWantFocus();
	
}









PartContentCtrl::PartContentCtrl(ScriptReferenceMakerCtrl& o) : o(o) {
	AddFrame(scroll);
	
	scroll.WhenScroll << THISBACK(Layout);
}

void PartContentCtrl::Paint(Draw& d) {
	
}

int PartContentCtrl::GetCursor() const {
	return selected_line;
}

int PartContentCtrl::Find(const PartLineCtrl* line) const {
	for(int i = 0; i < lines.GetCount(); i++)
		if (&lines[i] == line)
			return i;
	return -1;
}

void PartContentCtrl::Layout() {
	int y = -scroll;
	int w = GetSize().cx;
	for(int i = 0; i < lines.GetCount(); i++) {
		PartLineCtrl& line = lines[i];
		int h = line.line_i < 0 ? lh : lh * 2;
		Rect r = RectC(0, y, w, h);
		line.SetRect(r);
		
		y += h;
	}
}
void PartContentCtrl::Select(PartLineCtrl* line) {
	int prev_sel = selected_line;
	if (line) for(int i = 0; i < lines.GetCount(); i++) {
		if (line == &lines[i]) {
			selected_line = i;
			break;
		}
	}
	else selected_line = -1;
	
	if (prev_sel >= 0 && prev_sel < lines.GetCount())
		lines[prev_sel].Refresh();
	if (selected_line >= 0 && selected_line < lines.GetCount())
		lines[selected_line].Refresh();
}


void PartContentCtrl::MoveSelection(int diff) {
	int prev_sel = selected_line;
	selected_line += diff;
	selected_line = max(0, min(lines.GetCount()-1, selected_line));
	if (prev_sel >= 0 && prev_sel < lines.GetCount())
		lines[prev_sel].Refresh();
	if (selected_line >= 0 && selected_line < lines.GetCount()) {
		lines[selected_line].Refresh();
	}
	WhenCursor();
}

void PartContentCtrl::InitDefault(PartLineCtrl& l) {
	/*AddElements(l.element);
	
	int appmode = o.GetAppMode();
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	
	if (l.attr.GetCount() == 0) {
		for(int i = 0; i < b.attrs.GetCount(); i++) {
			const auto& ah = b.attrs[i];
			l.attr.Add(Capitalize(ah.group) + " / " + Capitalize(ah.value));
		}
	}
	
	if (l.clr.GetCount() == 0) {
		for(int i = 0; i < b.colors.GetCount(); i++) {
			const auto& c = b.colors[i];
			AttrText at(c.name);
			at	.NormalPaper(c.clr).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White());
			l.clr.Add(at);
		}
	}
	
	if (l.action.GetCount() == 0) {
		for(int i = 0; i < b.actions.GetCount(); i++) {
			l.action.Add(b.actions[i].action);
		}
		PartLineCtrl* p = &l;
		l.action.WhenAction = [this,p]{
			OnLineValueChange(p);
			DataSelAction(p);
		};
	}
	
	
	if (l.typeclass.GetCount() == 0) {
		l.typeclass.Add("");
		const auto& tcs = GetTypeclasses(appmode);
		for(int i = 0; i < tcs.GetCount(); i++) {
			l.typeclass.Add(tcs[i]);
		}
	}
	
	if (l.content.GetCount() == 0) {
		l.content.Add("");
		const auto& cons = GetContents(appmode);
		for(int i = 0; i < cons.GetCount(); i++) {
			l.content.Add(cons[i].key);
		}
	}
	
	l.attr			<<= THISBACK1(OnLineValueChange, &l);
	l.clr			<<= THISBACK1(OnLineValueChange, &l);
	//NO l.action		<<= THISBACK1(OnLineValueChange, &l);
	l.action_arg	<<= THISBACK1(OnLineValueChange, &l);
	l.typeclass		<<= THISBACK1(OnLineValueChange, &l);
	l.content		<<= THISBACK1(OnLineValueChange, &l);
	*/
}

void PartContentCtrl::DataLine(PartLineCtrl& pl) {
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	LineElement* el = 0;
	if (pl.sub_i < 0 && pl.line_i < 0) {
		el = &dp.el;
	}
	else if (pl.sub_i >= 0) {
		DynSub& ds = dp.sub[pl.sub_i];
		if (pl.line_i < 0) {
			el = &ds.el;
		}
		else {
			DynLine& dl = ds.lines[pl.line_i];
			el = &dl.el;
		}
	}
	if (!el)
		return;
	
	// Set attr
	/*int attr_i = pl.attr.GetIndex();
	if (attr_i <= 0) {
		el->attr.group = "";
		el->attr.value = "";
	}
	else {
		el->attr = b.attrs[attr_i].GetAttrHeader();
	}
	
	#define SET_INDEX(IDX, VAL) if (VAL+1 >= 0 && VAL+1 < IDX.GetCount()) IDX.SetIndex(VAL+1)

	// Set color
	SET_INDEX(pl.clr, el->clr_i);
	
	// Set action group
	int action_i = b.FindAction(el->act.action);
	SET_INDEX(pl.action, action_i);
	DataSelAction(&pl);
	int arg_i = b.FindArg(el->act.arg);
	SET_INDEX(pl.action_arg, arg_i);
	SET_INDEX(pl.typeclass, el->typeclass_i);
	SET_INDEX(pl.content, el->content_i);
	#undef SET_INDEX*/
}

void PartContentCtrl::OnLineValueChange(PartLineCtrl* l_) {
	PartLineCtrl& pl = *l_;
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	LineElement* el = 0;
	if (pl.sub_i < 0 && pl.line_i < 0) {
		el = &dp.el;
	}
	else if (pl.sub_i >= 0) {
		DynSub& ds = dp.sub[pl.sub_i];
		if (pl.line_i < 0) {
			el = &ds.el;
		}
		else {
			DynLine& dl = ds.lines[pl.line_i];
			el = &dl.el;
		}
	}
	if (!el)
		return;
	
	// set attr
	/*int attr_i = pl.attr.GetIndex();
	if (attr_i <= 0)
		el->attr = AttrHeader();
	else
		el->attr = b.attrs[attr_i].GetAttrHeader();
	
	// Set action
	int act_i = pl.action.GetIndex();
	if (act_i <= 0)
		el->act.action = "";
	else
		el->act.action = b.actions[act_i].action;
	
	// Set action arg
	int arg_i = pl.action_arg.GetIndex();
	if (arg_i <= 0)
		el->act.arg = "";
	else
		el->act.arg = b.args[arg_i].arg;
	
	#define SET_INDEX(IDX, VAL) VAL = IDX.GetIndex()-1;
	SET_INDEX(pl.clr, el->clr_i);
	SET_INDEX(pl.typeclass, el->typeclass_i);
	SET_INDEX(pl.content, el->content_i);
	#undef SET_INDEX*/
}

void PartContentCtrl::DataSelAction(PartLineCtrl* l_) {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	PartLineCtrl& l = *l_;
	
	
	/*b.SetGroup(l.action.GetIndex());
	
	l.action_arg.Clear();
	l.action_arg.Add("");
	for(int i = 0; i < b.actions.GetCount(); i++) {
		l.action_arg.Add(b.actions[i].action);
	}
	*/
}

void PartContentCtrl::Data() {
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	const DynPart& dp = s.parts[part_i];
	
	for (auto& e : lines)
		RemoveChild(&e);
	lines.Clear();
	
	int h = 0;
	
	{
		PartLineCtrl& line = lines.Add(new PartLineCtrl(*this));
		Add(line);
		line.sub_i = -1;
		
		InitDefault(line);
		//line.element.SetIndex(FindElement(dp.element)+1);
		//line.element <<= THISBACK3(OnElementChange, -1, -1, &line.element);
		
		h += lh;
	}
	
	for(int i = 0; i < dp.sub.GetCount(); i++) {
		const DynSub& ds = dp.sub[i];
		{
			PartLineCtrl& line = lines.Add(new PartLineCtrl(*this));
			Add(line);
			line.sub_i = i;
			
			InitDefault(line);
			//line.element.SetIndex(FindElement(ds.element0)+1);
			//line.element <<= THISBACK3(OnElementChange, i, -1, &line.element);
			
			h += lh;
		}
		
		for(int j = 0; j < ds.lines.GetCount(); j++) {
			const DynLine& dl = ds.lines[j];
			PartLineCtrl& line = lines.Add(new PartLineCtrl(*this));
			Add(line);
			line.sub_i = i;
			line.line_i = j;
			
			InitDefault(line);
			//line.element.SetIndex(FindElement(dl.element)+1);
			//line.element <<= THISBACK3(OnElementChange, i, j, &line.element);
			h += lh*2;
		}
	}
	
	for(int i = 0; i < lines.GetCount(); i++) {
		DataLine(lines[i]);
	}
	
	scroll.SetTotal(h);
	scroll.SetPage(GetSize().cy);
	
	Layout();
}

void PartContentCtrl::AddElements(DropList& dl) {
	#if 0
	const Vector<String>& el = GetElements();
	#else
	if (element_keys.IsEmpty()) {
		TextDatabase& db = o.GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		const auto& el = da.element_keys.GetKeys();
		element_keys <<= el;
		Sort(element_keys, StdLess<String>());
	}
	const auto& el = element_keys;
	#endif
	
	dl.Add("");
	for (const auto& e : el)
		dl.Add(Capitalize(e));
}

int PartContentCtrl::FindElement(const String& s) {
	const Vector<String>& el = element_keys;// GetElements();
	String e = ToLower(s);
	for(int i = 0; i < el.GetCount(); i++) {
		if (el[i] == e)
			return i;
	}
	return -1;
}

void PartContentCtrl::OnElementChange(int sub_i, int line_i, DropList* dl) {
	const EditorPtrs& p = o.GetPointers();
	if (!p.script || !o.parts.IsCursor())
		return;
	Script& s = *p.script;
	int part_i = o.parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	if (sub_i < 0) {
		dp.element = ToLower((String)dl->GetKey(dl->GetIndex()));
	}
	else {
		DynSub& ds = dp.sub[sub_i];
		if (line_i < 0)
			ds.element0 = ToLower((String)dl->GetKey(dl->GetIndex()));
		else
			ds.lines[line_i].element = ToLower((String)dl->GetKey(dl->GetIndex()));
	}
}

PartLineCtrl* PartContentCtrl::GetActiveLine() {
	if (selected_line >= 0 && selected_line < lines.GetCount())
		return &lines[selected_line];
	return 0;
}


END_TEXTLIB_NAMESPACE

