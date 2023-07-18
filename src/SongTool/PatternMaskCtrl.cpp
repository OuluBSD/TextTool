#include "SongTool.h"


PatternMaskCtrl::PatternMaskCtrl() {
	CtrlLayout(*this);
	data.AddColumn(t_("Key"));
	data.AddColumn(t_("Value"));
	data.ColumnWidths("1 4");
	
}

void PatternMaskCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.part) return;
	Attributes& a = db.attrs;
	Part& part = *p.part;
	PatternSnap& snap = part.snap[MALE];
	
	lyrics.SetData(snap.txt);
	
	for(int i = 0; i < snap.mask.GetCount(); i++) {
		const SnapAttrStr& sa = snap.mask[i];
		#if 0
		const Attributes::Group& gg = a.groups[sa.group];
		const String& item = gg.values[sa.item];
		#if 0
		data.Set(i, 0, Capitalize(gg.description));
		data.Set(i, 1, Capitalize(item));
		#else
		data.Set(i, 0, gg.description);
		data.Set(i, 1, item);
		#endif
		#endif
		data.Set(i, 0, sa.group);
		data.Set(i, 1, sa.item);
	}
	data.SetCount(snap.mask.GetCount());
	
}
