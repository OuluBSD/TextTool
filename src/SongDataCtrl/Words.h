#ifndef _SongDataCtrl_SongDataWords_h_
#define _SongDataCtrl_SongDataWords_h_


BEGIN_SONGLIB_NAMESPACE


class SongDataWords : public ToolAppCtrl {
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
	void DoWordFix(int fn);
	void DoWords(int fn);
	
};


END_SONGLIB_NAMESPACE


#endif
