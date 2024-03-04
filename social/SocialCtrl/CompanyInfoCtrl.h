#ifndef _SocialTool_CompanyInfoCtrl_h_
#define _SocialTool_CompanyInfoCtrl_h_

class SocialEditor;

class CompanyInfoCtrl : public WithCompanyInfo<ToolAppCtrl> {
	
	
public:
	typedef CompanyInfoCtrl CLASSNAME;
	CompanyInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SocialEditor* editor = 0;
	
};


#endif
