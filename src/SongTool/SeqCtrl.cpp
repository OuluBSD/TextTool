#include "SongTool.h"


SeqCtrl::SeqCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << seq << line;
	vsplit.SetPos(6666);
	
}

void SeqCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	
	if (!p.song)
		return;
	
	p.song->RealizeProduction();
	
	seq.Data();
	line.Data();
}
