#ifndef _SimplifiedCtrl_SongDataWords_h_
#define _SimplifiedCtrl_SongDataWords_h_


class SongDataWords : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl datasets, words;
	
public:
	typedef SongDataWords CLASSNAME;
	SongDataWords();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void UpdateWords();
	void UpdateWordFlags();
	void GetSyllables(int batch_i, bool start_next);
	void OnSyllables(String res, int batch_i, bool start_next);
	
};


#endif
