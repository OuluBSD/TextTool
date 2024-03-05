#ifndef _SongDataCtrl_PhrasePartAnalysis2_h_
#define _SongDataCtrl_PhrasePartAnalysis2_h_


BEGIN_SONGLIB_NAMESPACE


class PhrasePartAnalysis2 : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, typecasts, contrasts, profiles, archetypes, primaries, secondaries, parts;
	
public:
	typedef PhrasePartAnalysis2 CLASSNAME;
	PhrasePartAnalysis2();
	
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataTypecast();
	void DataContrast();
	void DataProfile();
	void DataArchetype();
	void DataPrimary();
	void DataSecondary();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	void UpdateCounts();
	
};


END_SONGLIB_NAMESPACE


#endif
