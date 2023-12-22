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
	action_args.WhenCursor << THISBACK(DataActionArg);
	
	transitions.AddColumn(t_("From action"));
	transitions.AddColumn(t_("From action arg"));
	transitions.AddColumn(t_("To action"));
	transitions.AddColumn(t_("To action arg"));
	transitions.AddColumn(t_("Score"));
	transitions.AddColumn(t_("Count"));
	transitions.AddIndex("IDX");
	transitions.ColumnWidths("2 2 2 2 1 1");
	
}

void ActionTransitionsPage::Data() {
	
}

void ActionTransitionsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update from cache"), AppImg::RedRing(), THISBACK(UpdateFromCache)).Key(K_F5);
	bar.Add(t_("Update transitions"), AppImg::RedRing(), THISBACK(UpdateTransitions)).Key(K_F6);
	
}

void ActionTransitionsPage::DataMain() {
	
}

void ActionTransitionsPage::DataDataset() {
	
}

void ActionTransitionsPage::DataAction() {
	
}

void ActionTransitionsPage::DataActionArg() {
	
}

void ActionTransitionsPage::UpdateFromCache() {
	
}

void ActionTransitionsPage::UpdateTransitions() {
	
}
