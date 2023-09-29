#ifndef _SongTool_TxtAutoCompare_h_
#define _SongTool_TxtAutoCompare_h_


class TxtAutoCompare : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl parts, rhymes, suggestions, attrs;
	DocEdit best;
	
	DocEdit edit_source;
	
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
	
	int attr_begin_row = -1;
	Vector<int> tmp_sug_ids;
	
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
	void SetSuggestionScore(EditIntNotNullSpin* e, Song::Suggestion* sug);
	void CopyAIToUser();
	void ImportEnglish();
	void ImproveSourceText(int style);
	void UpdateRhymes(Song::SongPart& sp, int src);
	void EvaluatePoeticStyles(int i, int src);
	void EvaluateSuggestionScores();
	void EvaluateExtraSuggestionScores();
	void GetAIAttributes();
	void PostOnPoeticRecv(String res, int part, int rhyme) {PostCallback(THISBACK3(OnPoeticRecv, res, part, rhyme));}
	void OnPoeticRecv(String res, int part, int rhyme);
	void OnAttrChange(Song::SongPart* sp, const char* s, DropList* dl);
	void OnAttrChangeRhyme(Song::Rhyme* r, const char* s, DropList* dl);
	void OnRhymeSchemeChange(DropList* dl, Song::SongPart* sp);
	void OnSongPartContentEdit();
	void OnAcceptEditSource();
	void OnSongPartContentChange(DocEdit* e, Song::SongPart* sp);
	void OnSuggestionScore(String res, Song::Rhyme* r);
	void OnSuggestionOrder(String res, Song::Rhyme* r);
	void OnAIAttributes(String res, Song::Rhyme* r);
	void OnSourceTextImprovements(String res, int begin, int end, Song* s);
	Song::SongPart* GetActiveSongPart();
	Song::Rhyme* GetActiveRhyme();
	String GetBestSuggestionSong(const Song::SongPart& sp) const;
	void DataBestSuggestion();
	void DisableAll();
	void EnableAll();
	void UpdateRhymesToSource(int src);
	void OnSongPartSyllableChane(Song::SongPart* sp, EditIntSpin* e);
	
};


#endif
