#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


SystemView::SystemView() {
	
}

void SystemView::Data() {
	
}





SystemCtrl::SystemCtrl() {
	Add(form.SizePos());
	
	CtrlLayout(form);
	
	form.platform <<= THISBACK(OnValueChange);
	form.os <<= THISBACK(OnValueChange);
	
	form.platform.Add("Any");
	form.platform.Add("Desktop");
	form.platform.Add("Mobile");
	form.platform.Add("Web");
	
	form.os.Add("Any");
	form.os.Add("Windows");
	form.os.Add("Mac");
	form.os.Add("Linux");
	form.os.Add("Android");
	
	form.lang.Add("Any");
	form.lang.Add("C++");
	form.lang.Add("C");
	form.lang.Add("C#");
	form.lang.Add("Java");
	form.lang.Add("EcmaScript");
	form.lang.Add("Python");
	form.lang.Add("Bash");
	form.lang.Add("PowerShell");
	
	form.ide.Add("Any");
	form.ide.Add("Makefile");
	form.ide.Add("Visual Studio");
	form.ide.Add("QtCreator");
	form.ide.Add("CodeBlocks");
	form.ide.Add("AndroidStudio");
	form.ide.Add("U++");
	
}

void SystemCtrl::OnValueChange() {
	Value& platform = view->node->data.GetAdd("platform");
	Value& os = view->node->data.GetAdd("os");
	Value& lang = view->node->data.GetAdd("lang");
	Value& ide = view->node->data.GetAdd("ide");
	
	platform = form.platform.GetKey(form.platform.GetIndex());
	os = form.os.GetKey(form.os.GetIndex());
	lang = form.lang.GetKey(form.lang.GetIndex());
	ide = form.ide.GetKey(form.ide.GetIndex());
	
}

void SystemCtrl::Data() {
	Value& platform = view->node->data.GetAdd("platform");
	if (platform.IsNull()) platform = "Any";
	
	Value& os = view->node->data.GetAdd("os");
	if (os.IsNull()) os = "Any";
	
	Value& lang = view->node->data.GetAdd("lang");
	if (lang.IsNull()) lang = "Any";
	
	Value& ide = view->node->data.GetAdd("ide");
	if (ide.IsNull()) ide = "Any";
	
	for(int i = 0; i < form.platform.GetCount(); i++) {
		if (form.platform.GetKey(i) == platform)
			form.platform.SetIndex(i);
	}
	
	for(int i = 0; i < form.os.GetCount(); i++) {
		if (form.os.GetKey(i) == os)
			form.os.SetIndex(i);
	}
	
	for(int i = 0; i < form.lang.GetCount(); i++) {
		if (form.lang.GetKey(i) == lang)
			form.lang.SetIndex(i);
	}
	
	for(int i = 0; i < form.ide.GetCount(); i++) {
		if (form.ide.GetKey(i) == ide)
			form.ide.SetIndex(i);
	}
	
}


END_TEXTLIB_NAMESPACE