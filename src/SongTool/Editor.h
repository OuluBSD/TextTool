#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_


class Editor : public Ctrl {
	Splitter	hsplit, menusplit;
	ArrayCtrl	tablist, artists, releases, songs;
	Ctrl		base;
	int			page = 0;
	
	RecruimentCtrl	recru;
	SocialCtrl		social;
	CalMgrCtrl		cal;
	TaskMgrCtrl		task;
	StoryCtrl		story;
	PatternMaskCtrl	patmask;
	PatternCtrl		pattern;
	CompositionCtrl	composition;
	AnalysisCtrl	analysis;
	ProductionCtrl	production;
	RhymesCtrl		rhymes;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	
	void Data();
	void DataArtist();
	void DataRelease();
	void DataSong();
	void DataPage();
	void SetView(int i);
	
	void ArtistMenu(Bar& bar);
	void ReleaseMenu(Bar& bar);
	void SongMenu(Bar& bar);
	
	void AddArtist();
	void RenameArtist();
	void RemoveArtist();
	void AddRelease();
	void RenameRelease();
	void RemoveRelease();
	void AddSong();
	void RenameSong();
	void RemoveSong();
};


#endif
