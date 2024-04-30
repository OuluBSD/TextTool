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
	void Clear();
	void OnValueChange();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
