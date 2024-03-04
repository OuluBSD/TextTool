#include "SongDataCtrl.h"


ActionTransitionsPage::ActionTransitionsPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << transitions;
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
	action_args.WhenCursor << THISBACK(DataActionHeader);
	
	transitions.AddColumn(t_("From action"));
	transitions.AddColumn(t_("From action arg"));
	transitions.AddColumn(t_("To action"));
	transitions.AddColumn(t_("To action arg"));
	transitions.AddColumn(t_("Score"));
	transitions.AddColumn(t_("Count"));
	transitions.AddColumn(t_("Weight"));
	transitions.AddIndex("IDX0");
	transitions.AddIndex("IDX1");
	transitions.ColumnWidths("2 2 2 2 1 1 1");
	
}

void ActionTransitionsPage::Data() {
	
}

void ActionTransitionsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update transitions"), AppImg::RedRing(), THISBACK(UpdateTransitions)).Key(K_F6);
	
}

void ActionTransitionsPage::DataMain() {
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

void ActionTransitionsPage::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	uniq_acts.Clear();
	for (const ActionHeader& ah : da.actions.GetKeys()) {
		uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
	}
	struct Sorter {
		bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
			return a.GetCount() > b.GetCount();
		}
	};
	SortByValue(uniq_acts, Sorter());
	for (auto& v : uniq_acts.GetValues())
		SortByValue(v, StdGreater<int>());
	
	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.actions.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	DataAction();
}

void ActionTransitionsPage::DataAction() {
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
		action_args.Set(0, 1, da.actions.GetCount());
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
	
	DataActionHeader();
}

void ActionTransitionsPage::DataActionHeader() {
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
	for(int i = 0; i < da.trans.GetCount(); i++) {
		int from_a = da.trans.GetKey(i);
		const ActionHeader& at0 = da.actions.GetKey(from_a);
		const Vector<ExportTransition>& to_v = da.trans.GetValues(i);
		
		bool at0_skipped = false;
		if (filter_action) {
			if (at0.action != action || (filter_action_arg && at0.arg != action_arg))
				at0_skipped = true;
		}
		
		for(int j = 0; j < to_v.GetCount(); j++) {
			int to_a = da.trans.GetKey(i, j);
			const ActionHeader& at1 = da.actions.GetKey(to_a);
			const ExportTransition& at = to_v[j];
			
			bool at1_skipped = false;
			if (filter_action && at0_skipped) {
				if (at1.action != action || (filter_action_arg && at1.arg != action_arg))
					at1_skipped = true;
			}
			if (at1_skipped && at1_skipped)
				continue;
			
			transitions.Set(row, "IDX0", i);
			transitions.Set(row, "IDX1", j);
			transitions.Set(row, 0, at0.action);
			transitions.Set(row, 1, at0.arg);
			if (at0.action != at1.action || at0.arg != at1.arg) {
				transitions.Set(row, 2, at1.action);
				transitions.Set(row, 3, at1.arg);
			}
			else {
				// no change
				transitions.Set(row, 2, Value());
				transitions.Set(row, 3, Value());
			}
			
			double score = at.count > 0 ? at.score_sum / at.count : 0;
			
			transitions.Set(row, 4, score);
			transitions.Set(row, 5, at.count);
			transitions.Set(row, 6, at.score_sum);
			
			row++;
		}
	}
	transitions.SetCount(row);
	transitions.SetSortColumn(6, true);
	if (!transitions.IsCursor() && transitions.GetCount())
		transitions.SetCursor(0);
	
}

void ActionTransitionsPage::UpdateTransitions() {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoActionTransition(0, 0);
}
