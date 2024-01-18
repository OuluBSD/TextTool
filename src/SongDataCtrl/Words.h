#ifndef _SongDataCtrl_SongDataWords_h_
#define _SongDataCtrl_SongDataWords_h_


class SongDataWords : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, colors, words;
	bool disabled = false;
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
	void ToolMenu(Bar& bar) override;
	//void UpdateWords();
	//void UpdateWordsProcess();
	//void UpdateWordFlagGroups();
	//void UpdateWordFlags();
	void DumpWordGroups();
	void DumpPhoneticChars();
	void DoWords(int fn);
	
};


#endif
