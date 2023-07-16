#include "SongTool.h"


Song::Song() {
	uniq = Random();
}

void Song::Store() {
	String dir = Database::Single().GetSongsDir(reversed);
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFile(*this, json_path, true);
}

void Song::LoadTitle(String title) {
	String dir = Database::Single().GetSongsDir(reversed);
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFile(*this, json_path);
}

void Song::RealizeTaskSnaps(bool force) {
	if (!force) {
		bool all_ok = true;
		for (ReverseTask& rt : rev_tasks) {
			if (!rt.snap) {
				all_ok = false;
				break;
			}
		}
		if (all_ok)
			return;
	}
	for (Part& p : parts) {
		Vector<PatternSnap*> snaps;
		p.GetSnapsLevel(0, snaps);
		if (force)
			for (ReverseTask& rt : rev_tasks)
				rt.snap = 0;
		for (PatternSnap* s: snaps) {
			for (ReverseTask& rt : rev_tasks) {
				if (!rt.snap && rt.txt == s->txt)
					rt.snap = s;
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
