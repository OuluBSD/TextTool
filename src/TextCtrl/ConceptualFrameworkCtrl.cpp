#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ConceptualFrameworkCtrl::ConceptualFrameworkCtrl() {
	Add(vsplit.VSizePos(0,20).HSizePos());
	Add(prog.BottomPos(0,20).HSizePos(300));
	Add(remaining.BottomPos(0,20).LeftPos(0,300));
	
	
	vsplit.Vert() << tsplit << bsplit;
	
	tsplit.Horz() << cfs << cf << ideas;
	tsplit.SetPos(1500, 0);
	tsplit.SetPos(4000, 1);
	
	bsplit.Horz() << idea << idea_header << idea_struct << idea_improved;
	
	CtrlLayout(cf);
	CtrlLayout(idea);
	
	
	cfs.AddColumn(t_("Framework"));
	cfs.AddColumn(t_("Snap. rev."));
	cfs.AddIndex("IDX");
	cfs <<= THISBACK(DataFramework);
	
	
}

void ConceptualFrameworkCtrl::Clear() {
	
}

void ConceptualFrameworkCtrl::Data() {
	
	
	
	DataFramework();
}

void ConceptualFrameworkCtrl::DataFramework() {
	
	
	
	DataIdea();
}

void ConceptualFrameworkCtrl::DataIdea() {
	
}

void ConceptualFrameworkCtrl::OnValueChange() {
	
}

void ConceptualFrameworkCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void ConceptualFrameworkCtrl::Do(int fn) {
	MetaPtrs& mp = MetaPtrs::Single();
	/*if (!mp.profile || !mp.snap)
		return;
	if (!mp.editable_biography) {
		PromptOK(t_("Only the latest (and editable) revision can be processed. Select the latest revision."));
		return;
	}
	BiographyProcess& sdi = BiographyProcess::Get(*mp.profile, *mp.snap);
	prog.Attach(sdi);
	sdi.WhenRemaining << [this](String s) {PostCallback([this,s](){remaining.SetLabel(s);});};
	if (fn == 0)
		sdi.Start();
	else
		sdi.Stop();*/
}


END_TEXTLIB_NAMESPACE
