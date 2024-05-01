#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


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

/*Component::Song() {
	uniq = Random();
}*/
/*
void Component::RealizeImpacts() {
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
Impact* Component::FindImpact(String impact_txt) {
	impact_txt = ToLower(impact_txt);
	for (Impact& i : impacts)
		if (i.impact == impact_txt)
			return &i;
	return 0;
}

Impact* Component::FindImpactByText(String txt) {
	txt = ToLower(txt);
	for (Impact& i : impacts)
		if (i.txt == txt)
			return &i;
	return 0;
}

Impact& Component::GetAddImpact(String impact_txt, String brk_txt) {
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

void Component::Store(Snapshot& snap) {
	snapshot = &snap;
	String dir =
		AppendFileName(
			AppendFileName(
				AppendFileName(
					GetAppModeDatabase().GetEntitiesDir(), snapshot->entity->file_title),
					snapshot->file_title),
				file_title);
	RealizeDirectory(dir);
	String json_path = dir + DIR_SEPS + "db.json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Component::LoadTitle(Snapshot& snap, String title) {
	snapshot = &snap;
	file_title = title;
	String dir =
		AppendFileName(
			AppendFileName(
				AppendFileName(
					GetAppModeDatabase().GetEntitiesDir(), snapshot->entity->file_title),
					snapshot->file_title),
				file_title);
	String json_path = dir + DIR_SEPS + "db.json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Component::GetAnyTitle(Entity& a) const {
	int focus_tc = -1, focus_arch = -1, focus_lyr = -1;
	a.FindComponent(focus_tc, focus_arch, focus_lyr, scripts_file_title);
	if (focus_tc >= 0) {
		const Script& l = a.typeclasses[focus_tc].contents[focus_arch].scripts[focus_lyr];
		if (l.native_title.GetCount())
			return l.native_title;
		
		if (l.english_title.GetCount())
			return l.english_title;
		
	}
	return file_title;
}


END_TEXTLIB_NAMESPACE

