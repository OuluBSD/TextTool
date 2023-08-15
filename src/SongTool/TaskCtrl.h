#ifndef _SongTool_TaskCtrl_h_
#define _SongTool_TaskCtrl_h_


class Tasks : public Ctrl {
	Splitter hsplit, vsplit;
	ArrayCtrl list;
	DocEdit input, output;
	ProgressIndicator prog;
	Label lbl;
	int data_cursor = -1;
	
public:
	typedef Tasks CLASSNAME;
	Tasks();
	
	void Data();
	void DataTask();
	void ValueChange();
	void ProcessItem();
	void RetryItem(bool skip_prompt);
	void OutputMenu(Bar& bar);
	
};


#endif
