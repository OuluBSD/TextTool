#ifndef _SimplifiedCtrl_Vocabulary_h_
#define _SimplifiedCtrl_Vocabulary_h_


class VocabularyCtrl : public SongToolCtrl {
	Splitter vsplit;
	ArrayCtrl list, examples;
	DropList dl;
	
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
	void CopyArtistPhrase();
	void CopyExamplePhrase();
	void ClearArtistPhrases();
	void OnPhraseList();
	void OnTranslateResult(String result, Artist* a, int list_i);
	void OnVocabularyVariations(String result, Artist* a, int list_i);
	int GetCount() const {return SIGNIFICANT_PHRASE_COUNT;}
	const char* Get(int type, int i) const {Chk(type,i); return SignificantPhrases[type][i].txt;}
	Color GetColor(int type, int i) const {Chk(type,i); return SignificantPhrases[type][i].GetColor();}
	void Chk(int type, int i) const {ASSERT(type >= 0 && type < VOCABULARYTYPE_COUNT); ASSERT(i >= 0 && i < SIGNIFICANT_PHRASE_COUNT);}
	
};


#endif
