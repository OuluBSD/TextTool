#ifndef _SimplifiedCtrl_AutoIdeas_h_
#define _SimplifiedCtrl_AutoIdeas_h_


class AutoIdeas : public SongToolCtrl {
	bool disabled = false;
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl themes, ideas;
	ArrayCtrl contents, allegories;
	ArrayCtrl tones, symbolisms, imageries;
	
public:
	typedef AutoIdeas CLASSNAME;
	AutoIdeas();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataSong(bool set_cursor);
	void DataTheme(bool set_cursor);
	void DataIdea(bool set_cursor);
	void DataTone(bool set_cursor);
	void DataAllegory(bool set_cursor);
	void DataContent(bool set_cursor);
	void DataImagery(bool set_cursor);
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void GetNovelThemes();
	void GetNovelIdeas();
	void GetToneSuggestions();
	void GetAllegorySuggestions();
	void GetContentSuggestions();
	void GetSymbolismSuggestions();
	void GetImagerySuggestions();
	void GetAttrs(const VectorMap<String,String>& data, Vector<String>& v);
	void OnNovelThemes(String result, Song* song);
	void OnNovelIdeas(String result, StaticTheme* theme);
	void OnToneSuggestions(String result, StaticIdea* idea);
	void OnAllegorySuggestions(String result, StaticToneSuggestion* tone);
	void OnContentSuggestions(String result, StaticAllegoricalDevice* alleg);
	void OnImagerySuggestions(String result, StaticContentIdea* c);
	void OnSymbolismSuggestions(String result, StaticImagery* img);
	
	void OnListTheme();
	void OnListIdea();
	void OnListTone();
	void OnListAllegory();
	void OnListContentIdea();
	void OnListImagery();
	void OnListSymbolism();
	void SetAsActiveIdea();
	
};


#endif
