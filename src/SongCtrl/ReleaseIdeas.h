#ifndef _SongTool_AlbumIdeas_h_
#define _SongTool_AlbumIdeas_h_


BEGIN_SONGLIB_NAMESPACE


class AlbumIdeas : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl list;
	WithAlbumIdeas<Ctrl> idea;
	
	
	
public:
	typedef AlbumIdeas CLASSNAME;
	AlbumIdeas();
	
	void Data();
	void Clear();
	void IdeaData();
	void OnListMenu(Bar& bar);
	void AddIdea();
	void RemoveIdea();
	void OnValueChange();
	
};


END_SONGLIB_NAMESPACE


#endif
