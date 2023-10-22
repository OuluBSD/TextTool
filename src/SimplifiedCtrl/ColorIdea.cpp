#include "SimplifiedCtrl.h"


struct ColorRowDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
		               Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, paper);
		ValueArray va(q);
		if (va.GetCount() == 0)
			return;
		Size sz = r.GetSize();
		double cw = (double)sz.cx / va.GetCount();
		for(int i = 0; i < va.GetCount(); i++) {
			Color clr = va[i];
			int x0 = (int)(cw * i);
			int x1 = (int)(cw * (i+1));
			Rect r0 = RectC(x0 + r.left, 0 + r.top, x1-x0, sz.cy);
			w.DrawRect(r0, clr);
		}
	}
	/*virtual void PaintBackground(Draw& w, const Rect& r, const Value& q,
	                             Color ink, Color paper, dword style) const {
		Paint(w, r, q, ink, paper, style);
	}*/
};

struct ColorRowCtrl : Ctrl {
	ColorRowDisplay disp;
	virtual void Paint(Draw& w)
	{
		Color paper = White();
		Rect r = GetSize();
		Value q = GetData();
		disp.Paint(w, r, q, Black(), White(), 0);
	}
};



ColorIdeaCtrl::ColorIdeaCtrl() {
	Add(list.SizePos());
	
	list.AddColumn(t_("Part #"));
	list.AddColumn(t_("Part"));
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Env. Decay"));
	list.AddColumn(t_("Listener"));
	list.AddColumn(t_("Env. Attack"));
	list.AddColumn(t_("Env. Sustain"));
	list.AddColumn(t_("Env. Release"));
	list.ColumnWidths("1 2 1 8 2 8 8 8");
	
	
	if (0) {
		for(int i = 0; i < 8; i++) {
			String l;
			for(int j = 0; j < 8; j++) {
				if (j) l << ",";
				Color c(Random(256),Random(256),Random(256));
				RGBA r = c;
				l << "RGB(" << IntStr(r.r) << "," << IntStr(r.g) << "," << IntStr(r.b) << ")";
			}
			LOG("list.Add(\"" << l << "\");");
		}
	}
}
	
void ColorIdeaCtrl::DisableAll() {
	
}
	
void ColorIdeaCtrl::EnableAll() {
	
}
	
void ColorIdeaCtrl::Data() {
	Song& s = GetSong();
	
	int row = 0;
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		
		part.colors.SetCount(4);
		int part_rows = max(1, part.colors[0].GetCount());
		
		for(int j = 0; j < part_rows; j++) {
			list.Set(row, 0, i);
			list.Set(row, 1, part.name);
			list.Set(row, 2, j);
			
			// Get color vectors for: main, attack, decay, release
			for(int k = 0; k < 4; k++) {
				int col = 3 + k;
				if (k > 0) col++; // jump over listener column (if not main)
				const auto& vv = part.colors[k];
				if (j >= vv.GetCount()) {
					list.Set(row, col, Value());
					continue;
				}
				const auto& v = vv[j];
				ValueArray va;
				for (const auto& c : v)
					va.Add(c);
				list.Set(row, col, va);
			}
			
			// Color vector for listeners
			{
				ValueArray va;
				for(int k = 0; k < part.listener_colors.GetCount(); k++) {
					const auto& v = part.listener_colors[k];
					if (j < v.GetCount())
						va.Add(v[j]);
				}
				list.Set(row, 4, va);
			}
			row++;
		}
	}
	list.SetCount(row);
	for(int i = 3; i < 8; i++) {
		list.SetColumnDisplay(i, Single<ColorRowDisplay>());
	}
}

void ColorIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update focused line main"), AppImg::BlueRing(), THISBACK2(GetMain, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update focused line listener"), AppImg::BlueRing(), THISBACK2(GetListener, -1, false)).Key(K_CTRL_W);
	bar.Add(t_("Update focused line envelope"), AppImg::BlueRing(), THISBACK2(GetEnvelope, -1, false)).Key(K_CTRL_E);
	bar.Separator();
	bar.Add(t_("Update main for all lines"), AppImg::RedRing(), THISBACK2(GetMain, 0, true)).Key(K_F5);
	bar.Add(t_("Update listener for all lines"), AppImg::RedRing(), THISBACK2(GetListener, 0, true)).Key(K_F6);
	bar.Add(t_("Update envelope for all lines"), AppImg::RedRing(), THISBACK2(GetEnvelope, 0, true)).Key(K_F7);
	
}

void ColorIdeaCtrl::GetMain(int part_i, bool start_next) {
	Song& s = GetSong();
	
	if (part_i < 0) {
		if (!list.IsCursor()) return;
		int cur = list.GetCursor();
		part_i = list.Get(cur, 0);
	}
	StaticPart& part = s.parts[part_i];
	
	s.RealizePipe();
	
	ColorIdeaArgs args;
	args.fn = 0;
	
	String style0 = part.data.Get("DIALOGUE_IDEA_STYLE1", "");
	String style1 = part.data.Get("DIALOGUE_IDEA_STYLE2", "");
	String s0 = part.data.Get("DIALOGUE_IDEA_1", "");
	String s1 = part.data.Get("DIALOGUE_IDEA_2", "");
	args.dialogue.Add(style0, Split(s0, "\n"));
	args.dialogue.Add(style1, Split(s1, "\n"));
	
	TaskMgr& m = *s.pipe;
	m.GetColorIdea(args, THISBACK3(OnMain, &s, part_i, start_next));
}

void ColorIdeaCtrl::GetEnvelope(int part_i, bool start_next) {
	Song& s = GetSong();
	
	if (part_i < 0) {
		if (!list.IsCursor()) return;
		int cur = list.GetCursor();
		part_i = list.Get(cur, 0);
	}
	StaticPart& part = s.parts[part_i];
	
	if (part.colors.IsEmpty()) {
		PromptOK("error: no main colors yet");
		return;
	}
	
	s.RealizePipe();
	
	ColorIdeaArgs args;
	args.fn = 1;
	
	Index<String> possible_previous, possible_next;
	GetPossibleParts(s, part, possible_previous, possible_next);
	
	for (const String& type : possible_next.GetKeys()) {
		for(int i = 0; i < s.parts.GetCount(); i++) {
			const auto& p = s.parts[i];
			if (p.type == type) {
				// any color -> main color -> first of main
				if (p.colors.GetCount() && p.colors[0].GetCount())
					args.next_line.Add() <<= p.colors[0][0];
			}
		}
	}
	for (const String& type : possible_previous.GetKeys()) {
		for(int i = 0; i < s.parts.GetCount(); i++) {
			const auto& p = s.parts[i];
			if (p.type == type) {
				// any color -> main color -> last of main
				if (p.colors.GetCount() && p.colors[0].GetCount())
					args.prev_line.Add() <<= p.colors[0].Top();
			}
		}
	}
	ASSERT(args.next_line.GetCount() || args.prev_line.GetCount());
	
	// copy main
	args.main <<= part.colors[0];
	
	TaskMgr& m = *s.pipe;
	m.GetColorIdea(args, THISBACK3(OnEnvelope, &s, part_i, start_next));
}

