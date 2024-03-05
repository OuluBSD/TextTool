#ifndef _SongTool_ArtistInfoCtrl_h_
#define _SongTool_ArtistInfoCtrl_h_


BEGIN_SONGLIB_NAMESPACE


class SongEditor;

class ArtistInfoCtrl : public WithCompanyInfo<ToolAppCtrl> {
	
	
public:
	typedef ArtistInfoCtrl CLASSNAME;
	ArtistInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SongEditor* editor = 0;
	
};


END_SONGLIB_NAMESPACE


#endif
