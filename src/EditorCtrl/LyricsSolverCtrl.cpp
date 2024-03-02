#include "EditorCtrl.h"


LyricsSolverCtrl::LyricsSolverCtrl() {
	
	Add(tabs.SizePos());
	
	// Tab 1
	tabs.Add(summary_split, t_("Summary"));
	summary_split.Vert() << summary_bar << summary_tasks << lyrics;
	summary_split.SetPos(250, 0);
	summary_split.SetPos(3000, 1);
	summary_tasks.AddColumn("#");
	summary_tasks.AddColumn("Title");
	summary_tasks.AddColumn("Description");
	summary_tasks.ColumnWidths("1 4 10");
	
	
	// Tab 2
	tabs.Add(part_colors.SizePos(), t_("Color process"));
	part_colors.AddColumn(t_("Part"));
	part_colors.AddColumn(t_("Primary colors"));
	part_colors.AddColumn(t_("Secondary colors"));
	part_colors.ColumnWidths("1 4 4");
	
	
	// Tab 3
	tabs.Add(song_attrs.SizePos(), t_("Attribute process"));
	song_attrs.AddColumn(t_("Key group"));
	song_attrs.AddColumn(t_("Positive"));
	song_attrs.AddColumn(t_("Negative"));
	song_attrs.AddColumn(t_("Value"));
	
	
	// Tab 4
	tabs.Add(action_split, t_("Action list"));
	action_split.Horz() << action_part_list << action_list;
	action_split.SetPos(1000);
	
	
	// Tab 5
	tabs.Add(filtered_split, t_("Filtered phrases"));
	filtered_split.Horz() << filtered_part_list << filtered_phrases;
	filtered_split.SetPos(1000);
	filtered_phrases.AddColumn(t_("Phrase"));
	filtered_phrases.AddColumn(t_("Group"));
	filtered_phrases.AddColumn(t_("Value"));
	filtered_phrases.AddColumn(t_("Actions"));
	filtered_phrases.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	filtered_phrases.AddColumn(t_("Total scores"));
	filtered_phrases.AddIndex("IDX");
	filtered_phrases.ColumnWidths("8 6 6 16 3 1");
	
	
	// Tab 6
	tabs.Add(primary_split, t_("Primary process"));
	primary_split.Horz() << primary << secondary;
	primary_split.SetPos(6666);
	primary.AddColumn(t_("Part"));
	primary.AddColumn(t_("Line #"));
	primary.AddColumn(t_("Primary phrase"));
	primary.AddColumn(t_("Secondary word-class"));
	primary.AddColumn(t_("Secondary phrase"));
	primary.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	primary.AddColumn(t_("Total scores"));
	primary.ColumnWidths("1 1 4 1 4 2 1");
	secondary.AddColumn(t_("#"));
	secondary.AddColumn(t_("Phrase"));
	secondary.ColumnWidths("1 6");
	
}

void LyricsSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::BlueRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::BlueRing(), THISBACK(StopProcess)).Key(K_F6);
	
}

void LyricsSolverCtrl::StartProcess() {
	SongLib::LyricsSolver& ls = SongLib::LyricsSolver::Get(GetArtist(), GetLyrics());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void LyricsSolverCtrl::StopProcess() {
	SongLib::LyricsSolver& ls = SongLib::LyricsSolver::Get(GetArtist(), GetLyrics());
	ls.Stop();
}

void LyricsSolverCtrl::Data() {
	
}
