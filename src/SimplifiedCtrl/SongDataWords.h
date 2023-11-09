#ifndef _SimplifiedCtrl_SongDataWords_h_
#define _SimplifiedCtrl_SongDataWords_h_


class SongDataWords : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, wordgroups, artists, words;
	bool disabled = false;
	int per_batch = 30;
	
public:
	typedef SongDataWords CLASSNAME;
	SongDataWords();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataDataset();
	void DataWordgroup();
	void DataArtist();
	void ToolMenu(Bar& bar) override;
	void UpdateWords();
	void UpdateWordsProcess();
	void UpdateWordFlagGroups();
	void UpdateWordFlags();
	void GetSyllables(int batch_i, bool start_next);
	void OnSyllables(String res, int batch_i, bool start_next);
	
};


#endif
