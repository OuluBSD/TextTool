#ifndef _TextTool_SnapIdeas_h_
#define _TextTool_SnapIdeas_h_


BEGIN_TEXTLIB_NAMESPACE


class SnapIdeas : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl list;
	WithSnapIdeas<Ctrl> idea;
	
	
	
public:
	typedef SnapIdeas CLASSNAME;
	SnapIdeas();
	
	void Data();
	void Clear();
	void IdeaData();
	void OnListMenu(Bar& bar);
	void AddIdea();
	void RemoveIdea();
	void OnValueChange();
	
};


END_TEXTLIB_NAMESPACE


#endif
