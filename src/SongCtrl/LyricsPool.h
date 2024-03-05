#ifndef _SongCtrl_SongPool_h_
#define _SongCtrl_SongPool_h_


BEGIN_SONGLIB_NAMESPACE


class SongPool : public ToolAppCtrl {
	ArrayCtrl list;
	ProgressIndicator summary_bar;
	
public:
	typedef SongPool CLASSNAME;
	SongPool();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void StartProcess();
	void StopProcess();
	
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {summary_bar.Set(a,t);}
	
};


END_SONGLIB_NAMESPACE


#endif