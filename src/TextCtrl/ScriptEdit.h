#ifndef _TextCtrl_ScriptEdit_h_
#define _TextCtrl_ScriptEdit_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptEditCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl lines, info, suggs;
	
	TextSuggestions* tmp = 0;
	int tmp_i = -1;
	
	TextSolver* txt_ts = 0;
	TextSolver* gen_ts = 0;
	TextSolver* ref_ts = 0;
public:
	typedef ScriptEditCtrl CLASSNAME;
	ScriptEditCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void Do(int fn);
	void DataLine();
	void GetSuggestions();
	void LineMenu(Bar& bar);
	void SuggMenu(Bar& bar);
	
};


END_TEXTLIB_NAMESPACE


#endif
