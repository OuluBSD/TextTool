#ifndef _TextCtrl_ScriptEditor_h_
#define _TextCtrl_ScriptEditor_h_

#if 0


class ScriptEditor : public ToolAppCtrl {
	Splitter vsplit, hsplit0, hsplit1, vsplit1;
	ArrayCtrl phrase_list, suggestions, line_actions, fitting;
	ArrayCtrl attrs, colors, actions, action_args, action_thrd, rhyme_level;
	
	int ds_i = 0;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
public:
	typedef ScriptEditor CLASSNAME;
	ScriptEditor();
	
	void Data() override;
	void DataPhrase();
	void DataDoActions();
	void DataAction();
	void DataActionHeader();
	//void GetSuggestionsAI();
	void SetPhrase();
	void MovePhrase(int i);
	void OnSuggestions(String res);
	void ToolMenu(Bar& bar) override;
	void CopyPronounciationToClipboard();
	
};


#endif
#endif
