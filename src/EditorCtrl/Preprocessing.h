#ifndef _EditorCtrl_Preprocessing_h_
#define _EditorCtrl_Preprocessing_h_


class Preprocessing : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl parts, lines, candidates;
	
public:
	typedef Preprocessing CLASSNAME;
	Preprocessing();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
