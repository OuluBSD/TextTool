#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


StructuredScriptEditor::StructuredScriptEditor() {
	WantFocus();
	line_h = 15;
	Ctrl::AddFrame(scroll_v);
	scroll_v.WhenScroll << [this]() {
		Refresh();
	};
}

void StructuredScriptEditor::Update() {
	if (!owner || !HasAnyEditor()) {Refresh(); return;}
	
	int total_h = 0;
	Script& s = owner->GetScript();
	for(int i = 0; i < s.parts.GetCount(); i++) {
		const DynPart& dp = s.parts[i];
		total_h += line_h;
		for(int j = 0; j < dp.sub.GetCount(); j++) {
			const DynSub& ds = dp.sub[j];
			total_h += line_h;
			for(int k = 0; k < ds.lines.GetCount(); k++) {
				const DynLine& dl = ds.lines[k];
				total_h += line_h;
			}
		}
		total_h += line_h;
	}
	scroll_v.SetTotal(total_h);
}

void StructuredScriptEditor::Layout() {
	Size sz = GetSize();
	scroll_v.SetPage(sz.cy);
	scroll_v.SetLine(line_h);
	Refresh();
}

void StructuredScriptEditor::MouseWheel(Point p, int zdelta, dword keyflags) {
	scroll_v.Wheel(zdelta);
}

void StructuredScriptEditor::LeftDown(Point p, dword keyflags) {
	for (const Area& a : areas) {
		if (a.r.Contains(p)) {
			selected_line = a.selected_line;
			selected_part = a.selected_part;
			selected_sub = a.selected_sub;
			SetFocus();
			Refresh();
			WhenCursor();
			return;
		}
	}
	selected_line = 0;
	selected_part = 0;
	selected_sub = 0;
	SetFocus();
	Refresh();
	WhenCursor();
}

bool StructuredScriptEditor::IsAnySelected() const {
	if (selected_line == 0 &&
		selected_part == 0 &&
		selected_sub  == 0)
		return false;
	else
		return true;
}

bool StructuredScriptEditor::Key(dword key, int count) {
	if (IsAnySelected()) {
		if (key == K_UP) {
			MoveSelected(-1);
			return true;
		}
		if (key == K_DOWN) {
			MoveSelected(+1);
			return true;
		}
		if (key == K_HOME) {
			MoveSelected(-1000);
			return true;
		}
		if (key == K_END) {
			MoveSelected(+1000);
			return true;
		}
		if (key == K_PAGEUP) {
			MoveSelected(-10);
			return true;
		}
		if (key == K_PAGEDOWN) {
			MoveSelected(+10);
			return true;
		}
	}
	return scroll_v.Key(key, count);
}

void StructuredScriptEditor::MoveSelected(int i) {
	if (!IsAnySelected()) {
		int new_sel = i >= 0 ? 0 : vert_areas.GetCount()-1;
		if (new_sel < 0 || new_sel >= vert_areas.GetCount())
			return;
		const Area& a = vert_areas[new_sel];
		selected_line = a.selected_line;
		selected_part = a.selected_part;
		selected_sub = a.selected_sub;
		ScrollView(a.r);
		Refresh();
		WhenCursor();
		return;
	}
	else {
		int j = 0;
		for (const Area& a : vert_areas) {
			if ((selected_line && a.selected_line == selected_line) ||
				(selected_part && a.selected_part == selected_part) ||
				(selected_sub  && a.selected_sub  == selected_sub)) {
				int new_sel = j + i;
				new_sel = min(vert_areas.GetCount()-1, max(0, new_sel));
				if (new_sel < 0 || new_sel >= vert_areas.GetCount())
					return;
				const Area& a1 = vert_areas[new_sel];
				selected_line = a1.selected_line;
				selected_part = a1.selected_part;
				selected_sub = a1.selected_sub;
				ScrollView(a1.r);
				Refresh();
				WhenCursor();
				return;
			}
			j++;
		}
	}
}

