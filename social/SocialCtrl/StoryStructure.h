#ifndef _SocialCtrl_StoryStructure_h_
#define _SocialCtrl_StoryStructure_h_


class StoryStructure : public ToolAppCtrl {
	Splitter vsplit, hsplit, attr_split;
	WithStoryStructure<Ctrl> active;
	ArrayCtrl params, structs, attributes, parts;
	bool disabled = false;
	
public:
	typedef StoryStructure CLASSNAME;
	StoryStructure();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataProgram();
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
	void OnStructureSuggestion(String result, Story* l);
	void OnSuggestionAttributes(String result, Story* l);
	void LoadActiveStruct();
	
	
};

#endif
