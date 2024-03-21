#ifndef _TextDataCtrl_TextDataWords_h_
#define _TextDataCtrl_TextDataWords_h_


BEGIN_TEXTLIB_NAMESPACE


class TextDataWords : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, colors, words;
	bool disabled = false;
	bool batch = false;
	
public:
	typedef TextDataWords CLASSNAME;
	TextDataWords();
	
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


END_TEXTLIB_NAMESPACE


#endif
