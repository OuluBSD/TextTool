#ifndef _SimplifiedCtrl_Vocabulary_h_
#define _SimplifiedCtrl_Vocabulary_h_


class VocabularyCtrl : public SongToolCtrl {
	Splitter vsplit;
	ArrayCtrl list, examples;
	DropList dl;
	bool is_wordmode = false;
	
public:
	typedef VocabularyCtrl CLASSNAME;
	VocabularyCtrl(bool is_wordmode);
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetWords();
	void GetExamplePhrases();
	void TranslateExamplePhrases();
	void TranslateArtistPhrases();
	void CopyArtistPhrase();
	void CopyExamplePhrase();
	void ClearArtistPhrases();
	void OnPhraseList();
	void OnTranslateResult(String result, Artist* a, int list_i);
	void OnVocabularyVariations(String result, Artist* a, int list_i);
	int GetCount() const {return is_wordmode ? SIGNIFICANT_WORD_COUNT : SIGNIFICANT_PHRASE_COUNT;}
	const char* Get(int i) const {return is_wordmode ? SignificantWords[i] : SignificantPhrases[i];}
	
	void SetWordMode() {is_wordmode = true;}
	
};


#endif
