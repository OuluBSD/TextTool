#ifndef _TextCtrl_ScriptEdit_h_
#define _TextCtrl_ScriptEdit_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptEditCtrl : public ToolAppCtrl {
	
	
public:
	typedef ScriptEditCtrl CLASSNAME;
	ScriptEditCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
