#ifndef _SongTool_ChecklistComposition_h_
#define _SongTool_ChecklistComposition_h_


class ChecklistBase : public SongToolCtrl {
	ArrayCtrl list;
	Vector<Option*> opts;
	Vector<EditString*> edits;
	
	void Init();
	
protected:
	Vector<String> descs;
	Vector<String> keys;
	
public:
	typedef ChecklistBase CLASSNAME;
	ChecklistBase();
	
	void Data();
	void OnOptionChange(int i);
	void OnValueChange(int i);
	virtual void InitKeys() = 0;
	
};

#endif
