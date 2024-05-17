#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ImageBiographyCtrl::ImageBiographyCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << categories << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << years << bsplit;
	vsplit.SetPos(3333);
	
	bsplit.Horz() << entries << year << img;
	bsplit.SetPos(1500,0);
	bsplit.SetPos(5000,1);
	
	CtrlLayout(year);
	
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
	
	
	years.AddColumn(t_("Year"));
	years.AddColumn(t_("Age"));
	years.AddColumn(t_("Class"));
	years.AddColumn(t_("Images"));
	years.AddColumn(t_(""));
	years.AddIndex("IDX");
	years.ColumnWidths("1 1 1 1 15");
	years.WhenCursor << THISBACK(DataYear);
	
	
	entries.AddColumn(t_("#"));
	entries.AddColumn(t_("Time"));
	entries.AddColumn(t_("Keywords"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("1 1 1 1 15");
	entries.WhenCursor << THISBACK(DataEntry);
	entries.WhenBar << THISBACK(EntryListMenu);
	
	year.keywords <<= THISBACK(OnValueChange);
	year.native_text <<= THISBACK(OnValueChange);
	year.text <<= THISBACK(OnValueChange);
	
}

void ImageBiographyCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner) {
		for(int i = 0; i < categories.GetCount(); i++)
			categories.Set(i, 1, 0);
		return;
	}
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	
	for(int i = 0; i < categories.GetCount(); i++) {
		int cat_i = categories.Get(i, "IDX");
		BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
		categories.Set(i, 1, bcat.GetFilledImagesCount());
	}
	DataCategory();
}

void ImageBiographyCtrl::DataCategory() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor()) {
		years.Clear();
		return;
	}
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
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
		years.Set(i, 3, by.images.GetCount());
		years.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(years);
	years.SetSortColumn(0, false);
	years.SetCount(bcat.years.GetCount());
	if (years.GetCount() && !years.IsCursor())
		years.SetCursor(0);
	
	DataYear();
}

void ImageBiographyCtrl::DataYear() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor() || !years.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	
	for(int i = 0; i < by.images.GetCount(); i++) {
		const BioImage& bimg = by.images[i];
		entries.Set(i, 0, i);
		entries.Set(i, "IDX", i);
		entries.Set(i, 1, bimg.time);
		entries.Set(i, 2, bimg.image_keywords);
	}
	INHIBIT_CURSOR(entries);
	entries.SetSortColumn(0, false);
	entries.SetCount(by.images.GetCount());
	if (entries.GetCount() && !entries.IsCursor())
		entries.SetCursor(0);
	
	
	DataEntry();
}

void ImageBiographyCtrl::DataEntry() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor() || !years.IsCursor() || !entries.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	int entry_i = entries.Get("IDX");
	BioImage& bimg = by.images[entry_i];
	
	
	year.time.SetData(bimg.time);
	year.keywords.SetData(bimg.keywords);
	year.native_text.SetData(bimg.native_text);
	year.text.SetData(bimg.text);
	year.text.SetData(bimg.image_text);
	
}

void ImageBiographyCtrl::OnValueChange() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor() || !years.IsCursor() || !entries.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	int entry_i = entries.Get("IDX");
	BioImage& bimg = by.images[entry_i];
	
	bimg.keywords = year.keywords.GetData();
	bimg.native_text = year.native_text.GetData();
	bimg.text = year.text.GetData();
	bimg.image_keywords = year.image_keywords.GetData();
	bimg.image_text = year.image_text.GetData();
	
}

void ImageBiographyCtrl::MakeKeywords(int fn) {
	TaskMgr& m = TaskMgr::Single();
	SocialArgs args;
	if (fn == 0)
		args.text = year.text.GetData();
	else
		args.text = year.image_text.GetData();
	m.GetSocial(args, [this,fn](String s) {PostCallback(THISBACK2(OnKeywords, fn, s));});
}

void ImageBiographyCtrl::Translate() {
	TaskMgr& m = TaskMgr::Single();
	
	String src = year.native_text.GetData();
	
	m.Translate("FI-FI", src, "EN-US", [this](String s) {PostCallback(THISBACK1(OnTranslate, s));});
}

void ImageBiographyCtrl::OnTranslate(String s) {
	year.text.SetData(s);
	OnValueChange();
}

void ImageBiographyCtrl::OnKeywords(int fn, String s) {
	RemoveEmptyLines2(s);
	Vector<String> parts = Split(s, "\n");
	s = Join(parts, ", ");
	if (fn == 0)
		year.keywords.SetData(s);
	else
		year.image_keywords.SetData(s);
	OnValueChange();
}

void ImageBiographyCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Translate"), AppImg::BlueRing(), THISBACK(Translate)).Key(K_F5);
	bar.Add(t_("Make keywords"), AppImg::BlueRing(), THISBACK1(MakeKeywords, 0)).Key(K_F6);
	bar.Add(t_("Make keywords (image)"), AppImg::BlueRing(), THISBACK1(MakeKeywords, 1)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Paste Image path"), AppImg::BlueRing(), THISBACK(PasteImagePath)).Key(K_CTRL_V);
}

void ImageBiographyCtrl::EntryListMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_T);
	bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL|K_SHIFT|K_W);
	
}

void ImageBiographyCtrl::AddEntry() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor() || !years.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	
	by.images.Add();
	DataYear();
}

void ImageBiographyCtrl::RemoveEntry() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !categories.IsCursor() || !years.IsCursor() || !entries.IsCursor())
		return;
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	int cat_i = categories.Get("IDX");
	BiographyCategory& bcat = biography.GetAdd(owner, cat_i);
	int year_i = years.Get("IDX");
	if (year_i >= bcat.years.GetCount()) return;
	BioYear& by = bcat.years[year_i];
	int entry_i = entries.Get("IDX");
	if (entry_i >= 0 && entry_i < by.images.GetCount())
		by.images.Remove(entry_i);
	DataYear();
}

void ImageBiographyCtrl::PasteImagePath() {
	Image img = ReadClipboardImage();
	if (!img.IsEmpty()) {
		SetCurrentImage(img);
	}
	else {
		String path = ReadClipboardText();
		if (path.Left(7) == "file://") {
			path = path.Mid(7);
			img = StreamRaster::LoadFileAny(path);
			if (!img.IsEmpty())
				SetCurrentImage(img);
		}
	}
}

void ImageBiographyCtrl::SetCurrentImage(Image img) {
	
	this->img.SetImage(img);
	
}








ImageAnalyserCtrl::ImageAnalyserCtrl() {
	
}

void ImageAnalyserCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, Black());
	
	if (!img.IsEmpty()) {
		Size orig_sz = img.GetSize();
		double orig_small = max(orig_sz.cx, orig_sz.cy);
		double new_small = max(sz.cx, sz.cy);
		double ratio = new_small / orig_small;
		Size new_sz = orig_sz * ratio;
		Image scaled_img = CachedRescale(img, new_sz, FILTER_BILINEAR);
		
		if (new_sz.cx < new_sz.cy) {
			int off = (sz.cx - new_sz.cx) / 2;
			d.DrawImage(off,0,scaled_img);
		}
		else {
			int off = (sz.cy - new_sz.cy) / 2;
			d.DrawImage(0,off,scaled_img);
		}
	}
}

void ImageAnalyserCtrl::SetImage(const Image& i) {
	img = i;
	PostCallback([this](){Refresh();});
	
}



END_TEXTLIB_NAMESPACE
