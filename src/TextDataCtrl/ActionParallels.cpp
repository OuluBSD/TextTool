#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ActionParallelsPage::ActionParallelsPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parallels;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << actions << action_args;
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionHeader);
	
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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

void ActionParallelsPage::DataAction() {
	if (!actions.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	
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

void ActionParallelsPage::DataActionHeader() {
	if (!actions.IsCursor() || !action_args.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	String action = actions.Get(0);
	String action_arg = action_args.Get(0);
	bool filter_action = action != "All";
	bool filter_action_arg = action_arg != "All";
	
	int idx = -1;
	int row = 0;
	for(int i = 0; i < da.parallel.GetCount(); i++) {
		int from_a = da.parallel.GetKey(i);
		const ActionHeader& at0 = da.actions.GetKey(from_a);
		const Vector<ExportParallel>& to_v = da.parallel.GetValues(i);
		
		bool at0_skipped = false;
		if (filter_action) {
			if (at0.action != action || (filter_action_arg && at0.arg != action_arg))
				at0_skipped = true;
		}
		
		for(int j = 0; j < to_v.GetCount(); j++) {
			int to_a = da.parallel.GetKey(i, j);
			const ActionHeader& at1 = da.actions.GetKey(to_a);
			const ExportParallel& at = to_v[j];
			
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
			if (row >= 10000) break;
		}
		if (row >= 10000) break;
	}
	parallels.SetCount(row);
	parallels.SetSortColumn(6, true);
	if (!parallels.IsCursor() && parallels.GetCount())
		parallels.SetCursor(0);
	
}

void ActionParallelsPage::UpdateParallels() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionParallel(0);
}


END_TEXTLIB_NAMESPACE

