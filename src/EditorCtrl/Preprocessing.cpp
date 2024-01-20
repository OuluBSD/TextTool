#include "EditorCtrl.h"

Preprocessing::Preprocessing() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << parts << lines << candidates;
	hsplit.SetPos(2000,0);
	hsplit.SetPos(6000,1);
	
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Type"));
	parts.WhenCursor << THISBACK(DataPart);
	
	lines.AddColumn(t_("Nana"));
	lines.AddColumn(t_("Primary option"));
	lines.AddColumn(t_("Option count"));
	lines.ColumnWidths("4 8 1");
	
	candidates.AddColumn(t_("Types"));
	candidates.AddColumn(t_("Struct"));
	candidates.AddColumn(t_("Text"));
	candidates.ColumnWidths("2 1 4");
	
}

void Preprocessing::Data() {
	
}

void Preprocessing::DataPart() {
	
}

void Preprocessing::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	
}
