#ifndef _ToolCtrl_OperationOverview_h_
#define _ToolCtrl_OperationOverview_h_


BEGIN_TEXTLIB_NAMESPACE


class OperationOverview : public WithOperationOverview<Ctrl> {
	
	
public:
	typedef OperationOverview CLASSNAME;
	OperationOverview();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
	
};


END_TEXTLIB_NAMESPACE


#endif
