#ifndef _TextCtrl_ScriptTextSolver_h_
#define _TextCtrl_ScriptTextSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class StructuredScriptEditor : public Ctrl{
	
	
public:
	typedef StructuredScriptEditor CLASSNAME;
	StructuredScriptEditor();
	
	void Paint(Draw& d) override;
	
};

class ScriptTextSolverCtrl : public ToolAppCtrl {
	Splitter hsplit, rsplit;
	StructuredScriptEditor editor;
	
public:
	typedef ScriptTextSolverCtrl CLASSNAME;
	ScriptTextSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
