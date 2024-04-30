#ifndef _TextCtrl_SocialContent_h_
#define _TextCtrl_SocialContent_h_


BEGIN_TEXTLIB_NAMESPACE


class LeadsCtrl;

class SocialContent : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl platforms, entries;
	WithSocialEntry<Ctrl> entry;
	
	
public:
	typedef SocialContent CLASSNAME;
	SocialContent();
	
	void Data();
	void DataPlatform();
	void DataEntry();
	void Clear();
	void ClearEntry();
	void OnValueChange();
	void AddEntry();
	void RemoveEntry();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
