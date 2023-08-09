#include "SongTool.h"


StoryCtrl::StoryCtrl() {
	Add(vsplit.SizePos());
	
	for (const SnapArg& a : ModeArgs())
		vsplit.Vert() << list[a];
	
	for (const SnapArg& a : ModeArgs()) {
		ArrayCtrl& list = this->list[a];
		list.AddColumn(t_("Key"));
		list.AddColumn(t_("Value"));
		list.ColumnWidths("1 5");
		list.SetLineCy(list.GetLineCy() * 2);
		
		list.WhenAction << THISBACK1(SelectLine, a);
	}
	
}

void StoryCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	if (!song.pipe) return;
	Pipe& pipe = *song.pipe;
	
	for (const SnapArg& a : ModeArgs()) {
		ArrayCtrl& list = this->list[a];
		Index<int> story_values;
		
		
		for(int i = 0; i < pipe.Get(a).data.GetCount(); i++) {
			String key = pipe.Get(a).data.GetKey(i);
			if (key.Find(" arc") >= 0 || key.Find("theme") >= 0)
				story_values.Add(i);
		}
		
		
		Color clr = Blend(LtRed(), White(), 256-32);
		int total = 0;
		for(int i = 0; i < story_values.GetCount(); i++) {
			int j = story_values[i];
			String key = pipe.Get(a).data.GetKey(j);
			String value = pipe.Get(a).data[j];
			
			list.Set(i, 0, AttrText(Capitalize(key)).NormalPaper(clr));
			list.Set(i, 1, AttrText(value).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&, this, j, a, key, edit]() {
				pipe.Get(a).data[j] = edit->GetData();
			};
			list.SetCtrl(i, 1, edit);
			
			total++;
		}
		
		clr = Blend(LtBlue(), White(), 256-32);
		for(int i = 0; i < pipe.parts.GetCount(); i++) {
			Part& part = pipe.parts[i];
			String& value = part.Get(a).data.GetAdd("storyline");
			
			list.Set(total, 0, AttrText("Story of part " + part.name).NormalPaper(clr));
			list.Set(total, 1, AttrText(value).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&, this, a, edit]() {
				part.Get(a).data.GetAdd("storyline") = edit->GetData();
			};
			list.SetCtrl(total, 1, edit);
			
			total++;
		}
		list.SetCount(total);
		
	}
}

void StoryCtrl::SelectLine(const SnapArg& match) {
	
	// Select same kay in other lists too
	ArrayCtrl& list = this->list[match];
	if (list.IsCursor()) {
		String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
		for(const SnapArg& a : ModeArgs()) {
			if (a == match) continue;
			ArrayCtrl& list = this->list[a];
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
