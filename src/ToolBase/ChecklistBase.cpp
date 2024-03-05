#include "ToolBase.h"

#if 0
#include <SongDatabase/SongDatabase.h>

ChecklistBase::ChecklistBase() {
	Add(list.SizePos());
	
	list.AddColumn(t_("Key"));
	list.AddColumn(t_("Ready?"));
	list.AddColumn(t_("Comments"));
	list.ColumnWidths("12 1 24");
	
}

void ChecklistBase::Init() {
	InitKeys();
	
	for(int i = 0; i < descs.GetCount(); i++) {
		list.Set(i, 0, descs[i]);
		
		Option* o = new Option;
		opts << o;
		o->WhenAction << THISBACK1(OnOptionChange, i);
		list.SetCtrl(i, 1, o);
		
		EditString* e = new EditString;
		edits << e;
		e->WhenAction << THISBACK1(OnValueChange, i);
		list.SetCtrl(i, 2, e);
	}
}

void ChecklistBase::Data() {
	SongDatabase& db = SongDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.song) return;
	Song& song = *p.song;
	
	if (list.GetCount() == 0)
		Init();
	
	for(int i = 0; i < list.GetCount(); i++) {
		int b = StrInt(song.data.Get(keys[i] + "_VALUE", "0"));
		opts[i]->Set(b);
		String comment = song.data.Get(keys[i] + "_COMMENT", "");
		list.Set(i, 2, comment);
	}
}

void ChecklistBase::OnValueChange(int i) {
	SongDatabase& db = SongDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.song) return;
	Song& song = *p.song;
	
	String key = keys[i] + "_COMMENT";
	String value = list.Get(i, 2);
	int j = song.data.Find(key);
	if (j < 0 && value.IsEmpty())
		return;
	song.data.GetAdd(key) = value;
}

void ChecklistBase::OnOptionChange(int i) {
	SongDatabase& db = SongDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.song) return;
	Song& song = *p.song;
	
	Option* o = dynamic_cast<Option*>(list.GetCtrl(i, 1));
	if (o) {
		int b = o->Get();
		String key = keys[i] + "_VALUE";
		song.data.GetAdd(key) = IntStr(b);
	}
}

#endif
