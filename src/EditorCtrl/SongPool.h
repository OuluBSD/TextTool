#ifndef _EditorCtrl_SongPool_h_
#define _EditorCtrl_SongPool_h_


class SongPool : public SongToolCtrl {
	ArrayCtrl list;
	
public:
	typedef SongPool CLASSNAME;
	SongPool();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void StartProcess();
	void StopProcess();
	
};


#endif
