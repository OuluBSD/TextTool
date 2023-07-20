#include "SongTool.h"


PatternMaskCtrl::PatternMaskCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	for(int i = 0; i < GENDER_COUNT; i++) {
		Splitter& vsplit = this->vsplit[i];
		ArrayCtrl& data = this->data[i];
		DocEdit& lyrics = this->lyrics[i];
		
		hsplit << vsplit;
		vsplit.Vert() << lyrics << data;
		vsplit.SetPos(2000);
		
		data.AddColumn(t_("Key"));
		data.AddColumn(t_("Value"));
		data.ColumnWidths("1 4");
		data.WhenAction << THISBACK1(SelectLine, i);
	}
	
}

void PatternMaskCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& song = *p.song;
	Attributes& a = db.attrs;
	if (!db.ctx.active_wholesong && !p.part)
		return;
	
	for(int i = 0; i < GENDER_COUNT; i++) {
		Splitter& vsplit = this->vsplit[i];
		ArrayCtrl& data = this->data[i];
		DocEdit& lyrics = this->lyrics[i];
		PatternSnap& snap = db.ctx.active_wholesong ? song.snap[i] : p.part->snap[i];
		
		lyrics.SetData(db.ctx.active_wholesong ? song.headers[i].content : snap.txt);
		
		for(int j = 0; j < snap.mask.GetCount(); j++) {
			const SnapAttrStr& sa = snap.mask[j];
			
			bool only_in_this = true;
			for(int k = 0; k < GENDER_COUNT; k++) {
				if (k == i) continue;
				PatternSnap& snap0 = (db.ctx.active_wholesong ? song.snap[k] : p.part->snap[k]);
				if (snap0.mask.Find(sa) >= 0)
					only_in_this = false;
			}
			
			Color gclr = GetGenderColor(i);
			Color clr = Blend(only_in_this ? gclr : White(), White(), 256-32);
			
			data.Set(j, 0, AttrText(sa.group).NormalPaper(clr));
			data.Set(j, 1, AttrText(sa.item).NormalPaper(clr));
		}
		data.SetCount(snap.mask.GetCount());
	}
}

void PatternMaskCtrl::SelectLine(int match) {
	
	// Select same kay in other lists too
	if (match >= 0 && match < GENDER_COUNT) {
		ArrayCtrl& list = this->data[match];
		if (list.IsCursor()) {
			String group = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			String value = AttrText(list.Get(list.GetCursor(), 1)).text.ToString();
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (i == match) continue;
				ArrayCtrl& list = this->data[i];
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
}
