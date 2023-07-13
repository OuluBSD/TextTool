#ifndef _SongTool_Editor_h_
#define _SongTool_Editor_h_


class Editor : public Ctrl {
	
protected:
	friend class BasicInfoCtrl;
	Splitter	hsplit, menusplit;
	ArrayCtrl	tablist, artists, releases, songs, parts;
	Ctrl		base;
	int			page = 0;
	
	BasicInfoCtrl	info;
	AttrDataCtrl	attr;
	RecruimentCtrl	recru;
	SocialCtrl		social;
	CalMgrCtrl		cal;
	TaskMgrCtrl		task;
	ImportCtrl		importer;
	PatternMaskCtrl	patmask;
	PatternCtrl		pattern;
	CompositionCtrl	composition;
	AnalysisCtrl	analysis;
	ProductionCtrl	production;
	RhymesCtrl		rhymes;
	ScoringCtrl		scoring;
	AttrScoreCtrl	attrscore;
	ReverseCtrl		reverse;
	
public:
	typedef Editor CLASSNAME;
	Editor();
	
	void Serialize(Stream& s) {s % page;}
	void Init();
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
};


#endif
