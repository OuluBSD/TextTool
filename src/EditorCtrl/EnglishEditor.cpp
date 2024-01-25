#include "EditorCtrl.h"

EnglishEditor::EnglishEditor() {
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

void EnglishEditor::Data() {
	Song& song = GetSong();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		Color clr = GetSongPartPaperColor(sp.type);
		SetColoredListValue(parts, i, 0, sp.name, clr, false);
		SetColoredListValue(parts, i, 1, sp.GetTypeString(), clr, false);
	}
	INHIBIT_CURSOR(parts);
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void EnglishEditor::DataPart() {
	
}

void EnglishEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	
}