void StructuredScriptEditor::ScrollView(const Rect& r) {
	Size sz = GetSize();
	if (r.top < 0) {
		int y = scroll_v + r.top;
		scroll_v.ScrollInto(y);
	}
	else if (r.bottom >= sz.cy) {
		int y = scroll_v + r.bottom;// - sz.cy;
		scroll_v.ScrollInto(y);
	}
}

void StructuredScriptEditor::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	Font fnt = Monospace(line_h-3);
	Script& s = owner->GetScript();
	int y = -scroll_v;
	int indent_cx = 30;
	Color part_bg = Color(233, 235, 255);
	Color part_border = Color(203, 205, 225);
	Color sub_bg = Color(241, 254, 240);
	Color line_bg = Color(253, 242, 241);
	Color sel_clr = LtRed();
	
	areas.Clear();
	vert_areas.Clear();
	for(int i = 0; i < s.parts.GetCount(); i++) {
		const DynPart& dp = s.parts[i];
		
		String txt = GetTextTypeString(dp.text_type) + " " + IntStr(dp.text_num+1);
		if (dp.element.GetCount()) txt << ": " << dp.element;
		
		Color part_clr = &dp == selected_part ? Blend(part_bg, sel_clr) : part_bg;
		Rect part_header_rect = RectC(0,y,sz.cx,line_h);
		d.DrawRect(part_header_rect, part_clr);
		d.DrawText(0,y,txt,fnt,Black());
		d.DrawLine(0, y, sz.cx, y, 1, part_border);
		areas.Add().Set(part_header_rect, dp);
		vert_areas.Add().Set(part_header_rect, dp);
		
		y += line_h;
		
		for(int j = 0; j < dp.sub.GetCount(); j++) {
			const DynSub& ds = dp.sub[j];
			
			txt.Clear();
			if (ds.element0.GetCount()) txt << ds.element0;
			if (ds.element1.GetCount()) {if (txt.GetCount()) txt << ", "; txt << ds.element1;}
			
			Color sub_clr = &ds == selected_sub ? Blend(sub_bg, sel_clr) : sub_bg;
			int left = indent_cx;
			part_header_rect = RectC(0,y,left,line_h);
			d.DrawRect(part_header_rect, part_clr);
			areas.Add().Set(part_header_rect, dp);
			
			Rect sub_header_rect = RectC(left,y,sz.cx-left,line_h);
			d.DrawRect(sub_header_rect, sub_clr);
			d.DrawText(left,y,txt,fnt,Black());
			areas.Add().Set(sub_header_rect, ds);
			vert_areas.Add().Set(sub_header_rect, ds);
			
			y += line_h;
			
			
			for(int k = 0; k < ds.lines.GetCount(); k++) {
				const DynLine& dl = ds.lines[k];
				
				int l0, l1;
				l0 = 0;
				l1 = indent_cx;
				part_header_rect = Rect(l0,y,l1,y+line_h);
				d.DrawRect(part_header_rect, part_clr);
				areas.Add().Set(part_header_rect, dp);
				
				l0 = indent_cx;
				l1 = indent_cx*2;
				sub_header_rect = Rect(l0,y,l1,y+line_h);
				d.DrawRect(sub_header_rect, sub_clr);
				
				int left = indent_cx*2;
				txt = dl.text;
				Rect line_header_rect = RectC(left,y,sz.cx-left,line_h);
				d.DrawRect(line_header_rect, &dl == selected_line ? Blend(line_bg, sel_clr) : line_bg);
				d.DrawText(left,y,txt,fnt,Black());
				areas.Add().Set(line_header_rect, dl);
				vert_areas.Add().Set(line_header_rect, dl);
				areas.Add().Set(line_header_rect, dl);
				
				y += line_h;
				
			}
		}
		
		d.DrawLine(0, y, sz.cx, y, 1, part_border);
		y += line_h;
	}
}


END_TEXTLIB_NAMESPACE
