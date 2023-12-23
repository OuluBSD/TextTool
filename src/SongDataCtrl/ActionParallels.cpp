#include "SongDataCtrl.h"

ActionParallelsPage::ActionParallelsPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parallels;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << actions << action_args;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionArg);
	
	parallels.AddColumn(t_("From action"));
	parallels.AddColumn(t_("From action arg"));
	parallels.AddColumn(t_("To action"));
	parallels.AddColumn(t_("To action arg"));
	parallels.AddColumn(t_("Score"));
	parallels.AddColumn(t_("Count"));
	parallels.AddColumn(t_("Weight"));
	parallels.AddIndex("IDX0");
	parallels.AddIndex("IDX1");
	parallels.ColumnWidths("2 2 2 2 1 1 1");
	
}

void ActionParallelsPage::Data() {
	
}

void ActionParallelsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update parallels"), AppImg::RedRing(), THISBACK(UpdateParallels)).Key(K_F6);
	
}

void ActionParallelsPage::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void ActionParallelsPage::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	uniq_acts.Clear();
	for (const ActionPhrase& ap : da.action_phrases) {
		for (const auto& a : ap.actions)
			uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
	}
	if (0) {
		SortByKey(uniq_acts, StdLess<String>());
		for (auto& v : uniq_acts.GetValues())
			SortByKey(v, StdLess<String>());
	}
	else {
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.action_phrases.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	DataAction();
}

void ActionParallelsPage::DataAction() {
	if (!datasets.IsCursor() || !actions.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	String action = actions.Get(0);
	int i = uniq_acts.Find(action);
	if (i < 0) {
		action_args.SetCount(1);
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, da.action_phrases.GetCount());
	}
	else {
		auto& args = uniq_acts[i];
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, args.GetCount());
		for(int i = 0; i < args.GetCount(); i++) {
			action_args.Set(1+i, 0, args.GetKey(i));
			action_args.Set(1+i, 1, args[i]);
		}
		action_args.SetCount(1+args.GetCount());
	}
	if (!action_args.IsCursor() && action_args.GetCount())
		action_args.SetCursor(0);
	
	DataActionArg();
}

void ActionParallelsPage::DataActionArg() {
	if (!datasets.IsCursor() || !actions.IsCursor() || !action_args.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String action = actions.Get(0);
	String action_arg = action_args.Get(0);
	bool filter_action = action != "All";
	bool filter_action_arg = action_arg != "All";
	
	int idx = -1;
	int row = 0;
	for(int i = 0; i < da.action_parallel.GetCount(); i++) {
		const ActionHeader& at0 = da.action_parallel.GetKey(i);
		const auto& v = da.action_parallel[i];
		
		bool at0_skipped = false;
		if (filter_action) {
			if (at0.action != action || (filter_action_arg && at0.arg != action_arg))
				at0_skipped = true;
		}
		
		for(int j = 0; j < v.GetCount(); j++) {
			const ActionHeader& at1 = v.GetKey(j);
			const ActionParallel& at = v[j];
			
			bool at1_skipped = false;
			if (filter_action && at0_skipped) {
				if (at1.action != action || (filter_action_arg && at1.arg != action_arg))
					at1_skipped = true;
			}
			if (at1_skipped && at1_skipped)
				continue;
			
			parallels.Set(row, "IDX0", i);
			parallels.Set(row, "IDX1", j);
			parallels.Set(row, 0, at0.action);
			parallels.Set(row, 1, at0.arg);
			parallels.Set(row, 2, at1.action);
			parallels.Set(row, 3, at1.arg);
			
			double score = at.count > 0 ? at.score_sum / at.count : 0;
			
			parallels.Set(row, 4, score);
			parallels.Set(row, 5, at.count);
			parallels.Set(row, 6, at.score_sum);
			
			row++;
		}
	}
	parallels.SetCount(row);
	parallels.SetSortColumn(6, true);
	if (!parallels.IsCursor() && parallels.GetCount())
		parallels.SetCursor(0);
	
}

void ActionParallelsPage::UpdateParallels() {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoActionParallel(0, 0);
}
