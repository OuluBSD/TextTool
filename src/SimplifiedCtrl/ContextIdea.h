#ifndef _SimplifiedCtrl_ContextIdea_h_
#define _SimplifiedCtrl_ContextIdea_h_


class ContextIdeaCtrl : public LineListCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl distribution, parts, percentage;
	ArrayCtrl notes, suggestion;
	DocEdit part_dialogue0, part_dialogue1;
	
	ContextIdeaArgs args;
	
public:
	typedef ContextIdeaCtrl CLASSNAME;
	ContextIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetWordGroupPercentages();
	void GetSuggestions();
	void UseSuggestions();
	void UpdateAll();
	void UpdateParts();
	void UpdateRoles();
	void OnWordGroupPercentages(String res);
	
};


#endif
