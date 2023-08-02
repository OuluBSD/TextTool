#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	Add(hsplit.SizePos());
	hsplit.Horz();
	
	for(int i = 0; i < GENDER_COUNT; i++) {
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
		
		data <<= THISBACK1(DataCursor, i);
	}
	
}

void AnalysisCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	Part& part = *p.part;
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		PatternSnap& snap =
			db.ctx.active_wholesong ? song.snap[mode] : part.snap[mode];
		Analysis& a =
			db.ctx.active_wholesong ? song.headers[mode].analysis : part.analysis[mode];
		
		DocEdit& full = this->full[mode];
		ArrayCtrl& data = this->data[mode];
		
		if (db.ctx.active_wholesong)
			edit[mode].SetData(song.headers[mode].content);
		else
			edit[mode].SetData(snap.txt);
		
		for(int i = 0; i < a.data.GetCount(); i++) {
			data.Set(i, 0, a.data.GetKey(i));
			data.Set(i, 1, a.data[i]);
		}
		data.SetCount(a.data.GetCount());
	}
	
	DataCursor(-1);
}

void AnalysisCtrl::DataCursor(int match) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	Part& part = *p.part;
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		Analysis& a =
			db.ctx.active_wholesong ? song.headers[mode].analysis : part.analysis[mode];
		
		DocEdit& full = this->full[mode];
		ArrayCtrl& data = this->data[mode];
		if (data.IsCursor()) {
			full.SetData(a.data[data.GetCursor()]);
		}
		else full.Clear();
	}
	
	// Select same kay in other lists too
	if (match >= 0 && match < GENDER_COUNT) {
		ArrayCtrl& list = this->data[match];
		if (list.IsCursor()) {
			String key = list.Get(list.GetCursor(), 0);
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (i == match) continue;
				ArrayCtrl& list = this->data[i];
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
