#ifndef _SimplifiedCtrl_AttributeIdea_h_
#define _SimplifiedCtrl_AttributeIdea_h_


class AttributeIdea : public SongToolCtrl {
	bool disabled = false;
	Splitter vsplit0, vsplit1, hsplit;
	ArrayCtrl colors, attrs, active, suggestions;
	
public:
	typedef AttributeIdea CLASSNAME;
	AttributeIdea();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
