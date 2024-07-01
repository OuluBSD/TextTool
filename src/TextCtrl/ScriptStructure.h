#ifndef _TextCtrl_ComponentStructure_h_
#define _TextCtrl_ComponentStructure_h_


BEGIN_TEXTLIB_NAMESPACE


class ComponentStructure : public ToolAppCtrl {
	Splitter hsplit, script_split, ref_split;
	ArrayCtrl script_parts, script_lines;
	ArrayCtrl ref_names, ref_lines;
	bool disabled = false;
	Vector<TextSolver*> v;
	
public:
	typedef ComponentStructure CLASSNAME;
	ComponentStructure();
	
	void Data() override;
	void DataScript();
	void DataComponent();
	void DataSuggestions();
	void DataSuggestionAttributes();
	void DataActive();
	void DataPart();
	void LoadReference();
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	
	#if 0
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
	void LoadReference();
	#endif
	
};


END_TEXTLIB_NAMESPACE


#endif
