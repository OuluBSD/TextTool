#ifndef _SongTool_StructureCtrl_h_
#define _SongTool_StructureCtrl_h_

class StructureCtrl;


class StructureDrawer : public Ctrl {
	
	struct Item : Moveable<Item> {
		Part* part;
		Color clr;
		int len;
	};
	Vector<Item> items;
public:
	typedef StructureDrawer CLASSNAME;
	StructureDrawer();
	
	void Paint(Draw& d);
	
	
	StructureCtrl* ctrl = 0;
};


class StructureCtrl : public Ctrl {
	WithStructure<Ctrl>	import_str;
	Splitter			hsplit, vsplit0, vsplit1;
	ArrayCtrl			part_list, line_list, break_list;
	ArrayCtrl			list;
	StructureDrawer		drawer;
public:
	int					mode = MALE;
	
public:
	typedef StructureCtrl CLASSNAME;
	StructureCtrl();
	
	void Data();
	void DataSong();
	void DataPart();
	void DataLine();
	void DataBreak();
	void DataList();
	
	
};


#endif
