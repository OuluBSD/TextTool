#include "SongTool.h"


StoryCtrl::StoryCtrl() {
	Add(list.SizePos());
	
	list.AddColumn(t_("Key"));
	list.AddColumn(t_("Value"));
	list.ColumnWidths("1 5");
	list.SetLineCy(list.GetLineCy() * 2);
	
}

void StoryCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& song = *p.song;
	int mode = p.mode;
	
	Index<int> story_values;
	
	
	for(int i = 0; i < song.snap[mode].data.GetCount(); i++) {
		String key = song.snap[mode].data.GetKey(i);
		if (key.Find(" arc") >= 0 || key.Find("theme") >= 0)
			story_values.Add(i);
	}
	
	
	for(int i = 0; i < story_values.GetCount(); i++) {
		int j = story_values[i];
		String key = song.snap[mode].data.GetKey(j);
		String value = song.snap[mode].data[j];
		
		list.Set(i, 0, Capitalize(key));
		list.Set(i, 1, value);
		
		EditString* edit = new EditString;
		edit->WhenAction << [&, this, j, mode, key, edit]() {
			song.snap[mode].data[j] = edit->GetData();
		};
		list.SetCtrl(i, 1, edit);
	}
	list.SetCount(story_values.GetCount());
	
	
	
}
