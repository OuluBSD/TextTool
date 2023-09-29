#include "AdvancedCtrl.h"


LyricsCtrl::LyricsCtrl() {
	
}

void LyricsCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song) return;
	Song& song = *p.song;
	if (!song.pipe) return;
	
}
