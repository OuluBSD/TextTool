#ifndef _TextCtrl_ImageBiographySummaryCtrl_h_
#define _TextCtrl_ImageBiographySummaryCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageBiographySummaryCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl categories, blocks;
	WithBiography<Ctrl> block;
	SocialTimelineCtrl timeline;
	
public:
	typedef ImageBiographySummaryCtrl CLASSNAME;
	ImageBiographySummaryCtrl();
	
	void Data() override;
	void DataCategory();
	void DataYear();
	void OnValueChange();
	void Translate();
	void MakeKeywords();
	void Do(int fn);
	void OnTranslate(String s);
	void OnKeywords(String s);
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
