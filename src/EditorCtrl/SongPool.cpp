#include "EditorCtrl.h"

SongPool::SongPool() {
	Add(list.SizePos());
	
	list.AddColumn("Phrase 1");
	list.AddColumn("Phrase 2");
	list.AddColumn("Phrase 3");
	list.AddColumn("Phrase 4");
	list.AddColumn("Score total");
	
}

void SongPool::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::BlueRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::BlueRing(), THISBACK(StopProcess)).Key(K_F6);
	
}

void SongPool::StartProcess() {
	SongLib::LyricsGenerator& ls = SongLib::LyricsGenerator::Get(GetArtist(), GetRelease(), GetSong());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void SongPool::StopProcess() {
	SongLib::LyricsGenerator& ls = SongLib::LyricsGenerator::Get(GetArtist(), GetRelease(), GetSong());
	ls.Stop();
}

void SongPool::Data() {
	
}

