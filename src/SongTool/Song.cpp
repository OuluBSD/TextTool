#include "SongTool.h"


/*Song::Song() {
	uniq = Random();
}*/
/*
void Song::RealizeImpacts() {
	for (Part& part : parts) {
		for (Line& line : part.lines) {
			for (Break& brk : line.breaks) {
				for(int i = 0; i < GENDER_COUNT; i++) {
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
				for(int i = 0; i < GENDER_COUNT; i++) {
					PatternSnap& snap = brk.snap[i];
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
		for (ReverseTask& rt : rev_pattern_tasks) {
			if (!rt.snap) {
				all_ok = false;
				break;
			}
		}
		if (all_ok)
			return;
	}
	for (Part& p : parts) {
		for(int i = 0; i < PTR_COUNT; i++) {
			Vector<PatternSnap*> snaps;
			p.GetSnapsLevel(i, 0, snaps);
			if (force)
				for (ReverseTask& rt : rev_pattern_tasks)
					rt.snap = 0;
			for (PatternSnap* s: snaps) {
				for (ReverseTask& rt : rev_pattern_tasks) {
					if (!rt.snap && rt.txt == s->txt)
						rt.snap = s;
				}
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

int Song::GetLength(int mode) const {
	CHKMODE(mode);
	int len = 0;
	for (const Part& p : parts)
		len += p.GetLength(mode);
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
