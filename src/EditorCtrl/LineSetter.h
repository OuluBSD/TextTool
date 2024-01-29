#ifndef _EditorCtrl_LineSetter_h_
#define _EditorCtrl_LineSetter_h_


class LineSetter : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl lines, picked, subpicked;
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
