#include "EditorCtrl.h"



LineStructureEditor::LineStructureEditor() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << parts << vsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << lines << line_options << phrase_parts << phrase_structs;
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Type"));
	parts.WhenCursor << THISBACK(DataPart);
	
	lines.AddColumn(t_("Nana"));
	lines.AddColumn(t_("Primary option"));
	lines.AddColumn(t_("Option count"));
	lines.ColumnWidths("4 8 1");
	
	line_options.AddColumn(t_("Phrase parts"));
	line_options.AddColumn(t_("Phrase struct"));
	
	phrase_parts.AddColumn(t_("Type"));
	phrase_parts.AddColumn(t_("Example #1"));
	phrase_parts.AddColumn(t_("Example #2"));
	
	phrase_structs.AddColumn(t_("Struct name"));
	phrase_structs.AddColumn(t_("Types"));
	phrase_structs.AddColumn(t_("Example #1"));
	phrase_structs.AddColumn(t_("Example #2"));
	
}

void LineStructureEditor::Data() {
	
}

void LineStructureEditor::DataPart() {
	
}

void LineStructureEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	
}

