#ifndef _SongTool_ImportCtrl_h_
#define _SongTool_ImportCtrl_h_


class ImportCtrl : public WithImport<Ctrl> {
	DocEdit input;
	ArrayCtrl output;
	
	
public:
	typedef ImportCtrl CLASSNAME;
	ImportCtrl();
	
	void Data();
	void OnValueChange();
	void OutputBar(Bar& bar);
	void RemoveLine();
	void MakeTasks();
	void ParseOriginalLyrics();
	
	
};


#endif
