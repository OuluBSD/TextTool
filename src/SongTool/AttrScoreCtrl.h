#ifndef _SongTool_AttrScoreCtrl_h_
#define _SongTool_AttrScoreCtrl_h_


class AttrScoreCtrl : public WithAttrScore<Ctrl> {
	ArrayCtrl attrscores, entries;
	ArrayCtrl src_entrygroups, src_entries;
	DocEdit scoregroup_data;
	ColumnList part_errors;
	Splitter entrygroup_split;
	WithAttrWords<TopWindow> attrwords;
	
public:
	typedef AttrScoreCtrl CLASSNAME;
	AttrScoreCtrl();
	
	void SrcEntryMenu(Bar& b);
	void OpenPromptScores();
	void Data();
	void DataAttrList();
	void DataGroup();
	void DataAttrScore();
	void AddAttrScoreGroup();
	bool AddAttrScore();
	bool AddAttrScoreEntry(String group, String entry_str);
	void AddAttrScoreId(const SnapAttr& a);
	void AddSrcEntryToScoringGroup();
	void OnEntrySel();
	void CheckErrors();
	void AddAttrTask();
	
	
};


#endif
