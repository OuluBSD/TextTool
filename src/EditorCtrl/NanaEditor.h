#ifndef _EditorCtrl_NanaEditor_h_
#define _EditorCtrl_NanaEditor_h_


class NanaEditor : public SongToolCtrl {
	Splitter hsplit;
	DocEdit code;
	ArrayCtrl parts, data;
	
public:
	typedef NanaEditor CLASSNAME;
	NanaEditor();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	void ParseCode();
	
};


#endif
