#ifndef _OrgCtrl_Organization_h_
#define _OrgCtrl_Organization_h_


BEGIN_TEXTLIB_NAMESPACE


class OrganizationCtrl : public Ctrl {
	
	
public:
	typedef OrganizationCtrl CLASSNAME;
	OrganizationCtrl();
	
	void Init();
	void Data();
	void ToolMenu(Bar& bar);
	
	
	
};



END_TEXTLIB_NAMESPACE


#endif
