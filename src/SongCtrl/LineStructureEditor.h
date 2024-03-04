#ifndef _SongCtrl_LineStructureEditor_h_
#define _SongCtrl_LineStructureEditor_h_

#if 0


class LineStructureEditor : public SongToolCtrl {
	struct PartSug : Moveable<PartSug> {
		static const int EX_COUNT = 2;
		String examples[EX_COUNT];
	};
	
	struct StructSug : Moveable<StructSug> {
		static const int EX_COUNT = 2;
		String category, parts;
		String examples[EX_COUNT];
	};
	
	Splitter hsplit, vsplit;
	DocEdit code;
	ArrayCtrl parts, lines, line_options, phrase_parts, phrase_structs;
	int ds_i = 0;
	
	VectorMap<String, PartSug> part_sugs;
	VectorMap<String, StructSug> struct_sugs;
	
	void UpdateSuggestions();
	
public:
	typedef LineStructureEditor CLASSNAME;
	LineStructureEditor();
	
	void Data() override;
	void DataPart();
	void DataSuggestions();
	void ToolMenu(Bar& bar) override;
	
};


#endif
#endif
