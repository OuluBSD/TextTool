#ifndef _SongDataCtrl_SongDataWordnet_h_
#define _SongDataCtrl_SongDataWordnet_h_


BEGIN_SONGLIB_NAMESPACE


class SongDataWordnet : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, wordnets;
	Mutex lock;
	
	String tmp_first_line;
	/*
	int per_batch = 25;
	bool batch = false;*/
public:
	typedef SongDataWordnet CLASSNAME;
	SongDataWordnet();
	
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


END_SONGLIB_NAMESPACE


#endif
