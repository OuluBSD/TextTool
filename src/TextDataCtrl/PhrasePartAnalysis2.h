#ifndef _TextDataCtrl_PhrasePartAnalysis2_h_
#define _TextDataCtrl_PhrasePartAnalysis2_h_


BEGIN_TEXTLIB_NAMESPACE


class PhrasePartAnalysis2 : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl elements, typecasts, contrasts, parts,  colors;
	
public:
	typedef PhrasePartAnalysis2 CLASSNAME;
	PhrasePartAnalysis2();
	
	void Data() override;
	void DataMain();
	void DataElement();
	void DataTypeclass();
	void DataContrast();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void UpdateCounts();
	void ClearAll();
	
};


END_TEXTLIB_NAMESPACE


#endif
