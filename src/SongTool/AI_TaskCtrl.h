#ifndef _SongTool_AI_TaskCtrl_h_
#define _SongTool_AI_TaskCtrl_h_


class AI_Tasks : public Ctrl {
	Splitter hsplit, vsplit;
	ArrayCtrl list;
	DocEdit input, output;
	ProgressIndicator prog;
	Label lbl;
	int data_cursor = -1;
	
public:
	typedef AI_Tasks CLASSNAME;
	AI_Tasks();
	
	void Data();
	void DataTask();
	void ValueChange();
	void ProcessItem();
	void RetryItem();
	void OutputMenu(Bar& bar);
	
};


#endif
