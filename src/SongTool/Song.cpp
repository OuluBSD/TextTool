#include "SongTool.h"

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

void Song::RealizeTaskSnaps() {
	bool all_ok = true;
	for (ReverseTask& rt : rev_tasks) {
		if (!rt.snap) {
			all_ok = false;
			break;
		}
	}
	if (all_ok)
		return;
	for (Part& p : parts.GetValues()) {
		Vector<PatternSnap*> snaps;
		p.snap.GetSnapsLevel(0, snaps);
		for (PatternSnap* s: snaps) {
			for (ReverseTask& rt : rev_tasks) {
				if (!rt.snap && rt.txt == s->txt)
					rt.snap = s;
			}
		}
	}
}
