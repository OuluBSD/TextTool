#ifndef _SimplifiedCtrl_TxtSerialCompare_h_
#define _SimplifiedCtrl_TxtSerialCompare_h_



class TxtSerialCompare : public SongToolCtrl {
	
	
public:
	typedef TxtSerialCompare CLASSNAME;
	TxtSerialCompare();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	
	
	
};



#endif
