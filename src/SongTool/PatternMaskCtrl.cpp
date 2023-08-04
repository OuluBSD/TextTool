#include "SongTool.h"


PatternMaskCtrl::PatternMaskCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	for (const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		ArrayCtrl& data = this->data[a];
		DocEdit& lyrics = this->lyrics[a];
		
		hsplit << vsplit;
		vsplit.Vert() << lyrics << data;
		vsplit.SetPos(2000);
		
		data.AddColumn(t_("Key"));
		data.AddColumn(t_("Value"));
		data.ColumnWidths("1 4");
		data.WhenAction << THISBACK1(SelectLine, a);
	}
	
}

void PatternMaskCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Song& song = *p.song;
	Attributes& a = db.attrs;
	if (!db.ctx.active_wholesong && !p.part)
		return;
	
	for(const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		ArrayCtrl& data = this->data[a];
		DocEdit& lyrics = this->lyrics[a];
		PatternSnap& snap = db.ctx.active_wholesong ? song.Get(a) : p.part->Get(a);
		
		lyrics.SetData(db.ctx.active_wholesong ? song.headers[a].content : snap.txt);
		
		for(int j = 0; j < snap.mask.GetCount(); j++) {
			const SnapAttrStr& sa = snap.mask[j];
			
			bool only_in_this = true;
			for (const SnapArg& a0 : ModeArgs()) {
				if (&a == &a0) continue;
				PatternSnap& snap0 = (db.ctx.active_wholesong ? song.Get(a0) : p.part->Get(a0));
				if (snap0.mask.Find(sa) >= 0)
					only_in_this = false;
			}
			
			Color gclr = GetGenderColor(a.mode);
			Color clr = Blend(only_in_this ? gclr : White(), White(), 256-32);
			
			data.Set(j, 0, AttrText(sa.group).NormalPaper(clr));
			data.Set(j, 1, AttrText(sa.item).NormalPaper(clr));
		}
		data.SetCount(snap.mask.GetCount());
	}
}

void PatternMaskCtrl::SelectLine(const SnapArg& match) {
	
	// Select same kay in other lists too
	ArrayCtrl& list = this->data[match];
	if (list.IsCursor()) {
		String group = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
		String value = AttrText(list.Get(list.GetCursor(), 1)).text.ToString();
		for (const SnapArg& a : ModeArgs()) {
			if (a == match) continue;
			ArrayCtrl& list = this->data[a];
			bool found = false;
			for(int j = 0; j < list.GetCount(); j++) {
				String group0 = AttrText(list.Get(j, 0)).text.ToString();
				String value0 = AttrText(list.Get(j, 1)).text.ToString();
				if (group0 == group && value0 == value) {
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
