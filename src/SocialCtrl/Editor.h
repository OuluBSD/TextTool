#ifndef _SocialCtrl_Editor_h_
#define _SocialCtrl_Editor_h_


class SocialTool;
class ToolAppCtrl;

class SocialEditor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String item;
		ToolAppCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	void AddItem(String g, String i, ToolAppCtrl& c);
	void InitListItems();
	
protected:
	friend class SocialTool;
	
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
	
	
	ImageGenTool					image_gen;
	
public:
	typedef SocialEditor CLASSNAME;
	SocialEditor(SocialTool* app);
	
	void Serialize(Stream& s) {s % page_group % page % save_songdata;}
	void Init();
	void ViewPageGroup();
	void ViewPage();
	void Data();
	void DataPage();
	void DataArtist();
	void DataRelease();
	void DataSocial();
	void DataTypecast();
	void DataArchetype();
	void DataLyrics();
	void ToolMenu(Bar& bar);
	void SetView(int i, int j);
	void SetSubMenu(int i);
	void LoadLast();
	void StoreLast();
	void MovePageGroup(int d);
	void MovePage(int d);
	void MovePart(int d);
	String GetStatusText();
	bool GetSaveSocialdata() const {return save_songdata;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveSocialdata(bool b) {save_songdata = b;}
	void SetFastExit(bool b) {fast_exit = b;}
	
	void ArtistMenu(Bar& bar);
	void ReleaseMenu(Bar& bar);
	void SocialMenu(Bar& bar);
	void LyricsMenu(Bar& bar);
	
	void AddArtist();
	void RenameArtist();
	void RemoveArtist();
	void AddRelease();
	void RenameRelease();
	void RemoveRelease();
	void AddSocial();
	void RenameSocial();
	void RemoveSocial();
	void AddLyrics();
	void RemoveLyrics();
	
	void InitSimplified();
	
	Callback WhenStartUpdating, WhenStopUpdating;
	SocialTool& app;
	
};


#endif
