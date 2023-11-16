#include "SimplifiedCtrl.h"


AttributeDistribution::AttributeDistribution() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << attrs << vsplit;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << active << suggestions;
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	
	active.AddColumn(t_("Distribution"));
	active.AddIndex("IDX");
	
	suggestions.AddColumn(t_("Distribution"));
	suggestions.AddIndex("DATASET");
	suggestions.AddIndex("ARTIST");
	suggestions.AddIndex("SONG");
	suggestions.AddIndex("IDX");
	
}

void AttributeDistribution::DisableAll() {
	disabled = true;
	
}

void AttributeDistribution::EnableAll() {
	disabled = false;
	
}

void AttributeDistribution::Data() {
	
}

void AttributeDistribution::ToolMenu(Bar& bar) {
	
}

