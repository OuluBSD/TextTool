#include "ToolCore.h"
#include <SongDatabase/SongDatabase.h>



Lyrics& ToolAppCtrl::GetLyrics() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.lyrics)
		throw NoPointerExc("no lyrics");
	return *p.lyrics;
}

Song& ToolAppCtrl::GetSong() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.song || !p.artist)
		throw NoPointerExc("no song");
	Song& song = *p.song;
	return song;
}

Release& ToolAppCtrl::GetRelease() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.release)
		throw NoPointerExc("no release");
	return *p.release;
}

Artist& ToolAppCtrl::GetArtist() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.artist)
		throw NoPointerExc("no artist");
	return *p.artist;
}

String ToolAppCtrl::GetSongTitle() const {
	/*Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.song || !p.artist)
		throw NoPointerExc("no song");
	Song& song = *p.song;
	Artist& artist = *p.artist;
	String s;
	s << artist.english_name << " - " << song.english_title;
	return s;*/
	TODO
	return "";
}

/*int ToolAppCtrl::GetDataset() {
	Database& db = Database::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.song || !p.artist)
		throw NoPointerExc("no song");
	Song& song = *p.song;
	return ScanInt(song.data.Get("ATTR_DATASET", "0"));
}*/

void ToolAppCtrl::GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
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

void ToolAppCtrl::MakeSongParts(ArrayCtrl& parts) {
	/*Song& song = GetSong();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		//parts.Set(i, 0, sp.name);
		//parts.Set(i, 1, part.GetTypeString());
		parts.Set(i, 0,
			AttrText(sp.name).NormalPaper(GetSongPartPaperColor(sp.type)));
		
		DropList& dl = parts.CreateCtrl<DropList>(i, 1);
		for(int j = 0; j < StaticPart::PART_TYPE_COUNT; j++)
			dl.Add(StaticPart::GetTypeString(j));
		dl.SetIndex((int&)sp.part_type);
		dl.WhenAction << [&dl,i,&sp]() {(int&)sp.part_type = dl.GetIndex();};
	}
	INHIBIT_CURSOR(parts);
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);*/
}


String Capitalize(String s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}
