#ifndef _SongDataCtrl_RhymeContainer_h_
#define _SongDataCtrl_RhymeContainer_h_


class RhymeContainerPage : public SongToolCtrl {
	ProgressIndicator prog;
	Splitter hsplit, vsplit;
	ArrayCtrl syl_counts, colors, attrs, actions, data;
	bool running = false, stopped = true;
	
	void Start();
	void Stop();
	void Process();
public:
	typedef RhymeContainerPage CLASSNAME;
	RhymeContainerPage();
	~RhymeContainerPage();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void ProcessData();
	void SetProgress(int a, int t) {prog.Set(a,t);}
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	
};


#endif
