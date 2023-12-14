#ifndef _SongDataCtrl_SongDataTemplates_h_
#define _SongDataCtrl_SongDataTemplates_h_


class SongDataTemplates : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, tmpls;
	Mutex lock;
	
	bool disabled = false;
	bool running = false;
	int per_batch = 30;
	bool batch = false;
	
	VectorMap<int,int> line_to_src, ds_is;
	VectorMap<int,LyricsAnalysis::Phrase*> phrases;
	
public:
	typedef SongDataTemplates CLASSNAME;
	SongDataTemplates();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	void ToggleGettingTemplates();
	void GetTemplatePhrases(int batch_i);
	void OnTemplatePhrases(String res, int batch_i);
	
	
};


#endif
