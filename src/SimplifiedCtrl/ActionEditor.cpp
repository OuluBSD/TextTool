#include "SimplifiedCtrl.h"

ActionEditor::ActionEditor() {
	Add(hsplit.Horz());
	hsplit.Horz() << vsplit << mainsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << parts << thrds;
	
	mainsplit.Horz() << main_vsplit << song_actions;
	
	main_vsplit.Vert() << thrd_actions << thrd_suggestions;
	
	parts.AddColumn(t_("Part"));
	parts.WhenCursor << THISBACK(DataPart);
	
	thrds.AddColumn(t_("Thread"));
	thrds.WhenCursor << THISBACK(DataThread);
	
	thrd_actions.AddColumn(t_("Thread Actions"));
	
	thrd_suggestions.AddColumn(t_("Action Suggestions"));
	
	song_actions.AddColumn(t_(""));
	
}

void ActionEditor::Data() {
	
	DataSong();
}

void ActionEditor::DataPart() {
	
}

void ActionEditor::DataThread() {
	
}

void ActionEditor::DataSong() {
	
}

void ActionEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	
}

