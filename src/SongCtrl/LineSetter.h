#ifndef _SongCtrl_LineSetter_h_
#define _SongCtrl_LineSetter_h_

#if 0

class LineSetter : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl lines, picked, subpicked, lineconf;
	int ds_i = 0;
	
public:
	typedef LineSetter CLASSNAME;
	LineSetter();
	
	void Data() override;
	void PostData() {PostCallback(THISBACK(Data));}
	void DataPicked();
	void DataSubPicked();
	void DataSet();
	void ToolMenu(Bar& bar) override;
	void SetLine();
	void ClearLine();
	void CopyText();
	void MoveLine(int n);
	void DoNana(int fn);
	
};


#endif
#endif
