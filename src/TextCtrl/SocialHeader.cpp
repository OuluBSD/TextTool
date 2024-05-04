#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialHeaderCtrl::SocialHeaderCtrl() {
	CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << platforms << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << timeline << entries << entry;
	vsplit.SetPos(800, 0);
	vsplit.SetPos(3333, 1);
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	entries.AddColumn(t_("Published"));
	entries.AddColumn(t_("Title"));
	entries.AddColumn(t_("Message"));
	entries.AddColumn(t_("Hashtags"));
	entries.AddColumn(t_("Comments"));
	entries.AddColumn(t_("Score"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("3 5 10 4 1 1");
	entries.WhenBar << THISBACK(EntryListMenu);
	
}

void SocialHeaderCtrl::Data() {
	
}

void SocialHeaderCtrl::DataPlatform() {
	
}

void SocialHeaderCtrl::ToolMenu(Bar& bar) {
	ToolAppCtrl::ToolMenu(bar);
}

void SocialHeaderCtrl::EntryListMenu(Bar& bar) {
	
}


END_TEXTLIB_NAMESPACE

