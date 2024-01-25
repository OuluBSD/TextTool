#ifndef _EditorCtrl_ActionEditor_h_
#define _EditorCtrl_ActionEditor_h_

#if 0


class ActionEditor : public SongToolCtrl {
	Splitter hsplit, vsplit, mainsplit, main_vsplit, sug_hsplit, sug_vsplit;
	ArrayCtrl parts, thrds, thrd_actions, actions, args, song_actions;
	ArrayCtrl colors, attrs;
	
	int def_thread_count = 8;
	
	int ds_i = 0;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
	enum {ANY, TRANSITION, PARALLEL, T_AND_P};
	struct Sug : Moveable<Sug> {
		int src = 0;
		int act = -1;
		int attr0 = -1;
		int attr1 = -1;
	};
	VectorMap<String, VectorMap<String, Sug>> suggestions;
	
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
#endif
