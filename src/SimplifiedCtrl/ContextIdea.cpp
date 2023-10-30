#include "SimplifiedCtrl.h"


ContextIdeaCtrl::ContextIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << role_split << potential_use << in_use << notes << suggestion;
	hsplit.SetPos(1000, 0);
	hsplit.SetPos(2000, 1);
	hsplit.SetPos(3000, 2);
	hsplit.SetPos(6500, 3);
	
	role_split.Vert() << rslist << role_suggestion;
	role_split.SetPos(2500);
	
	rslist.AddColumn(t_("Role suggestion #"));
	role_suggestion.AddColumn(t_("Role"));
	role_suggestion.AddColumn(t_("Weight"));
	role_suggestion.ColumnWidths("2 1");
	
	potential_use.AddColumn(t_("Potential use"));
	potential_use.AddColumn(t_("Weight"));
	potential_use.ColumnWidths("2 1");
	
	in_use.AddColumn(t_("In use"));
	in_use.AddColumn(t_("Weight"));
	in_use.ColumnWidths("2 1");
	
	notes.AddColumn(t_("Rhyme #"));
	notes.AddColumn(t_("Note #"));
	notes.AddColumn(t_(""));
	notes.AddColumn(t_(""));
	
	suggestion.AddColumn(t_("Rhyme #"));
	suggestion.AddColumn(t_("Note #"));
	suggestion.AddColumn(t_(""));
	suggestion.AddColumn(t_(""));
	
	
}

void ContextIdeaCtrl::DisableAll() {
	
}

void ContextIdeaCtrl::EnableAll() {
	
}

void ContextIdeaCtrl::Data() {
	
}

void ContextIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get role suggestions"), AppImg::BlueRing(), THISBACK(GetRoleSuggestions)).Key(K_CTRL_Q);
	bar.Add(t_("Activate role suggestion"), AppImg::BlueRing(), THISBACK(ActivateSuggestion)).Key(K_CTRL_W);
	bar.Add(t_("Get suggestions"), AppImg::BlueRing(), THISBACK(GetSuggestions)).Key(K_CTRL_E);
	bar.Add(t_("Use suggestions"), AppImg::BlueRing(), THISBACK(UseSuggestions)).Key(K_CTRL_R);
	
}

void ContextIdeaCtrl::GetRoleSuggestions() {
	
}

void ContextIdeaCtrl::ActivateSuggestion() {
	
}

void ContextIdeaCtrl::GetSuggestions() {
	
}

void ContextIdeaCtrl::UseSuggestions() {
	
}
