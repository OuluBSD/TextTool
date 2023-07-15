#ifndef _SongTool_ReverseCtrl_h_
#define _SongTool_ReverseCtrl_h_


class TaskPlotter : public Ctrl {
	ReverseTask* rtask = 0;
	Color clr, line_clr;
	
public:
	TaskPlotter();
	typedef TaskPlotter CLASSNAME;
	
	void Paint(Draw& draw) override;
	
	void Set(ReverseTask& t) {rtask = &t; Refresh();}
	
};

class TaskCtrl : public Ctrl {
	
public:
	TaskPlotter plotter;
	ProgressIndicator total_prog, snap_prog;
	
public:
	TaskCtrl();
	typedef TaskCtrl CLASSNAME;
	
	
};


class ReverseCtrl : public Ctrl {
	//Array<ReverseTask> tasks;
	
	Splitter mainsplit, hsplit, vsplit;
	ArrayCtrl snaplist, tasklist, resultlist;
	TaskCtrl task;
	CodeEditor lyrics, translated;
	
	static const int gen_multiplier = 20;
	static const int gens = 200;
	inline static const int max_values = 12;
	
public:
	typedef ReverseCtrl CLASSNAME;
	ReverseCtrl();
	
	void Data();
	void DataWorker();
	
};


#endif
