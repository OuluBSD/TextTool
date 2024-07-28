#include "TextCtrl.h"

BEGIN_TEXTLIB_NAMESPACE


MarketplaceCtrl::MarketplaceCtrl() {
	Add(hsplit.VSizePos(0,20).HSizePos());
	Add(prog.BottomPos(0,20).HSizePos(300));
	Add(remaining.BottomPos(0,20).LeftPos(0,300));
	
	hsplit.Horz() << items << form << imgsplit;
	hsplit.SetPos(1500, 0);
	hsplit.SetPos(1500+4000, 1);
	
	imgsplit.Vert() << images << img;
	imgsplit.SetPos(2500);
	
	CtrlLayout(form);
	
	items.AddColumn(t_("Generic name"));
	items.AddColumn(t_("Brand"));
	items.AddColumn(t_("Model"));
	items.AddColumn(t_("Price"));
	items.AddIndex("IDX");
	items.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Add item"), THISBACK1(Do, 0));
		bar.Add(t_("Remove item"), THISBACK1(Do, 1));
	};
	items.WhenCursor << THISBACK(DataItem);
	
	images.AddColumn(t_("#"));
	images.WhenCursor << THISBACK(DataImage);
	images.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Add image from clipboard"), THISBACK1(Do, 2));
		bar.Add(t_("Set image from clipboard"), THISBACK1(Do, 3));
		bar.Add(t_("Remove image"), THISBACK1(Do, 4));
		bar.Add(t_("Move up"), THISBACK1(Do, 5));
		bar.Add(t_("Move down"), THISBACK1(Do, 6));
	};
	form.added		<<= THISBACK(OnValueChange);
	form.generic	<<= THISBACK(OnValueChange);
	form.brand		<<= THISBACK(OnValueChange);
	form.model		<<= THISBACK(OnValueChange);
	form.price		<<= THISBACK(OnValueChange);
	form.width		<<= THISBACK(OnValueChange);
	form.height		<<= THISBACK(OnValueChange);
	form.depth		<<= THISBACK(OnValueChange);
	form.weight		<<= THISBACK(OnValueChange);
	form.faults		<<= THISBACK(OnValueChange);
	form.works		<<= THISBACK(OnValueChange);
	form.broken		<<= THISBACK(OnValueChange);
	form.good		<<= THISBACK(OnValueChange);
}

void MarketplaceCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	if (!p.owner) {
		items.Clear();
		images.Clear();
		ClearForm();
		return;
	}
	
	for(int i = 0; i < p.owner->marketplace.items.GetCount(); i++) {
		MarketplaceItem& mi = p.owner->marketplace.items[i];
		items.Set(i, 0, mi.generic);
		items.Set(i, 1, mi.brand);
		items.Set(i, 2, mi.model);
		items.Set(i, 3, mi.price);
		items.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(items);
	items.SetCount(p.owner->marketplace.items.GetCount());
	if (!items.IsCursor() && items.GetCount())
		items.SetCursor(0);
	
	DataItem();
}

void MarketplaceCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Add item"), AppImg::BlueRing(), THISBACK1(Do, 0)).Key(K_CTRL_T);
	bar.Add(t_("Remove item"), AppImg::BlueRing(), THISBACK1(Do, 1)).Key(K_CTRL|K_W|K_SHIFT);
	bar.Separator();
	bar.Add(t_("Add image from clipboard"), AppImg::BlueRing(), THISBACK1(Do, 2)).Key(K_F5);
	bar.Add(t_("Set image from clipboard"), AppImg::BlueRing(), THISBACK1(Do, 3)).Key(K_F6);
	bar.Add(t_("Remove image"), AppImg::BlueRing(), THISBACK1(Do, 4)).Key(K_F7);
	bar.Add(t_("Move up"), AppImg::BlueRing(), THISBACK1(Do, 5)).Key(K_F8);
	bar.Add(t_("Move down"), AppImg::BlueRing(), THISBACK1(Do, 6)).Key(K_F9);
	
}

void MarketplaceCtrl::ClearForm() {
	form.added.Clear();
	form.generic.Clear();
	form.brand.Clear();
	form.model.Clear();
	form.price.Clear();
	form.width.Clear();
	form.height.Clear();
	form.depth.Clear();
	form.weight.Clear();
	form.faults.Clear();
	form.works.Clear();
	form.broken.Set(0);
	form.good.Set(0);
}

void MarketplaceCtrl::DataItem() {
	if (!items.IsCursor()) {
		images.Clear();
		ClearForm();
		img.Clear();
		return;
	}
	
	MetaPtrs& p = MetaPtrs::Single();
	int item_i = items.Get("IDX");
	MarketplaceItem& mi = p.owner->marketplace.items[item_i];
	
	form.added.SetData(mi.added);
	form.generic.SetData(mi.generic);
	form.brand.SetData(mi.brand);
	form.model.SetData(mi.model);
	form.price.SetData(mi.price);
	form.width.SetData(mi.cx);
	form.height.SetData(mi.cy);
	form.depth.SetData(mi.cz);
	form.weight.SetData(mi.weight);
	form.faults.SetData(mi.faults);
	form.works.SetData(mi.works);
	form.broken.SetData(mi.broken);
	form.good.SetData(mi.good);
	
	for(int i = 0; i < mi.images.GetCount(); i++) {
		hash_t h = mi.images[i];
		images.Set(i, 0, i);
	}
	images.SetCount(mi.images.GetCount());
	INHIBIT_CURSOR(images);
	if (images.GetCount() && !images.IsCursor())
		images.SetCursor(0);
	
	DataImage();
}

