#ifndef _EditorCtrl_Marketing_h_
#define _EditorCtrl_Marketing_h_


class MarketingCtrl : public SongToolCtrl {
	
	
public:
	typedef MarketingCtrl CLASSNAME;
	MarketingCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
