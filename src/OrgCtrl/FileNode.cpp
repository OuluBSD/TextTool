#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


FileView::FileView() {
	
}

void FileView::Data() {
	
}



FileCtrl::FileCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << form << edit;
	CtrlLayout(form);
	
	
}

void FileCtrl::Data() {
	String ext = GetFileExt(view->node->name);
	edit.Highlight(ext.Mid(1));
	
	edit.SetData(view->node->data.GetAdd("content"));
	
}

void FileCtrl::OnValueChange() {
	
}

void FileCtrl::Do(int fn) {
	
	if (fn == GENERATE_CODE) {
		TaskMgr& m = TaskMgr::Single();
		CodeArgs args;
		args.fn = CodeArgs::MAKE_CODE;
		
		Vector<Node*> systems;
		view->node->FindParentDeep(systems, NODE_SYSTEM);
		Reverse(systems);
		if (systems.IsEmpty())
			return;
		
		Vector<Node*> features;
		view->node->FindParentDeep(features, NODE_FEATURES);
		Reverse(features);
		if (features.IsEmpty())
			return;
		
		#define ITEM(x) if (args.x.IsEmpty()) args.x = n->data.GetAdd(#x);
		for (Node* n : systems) {
			ITEM(platform)
			ITEM(os)
			ITEM(lang)
			ITEM(ide)
		}
		for (Node* n : features) {
			ValueArray list = n->data.GetAdd("features");
			for(int i = 0; i < list.GetCount(); i++)
				args.features << list[i];
		}
		args.file = view->node->name;
		
		m.GetCode(args, [this](String res) {
			res = TrimBoth(res);
			if (res.Left(3) == "```") {
				int i = res.Find("\n");
				if (i >= 0) {
					i++;
					res = TrimLeft(res.Mid(i));
					int i = res.ReverseFind("```");
					if (i >= 0)
						res = TrimRight(res.Left(i));
				}
			}
			view->node->data.GetAdd("content") = res;
			
			PostCallback(THISBACK(Data));
		});
	}
	
}

void FileCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Generate code"), AppImg::RedRing(), THISBACK1(Do, GENERATE_CODE)).Key(K_F5);
	
}



END_TEXTLIB_NAMESPACE
