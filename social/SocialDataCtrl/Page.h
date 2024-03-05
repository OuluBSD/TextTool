#ifndef _SocialDataCtrl_SocialDataPage_h_
#define _SocialDataCtrl_SocialDataPage_h_


BEGIN_SOCIALLIB_NAMESPACE


class SocialDataPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, companies, programs;//, active_programs;
	DocEdit story, analysis;
	ProgressIndicator prog;
	bool disabled = false;
	
public:
	typedef SocialDataPage CLASSNAME;
	SocialDataPage();
	
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataDataset();
	void DataCompany();
	//void DataCompanyActivePrograms();
	void DataProgram();
	//void DataActiveProgram();
	void ToolMenu(Bar& bar) override;
	//void AddRandomProgramsToList(int count);
	//void AddProgramToActiveList();
	//void RemoveProgramFromActiveList();
	//void HotfixText();
	void ImportStory();
	//void StartHotfixText() {if (disabled) return; DisableAll(); Thread::Start(THISBACK(HotfixText));}
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {prog.Set(a,t);}
	
};


END_SOCIALLIB_NAMESPACE


#endif
