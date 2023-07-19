#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	Add(hsplit.SizePos());
	hsplit.Horz();
	
	for(int i = 0; i < 2; i++) {
		ArrayCtrl& data = this->data[i];
		DocEdit& edit = this->edit[i];
		DocEdit& full = this->full[i];
		Splitter& vsplit = this->vsplit[i];
		
		hsplit << vsplit;
		vsplit.Vert() << edit << data << full;
		vsplit.SetPos(1500, 0);
		vsplit.SetPos(8000, 1);
		
		data.AddColumn(t_("Key"));
		data.AddColumn(t_("Value"));
		data.ColumnWidths("1 4");
		
		data <<= THISBACK(DataCursor);
	}
	
}

void AnalysisCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[0];
	if (!p.part) return;
	Part& part = *p.part;
	
	for (int mode = 0; mode < 2; mode++) {
		PatternSnap& snap = part.snap[mode];
		Analysis& a = part.analysis[mode];
		DocEdit& full = this->full[mode];
		ArrayCtrl& data = this->data[mode];
		
		edit[mode].SetData(snap.txt);
		
		for(int i = 0; i < a.data.GetCount(); i++) {
			data.Set(i, 0, a.data.GetKey(i));
			data.Set(i, 1, a.data[i]);
		}
		data.SetCount(a.data.GetCount());
	}
	
	DataCursor();
}

void AnalysisCtrl::DataCursor() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[0];
	if (!p.part) return;
	Part& part = *p.part;
	
	for (int mode = 0; mode < 2; mode++) {
		Analysis& a = part.analysis[mode];
		DocEdit& full = this->full[mode];
		ArrayCtrl& data = this->data[mode];
		if (data.IsCursor()) {
			full.SetData(a.data[data.GetCursor()]);
		}
		else full.Clear();
	}
}
