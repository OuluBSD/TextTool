#include "SongCtrl.h"

#if 0


SongBriefing::SongBriefing() {
	Database& db = Database::Single();
	
	Add(vsplit.SizePos());
	vsplit.Vert() << list << values;
	
	CtrlLayout(values);
	list.AddIndex();
	list.AddColumn(t_("Key"));
	list.AddColumn(t_("Value"));
	list.ColumnWidths("1 4");
	list.WhenCursor << THISBACK(OnListCursor);
	values.value.WhenAction << THISBACK(OnValueChange);
	
	for(int i = 0; i < ITEM_COUNT; i++) {
		switch(i) {
			
			case ATTR_DATASET: {
				SongData& sd = Database::Single().song_data;
				DropList* dl = new DropList;
				list.SetCtrl(i, 1, dl);
				for(int j = 0; j < sd.GetCount(); j++) {
					dl->Add(sd.GetKey(j));
				}
				dl->SetIndex(0);
				dl->WhenAction << [this, dl]() {
					int tc = dl->GetIndex();
					GetSong().data.GetAdd("ATTR_DATASET") = IntStr(tc);
				};
			}
			break;
			
			case ATTR_TYPECAST: {
				DropList* dl = new DropList;
				list.SetCtrl(i, 1, dl);
				for(int j = 0; j < GetTypecasts().GetCount(); j++) {
					dl->Add(GetTypecasts()[j]);
				}
				dl->SetIndex(0);
				dl->WhenAction << [this, dl]() {
					int tc = dl->GetIndex();
					GetSong().data.GetAdd("ATTR_TYPECAST") = IntStr(tc);
				};
			}
			break;
			
			case ATTR_CONTRAST: {
				DropList* dl = new DropList;
				list.SetCtrl(i, 1, dl);
				for(int j = 0; j < GetContrasts().GetCount(); j++) {
					dl->Add(GetContrasts()[j].key);
				}
				dl->SetIndex(0);
				dl->WhenAction << [this, dl]() {
					int tc = dl->GetIndex();
					GetSong().data.GetAdd("ATTR_CONTRAST") = IntStr(tc);
				};
			}
			break;
			
			case ATTR_ARCHETYPE: {
				DropList* dl = new DropList;
				list.SetCtrl(i, 1, dl);
				for(int j = 0; j < GetArchetypes().GetCount(); j++) {
					dl->Add(GetArchetypes().GetKey(j));
				}
				dl->SetIndex(0);
				dl->WhenAction << [this, dl]() {
					int tc = dl->GetIndex();
					GetSong().data.GetAdd("ATTR_ARCHETYPE") = IntStr(tc);
				};
			}
			break;
			
			case ATTR_IS_RAPPER: {
				DropList* dl = new DropList;
				list.SetCtrl(i, 1, dl);
				dl->Add("Not a rap song");
				dl->Add("This is a rap song");
				dl->SetIndex(0);
				dl->WhenAction << [this, dl]() {
					int tc = dl->GetIndex();
					GetSong().data.GetAdd("ATTR_IS_RAPPER") = IntStr(tc);
				};
			}
			break;
			
			default: break;
			
		}
	}
	
	/*poll.AddColumn(t_("Group"));
	poll.AddColumn(t_("Positive"));
	poll.AddColumn(t_("Negative"));
	poll.AddColumn(t_("Value"));
	poll.ColumnWidths("1 1 1 3");
	
	#define ATTR_ITEM(e, g, i0, i1) \
		poll.Set(Attr::e, 0, Capitalize(db.Translate(g))); \
		poll.Set(Attr::e, 1, Capitalize(db.Translate(i0))); \
		poll.Set(Attr::e, 2, Capitalize(db.Translate(i1))); \
		poll.SetCtrl(Attr::e, 3, SetAgreementValues(new DropList, i0, i1, Attr::e, "POLL_" #e));
	ATTR_LIST
	#undef ATTR_ITEM*/
}

/*DropList* SongBriefing::SetAgreementValues(DropList* dl, const char* positive, const char* negative, int idx, const char* key) {
	Database& db = Database::Single();
	dl->WhenAction << THISBACK2(OnPollValueChange, idx, key);
	dl->Add("");
	dl->Add(t_("Very ") + db.Translate(positive));
	dl->Add(t_("Sligthly ") + db.Translate(positive));
	dl->Add(t_("Neutral"));
	dl->Add(t_("Sligthly ") + db.Translate(negative));
	dl->Add(t_("Very ") + db.Translate(negative));
	return dl;
}*/

