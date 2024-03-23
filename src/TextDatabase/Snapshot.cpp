#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


void Snapshot::Store() {
	String dir = GetAppModeDatabase().GetSnapshotsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Snapshot::LoadTitle(String title) {
	String dir = GetAppModeDatabase().GetSnapshotsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

/*Component& Snapshot::RealizeReversed(Component& s) {
	for (Component& r : reversed_components)
		if (r.linked_uniq == s.uniq)
			return r;
	Component& r = reversed_components.Add();
	r.file_title = s.file_title;
	r.linked_uniq = s.uniq;
	r.artist = s.artist;
	r.title = s.title;
	r.prj_name = s.prj_name;
	r.structure_str = s.structure_str;
	r.data <<= s.data;
	r.content = s.content;
	r.owner = this;
	r.SetReversed();
	r.SetId(reversed_components.GetCount()-1);
	r.FixPtrs();
	return r;
}*/


END_TEXTLIB_NAMESPACE

