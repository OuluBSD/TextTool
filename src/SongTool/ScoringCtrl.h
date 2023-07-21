#ifndef _SongTool_ScoringCtrl_h_
#define _SongTool_ScoringCtrl_h_


class ScoringCtrl : public Ctrl {
	Plotter plotter[GENDER_COUNT];
	ArrayCtrl list[GENDER_COUNT];
	//ArrayCtrl presets;
	Splitter vsplit[GENDER_COUNT], mainsplit;
	
	
	static const int group_begin = 4;
	
	void ListMenu(Bar& bar);
	void PresetMenu(Bar& bar);
public:
	typedef ScoringCtrl CLASSNAME;
	ScoringCtrl();
	
	void Data();
	void DataListAll() {for (int mode = 0; mode < GENDER_COUNT; mode++) DataList(mode);}
	void DataList(int mode);
	//void DataPresets();
	void AddPreset();
	/*void SavePreset();
	void UpdatePreset();
	void ApplyPreset();
	void RemovePreset();*/
	void ListValueChanged(int mode, int pos, int scoring);
	
};


#endif
