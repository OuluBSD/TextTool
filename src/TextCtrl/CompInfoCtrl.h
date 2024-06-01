#ifndef _TextCtrl_CompInfoCtrl_h_
#define _TextCtrl_CompInfoCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class CompInfoCtrl : public WithComponentInfo<ToolAppCtrl> {
	int focus_lyr = -1;
	
public:
	typedef CompInfoCtrl CLASSNAME;
	CompInfoCtrl();
	
	void Data() override;
	void Clear();
	void DataScript();
	void OnValueChange();
	void SetScript();
	void ToolMenu(Bar& bar) override;
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
