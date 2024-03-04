#ifndef _SongCtrl_PressRelease_h_
#define _SongCtrl_PressRelease_h_


class PressReleaseCtrl : public SongToolCtrl {
	
	
public:
	typedef PressReleaseCtrl CLASSNAME;
	PressReleaseCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
