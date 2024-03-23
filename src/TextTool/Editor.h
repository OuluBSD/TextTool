#ifndef _TextTool_Editor_h_
#define _TextTool_Editor_h_


BEGIN_TEXTLIB_NAMESPACE


class TextTool;

class ToolEditor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String item;
		ToolAppCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	void AddItem(String g, String i, ToolAppCtrl& c);
	void InitListItems();
	
protected:
	friend class EntityInfoCtrl;
	friend class SnapInfoCtrl;
	friend class CompInfoCtrl;
	friend class ScriptInfoCtrl;
	friend class TextTool;
	
	Ctrl				subsplit;
	Splitter			hsplit, menusplit, componentsplit, scriptssplit;
	ArrayCtrl			appmode_list, page_group_list, page_list, entities;
	ArrayCtrl			snaps, components, parts;
	ArrayCtrl			typecasts, contents, scripts;
	Ctrl				base;
	int					page_group = 0;
	VectorMap<int,int>	page;
	bool				save_songdata = false;
	bool				fast_exit = false;
	
	
	SnapBriefing					album_briefing;
	SnapIdeas						album_ideas;
	ImageGenTool					image_gen;
	SongStructure					song_struct;
	EntityInfoCtrl					company_info;
	SnapInfoCtrl					product_info;
	CompInfoCtrl					song_info;
	ScriptInfoCtrl					scripts_info;
	
	// Database Ctrls
	TextDataPage					src_data;
	TokensPage						tokens_data;
	TokenPhrases					token_phrases;
	AmbiguousWordPairs				ambiguous_word_pairs;
	VirtualPhrases					virtual_phrases;
	VirtualPhraseParts				virtual_phrase_parts;
	VirtualPhraseStructs			virtual_phrase_structs;
	PhraseParts						phrase_parts;
	PhrasePartAnalysis				phrase_part_analysis;
	PhrasePartAnalysis2				phrase_part_analysis2;
	RhymeContainerPage				rhyme_container;
	ActionAttrsPage					action_attrs;
	TextDataLineActions				song_lineactions;
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	TextDataWords					song_words;
	TextDataWordnet					song_wordnet;
	TextDataDiagnostics				db_diagnostics;
	Attributes						db_attrs;
	ScriptSolverCtrl				scripts_solver;
	ScriptPool						song_pool;
	
public:
	typedef ToolEditor CLASSNAME;
	ToolEditor(TextTool* app);
	
	void Serialize(Stream& s) {s % page_group % page % save_songdata;}
	void Init();
	void SwitchAppMode();
	void ViewPageGroup();
	void ViewPage();
	void Data();
	void DataPage();
	void DataEntity();
	void DataSnapshot();
	void DataComponent();
	void DataTypeclass();
	void DataContent();
	void DataScript();
	//void DataPart();
	void ToolMenu(Bar& bar);
	void SetView(int i, int j);
	void SetSubMenu(int i);
	void LoadLast();
	void StoreLast();
	void MovePageGroup(int d);
	void MovePage(int d);
	void MovePart(int d);
	String GetStatusText();
	bool GetSaveSongdata() const {return save_songdata;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveSongdata(bool b) {save_songdata = b;}
	void SetFastExit(bool b) {fast_exit = b;}
	
	void EntityMenu(Bar& bar);
	void SnapshotMenu(Bar& bar);
	void SongMenu(Bar& bar);
	void ScriptMenu(Bar& bar);
	
	void AddEntity();
	void RenameEntity();
	void RemoveEntity();
	void AddSnapshot();
	void RenameSnapshot();
	void RemoveSnapshot();
	void AddSong();
	void RenameSong();
	void RemoveSong();
	void AddScript();
	void RemoveScript();
	
	void InitSimplified();
	//void InitAdvanced();
	
	TextDatabase& GetDatabase();
	EditorPtrs& GetPointers();
	int GetAppMode() const;
	
	Callback WhenStartUpdating, WhenStopUpdating;
	TextTool& app;
	
};


END_TEXTLIB_NAMESPACE


#endif
