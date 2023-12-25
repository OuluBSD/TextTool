#ifndef _SongDataCtrl_LineTypes_h_
#define _SongDataCtrl_LineTypes_h_


class LineTypesPage : public SongToolCtrl {
	struct Batch : Moveable<Batch> {
		ArtistDataset* artist;
		LyricsDataset* lyrics;
		String txt;
		int ds_i;
		bool song_begins;
	};
	
	Vector<Batch> batches;
	int total_struct_count = 0;
	
	void UpdateBatches();
	
protected:
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, structures, clauses, phrases, next_lines;
	Mutex lock;
	
	Vector<Wordnet*> tmp_wordnets;
	Vector<ColorWordnet*> tmp_clr_wordnets;
	VectorMap<String, VectorMap<String, int>> uniq_st;
	Vector<int> ap_is;
	int prev_st_i = -1;
	int per_color_batch = 30;
	int ds_i = 0;
	
	int structure_cols = 7;
	bool disabled = false;
	bool running0 = false;
	bool running1 = false;
	int per_batch = 15;
	bool batch = false;
	
public:
	typedef LineTypesPage CLASSNAME;
	LineTypesPage();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataStructure();
	void DataStructureHeader();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingLineStructures();
	void GetLineStructures(int batch_i);
	void OnLineStructures(String res, int batch_i);
	
	void ToggleGettingStructureColors();
	void GetStructureColors(int batch_i);
	void OnStructureColors(String res, int batch_i);
	
};


#endif
