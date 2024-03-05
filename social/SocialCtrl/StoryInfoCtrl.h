#ifndef _SocialCtrl_StoryInfoCtrl_h_
#define _SocialCtrl_StoryInfoCtrl_h_


BEGIN_SOCIALLIB_NAMESPACE


class SocialEditor;

class StoryInfoCtrl : public WithStoryInfo<ToolAppCtrl> {
	
	
public:
	typedef StoryInfoCtrl CLASSNAME;
	StoryInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SocialEditor* editor = 0;
	
};


END_SOCIALLIB_NAMESPACE


#endif
