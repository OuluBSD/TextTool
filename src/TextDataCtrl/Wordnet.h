#ifndef _TextDataCtrl_TextDataWordnet_h_
#define _TextDataCtrl_TextDataWordnet_h_


BEGIN_TEXTLIB_NAMESPACE


class TextDataWordnet : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, colors, wordnets;
	Mutex lock;
	
	String tmp_first_line;
	/*
	int per_batch = 25;
	bool batch = false;*/
public:
	typedef TextDataWordnet CLASSNAME;
	TextDataWordnet();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataAttribute();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingColorAlternatives();
	void GetColorAlternatives(int batch_i);
	void OnColorAlternatives(String res, int batch_i);
	
	void DoWordnet(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
