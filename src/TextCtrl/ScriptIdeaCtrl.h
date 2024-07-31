#ifndef _TextCtrl_ScriptIdeaCtrl_h_
#define _TextCtrl_ScriptIdeaCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class ScriptIdeaCtrl : public ToolAppCtrl {
	
public:
	typedef ScriptIdeaCtrl CLASSNAME;
	ScriptIdeaCtrl();
	
	void Data() override;
	void Do(int fn);
	void ToolMenu(Bar& bar) override;
	
};


END_TEXTLIB_NAMESPACE


#endif
