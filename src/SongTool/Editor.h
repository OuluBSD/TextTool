#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_


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
	TxtStructToText					structure_to_text;
	TxtEditNative					text_edit_native;
	TxtEditEnglish					text_edit_english;
	ChecklistProduction				checklist_production;
	MusicVideoImport				videostruct_import;
	MusicVideoEdit					videostruct_edit;
	MusicVideoToPlan				videostruct_to_plan;
	ChecklistMusicVideoProduction	video_checklist;
	ChecklistRelease				release_checklist;
	
	
	
	
	// Advanced
	BasicInfoCtrl		info;
	AttrDataCtrl		attr;
	RecruimentCtrl		recru;
	SocialCtrl			social;
	CalMgrCtrl			cal;
	TaskMgrCtrl			task;
	ImportCtrl			importer;
	PatternMaskCtrl		patmask;
	PatternView			pattern, rev_pattern;
	CompositionCtrl		composition;
	AnalysisCtrl		analysis;
	ProductionCtrl		production;
	RhymesCtrl			rhymes;
	PatternScoringCtrl	scoring;
	AttrScoreCtrl		attrscore;
	ReverseImpactCtrl	reverse_impact;
	ReverseCtrl			reverse[3];
	ImpactCtrl			impact;
	StoryCtrl			story;
	StructureCtrl		structure;
	ImpactScoringCtrl	impact_scoring;
	MaskScoringCtrl		mask_scoring;
	LyricsCtrl			lyrics;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	
	void Serialize(Stream& s) {s % page;}
	void Init();
	void PostInit() {PostCallback(THISBACK(Init));}
	void UpdateView();
	void Data();
	void DataArtist();
	void DataRelease();
	void DataSong();
	void DataPart();
	void DataPage();
	void SetView(int i);
	
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
	
};


#endif
