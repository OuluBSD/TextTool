#include "SongTool.h"


/*Song::Song() {
	uniq = Random();
}*/
/*
void Song::RealizeImpacts() {
	for (Part& part : parts) {
		for (Line& line : part.lines) {
			for (Break& brk : line.breaks) {
				for(int i = 0; i < MODE_COUNT; i++) {
					PatternSnap& snap = brk.snap[i];
					if (snap.impact)
						continue;
					String impact = snap.impact;
					if (impact.GetCount()) {
						snap.impact = &GetAddImpact(impact, snap.txt);
					}
					else if (snap.txt.GetCount() > 0) {
						snap.impact = FindImpactByText(snap.txt);
					}
				}
			}
		}
	}
}*/

PatternSnap* Song::FindSnapByImpact(String impact_txt) {
	impact_txt = ToLower(impact_txt);
	for (Part& part : parts) {
		for (Line& line : part.lines) {
			for (Break& brk : line.breaks) {
				for(const SnapArg& a : ModeArgs()) {
					PatternSnap& snap = brk.snap[a];
					String impact = ToLower(snap.impact);
					if (impact == impact_txt)
						return &snap;
				}
			}
		}
	}
	return 0;
}

/*
Impact* Song::FindImpact(String impact_txt) {
	impact_txt = ToLower(impact_txt);
	for (Impact& i : impacts)
		if (i.impact == impact_txt)
			return &i;
	return 0;
}

Impact* Song::FindImpactByText(String txt) {
	txt = ToLower(txt);
	for (Impact& i : impacts)
		if (i.txt == txt)
			return &i;
	return 0;
}

Impact& Song::GetAddImpact(String impact_txt, String brk_txt) {
	impact_txt = ToLower(impact_txt);
	brk_txt = ToLower(brk_txt);
	ASSERT(brk_txt.GetCount());
	Impact* im = FindImpact(impact_txt);
	if (im)
		return *im;
	Impact& i = impacts.Add();
	i.impact = impact_txt;
	i.txt = brk_txt;
	return i;
}*/

void Song::Store() {
	String dir = Database::Single().GetSongsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFile(*this, json_path, true);
}

void Song::LoadTitle(String title) {
	String dir = Database::Single().GetSongsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFile(*this, json_path);
}

void Song::RealizeTaskSnaps(bool force) {
	if (!force) {
		bool all_ok = true;
		if (!rev_impact.ctx)
			all_ok = false;
		for (ReverseTask& rt : rev_common_mask_tasks) {
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		for (ReverseTask& rt : rev_separate_mask_tasks) {
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		for (ReverseTask& rt : rev_pattern_tasks) {
			if (!rt.ctx) {
				all_ok = false;
				break;
			}
		}
		if (all_ok)
			return;
	}
	for (Part& p : parts) {
		Vector<SnapContext*> ctxs;
		p.GetContextLevel(0, ctxs);
		if (force) {
			rev_impact.ctx = 0;
			for (ReverseTask& rt : rev_common_mask_tasks)
				rt.ctx = 0;
			for (ReverseTask& rt : rev_separate_mask_tasks)
				rt.ctx = 0;
			for (ReverseTask& rt : rev_pattern_tasks)
				rt.ctx = 0;
		}
		for (SnapContext* c: ctxs) {
			{
				ReverseTask& rt = rev_impact;
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_common_mask_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_separate_mask_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
			for (ReverseTask& rt : rev_pattern_tasks) {
				if (!rt.ctx && rt.txt == c->Get0().txt)
					rt.ctx = c;
			}
		}
	}
}

void Song::RealizeProduction() {
	if (tracks.IsEmpty()) {
		Track& t = tracks.Add();
		t.name = "Vocal";
		t.type = TrackType::TT_VOCAL;
	}
}

int Song::GetLength(const SnapArg& a) const {
	a.Chk();
	int len = 0;
	for (const Part& p : parts)
		len += p.GetLength(a);
	return len;
}

int Song::GetPartIdx(const Part& p0) const {
	int i = 0;
	for (const Part& p : parts) {
		if (&p == &p0)
			return i;
		i++;
	}
	return -1;
}
