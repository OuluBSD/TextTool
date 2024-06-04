#ifndef _TextCtrl_ComponentStructure_h_
#define _TextCtrl_ComponentStructure_h_


BEGIN_TEXTLIB_NAMESPACE


class ComponentStructure : public ToolAppCtrl {
	Splitter vsplit, hsplit, attr_split;
	WithComponentStructure<Ctrl> active;
	ArrayCtrl params, structs, attributes, parts;
	bool disabled = false;
	
public:
	typedef ComponentStructure CLASSNAME;
	ComponentStructure();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataComponent();
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
	void OnStructureSuggestion(String result, Script* l);
	void OnSuggestionAttributes(String result, Script* l);
	void LoadActiveStruct();
	void LoadSingers();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
