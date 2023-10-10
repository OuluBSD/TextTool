#include "SimplifiedCtrl.h"


TxtSerialCompare::TxtSerialCompare() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	
	vsplit0.Vert() << rhymes << suggestions;
	vsplit1.Vert() << params << attrs << best;
	vsplit0.SetPos(3333);
	vsplit1.SetPos(1111, 0);
	vsplit1.SetPos(6666, 1);
	
	rhymes.AddColumn(t_("User content"));
	rhymes.AddColumn(t_("Final"));
	rhymes.AddColumn(t_("Syllables"));
	rhymes.AddColumn(t_("Attributes"));
	rhymes.ColumnWidths("5 5 1 1");
	
	suggestions.AddColumn(t_("AI Score"));
	suggestions.AddColumn(t_("AI Score extra"));
	suggestions.AddColumn(t_("User score"));
	suggestions.AddColumn(t_("Style"));
	suggestions.AddColumn(t_("Text"));
	suggestions.ColumnWidths("1 1 2 6 12");
	
	params.AddColumn(t_("Key"));
	params.AddColumn(t_("Value"));
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Positive"));
	attrs.AddColumn(t_("Negative"));
	attrs.AddColumn(t_("Value"));
	attrs.ColumnWidths("1 1 1 2");
	
}

void TxtSerialCompare::ToolMenu(Bar& bar) {
	
}

void TxtSerialCompare::Data() {
	
	DataPart();
}

void TxtSerialCompare::DataPart() {
	
	
	DataBreak();
}

void TxtSerialCompare::DataBreak() {
	
}
