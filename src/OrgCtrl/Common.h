#ifndef _OrgCtrl_Common_h_
#define _OrgCtrl_Common_h_


BEGIN_TEXTLIB_NAMESPACE


struct NodeViewBase {
	virtual ~NodeViewBase() {}
	
	virtual void Data() = 0;
	
	void SetNode(Node* n) {node = n;}
	
	Node* node = 0;
	
};

struct NodeCtrlBase : public Ctrl {
	virtual ~NodeCtrlBase() {}
	
	virtual void Data() = 0;
	
	
	NodeViewBase* view = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
