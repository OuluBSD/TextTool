#ifndef _SimplifiedCtrl_SongDataWordnet_h_
#define _SimplifiedCtrl_SongDataWordnet_h_


class SongDataWordnet : public SongToolCtrl {
	bool disabled = false;
	
public:
	typedef SongDataWordnet CLASSNAME;
	SongDataWordnet();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
	
	
};


#endif
