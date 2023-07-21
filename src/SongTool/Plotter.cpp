#include "SongTool.h"

Color HSVToRGB(double H, double S, double V) {
	double r = 0, g = 0, b = 0;
	
	H *= 360.;
	
	if (S == 0)
	{
		r = V;
		g = V;
		b = V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (H == 360)
			H = 0;
		else
			H = H / 60;

		i = (int)trunc(H);
		f = H - i;

		p = V * (1.0 - S);
		q = V * (1.0 - (S * f));
		t = V * (1.0 - (S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = V;
			b = p;
			break;

		case 2:
			r = p;
			g = V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = V;
			break;

		case 4:
			r = t;
			g = p;
			b = V;
			break;

		default:
			r = V;
			g = p;
			b = q;
			break;
		}

	}
	
	if (0) {
		DUMP(r);
		DUMP(g);
		DUMP(b);
	}
	RGBA rgb;
	rgb.r = r * 255;
	rgb.g = g * 255;
	rgb.b = b * 255;
	rgb.a = 255;
	
	return rgb;
}



Plotter::Plotter() {
	AddFrame(InsetFrame());
	
}

void Plotter::Paint(Draw& d) {
	Size sz = GetSize();
	bool absolute_view = view == VIEW_ABSOLUTE  || view == VIEW_ABSOLUTE_WEIGHTED;
	bool weighted_view = view == VIEW_ABSOLUTE_WEIGHTED || view == VIEW_CUMULATIVE_WEIGHTED;
	bool cumulative_view = view == VIEW_CUMULATIVE || view == VIEW_CUMULATIVE_WEIGHTED;
	bool genderdiff_view = view == VIEW_GENDERDIFF;
	bool genderdiff_weighted_view = view == VIEW_GENDERDIFF_WEIGHTED;
	
	int other_mode = !mode;
	
	d.DrawRect(sz, White());
	
	Color txt_clr = Black();
	Font fnt = Monospace(10);
	
	
	
	Attributes& g = Database::Single().attrs;
	
	int y = sz.cy-15;
	int c = g.scorings.GetCount();
	
	values.SetCount(c);
	draw_group.SetCount(c);
	vert_lines.SetCount(0);
	
	String part_key;
	int vert_x = 0;
	Vector<int> caps;
	if (whole_song) {
		if (!song)
			return;
		for (auto& v : values) v.SetCount(0);
		for(int i = 0; i < song->structure.GetCount(); i++) {
			String key = song->structure[i];
			int j = song->FindPartIdx(key);
			if (j < 0) {
				return;
				DUMP(i);
				DUMPC(song->structure);
				DUMPC(song->parts);
			}
			ASSERT(j >= 0);
			
			Part& part = song->parts[j];
			int total = 0;
			for(int j = 0; j < part.lines.GetCount(); j++) {
				Line& line = part.lines[j];
				total += line.breaks.GetCount();
				for(int k = 0; k < line.breaks.GetCount(); k++) {
					Break& brk = line.breaks[k];
					
					// Read values to stack variables
					if (genderdiff_weighted_view) {
						int c0 = min(c, min(
							brk.snap[other_mode].partscore.GetCount(),
							brk.snap[mode].partscore.GetCount()));
						
						double av = 0;
						for (auto& val : brk.snap[mode].partscore) av += val;
						av /= brk.snap[mode].partscore.GetCount();
						
						double other_av = 0;
						for (auto& val : brk.snap[other_mode].partscore) other_av += val;
						other_av /= brk.snap[other_mode].partscore.GetCount();
						
						for(int k = 0; k < c0; k++) {
							double av_diff = brk.snap[mode].partscore[k] - av;
							double other_av_diff = brk.snap[other_mode].partscore[k] - other_av;
							
							values[k].Add(
								av_diff -
								other_av_diff
								);
						}
					}
					else if (genderdiff_view) {
						int c0 = min(c, min(
							brk.snap[other_mode].partscore.GetCount(),
							brk.snap[mode].partscore.GetCount()));
						for(int k = 0; k < c0; k++)
							values[k].Add(
								brk.snap[mode].partscore[k] -
								brk.snap[other_mode].partscore[k]
								);
					}
					else {
						int c0 = min(c, brk.snap[mode].partscore.GetCount());
						for(int k = 0; k < c0; k++)
							values[k].Add(brk.snap[mode].partscore[k]);
					}
				}
			}
			
			// Get positions of vertical lines
			vert_x += total;
			vert_lines.Add(vert_x);
			
			// Get key string for the whole song
			if (i)
				part_key << ", ";
			part_key << g.Translate(key);
		}
	}
	else {
		if (!part)
			return;
		part_key = part->name;
		for(int j = 0; j < this->values.GetCount(); j++)
			this->values[j].Clear();
		for(int i = 0; i < part->lines.GetCount(); i++) {
			Line& line = part->lines[i];
			for(int k = 0; k < line.breaks.GetCount(); k++) {
				Break& brk = line.breaks[k];
				
				int c0 = min(c, brk.snap[mode].partscore.GetCount());
				for(int j = 0; j < c0; j++)
					this->values[j].Add(brk.snap[mode].partscore[j]);
				
			}
			vert_x += line.breaks.GetCount();
			vert_lines.Add(vert_x);
		}
	}
	
	// Weighted value (constant sum of scoring groups)
	if (weighted_view) {
		int pos = values[0].GetCount();
		for(int i = 0; i < pos; i++) {
			double sum = 0;
			for(int j = 0; j < c; j++) {
				sum += fabs(values[j][i]);
			}
			if (sum != 0) {
				double av = 1.0 / sum;
				for(int j = 0; j < c; j++)
					values[j][i] *= av;
			}
		}
	}
	
	rids.Clear();
	double cx = (double)sz.cx / (vert_x-1);
	double xoff = absolute_view || genderdiff_view || genderdiff_weighted_view ? -cx / 2 : 0;
	if (whole_song) {
		int pos = 0;
		for(int i = 0; i < song->structure.GetCount(); i++) {
			const String& part_key = song->structure[i];
			const Part& part = *song->FindPart(part_key);
			int len = part.lines.GetCount();
			for(int j = 0; j < len; j++) {
				const Line& line = part.lines[j];
				for(int k = 0; k < line.breaks.GetCount(); k++) {
					int x = xoff + cx * pos;
					RectId& rid = rids.Add();
					rid.a = RectC(x, 0, cx, sz.cy);
					rid.b = i;
					rid.c = j;
					rid.d = k;
					pos++;
				}
			}
		}
	}
	else {
		int pos = 0;
		int part_i = song->GetPartIdx(*part);
		for(int i = 0; i < part->lines.GetCount(); i++) {
			const Line& line = part->lines[i];
			for(int j = 0; j < line.breaks.GetCount(); j++) {
				const Break& brk = line.breaks[j];
				int x = xoff + cx * pos;
				RectId& rid = rids.Add();
				rid.a = RectC(x, 0, cx, sz.cy);
				rid.b = part_i;
				rid.c = i;
				rid.d = j;
				pos++;
			}
		}
	}
	{
		for(int i = 0; i < vert_lines.GetCount(); i++) {
			int x = xoff + cx * vert_lines[i];
			d.DrawLine(x, 0, x, sz.cy, 1, Black());
		}
		for(int i = 1; i < vert_x; i++) {
			int x = cx * i;
			int y__ = sz.cy / 10;
			int y_2 = sz.cy / 2;
			int y0 = y_2 - y__;
			int y1 = y_2 + y__;
			d.DrawLine(x, y0, x, y1, 1, GrayColor(128));
		}
		
		d.DrawText(3,3, whole_song ? String(t_("Whole song")) : part_key,fnt,txt_clr);
		
		String t;
		switch (view) {
			case VIEW_ABSOLUTE: t = t_("absolute value"); break;
			case VIEW_CUMULATIVE: t = t_("accumulated value"); break;
			case VIEW_ABSOLUTE_WEIGHTED: t = t_("weighted absolute value"); break;
			case VIEW_CUMULATIVE_WEIGHTED: t = t_("weighted accumulated value"); break;
			case VIEW_GENDERDIFF: t = t_("gender difference value"); break;
			case VIEW_GENDERDIFF_WEIGHTED: t = t_("weighted gender difference value"); break;
		}
		d.DrawText(3,13, t, fnt,txt_clr);
	}
	
	// Accumulated value view
	if (cumulative_view) {
		for (auto& vv : this->values) {
			double a = 0;
			for (auto& v : vv) {
				a += v;
				v = a;
			}
		}
	}
	
	// Don't draw zero-lines
	draw_count = 0;
	for(int i = 0; i < draw_group.GetCount(); i++) {
		bool& b = draw_group[i];
		b = false;
		auto& vv = values[i];
		for (auto& v : vv) {
			if (v != 0) {
				b = true;
				break;
			}
		}
		if (b)
			draw_count++;
	}
	draw_count = max(1, draw_count);
	focused_group = focused_group % draw_count;
	
	double limit = 0;
	for(int i = 0, j = 0; i < c; i++) {
		if (!draw_group[i])
			continue;
		
		const Attributes::ScoringType& s = g.scorings[i];
		String str =
			g.Translate(s.klass) + ": " +
			g.Translate(s.axes0) + " / " +
			g.Translate(s.axes1);
		
		double h = (double)i / c;
		Color clr = HSVToRGB(h, 1, 0.5);
		Size sz = GetTextSize(str, fnt);
		Font fnt0 = fnt;
		Color bg = White;
		if (focused_group == j) {
			fnt0.Bold();
			bg = GrayColor(256-32);
			focused_group_i = i;
		}
		d.DrawRect(RectC(5, y, sz.cx, sz.cy), bg);
		d.DrawText(5, y, str, fnt0, clr);
		y-= 11;
		
		for (auto& v : values[i])
			limit = max(limit, abs(v));
		
		j++;
	}
	if (!limit)
		limit = 1;
	
	int h_2 = sz.cy / 2;
	d.DrawLine(0, h_2, sz.cx-1, h_2, 1, Black());
	
	for(int i = 0, j = 0; i < c; i++) {
		if (!draw_group[i])
			continue;
		
		auto& vv = values[i];
		if (vv.GetCount() < 2)
			continue;
		
		double h = (double)i / c;
		Color clr = HSVToRGB(h, 1, 0.9);
		polyline.SetCount(0);
		double cx = (double)sz.cx / (vv.GetCount()-1);
		double x = 0;
		for (auto& v : vv) {
			Point& pt = polyline.Add();
			double fy = (v + limit) / (2.0 * limit);
			pt.x = x;
			pt.y = (1.0 - fy) * sz.cy;
			x += cx;
		}
		int lh = (focused_group == j) ? 3 : 1;
		if (focused_group == j) {
			d.Offset(0,2);
			d.DrawPolyline(polyline, lh, Black());
			d.End();
		}
		d.DrawPolyline(polyline, lh, clr);
		j++;
	}
	
	
	
	if (last_brk) {
		const String& txt = last_brk->snap[mode].txt;
		Size tsz = GetTextSize(txt, fnt);
		int x = (sz.cx - tsz.cx) / 2;
		Rect r = RectC(x, 0, tsz.cx, tsz.cy);
		d.DrawRect(r, White());
		d.DrawText(x, 0, txt, fnt, Black());
	}
}

void Plotter::LeftDown(Point p, dword keyflags) {
	if (keyflags & K_SHIFT)
		view = view == 0 ? VIEW_COUNT-1 : view - 1;
	else
		view = (view + 1) % VIEW_COUNT;
	Refresh();
}

void Plotter::RightDown(Point p, dword keyflags) {
	if (keyflags & K_SHIFT)
		focused_group = (focused_group + 1) % draw_count;
	else
		focused_group = focused_group == 0 ? draw_count-1 : focused_group-1;
	Refresh();
}

void Plotter::MouseWheel(Point p, int zdelta, dword keyflags) {
	Database& d = Database::Single();
	if (keyflags & K_SHIFT) {
		if (zdelta > 0)
			focused_group = (focused_group + 1) % draw_count;
		else
			focused_group = focused_group == 0 ? draw_count-1 : focused_group-1;
	}
	else {
		//DUMPC(rids);
		const RectId* rid = FindPos(p);
		if (rid) {
			int change = zdelta > 0 ? +1 : -1;
			const String& part_key = song->structure[rid->b];
			int part_i = song->FindPartIdx(part_key);
			int line_i = rid->c;
			int brk_i = rid->d;
			
			if (part_i < 0)
				return;
			Part& part = song->parts[part_i];
			Line& line = part.lines[line_i];
			Break& brk = line.breaks[brk_i];
			auto& score = brk.snap[mode].partscore[focused_group_i];
			score += change;
			
			if (list) {
				ArrayCtrl& list = *this->list;
				for(int i = 0; i < list.GetCount(); i++) {
					int part_i0 = list.Get(i, 0);
					int line_i0 = list.Get(i, 1);
					int brk_i0 = list.Get(i, 2);
					if (part_i0 == part_i && line_i0 == line_i && brk_i0 == brk_i) {
						int col = ScoringCtrl::group_begin + focused_group_i;
						list.Set(i, col, score);
						list.SetCursor(i);
						break;
					}
				}
			}
		}
	}
	Refresh();
}

void Plotter::MouseMove(Point p, dword keyflags) {
	const RectId* rid = FindPos(p);
	if (rid) {
		const String& part_key = song->structure[rid->b];
		Part& part = *song->FindPart(part_key);
		Line& line = part.lines[rid->c];
		Break& brk = line.breaks[rid->d];
		if (&brk != last_brk) {
			last_brk = &brk;
			Refresh();
		}
	}
}

const Plotter::RectId* Plotter::FindPos(Point p) {
	for (const RectId& rid : rids) {
		if (rid.a.Contains(p)) {
			return &rid;
		}
	}
	return 0;
}

