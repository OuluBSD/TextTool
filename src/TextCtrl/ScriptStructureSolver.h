#ifndef _TextCtrl_ScriptStructureSolver_h_
#define _TextCtrl_ScriptStructureSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptStructureSolverCtrl : public ToolAppCtrl {
	
	// Left side: partial duplicate of ConceptualFrameworkCtrl
	ConceptualFrameworkNavigator concepts;
	
	// Right side
	Splitter hsplit;
	Splitter rsplit, rtsplit, rbsplit;
	ArrayCtrl genres, structs;
	DocEdit src_struct, morphed_struct;
	
public:
	typedef ScriptStructureSolverCtrl CLASSNAME;
	ScriptStructureSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void DataGenre();
	void DataStructure();
	void SetStructure();
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
