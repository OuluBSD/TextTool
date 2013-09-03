#include "Sequencer.h"


SeqCtrl::SeqCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << seq << line;
	vsplit.SetPos(6666);
	
}

void SeqCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (!p.song || !p.song->pipe)
		return;
	
	p.song->pipe->RealizeProduction();
	
	seq.Data();
	line.Data();
}
