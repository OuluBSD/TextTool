#ifndef _SimplifiedCtrl_SongDataSearchPhrases_h_
#define _SimplifiedCtrl_SongDataSearchPhrases_h_


class SongDataSearchPhrases : public SongToolCtrl {
	
public:
	typedef SongDataSearchPhrases CLASSNAME;
	SongDataSearchPhrases();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
