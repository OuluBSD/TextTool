#include "SongTool.h"


StoryCtrl::StoryCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << list[0] << list[1];
	
	for(int i = 0; i < GENDER_COUNT; i++) {
		ArrayCtrl& list = this->list[i];
		list.AddColumn(t_("Key"));
		list.AddColumn(t_("Value"));
		list.ColumnWidths("1 5");
		list.SetLineCy(list.GetLineCy() * 2);
		
		list.WhenAction << THISBACK1(SelectLine, i);
	}
	
}

void StoryCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		ArrayCtrl& list = this->list[mode];
		Index<int> story_values;
		
		
		for(int i = 0; i < song.snap[mode].data.GetCount(); i++) {
			String key = song.snap[mode].data.GetKey(i);
			if (key.Find(" arc") >= 0 || key.Find("theme") >= 0)
				story_values.Add(i);
		}
		
		
		Color clr = Blend(LtRed(), White(), 256-32);
		int total = 0;
		for(int i = 0; i < story_values.GetCount(); i++) {
			int j = story_values[i];
			String key = song.snap[mode].data.GetKey(j);
			String value = song.snap[mode].data[j];
			
			list.Set(i, 0, AttrText(Capitalize(key)).NormalPaper(clr));
			list.Set(i, 1, AttrText(value).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&, this, j, mode, key, edit]() {
				song.snap[mode].data[j] = edit->GetData();
			};
			list.SetCtrl(i, 1, edit);
			
			total++;
		}
		
		clr = Blend(LtBlue(), White(), 256-32);
		for(int i = 0; i < song.parts.GetCount(); i++) {
			Part& part = song.parts[i];
			String& value = part.snap[mode].data.GetAdd("storyline");
			
			list.Set(total, 0, AttrText("Story of part " + part.name).NormalPaper(clr));
			list.Set(total, 1, AttrText(value).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&, this, mode, edit]() {
				part.snap[mode].data.GetAdd("storyline") = edit->GetData();
			};
			list.SetCtrl(total, 1, edit);
			
			total++;
		}
		list.SetCount(total);
		
	}
}

void StoryCtrl::SelectLine(int match) {
	
	// Select same kay in other lists too
	if (match >= 0 && match < GENDER_COUNT) {
		ArrayCtrl& list = this->list[match];
		if (list.IsCursor()) {
			String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (i == match) continue;
				ArrayCtrl& list = this->list[i];
				bool found = false;
				for(int j = 0; j < list.GetCount(); j++) {
					if (AttrText(list.Get(j, 0)).text.ToString() == key) {
						list.SetCursor(j);
						found = true;
						break;
					}
				}
				if (!found)
					list.KillCursor();
			}
		}
	}
}
