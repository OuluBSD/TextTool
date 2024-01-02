#ifndef _SongDataCtrl_SongDataWordnet_h_
#define _SongDataCtrl_SongDataWordnet_h_

#if 0


class SongDataWordnet : public SongToolCtrl {
	Splitter vsplit, hsplit, vsplit1;
	ArrayCtrl datasets, attrs, colors, wordnets, clr_wordnets;
	Mutex lock;
	
	String tmp_first_line;
	VectorMap<String, int> word_ds;
	VectorMap<String, Color> word_clr;
	
	bool disabled = false;
	bool running = false;
	int per_batch = 25;
	bool batch = false;
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
	
	void MakeWordnetsFromTemplates();
	
};


#endif
#endif
