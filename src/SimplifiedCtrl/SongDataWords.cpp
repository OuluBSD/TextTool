#include "SimplifiedCtrl.h"

SongDataWords::SongDataWords() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << words;
	hsplit.SetPos(2500);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	words.AddColumn(t_("Word"));
	words.AddColumn(t_("Syllables"));
	words.AddColumn(t_("Phonetic syllables"));
	
}

void SongDataWords::Data() {
	
	DataDataset();
}

void SongDataWords::DataDataset() {
	
}

void SongDataWords::ToolMenu(Bar& bar) {
	bar.Add(t_("Update all words"), AppImg::RedRing(), THISBACK(UpdateWords)).Key(K_F5);
	bar.Add(t_("Update all word flags"), AppImg::RedRing(), THISBACK(UpdateWordFlags)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Update word syllables"), AppImg::BlueRing(), THISBACK2(GetSyllables, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all syllables"), AppImg::RedRing(), THISBACK2(GetSyllables, 0, true)).Key(K_F7);
	
}

void SongDataWords::UpdateWords() {
	
}

void SongDataWords::UpdateWordFlags() {
	// a = article
	// everything = hyperbole
	
}

void SongDataWords::GetSyllables(int batch_i, bool start_next) {
	
}

void SongDataWords::OnSyllables(String res, int batch_i, bool start_next) {
	
}

