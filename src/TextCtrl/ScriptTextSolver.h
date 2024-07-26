#ifndef _TextCtrl_ScriptTextSolver_h_
#define _TextCtrl_ScriptTextSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptTextSolverCtrl : public ToolAppCtrl {
	
	
public:
	typedef ScriptTextSolverCtrl CLASSNAME;
	ScriptTextSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
