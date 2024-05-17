#ifndef _TextCtrl_ImageBiography_h_
#define _TextCtrl_ImageBiography_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageAnalyserCtrl : public Ctrl {
	Image img;
	
public:
	typedef ImageAnalyserCtrl CLASSNAME;
	ImageAnalyserCtrl();
	
	void Paint(Draw& d);
	void SetImage(const Image& i);
	
	
};

class ImageBiographyCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit, bsplit;
	ArrayCtrl categories, years, entries;
	WithImageBiography<Ctrl> year;
	SocialTimelineCtrl timeline;
	ImageAnalyserCtrl img;
	
public:
	typedef ImageBiographyCtrl CLASSNAME;
	ImageBiographyCtrl();
	
	void Data() override;
	void DataCategory();
	void DataYear();
	void DataEntry();
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
	void SetCurrentImage(Image img);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
