#ifndef _SongDataCtrl_PhrasePartAnalysis_h_
#define _SongDataCtrl_PhrasePartAnalysis_h_


struct ScoreDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const;
};


class PhrasePartAnalysis : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, attrs, colors, actions, action_args, parts;
	
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
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
	void UpdateCounts();
	
};


#endif
