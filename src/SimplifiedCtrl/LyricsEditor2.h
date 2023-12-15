#ifndef _SimplifiedCtrl_LyricsEditor2_h_
#define _SimplifiedCtrl_LyricsEditor2_h_


class LyricsEditor2 : public SongToolCtrl {
	Splitter vsplit, top, mid, bottom;
	DocEdit mockup;
	ArrayCtrl thrds;
	ArrayCtrl thrd_phrases, thrd_settings, thrd_node;
	ArrayCtrl parts, phrases;
	
	Array<SongAgent::Solver> solvers;
	
	
public:
	typedef LyricsEditor2 CLASSNAME;
	LyricsEditor2();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	void OnMockupChange();
	void SolveThreads();
	
};


#endif
