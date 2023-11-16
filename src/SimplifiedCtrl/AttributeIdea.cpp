#include "SimplifiedCtrl.h"


AttributeIdea::AttributeIdea() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(2500);
	
	vsplit0.Vert() << colors << attrs;
	vsplit1.Vert() << active << suggestions;
	
	colors.AddColumn(t_("Color"));
	colors.AddColumn(t_("Count"));
	colors.ColumnWidths("4 1");
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	
	active.AddColumn(t_("Phrase"));
	active.AddColumn(t_("Artist"));
	active.AddColumn(t_("Song"));
	active.AddIndex("IDX");
	
	suggestions.AddColumn(t_("Phrase"));
	suggestions.AddColumn(t_("Artist"));
	suggestions.AddColumn(t_("Song"));
	suggestions.AddIndex("DATASET");
	suggestions.AddIndex("ARTIST");
	suggestions.AddIndex("SONG");
	suggestions.AddIndex("PHRASE");
	
}

void AttributeIdea::DisableAll() {
	disabled = true;
	
}

void AttributeIdea::EnableAll() {
	disabled = false;
	
}

void AttributeIdea::Data() {
	
}

void AttributeIdea::ToolMenu(Bar& bar) {
	
}

