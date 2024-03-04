#ifndef _SocialDataCtrl_ProgramDataAnalysisPage_h_
#define _SocialDataCtrl_ProgramDataAnalysisPage_h_

#if 0


class ProgramDataAnalysisPage : public ToolAppCtrl {
	struct Task : Moveable<Task> {
		String company, program, text;
		StoryDataset* ld = 0;
		StoryAnalysis* analysis = 0;
		int progress = 0;
	};
	
	int txt_limit = 6000;
	
private:
	Splitter hsplit;
	ArrayCtrl list;
	DocEdit analysis;
	Vector<Task> tasks;
	ProgressIndicator prog;
	bool batch = false;
	bool disabled = false;
public:
	typedef ProgramDataAnalysisPage CLASSNAME;
	ProgramDataAnalysisPage();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataTask();
	void StartBatch();
	void ToolMenu(Bar& bar) override;
	void UpdateTasks();
	//void GetRhymes(int list_i, bool start_next);
	void GetWords(int list_i, bool start_next);
	void GetAttributeDistributions(int list_i, bool start_next);
	//void GetRhymeLocations(int list_i, bool start_next);
	void OnRhymes(String res, int list_i, bool start_next);
	void OnWords(String res, int list_i, bool start_next);
	void OnRoleDistributions(String res, int list_i, bool start_next);
	void OnRhymeLocations(String res, int list_i, bool start_next);
	
};


#endif
#endif
