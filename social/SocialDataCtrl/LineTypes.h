#ifndef _SocialDataCtrl_LineTypes_h_
#define _SocialDataCtrl_LineTypes_h_


#if 0

class LineTypesPage : public ToolAppCtrl {
	struct Batch : Moveable<Batch> {
		CompanyDataset* company;
		StoryDataset* story;
		String txt;
		int ds_i;
		bool program_begins;
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
#endif