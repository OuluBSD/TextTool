#ifndef _SongTool_TxtAutoCompare_h_
#define _SongTool_TxtAutoCompare_h_


class TxtAutoCompare : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl parts, lines, line_results, attrs;
	DocEdit best;
	
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
	
public:
	typedef TxtAutoCompare CLASSNAME;
	TxtAutoCompare();
	
	void Init();
	void SetNative() {is_native = true;}
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void DataSong();
	void DataSongPart(bool skip_results);
	void DataSongRhymeData();
	void SetSuggestionScore(EditIntNotNullSpin* e, Song::Suggestion* sug);
	void ImportEnglish();
	void UpdateRhymes(Song::SongPart& sp);
	void EvaluatePoeticStyles(int i);
	void PostOnPoeticRecv(String res, int part, int rhyme) {PostCallback(THISBACK3(OnPoeticRecv, res, part, rhyme));}
	void OnPoeticRecv(String res, int part, int rhyme);
	void OnAttrChange(Song::SongPart* sp, const char* s, DropList* dl);
	void OnRhymeSchemeChange(DropList* dl, Song::SongPart* sp);
	Song::SongPart* GetActiveSongPart();
	Song::Rhyme* GetActiveRhyme();
	String GetBestSuggestionSong(const Song::SongPart& sp) const;
	void DataBestSuggestion();
	
};


#endif
