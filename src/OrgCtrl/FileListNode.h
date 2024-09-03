#ifndef _OrgCtrl_FileListNode_h_
#define _OrgCtrl_FileListNode_h_

BEGIN_TEXTLIB_NAMESPACE


class FileListView : public NodeViewBase {
	
public:
	typedef FileListView CLASSNAME;
	FileListView();
	
	void Data() override;
	
};

class FileListCtrl : public NodeCtrlBase {
	WithFeatures<Ctrl> form;
	
public:
	typedef FileListCtrl CLASSNAME;
	FileListCtrl();
	
	void Data() override;
	void OnValueChange();
	
};



END_TEXTLIB_NAMESPACE

#endif
