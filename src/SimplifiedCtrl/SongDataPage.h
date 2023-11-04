#ifndef _SimplifiedCtrl_SongDataPage_h_
#define _SimplifiedCtrl_SongDataPage_h_


class SongDataPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, artists, songs, active_songs;
	DocEdit lyrics, analysis;
	
public:
	typedef SongDataPage CLASSNAME;
	SongDataPage();
	
	
	void Data() override;
	void DataDataset();
	void DataArtist();
	void DataArtistActiveSongs();
	void DataSong();
	void DataActiveSong();
	void ToolMenu(Bar& bar) override;
	void AddRandomSongsToList(int count);
	void AddSongToActiveList();
	void RemoveSongFromActiveList();
	
};

#endif