void SongBriefing::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	
	{
		auto& value = values.value;
		String astr;
		for(int i = 0; i < ITEM_COUNT; i++) {
			list.Set(i, 0, i);
			switch(i) {
				case ATTR_REFERENCE_SONG: list.Set(i, 1, t_("Reference song")); list.Set(i, 2, song.data.Get("ATTR_REFERENCE_SONG", "")); break;
				case ATTR_BIRTH_OF_SONG: list.Set(i, 1, t_("Birth of song")); list.Set(i, 2, song.data.Get("ATTR_BIRTH_OF_SONG", "")); break;
				case ATTR_CONTENT_VISION: list.Set(i, 1, t_("Content vision")); list.Set(i, 2, song.data.Get("ATTR_CONTENT_VISION", "")); break;
				case ATTR_DATASET: {
					list.Set(i, 1, t_("Song uses dataset"));
					int tc = ScanInt(song.data.Get("ATTR_DATASET", "0"));
					DropList* dl = dynamic_cast<DropList*>(list.GetCtrl(i, 1));
					if (dl && tc >= 0 && tc < dl->GetCount()) dl->SetIndex(tc);
				}
				break;
				case ATTR_TYPECAST: {
					list.Set(i, 1, t_("Artist's typecast"));
					int tc = ScanInt(song.data.Get("ATTR_TYPECAST", "0"));
					DropList* dl = dynamic_cast<DropList*>(list.GetCtrl(i, 1));
					if (dl && tc >= 0 && tc < dl->GetCount()) dl->SetIndex(tc);
				}
				break;
				
				case ATTR_CONTRAST: {
					list.Set(i, 1, t_("Content's contrast"));
					int tc = ScanInt(song.data.Get("ATTR_CONTRAST", "0"));
					DropList* dl = dynamic_cast<DropList*>(list.GetCtrl(i, 1));
					if (dl && tc >= 0 && tc < dl->GetCount()) dl->SetIndex(tc);
				}
				break;
				
				case ATTR_ARCHETYPE: {
					list.Set(i, 1, t_("Content's archetype"));
					int tc = ScanInt(song.data.Get("ATTR_ARCHETYPE", "0"));
					DropList* dl = dynamic_cast<DropList*>(list.GetCtrl(i, 1));
					if (dl && tc >= 0 && tc < dl->GetCount()) dl->SetIndex(tc);
				}
				break;
				
				case ATTR_IS_RAPPER: {
					list.Set(i, 1, t_("Singing style"));
					int tc = ScanInt(song.data.Get("ATTR_IS_RAPPER", "0"));
					DropList* dl = dynamic_cast<DropList*>(list.GetCtrl(i, 1));
					if (dl && tc >= 0 && tc < dl->GetCount()) dl->SetIndex(tc);
				}
				break;
				
				/*#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: \
					astr = db.Translate(g) + ": " + db.Translate(i0) + "/" + db.Translate(i1); \
					list.Set(i, 1, Format(t_("Generic %"), astr)); list.Set(i, 2, song.data.Get("ATTR_GENERIC_"#e, "")); break;
				ATTR_LIST
				#undef ATTR_ITEM
				
				#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: \
					astr = db.Translate(g) + ": " + db.Translate(i0) + "/" + db.Translate(i1); \
					list.Set(i, 1, Format(t_("Impact %"), astr)); list.Set(i, 2, song.data.Get("ATTR_IMPACT_"#e, "")); break;
				ATTR_LIST
				#undef ATTR_ITEM*/
				
				default: break;
			}
		}
		list.SetCount(ITEM_COUNT);
		
		if (!list.IsCursor() && list.GetCount()) list.SetCursor(0);
	}
	
	/*{
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
	}*/
	
	OnListCursor();
}

void SongBriefing::OnListCursor() {
	Database& db = Database::Single();
	{
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
		
		String astr;
		String value_str;
		switch (list.GetCursor()) {
			case ATTR_REFERENCE_SONG:  key.SetData(t_("Reference song")); description.SetData(t_("Which song is used as the structural reference for lyrics?")); value_str = song.data.Get("ATTR_REFERENCE_SONG", ""); break;
			case ATTR_BIRTH_OF_SONG:  key.SetData(t_("Birth of song")); description.SetData(t_("How the song was born?")); value_str = song.data.Get("ATTR_BIRTH_OF_SONG", ""); break;
			case ATTR_CONTENT_VISION:  key.SetData(t_("Content vision")); description.SetData(t_("What is the vision for the song?")); value_str = song.data.Get("ATTR_CONTENT_VISION", ""); break;
			
			/*#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: \
				astr = db.Translate(g) + ": " + db.Translate(i0) + "/" + db.Translate(i1); \
				key.SetData(Format(t_("Generic %"), astr)); description.SetData(Format(t_("Generic information around %s"), astr)); value_str = song.data.Get("ATTR_GENERIC_"#e, ""); break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: \
				astr = db.Translate(g) + ": " + db.Translate(i0) + "/" + db.Translate(i1); \
				key.SetData(Format(t_("Impact %"), astr)); description.SetData(Format(t_("Desired impact of %s"), astr)); value_str = song.data.Get("ATTR_IMPACT_"#e, ""); break;
			ATTR_LIST
			#undef ATTR_ITEM*/
			
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
		auto& key = values.key;
		auto& description = values.description;
		auto& value = values.value;
		if (!list.IsCursor()) return;
		
		String key_str;
		switch (list.GetCursor()) {
			case ATTR_REFERENCE_SONG: key_str = "ATTR_REFERENCE_SONG"; break;
			case ATTR_BIRTH_OF_SONG: key_str = "ATTR_BIRTH_OF_SONG"; break;
			case ATTR_CONTENT_VISION: key_str = "ATTR_CONTENT_VISION"; break;
			
			/*#define ATTR_ITEM(e, g, i0, i1) case ITEM_GENERIC_##e: key_str = "ATTR_GENERIC_"#e; break;
			ATTR_LIST
			#undef ATTR_ITEM
			
			#define ATTR_ITEM(e, g, i0, i1) case ITEM_IMPACT_##e: key_str = "ATTR_IMPACT_"#e; break;
			ATTR_LIST
			#undef ATTR_ITEM*/
			
			default: return;
		}
		
		String value_str = value.GetData();
		song.data.GetAdd(key_str) = value_str;
		
		list.Set(2, value_str);
	}
}

/*void SongBriefing::OnPollValueChange(int i, const char* key) {
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
}*/

#endif
