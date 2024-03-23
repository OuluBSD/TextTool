#ifndef _TextCtrl_ScriptInfoCtrl_h_
#define _TextCtrl_ScriptInfoCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class ScriptInfoCtrl : public WithScriptInfo<ToolAppCtrl> {
	
	
public:
	typedef ScriptInfoCtrl CLASSNAME;
	ScriptInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
