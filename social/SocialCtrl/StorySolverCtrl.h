#ifndef _SocialCtrl_StorySolverCtrl_h_
#define _SocialCtrl_StorySolverCtrl_h_


BEGIN_SOCIALLIB_NAMESPACE


class StorySolverCtrl : public ToolAppCtrl {
	TabCtrl tabs;
	
	// Tab 1: process summary
	Splitter summary_split, result_split;
	ProgressIndicator summary_bar;
	ArrayCtrl summary_tasks, result_list;
	DocEdit suggestion, story;
	
	// Tab 2: metaphorical color process
	ArrayCtrl part_colors;
	
	// Tab 3: attribute process
	ArrayCtrl program_attrs;
	
	// Tab 4: action process
	Splitter action_split;
	ArrayCtrl action_part_list;
	ArrayCtrl action_list;
	
	// Tab 5: filtered phrases
	Splitter filtered_split;
	ArrayCtrl filtered_part_list;
	ArrayCtrl filtered_phrases;
	
	// Tab 6: primary phrase process
	Splitter primary_split;
	ArrayCtrl primary, secondary;
	
	
public:
	typedef StorySolverCtrl CLASSNAME;
	StorySolverCtrl();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void StartProcess();
	void StopProcess();
	void DataSuggestion();
	void UserStoryChange();
	void GetNewTitle();
	void OnNewTitle(String res);
	
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {summary_bar.Set(a,t);}
	
};


END_SOCIALLIB_NAMESPACE


#endif
