#include "SimplifiedCtrl.h"


LyricsEditor::LyricsEditor() {
	
}

void LyricsEditor::Data() {
	
}

void LyricsEditor::ToolMenu(Bar& bar) {
	
}





BeatRow::BeatRow() {
	bg = GrayColor();
}

void BeatRow::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
}

void BeatRow::Layout() {
	
	
	
}




BeatButton::BeatButton() {
	bg = GrayColor(128+64);
}

void BeatButton::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
	
	
}
