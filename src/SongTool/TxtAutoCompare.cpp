#include "SongTool.h"


TxtAutoCompare::TxtAutoCompare() {
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	hsplit << main << vsplit;
	vsplit.Vert() << lines << line_results << best;
	
	rhymetype.Add(t_("2 end rhymes in 1 line"));
	rhymetype.Add(t_("2 end rhymes in 2 lines"));
	rhymetype.Add(t_("3 end rhymes in 2 lines"));
	rhymetype.Add(t_("4 end rhymes in 2 lines"));
	rhymetype.Add(t_("2 end rhymes in 4 lines"));
	rhymetype.Add(t_("3 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 end rhymes in 4 lines"));
	rhymetype.Add(t_("2 begin rhymes and 2 end rhymes in 2 lines"));
	rhymetype.Add(t_("2 begin rhymes and 4 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 begin rhymes and 2 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 begin rhymes and 4 end rhymes in 4 lines"));
	rhymetype.SetIndex(1);
	
}

void TxtAutoCompare::Data() {
	
}

void TxtAutoCompare::DoMainAction(int i) {
	
}

void TxtAutoCompare::ToolMenu(Bar& bar) {
	Size sz = bar.GetStdSize();
	sz.cx = 300;
	bar.AddTool(rhymetype, sz);
}

