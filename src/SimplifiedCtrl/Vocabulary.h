#ifndef _SimplifiedCtrl_Vocabulary_h_
#define _SimplifiedCtrl_Vocabulary_h_


class VocabularyCtrl : public SongToolCtrl {
	ArrayCtrl list;
	
public:
	typedef VocabularyCtrl CLASSNAME;
	VocabularyCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetWords();
	void GetExamplePhrases();
	void TranslateExamplePhrases();
	void TranslateArtistPhrases();
	
};


#endif
