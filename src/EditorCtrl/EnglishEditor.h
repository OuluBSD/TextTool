#ifndef _EditorCtrl_EnglishEditor_h_
#define _EditorCtrl_EnglishEditor_h_


class EnglishEditor : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl parts, lines, candidates;
	
public:
	typedef EnglishEditor CLASSNAME;
	EnglishEditor();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
