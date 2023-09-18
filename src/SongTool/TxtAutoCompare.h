#ifndef _SongTool_TxtAutoCompare_h_
#define _SongTool_TxtAutoCompare_h_


class TxtAutoCompare : public SongToolCtrl {
	Splitter hsplit, vsplit;
	CodeEditor main;
	ArrayCtrl lines, line_results;
	DocEdit best;
	
	bool has_init = false;
	String unpacked_struct_key;
	String auto_poetic_styles_english_key;
	String main_key;
	String other_key;
	
	// Menu
	DropList rhymetype;
	
public:
	typedef TxtAutoCompare CLASSNAME;
	TxtAutoCompare();
	
	void Init();
	void Data() override;
	void DoMainAction(int i=0) override;
	virtual void ToolMenu(Bar& bar);
	void EvaluatePoeticStyles();
	void PostOnPoeticRecv() {PostCallback(THISBACK(OnPoeticRecv));}
	void OnPoeticRecv();
	
	
};


#endif
