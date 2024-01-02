#ifndef _SongDataCtrl_ActionTransitions_h_
#define _SongDataCtrl_ActionTransitions_h_


#if 0

class ActionTransitionsPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, actions, action_args, transitions;
	Mutex lock;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
public:
	typedef ActionTransitionsPage CLASSNAME;
	ActionTransitionsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void DataDataset();
	void DataAction();
	void DataActionHeader();
	void UpdateTransitions();
	
};


#endif
#endif
