#ifndef _SocialCtrl_StoryPool_h_
#define _SocialCtrl_StoryPool_h_


BEGIN_SOCIALLIB_NAMESPACE


class StoryPool : public ToolAppCtrl {
	ArrayCtrl list;
	ProgressIndicator summary_bar;
	
public:
	typedef StoryPool CLASSNAME;
	StoryPool();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void StartProcess();
	void StopProcess();
	
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {summary_bar.Set(a,t);}
	
};


END_SOCIALLIB_NAMESPACE


#endif
