#ifndef _SongTool_TxtAutoCompare_h_
#define _SongTool_TxtAutoCompare_h_


class TxtAutoCompare : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl parts, rhymes, suggestions, attrs, params;
	DocEdit best;
	
	DocEdit edit_source, edit_ai_source;
	Label lbl_def_syllables;
	EditIntSpin def_syllables;
	Label lbl_def_attr_count;
	EditIntSpin def_attr_count;
	
	bool is_disabled = false;
	int running_count = 0;
	Mutex lock;
	
	bool is_native = false;
	bool has_init = false;
	String unpacked_struct_key;
	String auto_natural_english_trans_key;
	String auto_poetic_styles_english_key;
	String main_natural_english_key;
	String stanza_key;
	String main_key;
	String other_key;
	String syllable_key, forbidden_words_key;
	String frozen_begin_key, frozen_end_key;
	
	int attr_begin_row = -1;
	Vector<Vector<int>> tmp_sug_ids;
	
public:
	typedef TxtAutoCompare CLASSNAME;
	TxtAutoCompare();
	
	void Init();
	void SetNative() {is_native = true;}
	void ToolMenu(Bar& bar) override;
	void PartMenu(Bar& bar);
	void Data() override;
	void DataSong();
	void DataSongPart(bool skip_results);
	void DataSongRhymeData();
	void SetSuggestionScore(EditIntNotNullSpin* e, StaticSuggestion* sug);
	void CopyAIToUser();
	void CopyUserToAI();
	void ImportEnglish();
	void ImproveSourceText(int style);
	void LimitContentSyllableCount();
	void UpdateRhymes(StaticPart& sp);
	void EvaluatePoeticStyles(int i);
	void EvaluateSuggestionScores();
	void EvaluateExtraSuggestionScores();
	void GetAIAttributes();
	void MorphAttrsTowardsContext();
	void MakeContentMoreLikeAttributes();
	
	void PostOnPoeticRecv(String res, int part, int rhyme) {PostCallback(THISBACK3(OnPoeticRecv, res, part, rhyme));}
	void OnPoeticRecv(String res, int part, int rhyme);
	void OnAttrChange(StaticPart* sp, const char* s, DropList* dl);
	void OnAttrChangeRhyme(StaticRhyme* r, const char* s, DropList* dl);
	void OnRhymeSchemeChange(DropList* dl, StaticPart* sp);
	void OnSongPartContentEdit(int src);
	void OnAcceptEditSource();
	void OnSongPartContentChange(DocEdit* e, StaticPart* sp);
	void OnSuggestionScore(String res, StaticRhyme* r, bool post_enable);
	void OnSuggestionOrder(String res, StaticRhyme* r, int idx);
	void OnAIAttributes(String res, StaticRhyme* r);
	void OnSourceTextImprovements(String res, StaticPart* s);
	void OnSongSyllableChange();
	void OnSongAttrCountChange();
	void OnMorphToAttributes(String res, StaticPart* s, StaticRhyme* r);
	void OnParamChange(EditString* e, int key);
	
	StaticPart* GetActiveSongPart();
	StaticRhyme* GetActiveRhyme();
	String GetBestSuggestionSong(const StaticPart& sp) const;
	void DataBestSuggestion();
	void DisableAll();
	void EnableAll();
	void UpdateRhymesToSource();
	void OnSongPartSyllableChange(StaticPart* sp, EditString* e);
	
};


#endif
