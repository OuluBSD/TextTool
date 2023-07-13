#ifndef _SongTool_ScoringCtrl_h_
#define _SongTool_ScoringCtrl_h_


class ScoringCtrl : public Ctrl {
	Plotter plotter;
	ArrayCtrl list;
	ArrayCtrl presets;
	Splitter mainsplit;
	
	
	static const int group_begin = 3;
	
	void ListMenu(Bar& bar);
	void PresetMenu(Bar& bar);
public:
	typedef ScoringCtrl CLASSNAME;
	ScoringCtrl();
	
	void Data();
	void DataList();
	void DataPresets();
	void AddPreset();
	void SavePreset();
	void UpdatePreset();
	void ApplyPreset();
	void RemovePreset();
	void ListValueChanged(int pos, int scoring);
	void GetScores(const PatternSnap& snap, Vector<int>& scores);
	
};


#endif
