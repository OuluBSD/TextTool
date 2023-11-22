#ifndef _SimplifiedCtrl_SongDataLineActions_h_
#define _SimplifiedCtrl_SongDataLineActions_h_


class SongDataLineActions : public SongToolCtrl {
	struct Batch : Moveable<Batch> {
		ArtistDataset* artist;
		LyricsDataset* lyrics;
		String txt;
		int ds_i;
	};
	
	Vector<Batch> batches;
	int tmp_batch_i = -1;
	
	void UpdateBatches();
	
protected:
	Splitter vsplit, hsplit, vsplit1;
	ArrayCtrl datasets, actions, action_args, phrases, clr_wordnets;
	Mutex lock;
	
	Vector<Wordnet*> tmp_wordnets;
	Vector<ColorWordnet*> tmp_clr_wordnets;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
	int action_cols = 7;
	bool disabled = false;
	bool running0 = false;
	bool running1 = false;
	int per_batch = 15;
	bool batch = false;
public:
	typedef SongDataLineActions CLASSNAME;
	SongDataLineActions();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAction();
	void DataActionArg();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingLineActions();
	void GetLineActions(int batch_i);
	void OnLineActions(String res, int batch_i);
	
	void ToggleGettingColorWordnetScores();
	void GetColorWordnetScores(int batch_i);
	void OnColorWordnetScores(String res, int batch_i);
	
};


#endif
