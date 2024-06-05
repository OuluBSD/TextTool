#ifndef _TextCtrl_ScriptIdeaCtrl_h_
#define _TextCtrl_ScriptIdeaCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class ScriptIdeaCtrl : public WithScriptInfo<ToolAppCtrl> {
	Splitter vsplit, ideasplit;
	ArrayCtrl typeclasses, contents, ideas;
	DocEdit idea;
	
public:
	typedef ScriptIdeaCtrl CLASSNAME;
	ScriptIdeaCtrl();
	
	void Data() override;
	void DataTypeclass();
	void DataContent();
	void DataIdea();
	void Clear();
	void Do(int fn);
	void ToolMenu(Bar& bar) override;
	void SetTypeclassContent();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
