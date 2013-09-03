#ifndef _SimplifiedCtrl_PartIdea_h_
#define _SimplifiedCtrl_PartIdea_h_


class PartIdea : public SongToolCtrl {
	Splitter vsplit, hsplit0, hsplit1;
	ArrayCtrl active, parts;
	ArrayCtrl contents, imageries, symbolisms, persons;
	
	bool disabled = false;
	
public:
	typedef PartIdea CLASSNAME;
	PartIdea();
	
	void Data() override;
	void DataSong(bool set_cursor);
	void DataPart(bool set_cursor);
	void DataContent(bool set_cursor);
	void DataImagery(bool set_cursor);
	void DataSymbolism(bool set_cursor);
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	
	void EnableAll();
	void DisableAll();
	bool HasActiveIdea();
	void GetContentSuggestions();
	void GetImagerySuggestions();
	void GetSymbolismSuggestions();
	void GetPersonSuggestions();
	
	void OnListPart();
	void OnListContent();
	void OnListImagery();
	void OnListSymbolism();
	void OnListPersons();
	void SetAsActiveIdea();
	void ClearAllIdeas();
	
	void OnContentSuggestions(String result, Song* song, int part_i);
	void OnImagerySuggestions(String result, StaticContentSuggestion* c);
	void OnSymbolismSuggestions(String result, StaticImagery* img);
	void OnPersonSuggestions(String result, StaticSymbolism* sym);
	
};


#endif
