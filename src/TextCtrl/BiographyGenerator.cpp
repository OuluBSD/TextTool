#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyGeneratorCtrl::BiographyGeneratorCtrl() {
	
}

void BiographyGeneratorCtrl::Data() {
	
}

void BiographyGeneratorCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void BiographyGeneratorCtrl::Do(int fn){
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile || !mp.snap)
		return;
	if (!mp.editable_biography) {
		PromptOK(t_("Only the latest (and editable) revision can be processed. Select the latest revision."));
		return;
	}
	BiographyGeneratorProcess& sdi = BiographyGeneratorProcess::Get(*mp.profile, *mp.snap);
	prog.Attach(sdi);
	sdi.WhenRemaining << [this](String s) {PostCallback([this,s](){remaining.SetLabel(s);});};
	if (fn == 0)
		sdi.Start();
	else
		sdi.Stop();
}


END_TEXTLIB_NAMESPACE
