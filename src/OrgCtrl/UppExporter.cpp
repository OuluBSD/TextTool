#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


UppExporterView::UppExporterView() : cache(data), data(assembly) {
	
}

void UppExporterView::Data() {
	ReadFiles();
}

void UppExporterView::ReadFiles() {
	main_prj = 0;
	
	String dir = node->data.GetAdd("dir");
	if (dir.IsEmpty() || !DirectoryExists(dir))
		return;
	if (dir.Right(1) == DIR_SEPS)
		dir = dir.Left(dir.GetCount()-1);
	String name = GetFileName(dir);
	String upp_path = AppendFileName(dir, name + ".upp");
	if (!FileExists(upp_path))
		return;
	
	if (assembly.GetDirectoryCount() == 0) {
		String ass_dir = GetFileDirectory(dir);
		assembly.AddPath(ass_dir);
	}
	
	int i = assembly.FindProject(name);
	ASSERT(i >= 0);
	
	UppProject& prj = data.GetProject(name);
	if (prj.Load(name, upp_path)) {
		main_prj = &prj;
	}
}



UppExporterCtrl::UppExporterCtrl() {
	Add(hsplit.SizePos());
	
	CtrlLayout(form);
	
	hsplit.Horz() << form << vsplit << edit;
	hsplit.SetPos(5000,1);
	
	vsplit.Vert() << pkgs << files;
	
	edit.Highlight("cpp");
	
	pkgs.AddColumn("Package");
	pkgs <<= THISBACK(DataPkg);
	
	files.AddColumn("File");
	files <<= THISBACK(DataFile);
	files.WhenBar << [this](Bar& bar) {
		bar.Add("Remove file", THISBACK1(Do, REM_FILE));
	};
	
	form.dir <<= THISBACK(OnValueChange);
	form.update.WhenAction << [this]{view->Data(); Data();};
	
}

void UppExporterCtrl::OnValueChange() {
	Node& n = *view->node;
	
	n.data.GetAdd("dir") = (String)form.dir.GetData();
}

void UppExporterCtrl::Data() {
	UppExporterView& view = dynamic_cast<UppExporterView&>(*this->view);
	Node& n = *view.node;
	
	form.dir.SetData(n.data.GetAdd("dir"));
	
	if (!view.main_prj) {
		pkgs.Clear();
		files.Clear();
		return;
	}
	
	pkg_list.Clear();
	view.main_prj->GetRecursiveUses(pkg_list, view.data);
	int c = pkg_list.GetCount();
	for(int i = 0; i < c; i++) {
		pkgs.Set(i, 0, pkg_list[i]);
	}
	INHIBIT_CURSOR(pkgs);
	pkgs.SetCount(c);
	if (!pkgs.IsCursor() && pkgs.GetCount())
		pkgs.SetCursor(0);
	
	DataPkg();
}

void UppExporterCtrl::DataPkg() {
	UppExporterView& view = dynamic_cast<UppExporterView&>(*this->view);
	
	if (!pkgs.IsCursor() || !view.main_prj) {
		files.Clear();
		return;
	}
	
	String pkg_name = pkgs.Get(0);
	UppProject& prj = view.data.GetProject(pkg_name);
	
	int c = prj.GetFileCount();
	for(int i = 0; i < c; i++) {
		files.Set(i, 0, prj.GetFile(i));
	}
	INHIBIT_CURSOR(files);
	files.SetCount(c);
	if (!files.IsCursor() && files.GetCount())
		files.SetCursor(0);
	
	DataFile();
}

void UppExporterCtrl::DataFile() {
	UppExporterView& view = dynamic_cast<UppExporterView&>(*this->view);
	
	if (!pkgs.IsCursor() || !files.IsCursor() || !view.main_prj) {
		files.Clear();
		return;
	}
	
	String pkg_name = pkgs.Get(0);
	UppProject& prj = view.data.GetProject(pkg_name);
	int file_i = files.GetCursor();
	
	String file_path = prj.GetFilePath(file_i);
	
	String content = LoadFile(file_path);
	edit.SetData(content);
}

void UppExporterCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Export code"), AppImg::RedRing(), THISBACK1(Do, EXPORT_CODE)).Key(K_F5);
	
}

void UppExporterCtrl::Do(int fn) {
	UppExporterView& view = dynamic_cast<UppExporterView&>(*this->view);
	
	if (fn == EXPORT_CODE) {
		if (!pkgs.IsCursor())
			return;
		String pkg_name = pkgs.Get(0);
		UppProject& prj = view.data.GetProject(pkg_name);
		
		Vector<Node*> files;
		view.node->owner->FindChildDeep(files, NODE_FILE);
		
		for(Node* file : files) {
			int i = prj.FindAddFile(file->name);
			String path = prj.GetFilePath(i);
			LOG(path);
			FileOut fout(path);
			String content = file->data.GetAdd("content");
			fout << content;
		}
		prj.Store();
		PostCallback(THISBACK(DataPkg));
	}
	
	if (fn == REM_FILE) {
		if (!files.IsCursor())
			return;
		int file_i = files.GetCursor();
		
		String pkg_name = pkgs.Get(0);
		UppProject& prj = view.data.GetProject(pkg_name);
		if (file_i >= 0 && file_i < prj.GetFileCount()) {
			String path = prj.GetFilePath(file_i);
			prj.RemoveFile(file_i);
			::remove(path);
			prj.Store();
			PostCallback(THISBACK(DataPkg));
		}
	}
	
}


END_TEXTLIB_NAMESPACE
