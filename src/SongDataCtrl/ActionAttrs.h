#ifndef _SongDataCtrl_ActionAttrs_h_
#define _SongDataCtrl_ActionAttrs_h_


class ActionAttrsPage : public SongToolCtrl {
	
public:
	typedef ActionAttrsPage CLASSNAME;
	ActionAttrsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataMain();
	void UpdateColors();
	void UpdateAttributes();
	
};


#endif
