#include "SongTool.h"


ChecklistComposition::ChecklistComposition() {
	Add(list.SizePos());
	
	list.AddColumn("Key");
	list.AddColumn("Ready?");
	list.AddColumn("Comments");
	list.ColumnWidths("6 1 24");
	
	#define CHK_COMP(e, s) list.Add(s); keys.Add(#e);
	CHK_COMPOSITION_LIST
	#undef CHK_COMP
	for(int i = 0; i < list.GetCount(); i++) {
		Option* o = new Option;
		opts.Add(o);
		o->WhenAction << THISBACK1(OnOptionChange, i);
		list.SetCtrl(i, 1, o);
		
		EditString* e = new EditString;
		edits << e;
		e->WhenAction << THISBACK1(OnValueChange, i);
		list.SetCtrl(i, 2, e);
	}
}

void ChecklistComposition::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	for(int i = 0; i < list.GetCount(); i++) {
		int b = StrInt(song.data.Get(keys[i] + "_VALUE", "0"));
		opts[i]->Set(b);
		String comment = song.data.Get(keys[i] + "_COMMENT", "");
		list.Set(i, 2, comment);
	}
	
}

void ChecklistComposition::OnValueChange(int i) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	String key = keys[i] + "_COMMENT";
	String value = list.Get(i, 2);
	int j = song.data.Find(key);
	if (j < 0 && value.IsEmpty())
		return;
	song.data.GetAdd(key) = value;
}

void ChecklistComposition::OnOptionChange(int i) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	Option* o = dynamic_cast<Option*>(list.GetCtrl(i, 1));
	if (o) {
		int b = o->Get();
		String key = keys[i] + "_VALUE";
		song.data.GetAdd(key) = IntStr(b);
	}
}