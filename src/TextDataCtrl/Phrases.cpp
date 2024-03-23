#include "TextDataCtrl.h"

#if 0

SongDataPhrases::SongDataPhrases() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	prog.Set(0,1);
	
	hsplit.Horz() << vsplit << components;
	hsplit.SetPos(4000);
	
	vsplit.Vert() << datasets << attrs;
	vsplit.SetPos(1000);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	components.AddColumn(t_("Phrase"));
	components.AddColumn(t_("Entity"));
	components.AddColumn(t_("Song"));
	components.ColumnWidths("3 1 1");
	
}

void SongDataPhrases::EnableAll() {
	disabled = false;
	datasets.Enable();
	attrs.Enable();
	components.Enable();
}

void SongDataPhrases::DisableAll() {
	disabled = true;
	datasets.Disable();
	attrs.Disable();
	components.Disable();
}

void SongDataPhrases::Data() {
	DataDataset();
}

void SongDataPhrases::DataDataset() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.comp_data;
	TextDataAnalysis& sda = db.comp_data.a;
	
	datasets.SetCount(sda.datasets.GetCount());
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	if (!datasets.IsCursor() && datasets.GetCount())
		datasets.SetCursor(0);
	
	DataMain();
}

void SongDataPhrases::DataMain() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.comp_data;
	TextDataAnalysis& sda = db.comp_data.a;
	
	if (!datasets.IsCursor()) {
		attrs.Clear();
		return;
	}
	
	
	int gi = 0;
	#define ATTR_ITEM(e, g, i0, i1) \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i0); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 0); \
		i++; \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i1); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 1); \
		i++, gi++;
	int i = 0;
	ATTR_LIST
	#undef ATTR_ITEM

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	DataAttribute();
}

void SongDataPhrases::DataAttribute() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.comp_data;
	TextDataAnalysis& sda = db.comp_data.a;
	
	if (!datasets.IsCursor() || !attrs.IsCursor()) {
		components.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String group_str = ToLower(Attr::AttrKeys[attr_group_i][1]);
	String value_str = ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]);
	
	int row = 0;
	for(int i = 0; i < da.entities.GetCount(); i++) {
		EntityAnalysis& artist = da.entities[i];
		for(int j = 0; j < artist.components.GetCount(); j++) {
			ScriptAnalysis& song = artist.components[j];
			for(int k = 0; k < song.phrases.GetCount(); k++) {
				ScriptAnalysis::Phrase& phrase = song.phrases[k];
				if (phrase.group == group_str && phrase.value == value_str) {
					components.Set(row, 0,
						AttrText(phrase.phrase)
							.NormalPaper(Blend(phrase.clr, White(), 128+64)).NormalInk(Black())
							.Paper(Blend(phrase.clr, GrayColor())).Ink(White()));
					components.Set(row, 1, da.entities.GetKey(i));
					components.Set(row, 2, song.name);
					row++;
				}
			}
		}
	}
	components.SetCount(row);
	if (!components.IsCursor() && components.GetCount())
		components.SetCursor(0);
	
}

void SongDataPhrases::ToolMenu(Bar& bar) {
	ToolAppCtrl::ToolMenu(bar);
}

#endif
