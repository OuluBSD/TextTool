#ifndef _SocialCtrl_PressRelease_h_
#define _SocialCtrl_PressRelease_h_


class PressReleaseCtrl : public ToolAppCtrl {
	
	
public:
	typedef PressReleaseCtrl CLASSNAME;
	PressReleaseCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
