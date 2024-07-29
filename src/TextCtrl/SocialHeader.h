#ifndef _TextCtrl_SocialHeader_h_
#define _TextCtrl_SocialHeader_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialHeaderCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl platforms, entries;
	//WithSocialHeader<Ctrl> entry;
	Splitter entry_split;
	ArrayCtrl attr_keys;
	DocEdit attr_value;
	
public:
	typedef SocialHeaderCtrl CLASSNAME;
	SocialHeaderCtrl();
	
	void Data() override;
	void DataPlatform();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	void OnValueChange();
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
