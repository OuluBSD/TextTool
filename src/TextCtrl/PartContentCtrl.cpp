#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE

const int PartLineHeader::indent = 30;

void PartLineHeader::Paint(Draw& d) {
	Size sz = GetSize();
	int fnt_h = 15;
	Font fnt = Monospace(fnt_h);
	
	Color part_bg_clr = White();
	Color sub_bg_clr = Color(225, 227, 255);
	Color line_bg_clr = Color(228, 255, 227);
	
	Rect part_bg_r = RectC(0,0,PartLineHeader::indent,sz.cy);
	Rect sub_bg_r = RectC(PartLineHeader::indent,0,PartLineHeader::indent,sz.cy);
	Rect line_bg_r = RectC(PartLineHeader::indent*2,0,PartLineHeader::indent,sz.cy);
	d.DrawRect(part_bg_r, part_bg_clr);
	d.DrawRect(sub_bg_r, sub_bg_clr);
	d.DrawRect(line_bg_r, line_bg_clr);
	
	if (o.sub_i < 0 && o.line_i < 0) {
		String text = "P";
		d.DrawText(2,2,text,fnt,Black());
	}
	else if (o.sub_i >= 0 && o.line_i < 0) {
		String text = IntStr(o.sub_i);
		d.DrawText(PartLineHeader::indent+2,2,text,fnt,Black());
	}
	else if (o.sub_i >= 0 && o.line_i >= 0) {
		String text = IntStr(o.line_i);
		d.DrawText(PartLineHeader::indent*2+2,2,text,fnt,Black());
	}
	
	if (HasFocus() || o.HasFocusDeep()) {
		Color c = LtRed();
		int y = sz.cy-1;
		d.DrawLine(0,0,sz.cx-1,0,1,c);
		d.DrawLine(0,y,sz.cx-1,y,1,c);
	}
}

void PartLineHeader::LeftDown(Point p, dword keyflags) {
	SetFocus();
	Refresh();
}

void PartLineHeader::LostFocus() {
	Refresh();
}

bool PartLineHeader::Key(dword key, int count) {
	
	if (key == K_UP) {
		o.o.MoveFocus(-1);
		return true;
	}
	if (key == K_DOWN) {
		o.o.MoveFocus(+1);
		return true;
	}
	
	return false;
}



PartLineCtrl::PartLineCtrl(PartContentCtrl& o) : o(o), header(*this) {
	NoWantFocus();
	
	header.WantFocus();
	
	int part = 10000 / 8;
	split.Horz()
		<< element
		<< attr
		<< clr
		<< action << action_arg
		<< typeclass
		<< content;
	for(int i = 0; i <= 1; i++)
		split.SetPos((1+i)*part, i);
	for(int i = 2; i < 1; i++)
		split.SetPos((2+i)*part, i);
	
	
	#define HIDE_BUTTON(x) x.GetButton(0).Show(false)
	HIDE_BUTTON(element);
	HIDE_BUTTON(attr);
	HIDE_BUTTON(clr);
	HIDE_BUTTON(action);
	HIDE_BUTTON(action_arg);
	HIDE_BUTTON(typeclass);
	HIDE_BUTTON(content);
	#undef HIDE_BUTTON
	
	Add(header);
	Add(split);
}

void PartLineCtrl::ChildGotFocus() {
	header.Refresh();
	o.WhenCursor();
}

void PartLineCtrl::ChildLostFocus() {
	header.Refresh();
}

void PartLineCtrl::Layout() {
	Size sz = GetSize();
	
	int left_off = 0;
	if (sub_i < 0) {
		left_off = PartLineHeader::indent;
	}
	else {
		if (line_i < 0)
			left_off = PartLineHeader::indent * 2;
		else
			left_off = PartLineHeader::indent * 3;
	}
	
	int w = sz.cx - left_off;
	
	header.LeftPos(0, left_off).VSizePos();
	
	if (line_i < 0)
		split.HSizePos(left_off,0);
	else {
		int cy_2 = sz.cy / 2;
		split.HSizePos(left_off,0).TopPos(0,cy_2);
	}
}








PartContentCtrl::PartContentCtrl(ScriptReferenceMakerCtrl& o) : o(o) {
	AddFrame(scroll);
	
	scroll.WhenScroll << THISBACK(Layout);
}

void PartContentCtrl::Paint(Draw& d) {
	
}

int PartContentCtrl::GetCursor() const {
	for(int i = 0; i < lines.GetCount(); i++)
		if (lines[i].HasFocusDeep())
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

void PartContentCtrl::MoveFocus(int diff) {
	for(int i = 0; i < lines.GetCount(); i++) {
		if (lines[i].HasFocusDeep()) {
			int new_i = i + diff;
			if (new_i >= 0 && new_i < lines.GetCount())
				lines[new_i].header.SetFocus();
			break;
		}
	}
}

void PartContentCtrl::InitDefault(PartLineCtrl& l) {
	AddElements(l.element);
	
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
	int attr_i = pl.attr.GetIndex();
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
	#undef SET_INDEX
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
	int attr_i = pl.attr.GetIndex();
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
	#undef SET_INDEX
}

void PartContentCtrl::DataSelAction(PartLineCtrl* l_) {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	PartLineCtrl& l = *l_;
	
	b.SetGroup(l.action.GetIndex());
	
	l.action_arg.Clear();
	l.action_arg.Add("");
	for(int i = 0; i < b.actions.GetCount(); i++) {
		l.action_arg.Add(b.actions[i].action);
	}
	
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
		line.element.SetIndex(FindElement(dp.element)+1);
		line.element <<= THISBACK3(OnElementChange, -1, -1, &line.element);
		
		h += lh;
	}
	
	for(int i = 0; i < dp.sub.GetCount(); i++) {
		const DynSub& ds = dp.sub[i];
		{
			PartLineCtrl& line = lines.Add(new PartLineCtrl(*this));
			Add(line);
			line.sub_i = i;
			
			InitDefault(line);
			line.element.SetIndex(FindElement(ds.element0)+1);
			line.element <<= THISBACK3(OnElementChange, i, -1, &line.element);
			
			h += lh;
		}
		
		for(int j = 0; j < ds.lines.GetCount(); j++) {
			const DynLine& dl = ds.lines[j];
			PartLineCtrl& line = lines.Add(new PartLineCtrl(*this));
			Add(line);
			line.sub_i = i;
			line.line_i = j;
			
			InitDefault(line);
			line.element.SetIndex(FindElement(dl.element)+1);
			line.element <<= THISBACK3(OnElementChange, i, j, &line.element);
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


END_TEXTLIB_NAMESPACE

