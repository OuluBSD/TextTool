#include "SongTool.h"


SongBriefing::SongBriefing() {
	Add(vsplit.SizePos());
	vsplit.Vert() << values << poll;
	
	CtrlLayout(values);
	values.list.AddIndex();
	values.list.AddColumn("Key");
	values.list.AddColumn("Value");
	values.list.ColumnWidths("1 4");
	values.list.WhenCursor << THISBACK(OnListCursor);
	values.value.WhenAction << THISBACK(OnValueChange);
	
	
	poll.AddColumn("Group");
	poll.AddColumn("Positive");
	poll.AddColumn("Negative");
	poll.AddColumn("Value");
	poll.ColumnWidths("1 1 1 3");
	
	#define ATTR_ITEM(e, g, i0, i1) \
		poll.Set(Attr::e, 0, g); \
		poll.Set(Attr::e, 1, i0); \
		poll.Set(Attr::e, 2, i1); \
		poll.SetCtrl(Attr::e, 3, SetAgreementValues(new DropList, i0, i1, Attr::e, "POLL_" #e));
	ATTR_LIST
	#undef ATTR_ITEM
}

DropList* SongBriefing::SetAgreementValues(DropList* dl, const char* positive, const char* negative, int idx, const char* key) {
	dl->WhenAction << THISBACK2(OnPollValueChange, idx, key);
	dl->Add("");
	dl->Add("Very " + String(positive));
	dl->Add("Sligthly " + String(positive));
	dl->Add("Neutral");
	dl->Add("Sligthly " + String(negative));
	dl->Add("Very " + String(negative));
	return dl;
}

void SongBriefing::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	{
		auto& list = values.list;
		auto& value = values.value;
		for(int i = 0; i < ITEM_COUNT; i++) {
			list.Set(i, 0, i);
			switch(i) {
				case ATTR_REFERENCE_SONG: list.Set(i, 1, "Reference song"); list.Set(i, 2, song.data.Get("ATTR_REFERENCE_SONG", "")); break;
				case ATTR_BIRTH_OF_SONG: list.Set(i, 1, "Birth of song"); list.Set(i, 2, song.data.Get("ATTR_BIRTH_OF_SONG", "")); break;
				
				#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: list.Set(i, 1, "Generic " g ": " i0 "/" i1); list.Set(i, 2, song.data.Get("ATTR_GENERIC_"#e, "")); break;
				ATTR_LIST
				#undef ATTR_ITEM
				
				#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: list.Set(i, 1, "Impact " g ": " i0 "/" i1); list.Set(i, 2, song.data.Get("ATTR_IMPACT_"#e, "")); break;
				ATTR_LIST
				#undef ATTR_ITEM
				
				default: break;
			}
		}
		list.SetCount(ITEM_COUNT);
		
		if (!list.IsCursor() && list.GetCount()) list.SetCursor(0);
	}
	
	{
		DropList* dl = 0;
		for(int i = 0; i < Attr::ATTR_COUNT; i++) {
			switch (i) {
				#define ATTR_ITEM(e, g, i0, i1) case Attr::e: \
					dl = dynamic_cast<DropList*>(poll.GetCtrl(i, 3)); \
					if (dl) \
						dl->SetIndex(StrInt(song.data.Get("POLL_"#e, "0"))); break;
				ATTR_LIST
				#undef ATTR_ITEM
			}
		}
	}
	
	OnListCursor();
}

void SongBriefing::OnListCursor() {
	{
		auto& list = values.list;
		auto& key = values.key;
		auto& description = values.description;
		auto& value = values.value;
		key.Clear();
		description.Clear();
		value.Clear();
		
		Database& db = Database::Single();
		EditorPtrs& p = db.ctx.ed;
		if (!p.song) return;
		Song& song = *p.song;
		
		String value_str;
		switch (list.GetCursor()) {
			case ATTR_REFERENCE_SONG:  key.SetData("Reference song"); description.SetData("Which song is used as the structural reference for lyrics?"); value_str = song.data.Get("ATTR_REFERENCE_SONG", ""); break;
			case ATTR_BIRTH_OF_SONG:  key.SetData("Birth of song"); description.SetData("How the song was born?"); value_str = song.data.Get("ATTR_BIRTH_OF_SONG", ""); break;
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: key.SetData("Generic " g ": " i0 "/" i1); description.SetData("Generic information around " g ": " i0 "/" i1); value_str = song.data.Get("ATTR_GENERIC_"#e, ""); break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: key.SetData("Impact " g ": " i0 "/" i1); description.SetData("Desired impact of " g ": " i0 "/" i1); value_str = song.data.Get("ATTR_IMPACT_"#e, ""); break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			default: break;
		}
		
		value.SetData(value_str);
	}
}

void SongBriefing::OnValueChange() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	{
		auto& list = values.list;
		auto& key = values.key;
		auto& description = values.description;
		auto& value = values.value;
		if (!list.IsCursor()) return;
		
		String key_str;
		switch (list.GetCursor()) {
			case ATTR_REFERENCE_SONG: key_str = "ATTR_REFERENCE_SONG"; break;
			case ATTR_BIRTH_OF_SONG: key_str = "ATTR_BIRTH_OF_SONG"; break;
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: key_str = "ATTR_GENERIC_"#e; break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: key_str = "ATTR_IMPACT_"#e; break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			default: return;
		}
		
		String value_str = value.GetData();
		song.data.GetAdd(key_str) = value_str;
		
		list.Set(2, value_str);
	}
}

void SongBriefing::OnPollValueChange(int i, const char* key) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	Ctrl* c = poll.GetCtrl(i, 3);
	DropList* dl = dynamic_cast<DropList*>(c);
	if (dl) {
		int val = dl->GetIndex();
		song.data.GetAdd(key) = IntStr(val);
	}
}

