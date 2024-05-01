#ifndef _TextTool_Editor_h_
#define _TextTool_Editor_h_


BEGIN_TEXTLIB_NAMESPACE


class TextTool;

class ToolEditorBase : public Ctrl {
	const char* title = 0;
	
protected:
	struct ListItem : Moveable<ListItem> {
		String item;
		ToolAppCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	
	TextTool&			app;
	ArrayCtrl			parts, page_group_list, page_list;
	ArrayCtrl			owners;
	ArrayCtrl			profiles;
	int					page_group = 0;
	VectorMap<int,int>	page;
	
	void AddItem(String g, String i, ToolAppCtrl& c);
	
public:
	Ctrl				base;
	Ctrl				subsplit;
	bool				filter_profile_language = false;
	
public:
	typedef ToolEditorBase CLASSNAME;
	ToolEditorBase(const char* title, TextTool& app);
	void UpdatePageList();
	void ViewPage();
	void Init();
	void SetView(int i, int j);
	void ToolMenu(Bar& bar);
	void OwnerMenu(Bar& bar);
	void ProfileMenu(Bar& bar);
	void DataMeta();
	void DataOwner();
	void DataProfile();
	void AddOwner();
	void RemoveOwner();
	void AddProfile();
	void RemoveProfile();
	virtual void Data() {}
	virtual void ViewPageGroup();
	virtual void DataPage();
	virtual void OnDataProfile() {}
	virtual void SetSubMenu(int i) = 0;
	
	Callback WhenStartUpdating, WhenStopUpdating;
	
};

class ToolEditor : public ToolEditorBase {
	
protected:
	friend class EntityInfoCtrl;
	friend class SnapInfoCtrl;
	friend class CompInfoCtrl;
	friend class ScriptInfoCtrl;
	friend class TextTool;
	
	Splitter			hsplit, menusplit, componentsplit, scriptssplit;
	ArrayCtrl			appmode_list;
	ArrayCtrl			snaps, components;
	ArrayCtrl			typeclasses, contents, scripts;
	bool				save_songdata = false;
	bool				fast_exit = false;
	
	
	ImageGenTool					image_gen;
	SnapBriefing					snap_briefing;
	SnapIdeas						snap_ideas;
	ComponentStructure				comp_struct;
	EntityInfoCtrl					entity_info;
	SnapInfoCtrl					snap_info;
	CompInfoCtrl					comp_info;
	ScriptInfoCtrl					script_info;
	
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
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	TextDataWords					component_words;
	TextDataWordnet					component_wordnet;
	TextDataDiagnostics				db_diagnostics;
	Attributes						db_attrs;
	ScriptSolverCtrl				scripts_solver;
	ScriptPool						script_pool;
	
public:
	typedef ToolEditor CLASSNAME;
	ToolEditor(TextTool* app);
	
	void Serialize(Stream& s) override {s % page_group % page % save_songdata;}
	void InitAppModes(const Index<int>& appmodes);
	void Init();
	void SwitchAppMode();
	void ViewPageGroup() override;
	void Data() override;
	void OnDataProfile() override;
	void DataSnapshot();
	void DataComponent();
	void DataTypeclass();
	void DataContent();
	void DataScript();
	//void DataPart();
	void StoreLast();
	void MovePageGroup(int d);
	void MovePage(int d);
	void MovePart(int d);
	String GetStatusText();
	bool GetSaveSongdata() const {return save_songdata;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveSongdata(bool b) {save_songdata = b;}
	void SetFastExit(bool b) {fast_exit = b;}
	void LoadLast();
	void SetSubMenu(int i) override;
	void DataPage() override;
	
	//void EntityMenu(Bar& bar);
	void SnapshotMenu(Bar& bar);
	void SongMenu(Bar& bar);
	void ScriptMenu(Bar& bar);
	
	//void AddEntity();
	//void RenameEntity();
	//void RemoveEntity();
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
	
};


END_TEXTLIB_NAMESPACE


#endif
