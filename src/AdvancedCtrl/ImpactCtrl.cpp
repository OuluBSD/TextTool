#include "AdvancedCtrl.h"


ImpactCtrl::ImpactCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert();
	
	for(const SnapArg& a : ModeArgs()) {
		ArrayCtrl& list = this->list[a];
		vsplit << list;
		list.AddColumn(t_("Position"));
		list.AddColumn(t_("Text"));
		list.AddColumn(t_("Impact"));
		list.ColumnWidths("1 2 5");
		list.SetLineCy((int)(list.GetLineCy() * 1.5));
		
		list.WhenAction << THISBACK1(SelectLine, a);
	}
	
}

void ImpactCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	if (!song.pipe) return;
	Pipe& pipe = *song.pipe;
	Part& part = *pipe.p.part;
	
	for (const SnapArg& a : ModeArgs()) {
		ArrayCtrl& list = this->list[a];
		Index<int> story_values;
		Vector<PatternSnap*> snaps;
		
		if (db.ctx.active_wholesong)
			pipe.GetSnapsLevel(a, 0, snaps);
		else
			part.GetSnapsLevel(a, 0, snaps);
		
		for(int i = 0; i < snaps.GetCount(); i++) {
			PatternSnap& snap = *snaps[i];
			String pos = snap.part->name;
			pos << ":" + IntStr(snap.part->GetLineIdx(*snap.line));
			pos << ":" + IntStr(snap.line->GetBreakIdx(*snap.brk));
			
			Color clr = Blend(
				GetPartColor(snap.part->name),
				White(),
				256-32);
			
			list.Set(i, 0, AttrText(pos).NormalPaper(clr));
			list.Set(i, 1, AttrText(snap.txt).NormalPaper(clr));
			list.Set(i, 2, AttrText(snap.impact).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&,edit]() {
				snap.impact = edit->GetData();
			};
			list.SetCtrl(i, 2, edit);
		}
		list.SetCount(snaps.GetCount());
	}
}

void ImpactCtrl::SelectLine(const SnapArg& match) {
	
	// Select same kay in other lists too
	ArrayCtrl& list = this->list[match];
	if (list.IsCursor()) {
		String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
		for (const SnapArg& a : ModeArgs()) {
			if (a == match) continue;
			ArrayCtrl& list = this->list[a];
			bool found = false;
			for(int j = 0; j < list.GetCount(); j++) {
				if (AttrText(list.Get(j, 0)).text.ToString() == key) {
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
