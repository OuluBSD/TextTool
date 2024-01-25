#ifndef _EditorCtrl_LineSetter_h_
#define _EditorCtrl_LineSetter_h_


class LineSetter : public SongToolCtrl {
	
public:
	typedef LineSetter CLASSNAME;
	LineSetter();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
