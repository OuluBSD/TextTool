#include "SongTool.h"


ImpactCtrl::ImpactCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert();
	
	for(int i = 0; i < GENDER_COUNT; i++) {
		ArrayCtrl& list = this->list[i];
		vsplit << list;
		list.AddColumn(t_("Position"));
		list.AddColumn(t_("Text"));
		list.AddColumn(t_("Impact"));
		list.ColumnWidths("1 2 5");
		list.SetLineCy(list.GetLineCy() * 1.5);
		
		list.WhenAction << THISBACK1(SelectLine, i);
	}
	
}

void ImpactCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& song = *p.song;
	Part& part = *p.part;
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		ArrayCtrl& list = this->list[mode];
		Index<int> story_values;
		Vector<PatternSnap*> snaps;
		
		if (db.ctx.active_wholesong)
			song.GetSnapsLevel(mode, 0, snaps);
		else
			part.GetSnapsLevel(mode, 0, snaps);
		
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
			list.Set(i, 2, AttrText(snap.data.Get("impact", "")).NormalPaper(clr));
			
			EditString* edit = new EditString;
			edit->WhenAction << [&,edit]() {
				snap.data.GetAdd("impact") = edit->GetData();
			};
			list.SetCtrl(i, 2, edit);
		}
		list.SetCount(snaps.GetCount());
	}
}

void ImpactCtrl::SelectLine(int match) {
	
	// Select same kay in other lists too
	if (match >= 0 && match < GENDER_COUNT) {
		ArrayCtrl& list = this->list[match];
		if (list.IsCursor()) {
			String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (i == match) continue;
				ArrayCtrl& list = this->list[i];
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
}
