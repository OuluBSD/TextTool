#ifndef _SimplifiedCtrl_Vocabulary_h_
#define _SimplifiedCtrl_Vocabulary_h_


class VocabularyCtrl : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl parts;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef VocabularyCtrl CLASSNAME;
	VocabularyCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
