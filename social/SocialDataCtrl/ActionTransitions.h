#ifndef _SocialDataCtrl_ActionTransitions_h_
#define _SocialDataCtrl_ActionTransitions_h_


BEGIN_SOCIALLIB_NAMESPACE


class ActionTransitionsPage : public ToolAppCtrl {
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


END_SOCIALLIB_NAMESPACE


#endif
