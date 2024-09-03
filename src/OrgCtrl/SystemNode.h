#ifndef _OrgCtrl_SystemNode_h_
#define _OrgCtrl_SystemNode_h_

BEGIN_TEXTLIB_NAMESPACE


class SystemView : public NodeViewBase {
	
public:
	typedef SystemView CLASSNAME;
	SystemView();
	
	void Data() override;
	
};

class SystemCtrl : public NodeCtrlBase {
	WithSystem<Ctrl> form;
	
public:
	typedef SystemCtrl CLASSNAME;
	SystemCtrl();
	
	void Data() override;
	void OnValueChange();
	
};



END_TEXTLIB_NAMESPACE

#endif
