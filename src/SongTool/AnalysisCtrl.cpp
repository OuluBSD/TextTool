#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	CtrlLayout(*this);
	data.AddColumn(t_("Key"));
	data.AddColumn(t_("Value"));
	data.ColumnWidths("1 4");
	
}

void AnalysisCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.part) return;
	Part& part = *p.part;
	PatternSnap& snap = part.snap[MALE];
	Analysis& a = part.analysis;
	
	lyrics.SetData(snap.txt);
	
	for(int i = 0; i < a.data.GetCount(); i++) {
		data.Set(i, 0, a.data.GetKey(i));
		data.Set(i, 1, a.data[i]);
	}
	data.SetCount(a.data.GetCount());
	
}
