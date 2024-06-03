#ifndef _TextCtrl_SocialNeeds_h_
#define _TextCtrl_SocialNeeds_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialNeedsCtrl : public ToolAppCtrl {
	Splitter hsplit, rolesplit, platsplit, eventsplit;
	ArrayCtrl roles, needs, causes, messages;
	ArrayCtrl platforms, actions, action_causes;
	ArrayCtrl events, entries;
	DocEdit event, entry;
	
public:
	typedef SocialNeedsCtrl CLASSNAME;
	SocialNeedsCtrl();
	
	void Data() override;
	void DataRole();
	void DataNeed();
	void DataAction();
	void DataEvent();
	void DataEntry();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
