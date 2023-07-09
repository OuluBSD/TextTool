#ifndef _SongTool_AI_TaskCtrl_h_
#define _SongTool_AI_TaskCtrl_h_


class AI_Tasks : public Ctrl {
	Splitter hsplit, vsplit;
	ArrayCtrl list;
	DocEdit input, output;
	
public:
	typedef AI_Tasks CLASSNAME;
	AI_Tasks();
	
	void Data();
	void DataTask();
	void ValueChange();
	
};


#endif
