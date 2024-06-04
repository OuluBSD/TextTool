#ifndef _TextCtrl_ScriptPostFix_h_
#define _TextCtrl_ScriptPostFix_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptPostFixCtrl : public ToolAppCtrl {
	Splitter hsplit, menusplit, vsplit, weaksplit, imprsplit, varsplit;
	ArrayCtrl generations, src_lines;
	ArrayCtrl weak_types, weaks, impr_types, imprs, vars;
	DocEdit var_content, weak_text;
	
	
public:
	typedef ScriptPostFixCtrl CLASSNAME;
	ScriptPostFixCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	void DataGeneration();
	void DataWeak();
	void DataWeakItem();
	void DataImprovement();
	void DataVariation();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
