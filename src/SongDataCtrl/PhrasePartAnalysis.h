#ifndef _SongDataCtrl_PhrasePartAnalysis_h_
#define _SongDataCtrl_PhrasePartAnalysis_h_


class PhrasePartAnalysis : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, actions, action_args, parts;
	
public:
	typedef PhrasePartAnalysis CLASSNAME;
	PhrasePartAnalysis();
	
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void DataAction();
	void DataActionHeader();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	
};


#endif
