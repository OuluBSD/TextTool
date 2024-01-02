#ifndef _SongDataCtrl_ActionAttrs_h_
#define _SongDataCtrl_ActionAttrs_h_


#if 0

class ActionAttrsPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, actions;
	
public:
	typedef ActionAttrsPage CLASSNAME;
	ActionAttrsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void UpdateFromCache();
	void UpdateColors();
	void UpdateAttributes();
	
};


#endif
#endif
