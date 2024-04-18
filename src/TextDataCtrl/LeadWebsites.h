#ifndef _TextDataCtrl_LeadWebsites_h_
#define _TextDataCtrl_LeadWebsites_h_


BEGIN_TEXTLIB_NAMESPACE



class LeadWebsites : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl websites, list, payouts, prices;
	
public:
	typedef LeadWebsites CLASSNAME;
	LeadWebsites();
	
	void Data() override;
	void DataWebsite();
	void DataPayout();
	void DataPrice();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
