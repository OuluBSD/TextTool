#ifndef _SimplifiedCtrl_AttributeDistribution_h_
#define _SimplifiedCtrl_AttributeDistribution_h_


class AttributeDistribution : public SongToolCtrl {
	bool disabled = false;
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, active, suggestions;
	
public:
	typedef AttributeDistribution CLASSNAME;
	AttributeDistribution();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
