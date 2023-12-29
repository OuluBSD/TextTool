#include "ToolCore.h"
#include <Database/Database.h>



Song& SongToolCtrl::GetSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		throw NoPointerExc("no song");
	Song& song = *p.song;
	return song;
}

void SongToolCtrl::GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v.GetAdd(key) = Attr::AttrKeys[i][2];
			}
			else {
				v.GetAdd(key) = Attr::AttrKeys[i][3];
			}
		}
	}
}


String Capitalize(String s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}
