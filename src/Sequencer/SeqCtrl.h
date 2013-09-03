#ifndef _Sequencer_SeqCtrl_h_
#define _Sequencer_SeqCtrl_h_


class SeqCtrl : public Ctrl {
	Splitter vsplit;
	Sequencer seq;
	LineEditCtrl line;
	
public:
	typedef SeqCtrl CLASSNAME;
	SeqCtrl();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
};


#endif
