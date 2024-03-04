#ifndef _SongDataCtrl_SongDataLineActions_h_
#define _SongDataCtrl_SongDataLineActions_h_


class SongDataLineActions : public ToolAppCtrl {
	
	
protected:
	Splitter vsplit, hsplit, vsplit1;
	ArrayCtrl datasets, actions, action_args, phrases, next_lines;
	Mutex lock;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	Vector<int> ap_is;
	
	int action_cols = 7;
	/*bool disabled = false;
	bool running0 = false;
	bool running1 = false;
	int per_batch = 15;
	bool batch = false;*/
	
public:
	typedef SongDataLineActions CLASSNAME;
	SongDataLineActions();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAction();
	void DataActionHeader();
	void DataNextLine();
	void ToolMenu(Bar& bar) override;
	
	/*void ToggleGettingLineActions();
	void GetLineActions(int batch_i);
	void OnLineActions(String res, int batch_i);
	
	void ToggleGettingLineChangeScores(int score_mode);
	void GetLineChangeScores(int batch_i, int score_mode);
	void OnLineChangeScores(String res, int batch_i, int score_mode);*/
	
};


#endif
