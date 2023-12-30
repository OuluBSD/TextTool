#ifndef _SongDataCtrl_Export_h_
#define _SongDataCtrl_Export_h_


class ExportPage : public SongToolCtrl {
	
public:
	typedef ExportPage CLASSNAME;
	ExportPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
