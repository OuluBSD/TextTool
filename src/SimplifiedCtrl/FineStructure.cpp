#include "SimplifiedCtrl.h"



FineStructureCtrl::FineStructureCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << rhymes << props;
	list.AddColumn(t_("Name"));
	list.AddColumn(t_("Rhyme-count"));
	list.AddColumn(t_("Chords"));
	LineListCtrl::Init();
	list.ColumnWidths("1 2 1 5");
	list.WhenCursor << THISBACK(DataRhyme);
	
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
		rc.MinMax(0, 100);
		rc.SetData(part.rhymes.GetCount());
		
		rc.WhenAction << THISBACK2(OnRhymeChange, &rc, &part);
	}
	list.SetCount(s.parts.GetCount());
	
	DataRhyme();
	DataRhymeParams();
}

void FineStructureCtrl::DataRhyme() {
	Song& s = GetSong();
	
	if (!list.IsCursor()) {
		rhymes.Clear();
		props.Clear();
		return;
	}
	
	int part_i = list.Get("PART_IDX");
	StaticPart& part = s.parts[part_i];
	
	for(int i = 0; i < part.rhymes.GetCount(); i++) {
		StaticRhyme& r = part.rhymes[i];
		
		DropList& dl = rhymes.CreateCtrl<DropList>(i, 0);
		dl.Add(t_("Singing"));
		dl.Add(t_("Rapping"));
		dl.Add(t_("Poetry"));
		dl.Add(t_("Dialog"));
		dl.Add(t_("Skip"));
		/*int idx = max(0, min(dl.GetCount()-1, s.part_types[i]));
		dl.SetIndex(idx);
		dl.WhenAction << [&song,i,&dl]() {song.active_struct.part_types[i] = dl.GetIndex();};
		*/
	}
	rhymes.SetCount(part.rhymes.GetCount());
	
}

void FineStructureCtrl::DataRhymeParams() {
	Song& s = GetSong();
	
	if (!list.IsCursor() || !rhymes.IsCursor()) {
		props.Clear();
		return;
	}
	
	int part_i = list.Get("PART_IDX");
	int rhyme_i = rhymes.GetCursor();
	StaticPart& part = s.parts[part_i];
	StaticRhyme& rhyme = part.rhymes[rhyme_i];
	
	int row = 0;
	{
		rhymes.Set(row, 0, t_("Frozen word/syllable"));
		auto& ctrl = rhymes.CreateCtrl<EditString>(row, 1);
	}
	{
		rhymes.Set(row, 0, t_("Only first syllable"));
		auto& ctrl = rhymes.CreateCtrl<Option>(row, 1);
	}
	{
		rhymes.Set(row, 0, t_("Must be a long and singable syllable"));
		auto& ctrl = rhymes.CreateCtrl<Option>(row, 1);
	}
	
}

void FineStructureCtrl::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

void FineStructureCtrl::OnRhymeChange(EditIntSpin* rc, StaticPart* part) {
	int count = rc->GetData();
	part->rhymes.SetCount(count);
	PostCallback(THISBACK(DataRhyme));
}
