#ifndef _SocialDataCtrl_ProgramDataWords_h_
#define _SocialDataCtrl_ProgramDataWords_h_


class ProgramDataWords : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, colors, words;
	bool disabled = false;
	bool batch = false;
	
public:
	typedef ProgramDataWords CLASSNAME;
	ProgramDataWords();
	
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


#endif
