#ifndef _SimplifiedCtrl_LyricsEditor_h_
#define _SimplifiedCtrl_LyricsEditor_h_


class LyricsEditor : public SongToolCtrl {
	struct ActionResult : Moveable<ActionResult> {
		int ap_i = -1;
		VectorMap<int, double> ta_matches;
		double av_match = 0;
	};
	TabCtrl pages;
	Splitter vsplit, hsplit;
	ArrayCtrl parts, lines;
	WithLineEditor<Ctrl> attr;
	Index<String> attr_groups;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int ds_i = 0;
	
	// First line picker
	Splitter line_split;
	ArrayCtrl line_actions, line_templates, line_phrases;
	Vector<ActionResult> action_results;
	VectorMap<int, double> ta_matches;
	Vector<String> sents;
	
	// Coherent idea
	Splitter idea_hsplit, idea_vsplit0, idea_vsplit1;
	//ArrayCtrl idea_lists[SCORE_COUNT];
	
	// Part idea
	Splitter part_hsplit, part_vsplit0, part_vsplit1;
	//ArrayCtrl part_lists[SCORE_COUNT];
	
	
	void OnAttribute();
	void OnAction();
	void UpdateActionList();
	void UpdateActions(Song& song, StaticPart& part, DatasetAnalysis& da);
	void UpdateTemplates(DatasetAnalysis& da);
	void UpdatePhrases(DatasetAnalysis& da);
	void GetSentences(const TemplatePhrase& tp, Vector<String>& sents, int limit);
	
public:
	typedef LyricsEditor CLASSNAME;
	LyricsEditor();
	
	void Data() override;
	void DataPart();
	void DataLine();
	void DataLinePicker();
	void DataIdea();
	void DataPartIdea();
	void ToolMenu(Bar& bar) override;
	
};


#endif
