#include "SocialCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


StorySolverCtrl::StorySolverCtrl() {
	
	Add(tabs.SizePos());
	
	// Tab 1
	tabs.Add(summary_split, t_("Summary"));
	summary_split.Vert() << summary_bar << summary_tasks << result_split;
	summary_split.SetPos(250, 0);
	summary_split.SetPos(3000, 1);
	summary_tasks.AddColumn("#");
	summary_tasks.AddColumn("Title");
	summary_tasks.AddColumn("Description");
	summary_tasks.ColumnWidths("1 4 10");
	
	result_split.Horz() << result_list << suggestion << story;
	result_split.SetPos(1000, 0);
	result_split.SetPos(5000, 1);
	
	result_list.AddColumn(t_("Rank"));
	result_list.AddIndex("IDX");
	result_list.WhenCursor << THISBACK(DataSuggestion);
	story.WhenAction << THISBACK(UserStoryChange);
	
	
	// Tab 2
	tabs.Add(part_colors.SizePos(), t_("Color process"));
	part_colors.AddColumn(t_("Part"));
	part_colors.AddColumn(t_("Primary colors"));
	part_colors.AddColumn(t_("Secondary colors"));
	part_colors.ColumnWidths("1 4 4");
	
	
	// Tab 3
	tabs.Add(program_attrs.SizePos(), t_("Attribute process"));
	program_attrs.AddColumn(t_("Key group"));
	program_attrs.AddColumn(t_("Positive"));
	program_attrs.AddColumn(t_("Negative"));
	program_attrs.AddColumn(t_("Value"));
	
	
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

void StorySolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::RedRing(), THISBACK(StopProcess)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Get new program title"), AppImg::RedRing(), THISBACK(GetNewTitle)).Key(K_F7);
}

void StorySolverCtrl::StartProcess() {
	SocialLib::StorySolver& ls = SocialLib::StorySolver::Get(GetCompany(), GetStory());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void StorySolverCtrl::StopProcess() {
	SocialLib::StorySolver& ls = SocialLib::StorySolver::Get(GetCompany(), GetStory());
	ls.Stop();
}

void StorySolverCtrl::Data() {
	Story& l = GetStory();
	
	if (tabs.Get() == 0) {
		for(int i = 0; i < l.suggestions.GetCount(); i++) {
			int rank = l.suggestions.GetKey(i);
			result_list.Set(i, 0, rank);
			result_list.Set(i, "IDX", i);
		}
		result_list.SetCount(l.suggestions.GetCount());
		
		if (!result_list.IsCursor() && result_list.GetCount())
			result_list.SetCursor(0);
		
		story.SetData(l.text);
		
		DataSuggestion();
	}
	
}

void StorySolverCtrl::DataSuggestion() {
	Story& l = GetStory();
	if (!result_list.IsCursor()) {
		suggestion.Clear();
		return;
	}
	
	int idx = result_list.Get("IDX");
	
	suggestion.SetData(l.suggestions[idx]);
}

void StorySolverCtrl::UserStoryChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.story)
		return;
	
	p.story->text = story.GetData();
}

void StorySolverCtrl::GetNewTitle() {
	Story& l = GetStory();
	
	StorySolverArgs args;
	args.fn = 8;
	args.part = l.text;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetStorySolver(args, THISBACK(OnNewTitle));
}

void StorySolverCtrl::OnNewTitle(String res) {
	Story& l = GetStory();
	
	res.Replace("\n","");
	res.Replace("\r","");
	res.Replace("\"","");
	res = TrimBoth(res);
	
	if (res.GetCount()) {
		l.english_title = res;
		l.native_title.Clear();
	}
}


END_SOCIALLIB_NAMESPACE
