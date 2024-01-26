#ifndef _EditorCtrl_LinePicker_h_
#define _EditorCtrl_LinePicker_h_


class LinePicker : public SongToolCtrl {
	WithLinePicker<Ctrl> ctrls;
	Splitter main_split, vsplit, hsplit;
	ArrayCtrl attrs, colors, actions, action_args, parts, picked;
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
	void DataPicked();
	void AddLine();
	void RemoveLine();
	
};


#endif
