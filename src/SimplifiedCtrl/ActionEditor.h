#ifndef _SimplifiedCtrl_ActionEditor_h_
#define _SimplifiedCtrl_ActionEditor_h_


class ActionEditor : public SongToolCtrl {
	Splitter hsplit, vsplit, mainsplit, main_vsplit;
	ArrayCtrl parts, thrds, thrd_actions, thrd_suggestions, song_actions;
	
public:
	typedef ActionEditor CLASSNAME;
	ActionEditor();
	
	void Data() override;
	void DataPart();
	void DataThread();
	void DataSong();
	void ToolMenu(Bar& bar) override;
	
};


#endif
