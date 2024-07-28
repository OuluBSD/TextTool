#ifndef _TextCtrl_Marketplace_h_
#define _TextCtrl_Marketplace_h_


BEGIN_TEXTLIB_NAMESPACE


class MarketplaceCtrl : public ToolAppCtrl {
	Splitter hsplit, imgsplit;
	ArrayCtrl items, images;
	WithMarketplace<Ctrl> form;
	ImageViewerCtrl img;
	
public:
	typedef MarketplaceCtrl CLASSNAME;
	MarketplaceCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void DataItem();
	void DataImage();
	void OnValueChange();
	void ClearForm();
	void PasteImagePath();
	void SetCurrentImage(Image img);
	
};


END_TEXTLIB_NAMESPACE


#endif
