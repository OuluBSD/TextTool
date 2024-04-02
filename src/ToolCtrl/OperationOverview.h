#ifndef _ToolCtrl_OperationOverview_h_
#define _ToolCtrl_OperationOverview_h_


class OperationOverview : public WithOperationOverview<Ctrl> {
	
	
public:
	typedef OperationOverview CLASSNAME;
	OperationOverview();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
	
};


#endif
