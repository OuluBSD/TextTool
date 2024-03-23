#ifndef _TextDataCtrl_SongDataTmplActions_h_
#define _TextDataCtrl_SongDataTmplActions_h_

#if 0


class SongDataTmplActions : public ToolAppCtrl {
	static const int per_batch = 15;
	
	struct Batch : Moveable<Batch> {
		String txt;
		int tp_i[per_batch];
		int ds_i;
	};
	
	Vector<Batch> batches;
	int tmp_batch_i = -1;
	
	void UpdateBatches();
	
protected:
	Splitter vsplit, hsplit, vsplit1;
	ArrayCtrl datasets, attrs, colors, phrases, matches;
	Mutex lock;
	
	Vector<Wordnet*> tmp_wordnets;
	Vector<ColorWordnet*> tmp_clr_wordnets;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	Vector<int> ap_is;
	
	int action_cols = 7;
	bool disabled = false;
	bool running0 = false;
	bool running1 = false;
	bool batch = false;
public:
	typedef SongDataTmplActions CLASSNAME;
	SongDataTmplActions();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void DataMatches();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingLineActions();
	void GetLineActions(int batch_i);
	void OnLineActions(String res, int batch_i);
	
};


#endif
#endif