void ColorIdeaCtrl::GetListener(int part_i, bool start_next) {
	Song& s = GetSong();
	
	if (part_i < 0) {
		if (!list.IsCursor()) return;
		int cur = list.GetCursor();
		part_i = list.Get(cur, 0);
	}
	StaticPart& part = s.parts[part_i];
	
	s.RealizePipe();
	
	ColorIdeaArgs args;
	args.fn = 2;
	
	
	Index<String> possible_previous, possible_next;
	GetPossibleParts(s, part, possible_previous, possible_next);
	
	for(int j = 0; j < LISTENERTYPE_COUNT; j++)
		args.begin_colors.Add(ListenerTypeString[j], White());
	
	for (const String& type : possible_previous.GetKeys()) {
		for(int i = 0; i < s.parts.GetCount(); i++) {
			const auto& p = s.parts[i];
			if (p.type == type) {
				for(int j = 0; j < LISTENERTYPE_COUNT; j++) {
					if (j >= p.listener_colors.GetCount()) continue;
					Color& clr = args.begin_colors.GetAdd(ListenerTypeString[j]);
					if (p.listener_colors[j].GetCount())
						clr = p.listener_colors[j].Top();
				}
			}
		}
	}
	
	// copy main
	args.main <<= part.colors[0];
	
	TaskMgr& m = *s.pipe;
	m.GetColorIdea(args, THISBACK3(OnListener, &s, part_i, start_next));
}

void ColorIdeaCtrl::GetPossibleParts(Song& s, StaticPart& part, Index<String>& possible_previous, Index<String>& possible_next) {
	int c = s.active_struct.parts.GetCount();
	for(int i = 0; i < c; i++) {
		const String& type = s.active_struct.parts[i];
		if (type == part.type) {
			if (i > 0)
				possible_previous.FindAdd(s.active_struct.parts[i-1]);
			if (i+1 < c)
				possible_next.FindAdd(s.active_struct.parts[i+1]);
		}
	}
}

void ColorIdeaCtrl::OnMain(String res, Song* song, int part_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	StaticPart& part = song->parts[part_i];
	
	part.colors.SetCount(4);
	auto& tgt = part.colors[0];
	tgt.Clear();
	
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		auto& tgt_line = tgt.Add();
		Vector<String> rgbs = Split(lines[i], "(");
		for(int j = 0; j < rgbs.GetCount(); j++) {
			String& rgb = rgbs[j];
			
			int a = 0;//rgb.Find("(");
			if (a >= 0) {
				a++;
				int b = rgb.Find(")", a);
				if (b >= 0) {
					rgb = rgb.Mid(a, b-a);
				}
			}
			
			RGBA rgba;
			Vector<String> parts = Split(rgb, ",");
			for (String& part : parts) {
				part = TrimBoth(part);
			}
			if (parts.GetCount() >= 3) {
				rgba.r = ScanInt(parts[0]);
				rgba.g = ScanInt(parts[1]);
				rgba.b = ScanInt(parts[2]);
				rgba.a = 255;
				tgt_line << Color(rgba);
			}
		}
	}
	
	PostCallback(THISBACK(Data));
	
	if (start_next && part_i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetMain, part_i+1, true));
}

void ColorIdeaCtrl::OnEnvelope(String res, Song* song, int part_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	StaticPart& part = song->parts[part_i];
	
	part.colors.SetCount(4);
	
	Vector<String> parts = Split(res, "\n\n");
	if (parts.GetCount() != 3) {
		PromptOK("Unexpected result");
		return;
	}
	for (int i = 0; i < parts.GetCount(); i++) {
		Vector<String> lines = Split(parts[i], "\n");
		auto& tgt = part.colors[1+i];
		tgt.Clear();
		
		// Remove "Imagine ..." line
		if (lines[0].Find("magine") >= 0) lines.Remove(0);
		
		for(int j = 0; j < lines.GetCount(); j++) {
			auto& tgt_line = tgt.Add();
			Vector<String> rgbs = Split(lines[j], "(");
			for(int k = 0; k < rgbs.GetCount(); k++) {
				String& rgb = rgbs[k];
				
				int a = 0;//rgb.Find("(");
				if (a >= 0) {
					a++;
					int b = rgb.Find(")", a);
					if (b >= 0) {
						rgb = rgb.Mid(a, b-a);
					}
				}
				
				RGBA rgba;
				Vector<String> parts = Split(rgb, ",");
				for (String& part : parts) {
					part = TrimBoth(part);
				}
				if (parts.GetCount() >= 3) {
					rgba.r = ScanInt(parts[0]);
					rgba.g = ScanInt(parts[1]);
					rgba.b = ScanInt(parts[2]);
					rgba.a = 255;
					tgt_line << Color(rgba);
				}
			}
		}
	}
	
	PostCallback(THISBACK(Data));
	
	if (start_next && part_i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetEnvelope, part_i+1, true));
}

