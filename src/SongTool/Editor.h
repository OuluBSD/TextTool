#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_

class SongTool;

class Editor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String item;
		SongToolCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	void AddItem(String g, String i, SongToolCtrl& c);
	void InitListItems();
	
protected:
	friend class BasicInfoCtrl;
	friend class SongTool;
	
	Splitter			hsplit, menusplit;
	ArrayCtrl			page_group_list, page_list, artists, releases, songs, parts;
	Ctrl				base;
	int					page_group = 0;
	VectorMap<int,int>	page;
	bool				save_songdata = false;
	bool				fast_exit = false;
	
	// Simplified
	AlbumBriefing					album_briefing;
	AlbumIdeas						album_ideas;
	CoverImage						cover_image;
	Pitching						pitching;
	SongBriefing					song_briefing;
	ChecklistComposition			checklist_composition;
	TxtStructTranslate				structure_native_english;
	TxtStructFromEnglish			structure_from_english;
	TxtStructEdit					structure_edit;
	TxtEditNative					text_edit_native;
	TxtEditEnglish					text_edit_english;
	TxtAutoCompare					text_autocompare_eng;
	TxtSerialCompare				serial_compare_eng;
	ChecklistProduction				checklist_production;
	MusicVideoImport				videostruct_import;
	MusicVideoEdit					videostruct_edit;
	MusicVideoToPlan				videostruct_to_plan;
	ChecklistMusicVideoProduction	video_checklist;
	ChecklistRelease				release_checklist;
	ImageGenTool					image_gen;
	ContextAttributesCtrl			ctx_attrs;
	//ProductionIdeaPage				prod_idea;
	PressReleaseCtrl				pressrel;
	PublicRelationsCtrl				pubrel;
	MarketingCtrl					marketing;
	StoryIdeaCtrl					story_idea;
	VisualIdeaCtrl					vocal_idea;
	SongDataPage					song_data;
	SongDataAnalysisPage			song_analysis;
	//RhymeStructureCtrl				rhyme_struct;
	SongDataWords					song_words;
	AttributeIdea					attr_idea;
	AttributeDistribution			attr_dist;
	SongDataTemplates				song_tmpl;
	RhymeContainerPage				rhyme_container;
	ActionAttrsPage					action_attrs;
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	LineTypesPage					line_types;
	
	
	
	// Advanced
	BasicInfoCtrl			info;
	AttrDataCtrl			attr;
	RecruimentCtrl			recru;
	SocialCtrl				social;
	CalMgrCtrl				cal;
	TaskMgrCtrl				task;
	ImportCtrl				importer;
	PatternMaskCtrl			patmask;
	PatternView				pattern, rev_pattern;
	CompositionCtrl			composition;
	AnalysisCtrl			analysis;
	ProductionCtrl			production;
	RhymesCtrl				rhymes;
	PatternScoringCtrl		scoring;
	AttrScoreCtrl			attrscore;
	ReverseImpactCtrl		reverse_impact;
	ReverseCtrl				reverse[3];
	ImpactCtrl				impact;
	StoryCtrl				story;
	StructureCtrl			structure;
	ImpactScoringCtrl		impact_scoring;
	MaskScoringCtrl			mask_scoring;
	LyricsCtrl				lyrics;
	SocialMediaChecklist	some_chk;
	SongStructure			song_struct;
	AutoIdeas				auto_ideas;
	PartIdea				part_idea;
	DialogueIdeaCtrl		dialogue_idea;
	ColorIdeaCtrl			color_idea;
	VocabularyIdeaCtrl		vocab_idea;
	WordSaladIdeaCtrl		wordsalad_idea;
	TextIdeaCtrl			text_idea;
	//VocabularyCtrl			vocabulary;
	//FineStructureCtrl		fine_struct;
	ContextIdeaCtrl			ctx_idea;
	SongDataPhrases			song_phrases;
	//SongDataSearchPhrases	song_searchphrases;
	LyricsEditor			lyrics_editor;
	SongDataWordnet			song_wordnet;
	SongDataTemplateScoring	song_tmplscoring;
	SongDataWordnetScoring	song_wnscoring;
	SongDataLineActions		song_lineactions;
	SongDataTmplActions		song_tmplactions;
	NanaEditor				nana_editor;
	ActionEditor			action_editor;
	
public:
	typedef Editor CLASSNAME;
	Editor(SongTool* app);
	
	void Serialize(Stream& s) {s % page_group % page % save_songdata;}
	void Init();
	void ViewPageGroup();
	void ViewPage();
	void Data();
	void DataPage();
	void DataArtist();
	void DataRelease();
	void DataSong();
	//void DataPart();
	void ToolMenu(Bar& bar);
	void SetView(int i, int j);
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
	
	void ArtistMenu(Bar& bar);
	void ReleaseMenu(Bar& bar);
	void SongMenu(Bar& bar);
	void PartMenu(Bar& bar);
	
	void AddArtist();
	void RenameArtist();
	void RemoveArtist();
	void AddRelease();
	void RenameRelease();
	void RemoveRelease();
	void AddSong();
	void RenameSong();
	void RemoveSong();
	
	void InitSimplified();
	void InitAdvanced();
	
	Callback WhenStartUpdating, WhenStopUpdating;
	SongTool& app;
	
};


#endif
