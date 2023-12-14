#ifndef _SongDataCtrl_SongDataWords_h_
#define _SongDataCtrl_SongDataWords_h_


class SongDataWords : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, colors, artists, words;
	bool disabled = false;
	int per_batch = 30;
	Vector<int> tmp_ds_i;
	VectorMap<String, int> tmp_map_ds_i;
	Index<String> tmp_words;
	bool batch = false;
	
public:
	typedef SongDataWords CLASSNAME;
	SongDataWords();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataColor();
	void DataArtist();
	void ToolMenu(Bar& bar) override;
	void UpdateWords();
	void UpdateWordsProcess();
	//void UpdateWordFlagGroups();
	//void UpdateWordFlags();
	void DumpWordGroups();
	void DumpPhoneticChars();
	void GetEverything();
	void GetSyllables(int batch_i, bool start_next);
	void GetDetails(int batch_i, bool start_next);
	void OnSyllables(String res, int batch_i, bool start_next);
	void OnDetails(String res, int batch_i, bool start_next);
	
};


#endif
