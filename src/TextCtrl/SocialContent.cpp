#include "TextCtrl.h"


#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialContent::SocialContent() {
	CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << platforms << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << entries << entry;
	vsplit.SetPos(3333);
	
	platforms.AddColumn(t_("Platform"));
	
}

void SocialContent::Data() {
	
	int row = 0;
	for (const Platform& p : GetPlatforms()) {
		platforms.Set(row, 0, p.name);
		row++;
	}
	
}

void SocialContent::Clear() {
	
	
	
}

void SocialContent::OnValueChange() {
	
	
	
}


END_TEXTLIB_NAMESPACE
