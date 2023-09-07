#ifndef _SongTool_ImportCtrl_h_
#define _SongTool_ImportCtrl_h_


class ImportCtrl : public WithImport<SongToolCtrl> {
	DocEdit input[HUMAN_INPUT_MODE_COUNT];
	Splitter hsplit;
	ArrayCtrl messages;
	
	SnapArg a[HUMAN_INPUT_MODE_COUNT];
	
	void AddMessage(int line, int severity, String msg);
public:
	typedef ImportCtrl CLASSNAME;
	ImportCtrl();
	
	void Data();
	void OnValueChange();
	//void OutputBar(Bar& bar);
	//void RemoveLine();
	void MakeTasks();
	
	Callback WhenStructureChange;
	
};


#endif
