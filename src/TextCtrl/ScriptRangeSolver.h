#ifndef _TextCtrl_ScriptRangeSolver_h_
#define _TextCtrl_ScriptRangeSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptRangeSolverCtrl : public ToolAppCtrl {
	
public:
	typedef ScriptRangeSolverCtrl CLASSNAME;
	ScriptRangeSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
