#ifndef _SocialDataCtrl_ProgramDataPhrases_h_
#define _SocialDataCtrl_ProgramDataPhrases_h_


#if 0

class ProgramDataPhrases : public ToolAppCtrl {
	int phrase_limit = 200000000;
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, programs;
	ProgressIndicator prog;
	bool disabled = false;
	
public:
	typedef ProgramDataPhrases CLASSNAME;
	ProgramDataPhrases();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataDataset();
	void DataMain();
	void DataAttribute();
	void ToolMenu(Bar& bar) override;
	
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {prog.Set(a,t);}
	
};


#endif
#endif
