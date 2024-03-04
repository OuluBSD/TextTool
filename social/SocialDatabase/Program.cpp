#include "SocialDatabase.h"


String StoryPart::GetTypeString() const {
	return GetTypeString(part_type);
}

String StoryPart::GetTypeString(int part_type) {
	return GetTypeString((PartType)part_type);
}

String StoryPart::GetTypeString(PartType part_type) {
	switch (part_type) {
		case SINGING: return t_("Singing");
		case RAPPING: return t_("Rapping");
		case POETRY: return t_("Poetry");
		case DIALOG: return t_("Dialog");
		case SKIP: return t_("Skip");
		default: return "<invalid>";
	}
}

/*Program::Program() {
	uniq = Random();
}*/
/*
void Program::RealizeImpacts() {
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
Impact* Program::FindImpact(String impact_txt) {
	impact_txt = ToLower(impact_txt);
	for (Impact& i : impacts)
		if (i.impact == impact_txt)
			return &i;
	return 0;
}

Impact* Program::FindImpactByText(String txt) {
	txt = ToLower(txt);
	for (Impact& i : impacts)
		if (i.txt == txt)
			return &i;
	return 0;
}

Impact& Program::GetAddImpact(String impact_txt, String brk_txt) {
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

void Program::Store() {
	String dir = SocialDatabase::Single().GetProgramsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Program::LoadTitle(String title) {
	String dir = SocialDatabase::Single().GetProgramsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Program::GetAnyTitle(Company& a) const {
	int focus_role = -1, focus_generic = -1, focus_story = -1;
	a.FindProgram(focus_role, focus_generic, focus_story, story_file_title);
	if (focus_role >= 0) {
		const Story& l = a.roles[focus_role].generics[focus_generic].stories[focus_story];
		if (l.native_title.GetCount())
			return l.native_title;
		
		if (l.english_title.GetCount())
			return l.english_title;
		
	}
	return file_title;
}
