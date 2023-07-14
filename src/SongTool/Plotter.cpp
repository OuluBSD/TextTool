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
	bool absolute_mode = mode == MODE_ABSOLUTE   || mode == MODE_ABSOLUTE_WEIGHTED;
	bool weighted_mode = mode == MODE_ABSOLUTE_WEIGHTED || mode == MODE_CUMULATIVE_WEIGHTED;
	
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
			int j = song->parts.Find(key);
			if (j < 0) {
				DUMP(i);
				DUMPC(song->structure);
				DUMPC(song->parts.GetKeys());
			}
			ASSERT(j >= 0);
			
			PartScore& part = song->parts[j].score;
			
			// Accumulate values
			int c0 = min(c, part.values.GetCount());
			for(int j = 0; j < c0; j++) {
				auto& to = values[j];
				const auto& from = part.values[j];
				for (auto& v : from)
					to.Add(v);
			}
			
			// Get positions of vertical lines
			vert_x += part.GetLen();
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
		part_key = this->part_key;
		int c = part->values.GetCount();
		this->values.SetCount(c);
		for(int i = 0; i < c; i++) {
			const auto& f = part->values[i];
			auto& d = this->values[i];
			int c0 = f.GetCount();
			d.SetCount(c0);
			for(int j = 0; j < c0; j++)
				d[j] = f[j];
		}
	}
	
	// Weighted value (constant sum of scoring groups)
	if (weighted_mode) {
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
	double xoff = absolute_mode ? -cx / 2 : 0;
	if (song) {
		double cx = (double)sz.cx / (vert_x-1);
		int k = 0;
		for(int i = 0; i < song->structure.GetCount(); i++) {
			const String& part_key = song->structure[i];
			const PartScore& part = song->parts.Get(part_key).score;
			int len = part.GetLen();
			for(int j = 0; j < len; j++) {
				int x = xoff + cx * k;
				RectId& rid = rids.Add();
				rid.a = RectC(x, 0, cx, sz.cy);
				rid.b = i;
				rid.c = j;
				k++;
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
		switch (mode) {
			case MODE_ABSOLUTE: t = t_("absolute value"); break;
			case MODE_CUMULATIVE: t = t_("accumulated value"); break;
			case MODE_ABSOLUTE_WEIGHTED: t = t_("weighted absolute value"); break;
			case MODE_CUMULATIVE_WEIGHTED: t = t_("weighted accumulated value"); break;
		}
		d.DrawText(3,13, t, fnt,txt_clr);
	}
	
	// Accumulated value mode
	if (!absolute_mode) {
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
		d.DrawPolyline(polyline, lh, clr);
		j++;
	}
	
}

void Plotter::LeftDown(Point p, dword keyflags) {
	if (keyflags & K_SHIFT)
		mode = mode == 0 ? MODE_COUNT-1 : mode - 1;
	else
		mode = (mode + 1) % MODE_COUNT;
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
		DUMPC(rids);
		for (const RectId& rid : rids) {
			if (rid.a.Contains(p)) {
				int change = zdelta > 0 ? +1 : -1;
				const String& part_key = song->structure[rid.b];
				PartScore& part = song->parts.Get(part_key).score;
				if (focused_group_i < 0 || focused_group_i > part.values.GetCount())
					return;
				
				auto& v = part.values[focused_group_i];
				if (rid.c < 0 || rid.c >= v.GetCount())
					return;
				
				auto& score = v[rid.c];
				score += change;
				break;
			}
		}
	}
	Refresh();
}
