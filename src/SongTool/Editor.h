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
	
	
	AlbumBriefing					album_briefing;
	AlbumIdeas						album_ideas;
	CoverImage						cover_image;
	Pitching						pitching;
	SongBriefing					song_briefing;
	ChecklistComposition			checklist_composition;
	ChecklistProduction				checklist_production;
	ChecklistMusicVideoProduction	video_checklist;
	ChecklistRelease				release_checklist;
	ImageGenTool					image_gen;
	ContextAttributesCtrl			ctx_attrs;
	PressReleaseCtrl				pressrel;
	PublicRelationsCtrl				pubrel;
	MarketingCtrl					marketing;
	SongDataPage					song_data;
	SongDataAnalysisPage			song_analysis;
	SongDataWords					song_words;
	AttributeDistribution			attr_dist;
	SongDataTemplates				song_tmpl;
	RhymeContainerPage				rhyme_container;
	ActionAttrsPage					action_attrs;
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	LineTypesPage					line_types;
	BasicInfoCtrl					info;
	CalMgrCtrl						cal;
	SongStructure					song_struct;
	SongDataPhrases					song_phrases;
	LyricsEditor					lyrics_editor;
	SongDataWordnet					song_wordnet;
	SongDataTemplateScoring			song_tmplscoring;
	SongDataWordnetScoring			song_wnscoring;
	SongDataLineActions				song_lineactions;
	SongDataTmplActions				song_tmplactions;
	NanaEditor						nana_editor;
	ActionEditor					action_editor;
	ExportPage						export_page;
	
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
	//void InitAdvanced();
	
	Callback WhenStartUpdating, WhenStopUpdating;
	SongTool& app;
	
};


#endif
