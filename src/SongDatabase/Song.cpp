#include "SongDatabase.h"


BEGIN_SONGLIB_NAMESPACE


String StaticPart::GetTypeString() const {
	return GetTypeString(part_type);
}

String StaticPart::GetTypeString(int part_type) {
	return GetTypeString((PartType)part_type);
}

String StaticPart::GetTypeString(PartType part_type) {
	switch (part_type) {
		case SINGING: return t_("Singing");
		case RAPPING: return t_("Rapping");
		case POETRY: return t_("Poetry");
		case DIALOG: return t_("Dialog");
		case SKIP: return t_("Skip");
		default: return "<invalid>";
	}
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
	String dir = SongDatabase::Single().GetSongsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Song::LoadTitle(String title) {
	String dir = SongDatabase::Single().GetSongsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Song::GetAnyTitle(Artist& a) const {
	int focus_tc = -1, focus_arch = -1, focus_lyr = -1;
	a.FindSong(focus_tc, focus_arch, focus_lyr, lyrics_file_title);
	if (focus_tc >= 0) {
		const Lyrics& l = a.typecasts[focus_tc].archetypes[focus_arch].lyrics[focus_lyr];
		if (l.native_title.GetCount())
			return l.native_title;
		
		if (l.english_title.GetCount())
			return l.english_title;
		
	}
	return file_title;
}


END_SONGLIB_NAMESPACE
