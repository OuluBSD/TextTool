#ifndef _SongTool_CompanyInfoCtrl_h_
#define _SongTool_CompanyInfoCtrl_h_

class Editor;

class CompanyInfoCtrl : public WithCompanyInfo<SongToolCtrl> {
	
	
public:
	typedef CompanyInfoCtrl CLASSNAME;
	CompanyInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
