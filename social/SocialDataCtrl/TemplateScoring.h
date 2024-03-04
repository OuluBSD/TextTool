#ifndef _SocialDataCtrl_ProgramDataTemplateScoring_h_
#define _SocialDataCtrl_ProgramDataTemplateScoring_h_

#if 0


class ProgramDataTemplateScoring : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, scores;
	Mutex lock;
	
	bool disabled = false;
	bool running = false;
	int per_batch = 30;
	bool batch = false;
	
	Vector<int> line_to_src, ds_is;
	Vector<TemplatePhrase*> phrases;
	
public:
	typedef ProgramDataTemplateScoring CLASSNAME;
	ProgramDataTemplateScoring();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	void ToggleGettingTemplates(int score_mode);
	void GetTemplateScores(int batch_i, int score_mode);
	void OnTemplateScores(String res, int batch_i, int score_mode);
	
	
};


#endif
#endif
