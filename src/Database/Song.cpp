#include "Database.h"

int Song::Rhyme::GetBestSuggestion() const {
	if (suggestions.IsEmpty())
		return -1;
	int best_score = 0, best_i = 0;
	for(int i = 0; i < suggestions.GetCount(); i++) {
		const Suggestion& sug = suggestions[i];
		if (sug.score > best_score) {
			best_score = sug.score;
			best_i = i;
		}
	}
	if (best_score == 0) {
		for(int i = 0; i < suggestions.GetCount(); i++) {
			const Suggestion& sug = suggestions[i];
			int sort_value = sug.ai_score + sug.ai_score_extra;
			if (sort_value > best_score) {
				best_score = sort_value;
				best_i = i;
			}
		}
	}
	return best_i;
}

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
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Song::LoadTitle(String title) {
	String dir = Database::Single().GetSongsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

