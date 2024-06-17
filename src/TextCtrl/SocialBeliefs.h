#ifndef _TextCtrl_SocialBeliefs_h_
#define _TextCtrl_SocialBeliefs_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialBeliefsCtrl : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl beliefs;
	ArrayCtrl attrs, user;
	WithSocialBelief<Ctrl> info;
	
public:
	typedef SocialBeliefsCtrl CLASSNAME;
	SocialBeliefsCtrl();
	
	void Data() override;
	void DataBelief();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void AddBelief();
	void RemoveBelief();
	void OnValueChange();
};


END_TEXTLIB_NAMESPACE


#endif
