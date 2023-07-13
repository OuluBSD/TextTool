#include "SongTool.h"

AttrDataCtrl::AttrDataCtrl() {
	Add(split.SizePos());
	
	split.Horz() << groups << items;
	
	groups.AddColumn(t_("Type"));
	groups.AddColumn(t_("Group description"));
	//groups.ColumnWidths("1 3");
	groups <<= THISBACK(DataGroup);
	
	items.AddColumn(t_("Key"));
	//items.AddColumn(t_("Value"));
	//items.ColumnWidths("1 3");
	
}

void AttrDataCtrl::Data() {
	Database& db = Database::Single();
	
	for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		Attributes::Group& gg = db.attrs.groups[i];
		groups.Set(i, 0, gg.type);
		groups.Set(i, 1, gg.description);
	}
	groups.SetCount(db.attrs.groups.GetCount());
	
	if (!groups.IsCursor() && groups.GetCount())
		groups.SetCursor(0);
	
	DataGroup();
}

void AttrDataCtrl::DataGroup() {
	Database& db = Database::Single();
	
	if (!groups.IsCursor())
		return;
	
	int cursor = groups.GetCursor();
	Attributes::Group& gg = db.attrs.groups[cursor];
	
	for(int i = 0; i < gg.values.GetCount(); i++) {
		items.Set(i, 0, gg.values[i]);
	}
	items.SetCount(gg.values.GetCount());
}
