#ifndef _SongDataCtrl_ActionParallels_h_
#define _SongDataCtrl_ActionParallels_h_


BEGIN_SONGLIB_NAMESPACE


class ActionParallelsPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, actions, action_args, parallels;
	Mutex lock;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
public:
	typedef ActionParallelsPage CLASSNAME;
	ActionParallelsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void DataDataset();
	void DataAction();
	void DataActionHeader();
	void UpdateParallels();
	
};


END_SONGLIB_NAMESPACE


#endif
