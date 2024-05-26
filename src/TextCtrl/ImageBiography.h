#ifndef _TextCtrl_ImageBiography_h_
#define _TextCtrl_ImageBiography_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageViewerCtrl : public Ctrl {
	Image img;
	
public:
	typedef ImageViewerCtrl CLASSNAME;
	ImageViewerCtrl();
	
	void Paint(Draw& d);
	void SetImage(const Image& i);
	void Clear();
	
};

class ImageBiographyCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit, bsplit;
	ArrayCtrl categories, years, entries;
	WithImageBiography<Ctrl> year;
	SocialTimelineCtrl timeline;
	ImageViewerCtrl img;
	
public:
	typedef ImageBiographyCtrl CLASSNAME;
	ImageBiographyCtrl();
	
	void Data() override;
	void DataCategory();
	void DataYear();
	void DataEntry();
	void OnCategoryCursor();
	void OnValueChange();
	void Translate();
	void MakeKeywords(int fn);
	void OnTranslate(String s);
	void OnKeywords(int fn, String s);
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	void AddEntry();
	void RemoveEntry();
	void PasteImagePath();
	void AnalyseImage();
	void SetCurrentImage(Image img);
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
