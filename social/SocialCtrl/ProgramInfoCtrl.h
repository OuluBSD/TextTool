#ifndef _SocialCtrl_ProgramInfoCtrl_h_
#define _SocialCtrl_ProgramInfoCtrl_h_


class SocialEditor;

class ProgramInfoCtrl : public WithProgramInfo<ToolAppCtrl> {
	int focus_role = -1, focus_generic = -1, focus_story = -1;
	
public:
	typedef ProgramInfoCtrl CLASSNAME;
	ProgramInfoCtrl();
	
	void Data();
	void Clear();
	void DataRole();
	void DataGeneric();
	void DataStory();
	void OnValueChange();
	void SetStory();
	
	SocialEditor* editor = 0;
	
};


#endif
