#ifndef _TextDataCtrl_SongDataPhrases_h_
#define _TextDataCtrl_SongDataPhrases_h_


#if 0

class SongDataPhrases : public ToolAppCtrl {
	int phrase_limit = 200000000;
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, components;
	ProgressIndicator prog;
	bool disabled = false;
	
public:
	typedef SongDataPhrases CLASSNAME;
	SongDataPhrases();
	
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