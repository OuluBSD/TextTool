#ifndef _TextDataCtrl_ActionParallels_h_
#define _TextDataCtrl_ActionParallels_h_


BEGIN_TEXTLIB_NAMESPACE


class ActionParallelsPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl actions, action_args, parallels;
	Mutex lock;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
public:
	typedef ActionParallelsPage CLASSNAME;
	ActionParallelsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void DataAction();
	void DataActionHeader();
	void UpdateParallels();
	
};


END_TEXTLIB_NAMESPACE


#endif