void ColorIdeaCtrl::OnListener(String res, Song* song, int part_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	StaticPart& part = song->parts[part_i];
	
	part.colors.SetCount(4);
	
	int line_count = 8;
	int exp_count = line_count * LISTENERTYPE_COUNT;
	Vector<String> lines = Split(res, "\n");
	
	// Remove all but digit-starting lines
	int begin = lines.GetCount() >= exp_count ? exp_count : line_count;
	if (lines.GetCount() >= begin)
		lines.Remove(begin, lines.GetCount()-begin);
	/*for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (!IsDigit(l[0])) {
			lines.Remove(i, lines.GetCount()-i);
			break;
		}
	}*/
	
	if (lines.GetCount() != exp_count && lines.GetCount() != line_count) {
		PromptOK("Unexpected line count in result");
		return;
	}
	part.listener_colors.Clear();
	part.listener_colors.SetCount(LISTENERTYPE_COUNT);
	
	if (lines.GetCount() == line_count) {
		for(int i = 0; i < lines.GetCount(); i++) {
			static_assert(LISTENERTYPE_COUNT == 2, "only 2 is supported here");
			Vector<String> parts = Split(lines[i], "b)");
			if (parts.GetCount() != LISTENERTYPE_COUNT) {
				PromptOK("Unexpected line count in result");
				return;
			}
			for(int listener_i = 0; listener_i < LISTENERTYPE_COUNT; listener_i++) {
				String& rgb = parts[listener_i];
				Color& tgt = part.listener_colors[listener_i].Add();
				tgt = GrayColor();
				
				int a = rgb.Find("(");
				if (a >= 0) {
					a++;
					int b = rgb.Find(")", a);
					if (b >= 0) {
						rgb = rgb.Mid(a, b-a);
					}
				}
				
				RGBA rgba;
				Vector<String> parts = Split(rgb, ",");
				for (String& part : parts) {
					part = TrimBoth(part);
				}
				if (parts.GetCount() >= 3) {
					rgba.r = ScanInt(parts[0]);
					rgba.g = ScanInt(parts[1]);
					rgba.b = ScanInt(parts[2]);
					rgba.a = 255;
					tgt = Color(rgba);
				}
			}
		}
	}
	else {
		for(int i = 0; i < exp_count; i++) {
			int listener_i = i % LISTENERTYPE_COUNT;
			int line_i = i / LISTENERTYPE_COUNT;
			String& rgb = lines[i];
			
			Color& tgt = part.listener_colors[listener_i].Add();
			tgt = GrayColor();
			
			int a = rgb.Find("(");
			if (a >= 0) {
				a++;
				int b = rgb.Find(")", a);
				if (b >= 0) {
					rgb = rgb.Mid(a, b-a);
				}
			}
			
			RGBA rgba;
			Vector<String> parts = Split(rgb, ",");
			for (String& part : parts) {
				part = TrimBoth(part);
			}
			if (parts.GetCount() >= 3) {
				rgba.r = ScanInt(parts[0]);
				rgba.g = ScanInt(parts[1]);
				rgba.b = ScanInt(parts[2]);
				rgba.a = 255;
				tgt = Color(rgba);
			}
		}
	}
	
	PostCallback(THISBACK(Data));
	
	if (start_next && part_i+1 < song->parts.GetCount())
		PostCallback(THISBACK2(GetListener, part_i+1, true));
}

int ColorIdeaCtrl::FindListIndex(int part, int line) const {
	for(int i = 0; i < list.GetCount(); i++) {
		int part0 = list.Get(i, 0);
		if (part0 == part) {
			int line0 = list.Get(i, 2);
			if (line0 == line)
				return i;
		}
	}
	return -1;
}
