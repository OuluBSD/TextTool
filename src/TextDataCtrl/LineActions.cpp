#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TextDataLineActions::TextDataLineActions() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << actions << action_args;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << phrases << next_lines;
	vsplit1.SetPos(8000);
	
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
	
	phrases.AddColumn(t_("Txt"));
	String cw = "2";
	for(int i = 0; i < action_cols; i++) {
		cw << " 1";
		phrases.AddColumn("#" + IntStr(i));
	}
	cw << " 1";
	phrases.AddColumn(t_("First line count"));
	phrases.AddIndex("IDX");
	phrases.ColumnWidths(cw);
	phrases.WhenCursor << THISBACK(DataNextLine);
	
	next_lines.AddColumn(t_("Txt"));
	next_lines.AddColumn(t_("Total score"));
	String sc;
	for(int i = 0; i < SCORE_COUNT; i++) {
		next_lines.AddColumn(ScoreTitles[i]);
		sc << " 1";
	}
	next_lines.ColumnWidths("3 1" + sc);
	
}

void TextDataLineActions::EnableAll() {
	
}

void TextDataLineActions::DisableAll() {
	
}

void TextDataLineActions::Data() {
	
}

void TextDataLineActions::DataMain() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}
	
void TextDataLineActions::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
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

void TextDataLineActions::DataAction() {
	if (!datasets.IsCursor() || !actions.IsCursor())
		return;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
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

void TextDataLineActions::DataActionHeader() {
	if (!datasets.IsCursor() || !actions.IsCursor() || !action_args.IsCursor())
		return;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String action = actions.Get(0);
	String action_arg = action_args.Get(0);
	bool filter_action = action != "All";
	bool filter_action_arg = action_arg != "All";
	
	int row = 0;
	for(int i = 0; i < da.action_phrases.GetCount(); i++) {
		const String& txt = da.action_phrases.GetKey(i);
		const ExportDepActionPhrase& ap = da.action_phrases[i];
		
		// TODO optimize: use attribute idx integer comparison
		
		if (filter_action) {
			bool found = false;
			for (int a : ap.actions) {
				const ActionHeader& ah = da.actions.GetKey(a);
				if (ah.action == action) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		if (filter_action_arg) {
			bool found = false;
			for (int a : ap.actions) {
				const ActionHeader& ah = da.actions.GetKey(a);
				if (ah.arg == action_arg) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		
		phrases.Set(row, 0, txt);
		int c = min(action_cols, ap.actions.GetCount());
		for(int j = 0; j < c; j++) {
			int aa = ap.actions[j];
			const ActionHeader& ah = da.actions.GetKey(aa);
			String s = ah.action + "("+ ah.arg + ")";
			phrases.Set(row, 1+j, s);
		}
		for(int j = c; j < action_cols; j++)
			phrases.Set(row, 1+j, Value());
		
		if (ap.first_lines > 0)
			phrases.Set(row, 1+action_cols, ap.first_lines);
		else
			phrases.Set(row, 1+action_cols, Value());
		
		phrases.Set(row, "IDX", i);
		row++;
	}
	phrases.SetCount(row);
	phrases.SetSortColumn(1+action_cols, true);
	if (!phrases.IsCursor() && phrases.GetCount())
		phrases.SetCursor(0);
	
	DataNextLine();
}

void TextDataLineActions::DataNextLine() {
	if (!datasets.IsCursor() || !actions.IsCursor() || !action_args.IsCursor() || !phrases.IsCursor()) {
		next_lines.Clear();
		return;
	}
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	int phrase_i = phrases.Get("IDX");
	const ExportDepActionPhrase& ap = da.action_phrases[phrase_i];
	
	for(int i = 0; i < ap.next_phrases.GetCount(); i++) {
		int next_i = ap.next_phrases[i];
		const String& next_txt = da.action_phrases.GetKey(next_i);
		const ExportDepActionPhrase& next = da.action_phrases[next_i];
		next_lines.Set(i, 0, next_txt);
		if (i < next.next_scores.GetCount()) {
			const Vector<int>& sc = next.next_scores[i];
			int total_score = 0, l = 0;
			for(int v : sc) {
				total_score += v;
				next_lines.Set(i, 2+l, v);
				l++;
			}
			next_lines.Set(i, 1, total_score);
		}
		else {
			next_lines.Set(i, 1, Value());
			for(int j = 0; j < SCORE_COUNT; j++) {
				next_lines.Set(i, 2+j, Value());
			}
		}
	}
	next_lines.SetCount(ap.next_phrases.GetCount());
	
}

void TextDataLineActions::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	/*bar.Separator();
	if (running0)
		bar.Add(t_("Stop getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	else
		bar.Add(t_("Start getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	for(int i = 0; i < SCORE_COUNT; i++) {
		bar.Separator();
		
		if (running1)
			bar.Add(t_("Stop getting colored wordnet scores") + Format(" %d",i), AppImg::RedRing(), THISBACK1(ToggleGettingLineChangeScores,i)).Key(K_F7 + i);
		else
			bar.Add(t_("Start getting colored wordnet scores") + Format(" %d",i), AppImg::RedRing(), THISBACK1(ToggleGettingLineChangeScores,i)).Key(K_F7 + i);
	}*/
}

/*void TextDataLineActions::ToggleGettingLineActions() {
	running0 = !running0;
	if (running0) {
		UpdateBatches();
		Thread::Start(THISBACK1(GetLineActions, 0));
	}
}

void TextDataLineActions::ToggleGettingLineChangeScores(int score_mode) {
	running1 = !running1;
	if (running1) {
		Thread::Start(THISBACK2(GetLineChangeScores, 0, score_mode));
	}
}*/


END_TEXTLIB_NAMESPACE
