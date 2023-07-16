#include "SongTool.h"


SeqCtrl::SeqCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << seq << line;
	vsplit.SetPos(6666);
	
}

void SeqCtrl::Data() {
	Database& db = Database::Single();
	
	if (!db.active.song)
		return;
	
	db.active.song->RealizeProduction();
	
	seq.Data();
	line.Data();
}
