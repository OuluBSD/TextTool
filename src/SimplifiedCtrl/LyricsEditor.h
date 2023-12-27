#ifndef _SimplifiedCtrl_LyricsEditor_h_
#define _SimplifiedCtrl_LyricsEditor_h_


class LyricsEditor : public SongToolCtrl {
	Splitter vsplit, hsplit0, hsplit1;
	ArrayCtrl phrase_list, suggestions, actions, fitting;
	TabCtrl tabs;
	
	int ds_i = 0;
	
public:
	typedef LyricsEditor CLASSNAME;
	LyricsEditor();
	
	void Data() override;
	void DataPhrase();
	void GetSuggestions();
	void GetSuggestionsAI();
	void SetPhrase();
	void MovePhrase(int i);
	void OnSuggestions(String res);
	void ToolMenu(Bar& bar) override;
	void CopyPronounciationToClipboard();
	
};


#endif
