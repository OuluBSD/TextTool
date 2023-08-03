#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	Add(hsplit.SizePos());
	hsplit.Horz();
	
	for(const SnapArg& a : GenderArgs()) {
		ArrayCtrl& data = this->data[a];
		DocEdit& edit = this->edit[a];
		DocEdit& full = this->full[a];
		Splitter& vsplit = this->vsplit[a];
		
		hsplit << vsplit;
		vsplit.Vert() << edit << data << full;
		vsplit.SetPos(1500, 0);
		vsplit.SetPos(8000, 1);
		
		data.AddColumn(t_("Key"));
		data.AddColumn(t_("Value"));
		data.ColumnWidths("1 4");
		
		data <<= THISBACK1(DataCursor, a);
	}
	
}

void AnalysisCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	Part& part = *p.part;
	
	for (const SnapArg& a : GenderArgs()) {
		PatternSnap& snap =
			db.ctx.active_wholesong ? song.Get(a) : part.Get(a);
		Analysis& an =
			db.ctx.active_wholesong ? song.headers[a].analysis : part.analysis[a];
		
		DocEdit& full = this->full[a];
		ArrayCtrl& data = this->data[a];
		
		if (db.ctx.active_wholesong)
			edit[a].SetData(song.headers[a].content);
		else
			edit[a].SetData(snap.txt);
		
		for(int i = 0; i < an.data.GetCount(); i++) {
			data.Set(i, 0, an.data.GetKey(i));
			data.Set(i, 1, an.data[i]);
		}
		data.SetCount(an.data.GetCount());
	}
	
	DataCursor(SnapArg());
}

void AnalysisCtrl::DataCursor(const SnapArg& match) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	Part& part = *p.part;
	
	for (const SnapArg& a : GenderArgs()) {
		Analysis& an =
			db.ctx.active_wholesong ? song.headers[a].analysis : part.analysis[a];
		
		DocEdit& full = this->full[a];
		ArrayCtrl& data = this->data[a];
		if (data.IsCursor()) {
			full.SetData(an.data[data.GetCursor()]);
		}
		else full.Clear();
	}
	
	// Select same kay in other lists too
	for (const SnapArg& match : GenderArgs()) {
		ArrayCtrl& list = this->data[match];
		if (list.IsCursor()) {
			String key = list.Get(list.GetCursor(), 0);
			for (const SnapArg& a : GenderArgs()) {
				if (a == match) continue;
				ArrayCtrl& list = this->data[a];
				bool found = false;
				for(int j = 0; j < list.GetCount(); j++) {
					if (list.Get(j, 0) == key) {
						list.SetCursor(j);
						found = true;
						break;
					}
				}
				if (!found)
					list.KillCursor();
			}
		}
	}
}
