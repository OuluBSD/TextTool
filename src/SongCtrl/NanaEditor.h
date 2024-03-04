#ifndef _SongCtrl_NanaEditor_h_
#define _SongCtrl_NanaEditor_h_

#if 0


class NanaEditor : public ToolAppCtrl {
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
#endif
