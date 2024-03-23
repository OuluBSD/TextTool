#ifndef _TextCtrl_ScriptPool_h_
#define _TextCtrl_ScriptPool_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptPool : public ToolAppCtrl {
	ArrayCtrl list;
	ProgressIndicator summary_bar;
	
public:
	typedef ScriptPool CLASSNAME;
	ScriptPool();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void StartProcess();
	void StopProcess();
	
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {summary_bar.Set(a,t);}
	
};


END_TEXTLIB_NAMESPACE


#endif
