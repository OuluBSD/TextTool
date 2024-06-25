#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptSolverCtrl::ScriptSolverCtrl() {
	
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
	
	result_split.Horz() << result_list << suggestion << scripts;
	result_split.SetPos(1000, 0);
	result_split.SetPos(5000, 1);
	
	result_list.AddColumn(t_("Rank"));
	result_list.AddIndex("IDX");
	result_list.WhenCursor << THISBACK(DataSuggestion);
	scripts.WhenAction << THISBACK(UserScriptChange);
	
	
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

void ScriptSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK(StartProcess)).Key(K_F5);
	bar.Add(t_("Stop process"), AppImg::RedRing(), THISBACK(StopProcess)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Get new song title"), AppImg::RedRing(), THISBACK(GetNewTitle)).Key(K_F7);
}

void ScriptSolverCtrl::StartProcess() {
	TextLib::ScriptSolver& ls = TextLib::ScriptSolver::Get(GetAppMode(), GetEntity(), GetScript());
	ls.WhenProgress = THISBACK(PostProgress);
	ls.Start();
}

void ScriptSolverCtrl::StopProcess() {
	TextLib::ScriptSolver& ls = TextLib::ScriptSolver::Get(GetAppMode(), GetEntity(), GetScript());
	ls.Stop();
}

void ScriptSolverCtrl::Data() {
	Script& l = GetScript();
	
	if (tabs.Get() == 0) {
		for(int i = 0; i < l.__suggestions.GetCount(); i++) {
			int rank = l.__suggestions.GetKey(i);
			result_list.Set(i, 0, rank);
			result_list.Set(i, "IDX", i);
		}
		result_list.SetCount(l.__suggestions.GetCount());
		
		if (!result_list.IsCursor() && result_list.GetCount())
			result_list.SetCursor(0);
		
		scripts.SetData(l.GetText());
		
		DataSuggestion();
	}
}

void ScriptSolverCtrl::DataSuggestion() {
	Script& l = GetScript();
	if (!result_list.IsCursor()) {
		suggestion.Clear();
		return;
	}
	
	int idx = result_list.Get("IDX");
	
	suggestion.SetData(l.__suggestions[idx]);
}

void ScriptSolverCtrl::UserScriptChange() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if (!p.script)
		return;
	
	TODO
	#if 0
	p.script->text = scripts.GetData();
	#endif
}

void ScriptSolverCtrl::GetNewTitle() {
	Script& l = GetScript();
	
	ScriptSolverArgs args; // 8
	args.fn = 8;
	
	TODO
	#if 0
	args.part = l.text;
	args.lng_i = l.lng_i;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetScriptSolver(GetAppMode(), args, THISBACK(OnNewTitle));
	#endif
}

void ScriptSolverCtrl::OnNewTitle(String res) {
	Script& l = GetScript();
	
	if (res.Left(1) == "\"") {
		int a = res.Find("\"", 1);
		if (a >= 0)
			res = res.Mid(1, a-1);
	}
	res.Replace("\n","");
	res.Replace("\r","");
	res.Replace("\"","");
	res = TrimBoth(res);
	
	if (res.GetCount())
		l.native_title = res;
}


END_TEXTLIB_NAMESPACE
