#ifndef _TextCtrl_VideoPromptMaker_h_
#define _TextCtrl_VideoPromptMaker_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class VideoPromptMakerCtrl : public ToolAppCtrl {
	Splitter vsplit, storyboard_prompt_split;
	ArrayCtrl storyboard_parts;
	ArrayCtrl storyboard_prompt_keys, storyboard_prompt_values;
	ArrayCtrl text_storyboard_parts;
	
public:
	typedef VideoPromptMakerCtrl CLASSNAME;
	VideoPromptMakerCtrl();
	
	void Data() override;
	void DataPrompt();
	void OnValueChange();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
