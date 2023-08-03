#ifndef _SongTool_PatternScoringCtrl_h_
#define _SongTool_PatternScoringCtrl_h_


class PatternScoringCtrl : public Ctrl {
	PArr<Plotter> plotter;
	PArr<ArrayCtrl> list;
	PArr<Splitter> vsplit;
	Splitter mainsplit;
	
	
	
	void ListMenu(Bar& bar);
	void PresetMenu(Bar& bar);
public:
	typedef PatternScoringCtrl CLASSNAME;
	PatternScoringCtrl();
	
	void Data();
	void DataListAll() {for (const SnapArg& a : GenderArgs()) DataList(a);}
	void DataList(const SnapArg& a);
	//void DataPresets();
	//void AddPreset();
	/*void SavePreset();
	void UpdatePreset();
	void ApplyPreset();
	void RemovePreset();*/
	void ListValueChanged(const SnapArg& a, int pos, int scoring);
	
	
	static const int group_begin = 4;
	
};


#endif
