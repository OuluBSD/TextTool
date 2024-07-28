#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyElementsCtrl::BiographyElementsCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << categories << vsplit;
	hsplit.SetPos(1500, 0);
	hsplit.SetPos(3000, 1);
	
	vsplit.Vert() << blocks << block;
	vsplit.SetPos(3333);
	
	categories.AddColumn(t_("Category"));
	categories.AddColumn(t_("Entries"));
	categories.AddIndex("IDX");
	for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
		categories.Set(i, 0, GetBiographyCategoryKey(i));
		categories.Set(i, "IDX", i);
	}
	categories.ColumnWidths("5 1");
	categories.SetSortColumn(0);
	categories.SetCursor(0);
	categories <<= THISBACK(DataCategory);
	
	
	blocks.AddColumn(t_("First Year"));
	blocks.AddColumn(t_("Last Year"));
	blocks.AddColumn(t_("Year count"));
	blocks.AddColumn(t_("Age"));
	blocks.AddColumn(t_("Class"));
	blocks.AddColumn(t_("Keywords"));
	blocks.AddColumn(t_("Text"));
	blocks.AddIndex("IDX");
	blocks.AddIndex("SRC");
	blocks.ColumnWidths("1 1 1 1 1 5 10");
	blocks.WhenCursor << THISBACK(DataYear);
	
	
}

void BiographyElementsCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !mp.biography) {
		for(int i = 0; i < categories.GetCount(); i++)
			categories.Set(i, 1, 0);
		return;
	}
	Owner& owner = *mp.owner;
	Biography& biography = *mp.biography;
	
	for(int i = 0; i < categories.GetCount(); i++) {
		int cat_i = categories.Get(i, "IDX");
		BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
		//categories.Set(i, 1, bcat.GetFilledCount());
	}
	
	DataCategory();
}

void BiographyElementsCtrl::DataCategory() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !mp.biography || !categories.IsCursor()) {
		blocks.Clear();
		return;
	}
	Owner& owner = *mp.owner;
	Biography& biography = *mp.biography;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	bcat.RealizeSummaries();
	
	int r = 0;
	for(int i = 0; i < bcat.years.GetCount(); i++) {
		const BioYear& by = bcat.years[i];
		if (by.text.IsEmpty())
			continue;
		int age = by.year - owner.year_of_birth;
		int cls = age - 7;
		String cls_str;
		if (cls >= 0) {
			int round = cls / 12;
			cls = cls % 12;
			cls_str.Cat('A' + round);
			cls_str += " " + IntStr(1+cls);
		}
		blocks.Set(r, 0, by.year);
		blocks.Set(r, 1, by.year);
		blocks.Set(r, 2, 1);
		blocks.Set(r, 3, age);
		blocks.Set(r, 4, cls_str);
		blocks.Set(r, 5, by.keywords);
		blocks.Set(r, 6, by.text);
		blocks.Set(r, "IDX", i);
		blocks.Set(r, "SRC", 0);
		r++;
	}
	for(int i = 0; i < bcat.summaries.GetCount(); i++) {
		const auto& range = bcat.summaries.GetKey(i);
		const BioYear& by = bcat.summaries[i];
		if (by.text.IsEmpty())
			continue;
		int age = by.year - owner.year_of_birth;
		int cls = age - 7;
		String cls_str;
		if (cls >= 0) {
			int round = cls / 12;
			cls = cls % 12;
			cls_str.Cat('A' + round);
			cls_str += " " + IntStr(1+cls);
		}
		blocks.Set(r, 0, range.off);
		blocks.Set(r, 1, range.off + range.len - 1);
		blocks.Set(r, 2, range.len);
		blocks.Set(r, 3, age);
		blocks.Set(r, 4, cls_str);
		blocks.Set(r, 5, by.keywords);
		blocks.Set(r, 6, by.text);
		blocks.Set(r, "IDX", i);
		blocks.Set(r, "SRC", 1);
		r++;
	}
	INHIBIT_CURSOR(blocks);
	blocks.SetCount(r);
	blocks.SetSortColumn(1, false);
	if (blocks.GetCount() && !blocks.IsCursor())
		blocks.SetCursor(0);
	
	DataYear();
}

void BiographyElementsCtrl::DataYear() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !mp.biography || !categories.IsCursor() || !blocks.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = *mp.biography;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int src_i = blocks.Get("SRC");
	int block_i = blocks.Get("IDX");
	if (src_i == 0) {
		if (block_i >= bcat.years.GetCount()) return;
		BioYear& by = bcat.years[block_i];
	}
	else if (src_i == 1) {
		if (block_i >= bcat.summaries.GetCount()) return;
		BioYear& by = bcat.summaries[block_i];
	}
	
	/*block.year.SetData(by.year);
	block.keywords.SetData(by.keywords);
	block.native_text.SetData(by.native_text);
	block.text.SetData(by.text);*/
}

void BiographyElementsCtrl::OnValueChange() {
	
}

void BiographyElementsCtrl::ToolMenu(Bar& bar) {
	
}


END_TEXTLIB_NAMESPACE

