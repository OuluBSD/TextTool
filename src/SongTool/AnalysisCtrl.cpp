#include "SongTool.h"


AnalysisCtrl::AnalysisCtrl() {
	Add(hsplit.SizePos());
	hsplit.Horz();
	
	for(const SnapArg& a : HumanInputTextArgs()) {
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
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	PipePtrs& pp = pipe.p;
	Part& part = *pp.part;
	
	for (const SnapArg& a : HumanInputTextArgs()) {
		PatternSnap& snap =
			db.ctx.active_wholesong ? pipe.Get(a) : part.Get(a);
		const auto& an =
			db.ctx.active_wholesong ? pipe.analysis[a] : part.analysis[a];
		
		DocEdit& full = this->full[a];
		ArrayCtrl& data = this->data[a];
		
		if (db.ctx.active_wholesong)
			edit[a].SetData(pipe.content[a]);
		else
			edit[a].SetData(snap.txt);
		
		for(int i = 0; i < an.GetCount(); i++) {
			data.Set(i, 0, an.GetKey(i));
			data.Set(i, 1, an[i]);
		}
		data.SetCount(an.GetCount());
	}
	
	DataCursor(SnapArg());
}

void AnalysisCtrl::DataCursor(const SnapArg& match) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	Part& part = *pipe.p.part;
	
	for (const SnapArg& a : HumanInputTextArgs()) {
		const auto& an =
			db.ctx.active_wholesong ? pipe.analysis[a] : part.analysis[a];
		
		DocEdit& full = this->full[a];
		ArrayCtrl& data = this->data[a];
		if (data.IsCursor()) {
			full.SetData(an[data.GetCursor()]);
		}
		else full.Clear();
	}
	
	// Select same kay in other lists too
	for (const SnapArg& match : HumanInputTextArgs()) {
		ArrayCtrl& list = this->data[match];
		if (list.IsCursor()) {
			String key = list.Get(list.GetCursor(), 0);
			for (const SnapArg& a : HumanInputTextArgs()) {
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
