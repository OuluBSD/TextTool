#ifndef _EditorCtrl_EnglishEditor_h_
#define _EditorCtrl_EnglishEditor_h_


class EnglishEditor : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl parts, lines, candidates;
	int ds_i = 0;
	
public:
	typedef EnglishEditor CLASSNAME;
	EnglishEditor();
	
	void Data() override;
	void PostData() {PostCallback(THISBACK(Data));}
	void DataPart();
	void ToolMenu(Bar& bar) override;
	void DoNana(int fn);
	
};


#endif
