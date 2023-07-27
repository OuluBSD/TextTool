#ifndef _SongTool_ImportCtrl_h_
#define _SongTool_ImportCtrl_h_


class ImportCtrl : public WithImport<Ctrl> {
	DocEdit input[2];
	Splitter hsplit;
	ArrayCtrl messages;
	
	
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
