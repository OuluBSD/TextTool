#include "TextOS.h"



BEGIN_TEXTLIB_NAMESPACE


EscInterpreter::EscInterpreter(Shell& s) : shell(s) {
	Escape(global, "Print(x)", THISBACK(Print));
	Escape(global, "GetCurrentDir()", THISBACK(GetCurrentDir));
	Escape(global, "GetDirFiles(dir)", THISBACK(GetDirFiles));
	
	
	StdLib(global);
	
}

void EscInterpreter::Print(EscEscape& e) {
	String str = e[0];
	OnPrint(str);
}

void EscInterpreter::GetCurrentDir(EscEscape& e) {
	e = shell.GetCurrentDir();
}

void EscInterpreter::GetDirFiles(EscEscape& e) {
	String dir = e[0];
	VDirectory* vdir = shell.vm.vfs.FindDirectory(dir);
	EscValue ret;
	ret.SetEmptyArray();
	if (vdir) {
		for(int i = 0; i < vdir->dirs.GetCount(); i++) {
			ret.ArrayAdd(vdir->dirs[i].filename);
		}
		for(int i = 0; i < vdir->files.GetCount(); i++) {
			ret.ArrayAdd(vdir->files[i].filename);
		}
	}
	e = ret;
}

bool EscInterpreter::Load(String esc) {
	esc_content = esc;
	try {
		Scan(global, esc);
	}
	catch(CParser::Error e) {
		Cout() << "ERROR: " << e << "\n";
		OnError(e);
		return false;
	}

	return true;
}

bool EscInterpreter::LoadFile(String path) {
	return Load(UPP::LoadFile(path));
}

bool EscInterpreter::Run(String fn) {
	try {
		Execute(global, fn, INT_MAX);
	}
	catch(CParser::Error e) {
		Cout() << "ERROR: " << e << "\n";
		OnError(e);
		return false;
	}

	return true;
}



END_TEXTLIB_NAMESPACE
