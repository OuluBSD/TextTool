#ifndef _SimplifiedCtrl_WordSaladIdea_h_
#define _SimplifiedCtrl_WordSaladIdea_h_


class WordSaladIdeaCtrl : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl parts;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef WordSaladIdeaCtrl CLASSNAME;
	WordSaladIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
