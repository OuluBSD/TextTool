#ifndef _SongDataCtrl_SongDataPage_h_
#define _SongDataCtrl_SongDataPage_h_


class SongDataPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, artists, songs;//, active_songs;
	DocEdit lyrics, analysis;
	ProgressIndicator prog;
	bool disabled = false;
	
public:
	typedef SongDataPage CLASSNAME;
	SongDataPage();
	
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataDataset();
	void DataArtist();
	//void DataArtistActiveSongs();
	void DataSong();
	//void DataActiveSong();
	void ToolMenu(Bar& bar) override;
	//void AddRandomSongsToList(int count);
	//void AddSongToActiveList();
	//void RemoveSongFromActiveList();
	//void HotfixText();
	void ImportLyrics();
	//void StartHotfixText() {if (disabled) return; DisableAll(); Thread::Start(THISBACK(HotfixText));}
	void PostProgress(int a, int t) {PostCallback(THISBACK2(SetProgress, a, t));}
	void SetProgress(int a, int t) {prog.Set(a,t);}
	
};

#endif
