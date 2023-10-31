#ifndef _SimplifiedCtrl_ContextIdea_h_
#define _SimplifiedCtrl_ContextIdea_h_


class ContextIdeaCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl in_use, potential_use;
	ArrayCtrl notes, suggestion;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef ContextIdeaCtrl CLASSNAME;
	ContextIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetRoleSuggestions();
	void ActivateSuggestion();
	void GetSuggestions();
	void UseSuggestions();
	
};


#endif
