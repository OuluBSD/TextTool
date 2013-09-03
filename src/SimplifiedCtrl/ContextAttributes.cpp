#include "SimplifiedCtrl.h"
#include "SimplifiedCtrl.h"


ContextAttributesCtrl::ContextAttributesCtrl() {
	Add(hsplit.SizePos());
	
	hsplit << artist << release << song;
	
	InitList(artist);
	InitList(release);
	InitList(song);
	
	
}

void ContextAttributesCtrl::InitList(ArrayCtrl& attrs) {
	Database& db = Database::Single();
	
	attrs.AddColumn(t_("Key group"));
	attrs.AddColumn(t_("Positive"));
	attrs.AddColumn(t_("Negative"));
	attrs.AddColumn(t_("Value"));
	attrs.ColumnWidths("1 1 1 1");
	
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
}

void ContextAttributesCtrl::SetData(VectorMap<String,String>& data, ArrayCtrl& attrs) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		for(int j = 0; j < 1; j++) {
			DropList* dl = dynamic_cast<DropList*>(attrs.GetCtrl(i, 3+j));
			int value;
			value = StrInt(data.Get(key, "0"));
			dl->WhenAction = THISBACK3(OnAttrChange, &data, key, dl);
			int idx = 1 - value;
			dl->SetIndex(idx);
		}
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
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.release || !p.artist)
		return;
	
	SetData(p.artist->data, artist);
	SetData(p.release->data, release);
	SetData(p.song->data, song);
	
}
