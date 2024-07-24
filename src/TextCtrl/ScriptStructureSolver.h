#ifndef _TextCtrl_ScriptStructureSolver_h_
#define _TextCtrl_ScriptStructureSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptStructureSolverCtrl : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl genres, structs;
	DocEdit src_struct;
	
public:
	typedef ScriptStructureSolverCtrl CLASSNAME;
	ScriptStructureSolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void DataGenre();
	void DataStructure();
	void Do(int fn) {DoT<ScriptStructureProcess>(fn);}
	
};


END_TEXTLIB_NAMESPACE


#endif
