#ifndef _TextTool_Editor_h_
#define _TextTool_Editor_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor : public ToolEditorBase {
	
protected:
	friend class EntityInfoCtrl;
	friend class SnapInfoCtrl;
	friend class CompInfoCtrl;
	friend class ScriptInfoCtrl;
	friend class TextTool;
	
	Splitter			hsplit, menusplit, componentsplit;
	ArrayCtrl			appmode_list;
	ArrayCtrl			snaps, components;
	ArrayCtrl			scripts;
	bool				fast_exit = false;
	
	ImageGenTool					image_gen;
	EntityInfoCtrl					entity_info;
	SnapInfoCtrl					snap_info;
	SnapBriefing					snap_briefing;
	SnapIdeas						snap_ideas;
	SnapCoverImage					snap_cover;
	CompInfoCtrl					comp_info;
	VideoPromptMakerCtrl			video_prompt_maker;
	ScriptInfoCtrl					script_info;
	ScriptIdeaCtrl					script_idea;
	ScriptStructureSolverCtrl		script_struct_solver;
	ScriptRangeSolverCtrl			script_range_solver;
	ScriptTextSolverCtrl			script_text_solver;
	
	// Deprecated
	ComponentStructure				comp_struct;
	ScriptPool						script_pool;
	ScriptSolverCtrl				scripts_solver;
	ScriptEditCtrl					script_edit;
	ScriptPostFixCtrl				script_post_fix;
	
	
	// Database Ctrls
	SourceDataCtrl					src_data;
	SourceAnalysisCtrl				src_anal;
	TokensPage						tokens_data;
	TextDataWords					component_words;
	TokenPhrases					token_phrases;
	AmbiguousWordPairs				ambiguous_word_pairs;
	VirtualPhrases					virtual_phrases;
	VirtualPhraseParts				virtual_phrase_parts;
	VirtualPhraseStructs			virtual_phrase_structs;
	PhrasePartAnalysis				phrase_part_analysis;
	PhrasePartAnalysis2				phrase_part_analysis2;
	ActionAttrsPage					action_attrs;
	Attributes						db_attrs;
	TextDataDiagnostics				db_diagnostics;
	/*
	StructuredScriptCtrl			structured_script;
	TextDataPage					src_data;
	PhraseParts						phrase_parts;
	RhymeContainerPage				rhyme_container;
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	TextDataWordnet					component_wordnet;
	*/
public:
	typedef ToolEditor CLASSNAME;
	ToolEditor(TextTool* app);
	
	void InitAppModes(const Index<int>& appmodes);
	void Init();
	void SwitchAppMode();
	void ViewPageGroup() override;
	void Data() override;
	void OnDataSnapshot() override;
	void DataSnapshot();
	void DataComponent();
	void DataScript();
	//void DataPart();
	void MovePageGroup(int d);
	void MovePage(int d);
	void MovePart(int d);
	String GetStatusText();
	bool GetSaveSongdata() const {return save_data;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveSongdata(bool b) {save_data = b;}
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
