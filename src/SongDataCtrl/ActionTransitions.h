#ifndef _SongDataCtrl_ActionTransitions_h_
#define _SongDataCtrl_ActionTransitions_h_


class ActionTransitionsPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, actions, action_args, transitions;
	Mutex lock;
	
public:
	typedef ActionTransitionsPage CLASSNAME;
	ActionTransitionsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void DataDataset();
	void DataAction();
	void DataActionArg();
	void UpdateFromCache();
	void UpdateTransitions();
	
};


#endif
