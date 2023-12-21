#include "SongDataCtrl.h"


ActionAttrsPage::ActionAttrsPage() {
	
}

void ActionAttrsPage::Data() {
	
}

void ActionAttrsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update action colors"), AppImg::RedRing(), THISBACK(UpdateColors)).Key(K_F5);
	bar.Add(t_("Update action attributes"), AppImg::RedRing(), THISBACK(UpdateAttributes)).Key(K_F6);
	
}

void ActionAttrsPage::DataMain() {
	
}

void ActionAttrsPage::UpdateColors() {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoActionlist(0, 0);
}

void ActionAttrsPage::UpdateAttributes() {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoActionlist(0, 1);
}
