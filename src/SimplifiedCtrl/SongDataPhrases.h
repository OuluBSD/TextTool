#ifndef _SimplifiedCtrl_SongDataPhrases_h_
#define _SimplifiedCtrl_SongDataPhrases_h_


class SongDataPhrases : public SongToolCtrl {
	int phrase_limit = 200000000;
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, vphrases, phrases, songs;
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
	void DataVirtualPhrase();
	void ToolMenu(Bar& bar) override;
	void StartMakeUniquePhrases();
	void MakeUniquePhrases();
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {prog.Set(a,t);}
	
};


#endif
