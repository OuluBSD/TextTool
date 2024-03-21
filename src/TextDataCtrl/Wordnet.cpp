#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TextDataWordnet::TextDataWordnet() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << wordnets;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);
	
	wordnets.AddColumn(t_("Group"));
	wordnets.AddColumn(t_("Value"));
	wordnets.AddColumn(t_("Main class"));
	wordnets.AddColumn(t_("Anchor word"));
	wordnets.AddColumn(t_("#1 alternative"));
	wordnets.AddColumn(t_("#2 alternative"));
	wordnets.AddColumn(t_("#3 alternative"));
	wordnets.AddColumn(t_("#4 alternative"));
	wordnets.AddColumn(t_("#5 alternative"));
	wordnets.AddColumn(t_("#6 alternative"));
	wordnets.AddColumn(t_("#7 alternative"));
	wordnets.AddIndex("IDX");
	wordnets.ColumnWidths("1 2 1 1 1 1 1 1 1 1 1");
	
	
}

void TextDataWordnet::EnableAll() {
	
}

void TextDataWordnet::DisableAll() {
	
}

void TextDataWordnet::Data() {
	
}

void TextDataWordnet::DataMain() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}
	
void TextDataWordnet::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	
	int gi = 0;
	int i = 0;
	
	attrs.Set(i, 0, "All");
	attrs.Set(i, 1, "All");
	attrs.Set(i, "GROUP", -1);
	attrs.Set(i, "VALUE", -1);
	i++;
	
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
	ATTR_LIST
	#undef ATTR_ITEM

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	
	DataAttribute();
}

void TextDataWordnet::DataAttribute() {
	if (!attrs.IsCursor())
		return;
	
	
	
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	
	
	DataColor();
}

void TextDataWordnet::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	lock.Enter();
	
	int row = 0;
	for(int i = 0; i < da.wordnets.GetCount(); i++) {
		ExportWordnet& wn = da.wordnets[i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(wn.clr) != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter) {
			if (wn.attr < 0)
				continue;
			const AttrHeader& ah = da.attrs.GetKey(wn.attr);
			if (ah.group != group_str || ah.value != value_str)
				continue;
		}
		
		wordnets.Set(row, "IDX", i);
		
		if (wn.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(wn.attr);
			wordnets.Set(row, 0, ah.group);
			wordnets.Set(row, 1, ah.value);
		}
		else {
			wordnets.Set(row, 0, Value());
			wordnets.Set(row, 1, Value());
		}
		
		// Colored main class
		/*bool has_main_class_clr = false;
		Color main_class_clr;
		{
			int j = GetWordgroupColors().Find(wn.main_class);
			if (j >= 0) {
				main_class_clr = GetWordgroupColors()[j];
				has_main_class_clr = true;
			}
		}
		if (has_main_class_clr) {
			wordnets.Set(row, 2,
				AttrText(wn.main_class)
					.NormalPaper(Blend(main_class_clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(main_class_clr, GrayColor())).Ink(White())
			);
		}
		else {*/
		if (wn.main_class >= 0)
			wordnets.Set(row, 2, da.word_classes[wn.main_class]);
		else
			wordnets.Set(row, 2, Value());
		//}
		
		
		// Anchor word
		if (wn.word_count > 0) {
			wordnets.Set(row, 3,
				AttrText(wn.words[0])
					.NormalPaper(Blend(wn.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), wn.clr)).Ink(White()));
		}
		
		
		// Alternative words
		int c = min(8, wn.word_count);
		for(int j = 1; j < c; j++) {
			const String& w = da.words.GetKey(wn.words[j]);
			SetColoredListValue(
				wordnets,
				row, 3+j, w,
				wn.word_clrs[j]);
		}
		for(int j = c; j < 8; j++)
			wordnets.Set(row, 3+j, Value());
		
		row++;
	}
	wordnets.SetCount(row);
	wordnets.SetSortColumn(1, false);
	
	
	lock.Leave();
}

void TextDataWordnet::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	//bar.Add(t_("Make wordnets from template phrases"), AppImg::RedRing(), THISBACK1(DoWordnet, 0)).Key(K_F5);
	bar.Add(t_("Get color alternatives"), AppImg::RedRing(), THISBACK1(DoWordnet, 1)).Key(K_F6);
}

void TextDataWordnet::DoWordnet(int fn) {
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	tm.DoWordnet(0, fn);
}


END_TEXTLIB_NAMESPACE

