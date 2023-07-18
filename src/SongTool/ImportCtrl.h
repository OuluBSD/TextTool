#ifndef _SongTool_ImportCtrl_h_
#define _SongTool_ImportCtrl_h_


class ImportCtrl : public WithImport<Ctrl> {
	DocEdit input[2];
	Splitter hsplit;
	ArrayCtrl messages;
	
	
public:
	typedef ImportCtrl CLASSNAME;
	ImportCtrl();
	
	void Data();
	void OnValueChange();
	//void OutputBar(Bar& bar);
	//void RemoveLine();
	void MakeTasks();
	void ParseOriginalLyrics();
	
	Callback WhenStructureChange;
	
};


#endif
