#ifndef _TextCtrl_Audience_h_
#define _TextCtrl_Audience_h_


BEGIN_TEXTLIB_NAMESPACE


class AudienceCtrl : public ToolAppCtrl {
	Splitter menusplit, hsplit, vsplit, bsplit;
	ArrayCtrl roles, profiles, responses, entries;
	WithAudience<Ctrl> entry;
	ImageViewerCtrl img;
	
public:
	typedef AudienceCtrl CLASSNAME;
	AudienceCtrl();
	
	void Data() override;
	void DataRole();
	void DataProfile();
	void DataResponse();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
