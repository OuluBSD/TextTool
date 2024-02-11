#ifndef _SongDataCtrl_PhrasePartAnalysis2_h_
#define _SongDataCtrl_PhrasePartAnalysis2_h_


class PhrasePartAnalysis2 : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, typecasts, profiles, primaries, secondaries, parts;
	
public:
	typedef PhrasePartAnalysis2 CLASSNAME;
	PhrasePartAnalysis2();
	
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataTypecast();
	void DataProfile();
	void DataPrimary();
	void DataSecondary();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	void UpdateCounts();
	
};


#endif
