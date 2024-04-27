#include "TextOS.h"



BEGIN_TEXTLIB_NAMESPACE


VM::VM() {
	
}

void VM::InitDefault() {
	String& share = MetaDatabase::Single().share;
	AddPath("/bin");
	AddPath("/local/bin");
	
	// bind TextTool share dir
	{
		String dir = GetHomeDirFile("TextTool" DIR_SEPS + share + DIR_SEPS "script" DIR_SEPS "bin");
		if (DirectoryExists(dir))
			vfs.BindHost("/bin", dir);
	}
	
	// bind TextTool share dir
	{
		String dir = GetHomeDirFile("TextTool" DIR_SEPS + share);
		if (DirectoryExists(dir))
			vfs.BindHost("/share", dir);
	}
	
	
	// bind MyTextTool directory
	{
		String dir = GetHomeDirFile("MyTextTool");
		if (DirectoryExists(dir))
			vfs.BindHost("/local", dir);
	}
}

void VM::AddPath(String path) {
	String& s = env_vars.GetAdd("PATH");
	if (s.IsEmpty())
		s = path;
	else
		s << ";" << path;
}

Shell& VM::AddShell() {
	Shell& s = shells.Add(new Shell(*this));
	return s;
}

Vector<String> VM::GetPath() const {
	String s = env_vars.Get("PATH", "");
	return Split(s, ";");
}

String VM::FindInPaths(String filename) {
	Vector<String> paths = GetPath();
	for (const String& path : paths) {
		String filepath = AppendUnixFilename(path, filename);
		
		if (vfs.FileExists(filepath))
			return filepath;
	}
	return String();
}

END_TEXTLIB_NAMESPACE
