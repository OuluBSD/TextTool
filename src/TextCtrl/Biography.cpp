#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyCtrl::BiographyCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << categories << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << years << year;
	vsplit.SetPos(3333);
	
	CtrlLayout(year);
	
	categories.AddColumn(t_("Required"));
	categories.AddColumn(t_("Category"));
	categories.AddColumn(t_("Entries"));
	categories.AddIndex("IDX");
	for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
		categories.Set(i, 1, GetBiographyCategoryKey(i));
		categories.Set(i, "IDX", i);
	}
	categories.ColumnWidths("1 5 1");
	categories.SetSortColumn(0);
	categories.SetCursor(0);
	categories <<= THISBACK(DataCategory);
	
	
	years.AddColumn(t_("Year"));
	years.AddColumn(t_("Age"));
	years.AddColumn(t_("Class"));
	years.AddColumn(t_("Keywords"));
	years.AddColumn(t_("Text"));
	years.AddIndex("IDX");
	years.ColumnWidths("1 1 1 5 10");
	years.WhenCursor << THISBACK(DataYear);
	
	
	year.keywords <<= THISBACK(OnValueChange);
	year.native_text <<= THISBACK(OnValueChange);
	year.text <<= THISBACK(OnValueChange);
	
}

void BiographyCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile) {
		for(int i = 0; i < categories.GetCount(); i++) {
			categories.Set(i, 0, 0);
			categories.Set(i, 2, 0);
		}
		return;
	}
	Owner& owner = *mp.owner;
	Profile& profile = *mp.profile;
	Biography& biography = mp.profile->biography_detailed;
	
	Index<int> req_cats = mp.profile->biography_analysis.GetRequiredCategories();
	for(int i = 0; i < categories.GetCount(); i++) {
		int cat_i = categories.Get(i, "IDX");
		bool req = req_cats.Find(cat_i) >= 0;
		categories.Set(i, 0, req ? "X" : "");
		BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
		int c = bcat.GetFilledCount();
		categories.Set(i, 2, c > 0 ? Value(c) : Value());
	}
	DataCategory();
}

void BiographyCtrl::DataCategory() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile || !categories.IsCursor()) {
		years.Clear();
		return;
	}
	Owner& owner = *mp.owner;
	Profile& profile = *mp.profile;
	Biography& biography = mp.profile->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	
	for(int i = 0; i < bcat.years.GetCount(); i++) {
		const BioYear& by = bcat.years[i];
		int age = by.year - owner.year_of_birth;
		int cls = age - 7;
		String cls_str;
		if (cls >= 0) {
			int round = cls / 12;
			cls = cls % 12;
			cls_str.Cat('A' + round);
			cls_str += " " + IntStr(1+cls);
		}
		years.Set(i, 0, by.year);
		years.Set(i, 1, age);
		years.Set(i, 2, cls_str);
		years.Set(i, 3, by.keywords);
		years.Set(i, 4, by.text);
		years.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(years);
	years.SetSortColumn(0, false);
	years.SetCount(bcat.years.GetCount());
	if (years.GetCount() && !years.IsCursor())
		years.SetCursor(0);
	
	DataYear();
}

void BiographyCtrl::DataYear() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile || !categories.IsCursor() || !years.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Profile& profile = *mp.profile;
	Biography& biography = mp.profile->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	
	year.year.SetData(by.year);
	year.keywords.SetData(by.keywords);
	year.native_text.SetData(by.native_text);
	year.text.SetData(by.text);
}

void BiographyCtrl::OnValueChange() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile || !categories.IsCursor() || !years.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Profile& profile = *mp.profile;
	Biography& biography = mp.profile->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	by.keywords = year.keywords.GetData();
	by.native_text = year.native_text.GetData();
	by.text = year.text.GetData();
	
	years.Set(year_i, 3, by.keywords);
	years.Set(year_i, 4, by.text);
}

void BiographyCtrl::MakeKeywords () {
	TaskMgr& m = TaskMgr::Single();
	SocialArgs args;
	args.text = year.text.GetData();
	m.GetSocial(args, [this](String s) {PostCallback(THISBACK1(OnKeywords, s));});
}

void BiographyCtrl::Translate() {
	TaskMgr& m = TaskMgr::Single();
	
	String src = year.native_text.GetData();
	
	m.Translate("FI-FI", src, "EN-US", [this](String s) {PostCallback(THISBACK1(OnTranslate, s));});
}

void BiographyCtrl::OnTranslate(String s) {
	year.text.SetData(s);
	OnValueChange();
}

void BiographyCtrl::OnKeywords(String s) {
	RemoveEmptyLines2(s);
	Vector<String> parts = Split(s, "\n");
	s = Join(parts, ", ");
	year.keywords.SetData(s);
	OnValueChange();
}

void BiographyCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Translate"), AppImg::BlueRing(), THISBACK(Translate)).Key(K_F5);
	bar.Add(t_("Make keywords"), AppImg::BlueRing(), THISBACK(MakeKeywords)).Key(K_F6);
}

void BiographyCtrl::EntryListMenu(Bar& bar) {
	
}


END_TEXTLIB_NAMESPACE
