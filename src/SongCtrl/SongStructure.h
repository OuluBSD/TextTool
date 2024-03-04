#ifndef _SongCtrl_SongStructure_h_
#define _SongCtrl_SongStructure_h_


class SongStructure : public ToolAppCtrl {
	Splitter vsplit, hsplit, attr_split;
	WithSongStructure<Ctrl> active;
	ArrayCtrl params, structs, attributes, parts;
	bool disabled = false;
	
public:
	typedef SongStructure CLASSNAME;
	SongStructure();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataSong();
	void DataSuggestions();
	void DataSuggestionAttributes();
	void DataActive();
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void LoadStructure();
	void LoadUserStructure();
	void GetStructureSuggestions();
	void GetSuggestionAttributes();
	//void SetParam(String key, String value);
	//String GetParam(String key, String def="");
	void LoadStructureString(String struct_str);
	void OnStructureSuggestion(String result, Lyrics* l);
	void OnSuggestionAttributes(String result, Lyrics* l);
	void LoadActiveStruct();
	
	
};

#endif
