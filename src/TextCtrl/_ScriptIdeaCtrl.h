#ifndef _TextCtrl_ScriptIdeaCtrl_h_
#define _TextCtrl_ScriptIdeaCtrl_h_

#if 0

BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class ScriptIdeaCtrl : public WithScriptInfo<ToolAppCtrl> {
	DropList src;
	Splitter ideasplit, singersplit;
	ArrayCtrl typeclasses, contents, ideas;
	DocEdit idea[3], lead;
	
public:
	typedef ScriptIdeaCtrl CLASSNAME;
	ScriptIdeaCtrl();
	
	void Data() override;
	void DataTypeclass(bool keep_content_idx);
	void DataContent();
	void DataIdea();
	void Clear();
	void Do(int fn);
	void ToolMenu(Bar& bar) override;
	void SetTypeclassContent();
	ContentVisionOwner& GetCVO();
	
};


END_TEXTLIB_NAMESPACE


#endif
#endif
