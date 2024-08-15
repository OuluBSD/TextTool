#ifndef _TextCtrl_Marketplace_h_
#define _TextCtrl_Marketplace_h_


BEGIN_TEXTLIB_NAMESPACE


class MarketplaceCtrl : public ToolAppCtrl {
	Splitter hsplit, imgsplit;
	ArrayCtrl items, images;
	TabCtrl tabs;
	WithMarketplace<Ctrl> form;
	WithMarketplaceViewer<Ctrl> viewer;
	ImageViewerCtrl img;
	int filter_priority = -1;
	
public:
	typedef MarketplaceCtrl CLASSNAME;
	MarketplaceCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void DataItem();
	void DataImage();
	void DataCategory();
	void DataSubCategory();
	void OnValueChange();
	void ClearForm();
	void PasteImagePath();
	void MakeTempImages();
	void OnCategory();
	void OnSubCategory();
	void SetCategoryShorcut(int i);
	void SetCurrentImage(Image img);
	void LoadImagePath(String path);
	void ShowItems(int priority) {filter_priority = priority; PostCallback(THISBACK(Data));}
	
};


END_TEXTLIB_NAMESPACE


#endif
