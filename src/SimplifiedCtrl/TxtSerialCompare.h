#ifndef _SimplifiedCtrl_TxtSerialCompare_h_
#define _SimplifiedCtrl_TxtSerialCompare_h_



class TxtSerialCompare : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	ArrayCtrl rhymes, suggestions, params, attrs;
	DocEdit best;
	
public:
	typedef TxtSerialCompare CLASSNAME;
	TxtSerialCompare();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void DataPart();
	void DataBreak();
	
	
};



#endif
