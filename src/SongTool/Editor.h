#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_

class SongTool;

class Editor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String item;
		ToolAppCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	void AddItem(String g, String i, ToolAppCtrl& c);
	void InitListItems();
	
protected:
	friend class ArtistInfoCtrl;
	friend class ReleaseInfoCtrl;
	friend class SongInfoCtrl;
	friend class LyricsInfoCtrl;
	friend class SongTool;
	
	Ctrl				subsplit;
	Splitter			hsplit, menusplit, songsplit, lyricssplit;
	ArrayCtrl			page_group_list, page_list, artists;
	ArrayCtrl			releases, songs, parts;
	ArrayCtrl			typecasts, archetypes, lyrics;
	Ctrl				base;
	int					page_group = 0;
	VectorMap<int,int>	page;
	bool				save_songdata = false;
	bool				fast_exit = false;
	
	
	AlbumBriefing					album_briefing;
	AlbumIdeas						album_ideas;
	CoverImage						cover_image;
	//Pitching						pitching;
	//SongBriefing					song_briefing;
	/*ChecklistComposition			checklist_composition;
	ChecklistProduction				checklist_production;
	ChecklistMusicVideoProduction	video_checklist;
	ChecklistRelease				release_checklist;*/
	ImageGenTool					image_gen;
	//ContextAttributesCtrl			ctx_attrs;
	PressReleaseCtrl				pressrel;
	PublicRelationsCtrl				pubrel;
	MarketingCtrl					marketing;
	//AttributeDistribution			attr_dist;
	CalMgrCtrl						cal;
	SongStructure					song_struct;
	//LyricsEditor					lyrics_editor;
	//ActionEditor					action_editor;
	ArtistInfoCtrl					company_info;
	ReleaseInfoCtrl					product_info;
	SongInfoCtrl					song_info;
	LyricsInfoCtrl					lyrics_info;
	
	// Database Ctrls
	SongDataPage					song_data;
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
	SongDataLineActions				song_lineactions;
	ActionParallelsPage				action_parallels;
	ActionTransitionsPage			action_transitions;
	SongDataWords					song_words;
	SongDataWordnet					song_wordnet;
	SongDataDiagnostics				db_diagnostics;
	//LineStructureEditor				line_struct;
	//LinePicker						line_picker;
	//LineSetter						line_setter;
	//NativeEditor					nat_editor;
	//NanaEditor						nana_editor;
	//EnglishEditor					eng_edit;
	Attributes						db_attrs;
	LyricsSolverCtrl				lyrics_solver;
	SongPool						song_pool;
	//SecondarySetter					sec_setter;
	
	/*
	SongDataAnalysisPage			song_analysis;
	SongDataTemplates				song_tmpl;
	LineTypesPage					line_types;
	SongDataPhrases					song_phrases;
	SongDataTemplateScoring			song_tmplscoring;
	SongDataWordnetScoring			song_wnscoring;
	SongDataTmplActions				song_tmplactions;
	ExportPage						export_page;
	*/
	
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
	void DataTypecast();
	void DataArchetype();
	void DataLyrics();
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
	
	void ArtistMenu(Bar& bar);
	void ReleaseMenu(Bar& bar);
	void SongMenu(Bar& bar);
	void LyricsMenu(Bar& bar);
	
	void AddArtist();
	void RenameArtist();
	void RemoveArtist();
	void AddRelease();
	void RenameRelease();
	void RemoveRelease();
	void AddSong();
	void RenameSong();
	void RemoveSong();
	void AddLyrics();
	void RemoveLyrics();
	
	void InitSimplified();
	//void InitAdvanced();
	
	Callback WhenStartUpdating, WhenStopUpdating;
	SongTool& app;
	
};


#endif
