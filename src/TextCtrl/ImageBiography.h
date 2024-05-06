#ifndef _TextCtrl_ImageBiography_h_
#define _TextCtrl_ImageBiography_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageAnalyserCtrl : public Ctrl {
	
	
public:
	typedef ImageAnalyserCtrl CLASSNAME;
	ImageAnalyserCtrl();
	
	void Paint(Draw& d);
	
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
	
	
};


END_TEXTLIB_NAMESPACE


#endif
