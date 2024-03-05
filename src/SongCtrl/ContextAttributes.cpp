#include "SongCtrl.h"

#if 0


ContextAttributesCtrl::ContextAttributesCtrl() {
	Add(hsplit.SizePos());
	
	hsplit << artist << release << song;
	
	InitList(artist);
	InitList(release);
	InitList(song);
	
	
}

void ContextAttributesCtrl::InitList(ArrayCtrl& attrs) {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	if (sda.datasets.IsEmpty()) return;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	attrs.AddColumn(t_("Key group"));
	attrs.AddColumn(t_("Positive"));
	attrs.AddColumn(t_("Negative"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("IDX");
	attrs.ColumnWidths("1 1 1 1");
	
	#if 0
	int row = 0;
	DropList* dl;
	#define ATTR_ITEM(e, g, i0, i1) \
	attrs.Add(Capitalize(db.Translate(g)), Capitalize(db.Translate(i0)), Capitalize(db.Translate(i1))); \
	for (int i = 3; i < 4; i++) {\
		dl = new DropList; \
		dl->Add(GreenRedAttr(AttrText(db.Translate(i0)), 0)); \
		dl->Add(""); \
		dl->Add(GreenRedAttr(AttrText(db.Translate(i1)), 1)); \
		attrs.SetCtrl(row, i, dl); \
		dl->SetIndex(1); \
	} \
	row++;\
	
	ATTR_LIST
	#undef ATTR_ITEM
	
	#else
	
	int row = 0;
	for(int i = 0; i < da.simple_attrs.GetCount(); i++) {
		const String& group = da.simple_attrs.GetKey(i);
		const ExportSimpleAttr& esa = da.simple_attrs[i];
		const AttrHeader& ah0 = da.attrs.GetKey(esa.attr_i0);
		const AttrHeader& ah1 = da.attrs.GetKey(esa.attr_i1);
		ASSERT(ah0.group == ah1.group && ah1.group == group);
		
		String g = Capitalize(group);
		String a0 = Capitalize(ah0.value);
		String a1 = Capitalize(ah1.value);
		
		attrs.Set(row, 0, g);
		attrs.Set(row, 1, a0);
		attrs.Set(row, 2, a1);
		attrs.Set(row, "IDX", i);
		
		DropList* dl = new DropList;
		dl->Add(GreenRedAttr(AttrText(a0), 0));
		dl->Add("");
		dl->Add(GreenRedAttr(AttrText(a1), 1));
		dl->SetIndex(1);
		attrs.SetCtrl(row, 3, dl);
		
		row++;
	}
	attrs.SetCount(row);
	
	#endif
}

void ContextAttributesCtrl::SetData(VectorMap<String,String>& data, ArrayCtrl& attrs) {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	for(int i = 0; i < attrs.GetCount(); i++) {
		int j = attrs.Get(i, "IDX");
		const String& group = da.simple_attrs.GetKey(j);
		const ExportSimpleAttr& esa = da.simple_attrs[j];
		const AttrHeader& ah0 = da.attrs.GetKey(esa.attr_i0);
		const AttrHeader& ah1 = da.attrs.GetKey(esa.attr_i1);
		ASSERT(ah0.group == ah1.group && ah1.group == group);
		
		DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3));
		String key = "A" + IntStr(j);
		
		int value;
		value = StrInt(data.Get(key, "0"));
		dl->WhenAction = THISBACK3(OnAttrChange, &data, key, dl);
		int idx = 1 - value;
		dl->SetIndex(idx);
	}
}

void ContextAttributesCtrl::OnAttrChange(VectorMap<String,String>* data, const char* key, DropList* dl) {
	if (!data || !key || !dl) return;
	
	int idx = dl->GetIndex();
	int value = -(idx - 1);
	if (!value)
		data->RemoveKey(key);
	else
		data->GetAdd(key) = IntStr(value);
}

void ContextAttributesCtrl::Init() {
	
}

void ContextAttributesCtrl::ToolMenu(Bar& bar) {
	
	bar.Add("", AppImg::placeholder16(), Callback());
	
}

void ContextAttributesCtrl::Data() {
	SongDatabase& db = SongDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.song || !p.release || !p.artist)
		return;
	
	SetData(p.artist->data, artist);
	SetData(p.release->data, release);
	SetData(p.song->data, song);
	
}

#endif
