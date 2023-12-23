#ifndef _SimplifiedCtrl_ActionEditor_h_
#define _SimplifiedCtrl_ActionEditor_h_


class ActionEditor : public SongToolCtrl {
	Splitter hsplit, vsplit, mainsplit, main_vsplit, sug_hsplit, sug_vsplit;
	ArrayCtrl parts, thrds, thrd_actions, actions, args, song_actions;
	ArrayCtrl colors, attrs;
	
	int def_thread_count = 8;
	
	bool mode_all_args = false;
	int ds_i = 0;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
public:
	typedef ActionEditor CLASSNAME;
	ActionEditor();
	
	void Data() override;
	void DataPart();
	void DataThread();
	void DataSong();
	void DataSuggestions();
	void DataAction();
	void ToolMenu(Bar& bar) override;
	void AddAction();
	void RemoveAction();
	
};


#endif
