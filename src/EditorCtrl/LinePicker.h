#ifndef _EditorCtrl_LinePicker_h_
#define _EditorCtrl_LinePicker_h_


class LinePicker : public SongToolCtrl {
	WithLinePicker<Ctrl> ctrls;
	Splitter vsplit, hsplit;
	ArrayCtrl attrs, colors, actions, action_args, parts;
	int ds_i = 0;
	
public:
	typedef LinePicker CLASSNAME;
	LinePicker();
	
	void Data() override;
	void DataMain();
	void DataAttribute();
	void DataColor();
	void DataAction();
	void DataActionHeader();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	
};


#endif
