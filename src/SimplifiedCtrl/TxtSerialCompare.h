#ifndef _SimplifiedCtrl_TxtSerialCompare_h_
#define _SimplifiedCtrl_TxtSerialCompare_h_



class TxtSerialCompare : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl rhymes, suggestions, params, attrs;
	bool disabled = false;
	
	String syllables_key, forbidden_words_key;
	String frozen_begin_key, frozen_end_key;
	String specific_imagery_key;
	String symbolism_key;
	
	Vector<int> tmp_sug_ids;
	
public:
	typedef TxtSerialCompare CLASSNAME;
	TxtSerialCompare();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void EnableAll();
	void DisableAll();
	void DataPart(bool focus_last, bool skip_suggs);
	void DataRhyme();
	void GetFirstLine();
	void RedoLine();
	void GetNextLine();
	void ContinueLine(bool add_rhyme);
	void OnParamChangeString(EditString* e, int key);
	void OnParamChangeInt(EditIntSpin* e, int key);
	StaticPart* GetActiveSongPart();
	StaticRhyme* GetActiveRhyme();
	void GetAttrs(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	void GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	void GetActiveAttrs(VectorMap<String,String>& v);
	void OnFirstLine(String result, StaticRhyme* r);
	void OnContinueLine(String result, StaticPart* part, StaticRhyme* r);
	void CopyIdeaVariables();
	void ClearAttributes();
	void CopyAttributes(int src);
	
	// DUPLICATE FROM TxtAutoCompare
	void SetSuggestionScore(EditIntNotNullSpin* e, StaticSuggestion* sug);
	void EvaluateSuggestionScores();
	void EvaluateExtraSuggestionScores();
	void OnSuggestionScore(String res, StaticRhyme* r);
	void OnSuggestionOrder(String res, StaticRhyme* r);
	
};



#endif
