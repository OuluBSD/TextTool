#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


void UppExporterView::MakeFiles() {
	String ass_dir = node->data.GetAdd("assembly");
	String name = node->data.GetAdd("main-package");
	
	if (ass_dir.IsEmpty() || name.IsEmpty())
		return;
	
	Node& prj_file = *node->owner;
	RealizeDirectory(ass_dir);
	
	Init(prj_file, org);
	Data();
	
	String main_pkg = "MainApplication";
	UppProject& main_prj = data.RealizeProject(main_pkg);
	main_prj.ClearContent();
	main_prj.GetAddConfig("");
	main_prj.FindAddFile(main_pkg + ".h");
	main_prj.FindAddFile(main_pkg + ".cpp");
	
	Vector<Node*> packages;
	prj_file.FindChildDeep(packages, NODE_PACKAGE);
	
	CppFileWriter writer;
	
	for (Node* pkg_ptr : packages) {
		Node& pkg = *pkg_ptr;
		String pkg_name = pkg.name;
		ASSERT(pkg_name.GetCount());
		
		UppProject& upp_prj = data.RealizeProject(pkg_name);
		bool write_prj = pkg_name != main_pkg;
		if (write_prj) {
			upp_prj.ClearContent();
			upp_prj.AddFile(pkg_name + ".h");
			main_prj.AddUse(pkg_name); // TODO use dependency hierarchy instead of this
			
			String dir = upp_prj.GetDirectory();
			String h_path = AppendFileName(dir, pkg_name + ".h");
			writer.WriteHeader(pkg, h_path);
		}
		String prj_dir = AppendFileName(ass_dir, pkg_name);
		
		Vector<Node*> classes;
		pkg.FindChildDeep(classes, NODE_CLASS);
		
		for(int i = 0; i < classes.GetCount(); i++) {
			Node& cls = *classes[i];
			
			// TODO find class path
			
			String cls_name = cls.name;
			ASSERT(!cls_name.IsEmpty());
			
			String h_file = cls_name + ".h";
			String cpp_file = cls_name + ".cpp";
			upp_prj.FindAddFile(h_file);
			upp_prj.FindAddFile(cpp_file);
			
			String h_path = AppendFileName(prj_dir, h_file);
			
			writer.WriteHeader(cls, h_path);
		}
		
		upp_prj.Store();
	}
	main_prj.Store();
	//WhenTree();
}

void UppExporterCtrl::OnTreeChange() {
	if (org)
		PostCallback(callback(org, &OrganizationCtrl::Data));
}


END_TEXTLIB_NAMESPACE
