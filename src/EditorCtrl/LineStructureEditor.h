#ifndef _EditorCtrl_LineStructureEditor_h_
#define _EditorCtrl_LineStructureEditor_h_


class LineStructureEditor : public SongToolCtrl {
	Splitter hsplit, vsplit;
	DocEdit code;
	ArrayCtrl parts, lines, line_options, phrase_parts, phrase_structs;
	
public:
	typedef LineStructureEditor CLASSNAME;
	LineStructureEditor();
	
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	
};


#endif
