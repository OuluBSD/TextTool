#ifndef _TextCtrl_Biography_h_
#define _TextCtrl_Biography_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl categories, years;
	WithBiography<Ctrl> year;
	SocialTimelineCtrl timeline;
	
public:
	typedef BiographyCtrl CLASSNAME;
	BiographyCtrl();
	
	void Data() override;
	void DataCategory();
	void DataYear();
	void OnValueChange();
	void Translate();
	void MakeKeywords();
	void OnTranslate(String s);
	void OnKeywords(String s);
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	void SnapshotMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
