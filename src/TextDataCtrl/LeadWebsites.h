#ifndef _TextDataCtrl_LeadWebsites_h_
#define _TextDataCtrl_LeadWebsites_h_


BEGIN_TEXTLIB_NAMESPACE



class LeadWebsites : public ToolAppCtrl {
	Splitter vsplit, hsplit, mainsplit, bsplit, bvsplit, bssplit;
	ArrayCtrl websites, list, payouts, prices, attrs;
	ArrayCtrl bools, strings, list_names, list_values;
	ArrayCtrl song_typecasts, lyrics_ideas, music_styles;
	
public:
	typedef LeadWebsites CLASSNAME;
	LeadWebsites();
	
	void Data() override;
	void DataWebsite();
	void DataPayout();
	void DataPrice();
	void DataOpportunity();
	void DataAnalyzedList();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
