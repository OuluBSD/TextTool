#ifndef _SocialDataCtrl_ProgramDataWordnet_h_
#define _SocialDataCtrl_ProgramDataWordnet_h_


class ProgramDataWordnet : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, wordnets;
	Mutex lock;
	
	String tmp_first_line;
	/*
	int per_batch = 25;
	bool batch = false;*/
public:
	typedef ProgramDataWordnet CLASSNAME;
	ProgramDataWordnet();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingColorAlternatives();
	void GetColorAlternatives(int batch_i);
	void OnColorAlternatives(String res, int batch_i);
	
	void DoWordnet(int fn);
	
};


#endif
