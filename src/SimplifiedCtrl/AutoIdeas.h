#ifndef _SimplifiedCtrl_AutoIdeas_h_
#define _SimplifiedCtrl_AutoIdeas_h_


class AutoIdeas : public SongToolCtrl {
	bool disabled = false;
	Splitter hsplit, ideasplit;
	ArrayCtrl themes, ideas;
	ArrayCtrl contents, allegories;
	
public:
	typedef AutoIdeas CLASSNAME;
	AutoIdeas();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataSong();
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void GetNovelThemes();
	void GetNovelIdeas();
	void GetContentSuggestions();
	void GetAllegoricalDevices();
	void GetAttrs(const VectorMap<String,String>& data, Vector<String>& v);
	void OnNovelThemes(String result, Song* song);
	
};


#endif
