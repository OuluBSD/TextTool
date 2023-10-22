#ifndef _SimplifiedCtrl_TextIdea_h_
#define _SimplifiedCtrl_TextIdea_h_


class TextIdeaCtrl : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl parts;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef TextIdeaCtrl CLASSNAME;
	TextIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
