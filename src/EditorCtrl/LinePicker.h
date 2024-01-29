#ifndef _EditorCtrl_LinePicker_h_
#define _EditorCtrl_LinePicker_h_


class LinePicker : public SongToolCtrl {
	WithLinePicker<Ctrl> ctrls;
	Splitter main_split, vsplit, hsplit;
	ArrayCtrl attrs, colors, actions, action_args, parts, picked, subpicked;
	int ds_i = 0;
	int mode = 0;
	
	enum {
		MODE_PICKED,
		MODE_SUBPICKED,
	};
	
public:
	typedef LinePicker CLASSNAME;
	LinePicker();
	
	void SetBrowserMode(int i);
	void Data() override;
	void DataMain(int cur);
	void DataPhrases();
	void ToolMenu(Bar& bar) override;
	void DoPhrases(int fn);
	void DataPicked();
	void DataSubPicked();
	void AddLine();
	void RemoveLine();
	void ClearAll();
	void SetView(int i);
	void ToggleView() {SetView(!mode);}
	
};


#endif
