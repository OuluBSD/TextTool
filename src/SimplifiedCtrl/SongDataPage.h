#ifndef _SimplifiedCtrl_SongDataPage_h_
#define _SimplifiedCtrl_SongDataPage_h_


class SongDataPage : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, artists, songs;
	DocEdit lyrics;
	
public:
	typedef SongDataPage CLASSNAME;
	SongDataPage();
	
	
	void Data();
	void DataDataset();
	void DataArtist();
	void DataSong();
	
};

#endif
