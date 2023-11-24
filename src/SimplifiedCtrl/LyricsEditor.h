#ifndef _SimplifiedCtrl_LyricsEditor_h_
#define _SimplifiedCtrl_LyricsEditor_h_


class LyricsEditor : public SongToolCtrl {
	TabCtrl pages;
	Splitter vsplit, hsplit;
	ArrayCtrl parts, lines;
	WithLineEditor<Ctrl> attr;
	
	// First line picker
	Splitter line_split;
	ArrayCtrl line_actions, line_templates, line_phrases;
	
	// Coherent idea
	Splitter idea_hsplit, idea_vsplit0, idea_vsplit1;
	ArrayCtrl idea_lists[SCORE_COUNT];
	
	// Part idea
	Splitter part_hsplit, part_vsplit0, part_vsplit1;
	ArrayCtrl part_lists[SCORE_COUNT];
	
	
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
