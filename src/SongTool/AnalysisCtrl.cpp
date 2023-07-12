#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	CtrlLayout(*this);
	data.AddColumn(t_("Key"));
	data.AddColumn(t_("Value"));
	data.ColumnWidths("1 4");
	
}

void AnalysisCtrl::Data() {
	Database& db = Database::Single();
	if (!db.active_part) return;
	Part& p = *db.active_part;
	Analysis& a = p.analysis;
	
	lyrics.SetData(Join(p.lines, "\n"));
	
	for(int i = 0; i < a.data.GetCount(); i++) {
		data.Set(i, 0, a.data.GetKey(i));
		data.Set(i, 1, a.data[i]);
	}
	data.SetCount(a.data.GetCount());
	
}
