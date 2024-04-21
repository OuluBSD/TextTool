#ifndef _TextTool_EntityInfoCtrl_h_
#define _TextTool_EntityInfoCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;


class EntityInfoCtrl : public WithEntityInfo<ToolAppCtrl> {
	
	
public:
	typedef EntityInfoCtrl CLASSNAME;
	EntityInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
