#ifndef _TextCtrl_CompInfoCtrl_h_
#define _TextCtrl_CompInfoCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class CompInfoCtrl : public WithComponentInfo<ToolAppCtrl> {
	int focus_tc = -1, focus_arch = -1, focus_lyr = -1;
	
public:
	typedef CompInfoCtrl CLASSNAME;
	CompInfoCtrl();
	
	void Data();
	void Clear();
	void DataTypeclass();
	void DataContent();
	void DataScript();
	void OnValueChange();
	void SetScript();
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
