#ifndef _SimplifiedCtrl_PartIdea_h_
#define _SimplifiedCtrl_PartIdea_h_


class PartIdea : public SongToolCtrl {
	Splitter vsplit, hsplit0, hsplit1;
	ArrayCtrl active, parts;
	ArrayCtrl contents, imageries, symbolisms;
	
public:
	typedef PartIdea CLASSNAME;
	PartIdea();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	
};


#endif
