#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_

class SongTool;

class Editor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String group, item;
		SongToolCtrl* ctrl = 0;
	};
	Vector<ListItem> items;
	
	void AddItem(String g, String i, SongToolCtrl& c);
	void InitListItems();
	
protected:
	friend class BasicInfoCtrl;
	friend class SongTool;
	
	Splitter			hsplit, menusplit;
	ArrayCtrl			tablist, artists, releases, songs, parts;
	Ctrl				base;
	int					page = 0;
	
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
	ProductionIdea					prod_idea;
	PressReleaseCtrl				pressrel;
	PublicRelationsCtrl				pubrel;
	MarketingCtrl					marketing;
	StoryIdeaCtrl					story_idea;
	VisualIdeaCtrl					vocal_idea;
	
	
	
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
	
public:
	typedef Editor CLASSNAME;
	Editor(SongTool* app);
	
	void Serialize(Stream& s) {s % page;}
	void Init();
	void UpdateView();
	void Data();
	void DataArtist();
	void DataRelease();
	void DataSong();
	void DataPart();
	void DataPage();
	void ToolMenu(Bar& bar);
	void SetView(int i);
	void LoadLast();
	void StoreLast();
	void MoveTab(int d);
	void MovePart(int d);
	String GetStatusText();
	
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
