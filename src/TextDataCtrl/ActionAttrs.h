#ifndef _TextDataCtrl_ActionAttrs_h_
#define _TextDataCtrl_ActionAttrs_h_


BEGIN_TEXTLIB_NAMESPACE


class ActionAttrsPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, colors, actions;
	
public:
	typedef ActionAttrsPage CLASSNAME;
	ActionAttrsPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DataAttribute();
	void DataColor();
	void UpdateFromCache();
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
