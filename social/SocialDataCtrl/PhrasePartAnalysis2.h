#ifndef _SocialDataCtrl_PhrasePartAnalysis2_h_
#define _SocialDataCtrl_PhrasePartAnalysis2_h_


class PhrasePartAnalysis2 : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, roles, generics, parts;
	
public:
	typedef PhrasePartAnalysis2 CLASSNAME;
	PhrasePartAnalysis2();
	
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataRole();
	void DataGeneric();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	void UpdateCounts();
	
};


#endif
