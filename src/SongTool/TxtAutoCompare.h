#ifndef _SongTool_TxtAutoCompare_h_
#define _SongTool_TxtAutoCompare_h_


class TxtAutoCompare : public EditorCtrl {
	Splitter hsplit, vsplit;
	CodeEditor main;
	ArrayCtrl lines, line_results;
	DocEdit best;
	
	
	// Menu
	DropList rhymetype;
	
public:
	typedef TxtAutoCompare CLASSNAME;
	TxtAutoCompare();
	
	void Data() override;
	void DoMainAction(int i=0) override;
	virtual void ToolMenu(Bar& bar);
	
	
};


#endif
