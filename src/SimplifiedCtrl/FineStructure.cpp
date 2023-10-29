#include "SimplifiedCtrl.h"



FineStructureCtrl::FineStructureCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << rhymes << props;
	list.AddColumn(t_("Name"));
	list.AddColumn(t_("Rhyme-count"));
	list.AddColumn(t_("Chords"));
	LineListCtrl::Init();
	list.ColumnWidths("1 2 1 5");
	
	rhymes.AddColumn(t_("Type"));
	rhymes.AddColumn(t_("Beats"));
	rhymes.AddColumn(t_("Beat selector"));
	rhymes.ColumnWidths("2 1 6");
	
	props.AddColumn(t_("Key"));
	props.AddColumn(t_("Value"));
	
}

void FineStructureCtrl::DisableAll() {
	
}

void FineStructureCtrl::EnableAll() {
	
}

void FineStructureCtrl::Data() {
	Song& s = GetSong();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		
		list.Set(i, "PART_IDX", i);
		list.Set(i, "LINE_IDX", -1);
		
		Color clr = GetSongPartPaperColor(part.type);
		list.Set(i, 0, AttrText(part.name).NormalPaper(clr));
		
		EditIntSpin& rc = list.CreateCtrl<EditIntSpin>(i, 1);
		rc.SetData(part.rhymes.GetCount());
	}
	list.SetCount(s.parts.GetCount());
	
	DataRhyme();
	DataRhymeParams();
}

void FineStructureCtrl::DataRhyme() {
	
}

void FineStructureCtrl::DataRhymeParams() {
	
}

void FineStructureCtrl::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

