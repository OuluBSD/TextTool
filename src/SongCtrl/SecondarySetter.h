#ifndef _SongCtrl_SecondarySetter_h_
#define _SongCtrl_SecondarySetter_h_

#if 0
class SecondarySetter : public SongToolCtrl {
	
public:
	typedef SecondarySetter CLASSNAME;
	SecondarySetter();
	
	void Data() override;
	void PostData() {PostCallback(THISBACK(Data));}
	void ToolMenu(Bar& bar) override;
	
};


#endif
#endif