void MarketplaceCtrl::DataImage() {
	if (!images.IsCursor()) {
		img.Clear();
		return;
	}
	MetaPtrs& p = MetaPtrs::Single();
	int item_i = items.Get("IDX");
	MarketplaceItem& mi = p.owner->marketplace.items[item_i];
	int img_i = images.GetCursor();
	hash_t h = mi.images[img_i];
	
	if (h) {
		String path = CacheImageFile(h);
		Image i = StreamRaster::LoadFileAny(path);
		this->img.SetImage(i);
	}
	else {
		this->img.Clear();
	}
}

void MarketplaceCtrl::Do(int fn) {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	if (fn == 0) {
		if (!p.owner) return;
		MarketplaceItem& mi = p.owner->marketplace.items.Add();
		mi.added = GetSysTime();
		PostCallback(THISBACK(Data));
	}
	else if (fn == 1) {
		if (!p.owner || !items.IsCursor()) return;
		int item_i = items.Get("IDX");
		p.owner->marketplace.items.Remove(item_i);
		PostCallback(THISBACK(Data));
	}
	else if (fn == 2) {
		if (!p.owner || !items.IsCursor()) return;
		int item_i = items.Get("IDX");
		MarketplaceItem& mi = p.owner->marketplace.items[item_i];
		int img_i = mi.images.GetCount();
		mi.images.Add(0);
		images.Set(img_i,0,img_i);
		images.SetCursor(img_i);
		PasteImagePath();
	}
	else if (fn == 3) {
		PasteImagePath();
	}
	else if (fn == 4) {
		if (!p.owner || !items.IsCursor() || !images.IsCursor()) return;
		int item_i = items.Get("IDX");
		int img_i = images.GetCursor();
		MarketplaceItem& mi = p.owner->marketplace.items[item_i];
		mi.images.Remove(img_i);
	}
	else if (fn == 5) {
		if (!p.owner || !items.IsCursor() || !images.IsCursor()) return;
		int item_i = items.Get("IDX");
		MarketplaceItem& mi = p.owner->marketplace.items[item_i];
		int img_i = images.GetCursor();
		if (img_i == 0) return;
		hash_t h = mi.images[img_i];
		mi.images.Remove(img_i);
		mi.images.Insert(img_i-1, h);
		images.SetCursor(img_i-1);
	}
	else if (fn == 6) {
		if (!p.owner || !items.IsCursor() || !images.IsCursor()) return;
		int item_i = items.Get("IDX");
		MarketplaceItem& mi = p.owner->marketplace.items[item_i];
		int img_i = images.GetCursor();
		if (img_i == mi.images.GetCount()-1) return;
		hash_t h = mi.images[img_i];
		mi.images.Remove(img_i);
		mi.images.Insert(img_i+1, h);
		images.SetCursor(img_i+1);
	}
	
}

void MarketplaceCtrl::OnValueChange(){
	if (!items.IsCursor()) {
		return;
	}
	
	MetaPtrs& p = MetaPtrs::Single();
	int item_i = items.Get("IDX");
	MarketplaceItem& mi = p.owner->marketplace.items[item_i];
	
	mi.added = form.added.GetData();
	mi.generic = form.generic.GetData();
	mi.brand = form.brand.GetData();
	mi.model = form.model.GetData();
	mi.price = form.price.GetData();
	mi.cx = form.width.GetData();
	mi.cy = form.height.GetData();
	mi.cz = form.depth.GetData();
	mi.weight = form.weight.GetData();
	mi.faults = form.faults.GetData();
	mi.works = form.works.GetData();
	mi.broken = form.broken.GetData();
	mi.good = form.good.GetData();
	
	items.Set(0, mi.generic);
	items.Set(1, mi.brand);
	items.Set(2, mi.model);
	items.Set(3, mi.price);
}

void MarketplaceCtrl::PasteImagePath() {
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

void MarketplaceCtrl::SetCurrentImage(Image img) {
	this->img.SetImage(img);
	
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner || !items.IsCursor() || !images.IsCursor())
		return;
	int item_i = items.Get("IDX");
	int img_i = images.GetCursor();
	MarketplaceItem& mi = mp.owner->marketplace.items[item_i];
	
	hash_t h = img.GetHashValue();
	String cache_path = CacheImageFile(h);
	RealizeDirectory(GetFileDirectory(cache_path));
	
	if (!FileExists(cache_path)) {
		Image small_img = RescaleToFit(img, 1024);
		JPGEncoder enc(98);
		enc.SaveFile(cache_path, small_img);
	}
	
	if (img_i >= mi.images.GetCount())
		mi.images.SetCount(img_i+1,0);
	mi.images[img_i] = h;
	
	PostCallback(THISBACK(DataImage));
}


END_TEXTLIB_NAMESPACE

