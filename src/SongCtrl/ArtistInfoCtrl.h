#ifndef _SongTool_ArtistInfoCtrl_h_
#define _SongTool_ArtistInfoCtrl_h_

class Editor;

class ArtistInfoCtrl : public WithCompanyInfo<ToolAppCtrl> {
	
	
public:
	typedef ArtistInfoCtrl CLASSNAME;
	ArtistInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
