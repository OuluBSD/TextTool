#ifndef _EditorCtrl_LinePicker_h_
#define _EditorCtrl_LinePicker_h_


class LinePicker : public SongToolCtrl {
	
public:
	typedef LinePicker CLASSNAME;
	LinePicker();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
