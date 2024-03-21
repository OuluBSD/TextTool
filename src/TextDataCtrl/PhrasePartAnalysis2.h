#ifndef _TextDataCtrl_PhrasePartAnalysis2_h_
#define _TextDataCtrl_PhrasePartAnalysis2_h_


BEGIN_TEXTLIB_NAMESPACE


class PhrasePartAnalysis2 : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, typecasts, contrasts, profiles, contents, primaries, secondaries, parts;
	
public:
	typedef PhrasePartAnalysis2 CLASSNAME;
	PhrasePartAnalysis2();
	
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataTypeclass();
	void DataContrast();
	void DataProfile();
	void DataContent();
	void DataPrimary();
	void DataSecondary();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	void UpdateCounts();
	
};


END_TEXTLIB_NAMESPACE


#endif
