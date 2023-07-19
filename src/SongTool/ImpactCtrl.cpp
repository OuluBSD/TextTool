#include "SongTool.h"


ImpactCtrl::ImpactCtrl() {
	Add(list.SizePos());
	
	list.AddColumn(t_("Position"));
	list.AddColumn(t_("Male Text"));
	list.AddColumn(t_("Male Impact"));
	list.AddColumn(t_("Female Text"));
	list.AddColumn(t_("Female Impact"));
	list.ColumnWidths("1 2 5 2 5");
	list.SetLineCy(list.GetLineCy() * 1.2);
	
}

void ImpactCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& song = *p.song;
	int mode = p.mode;
	
	Index<int> story_values;
	
	
	
}
