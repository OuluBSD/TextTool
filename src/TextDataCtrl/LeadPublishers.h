#ifndef _TextDataCtrl_LeadPublishers_h_
#define _TextDataCtrl_LeadPublishers_h_


BEGIN_TEXTLIB_NAMESPACE


class LeadPublishers : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl list, artists;
	WithPublisherInfo<Ctrl> info;
	
public:
	typedef LeadPublishers CLASSNAME;
	LeadPublishers();
	
	void Data() override;
	void DataItem();
	void ToolMenu(Bar& bar) override;
	void ListMenu(Bar& bar);
	void Do(int fn);
	void AddPublisher();
	void RemovePublisher();
	void ValueChange();
	void PasteArtists();
	void ExportJson();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
