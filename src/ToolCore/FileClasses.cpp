#include "ToolCore.h"

void IndexFile::Load(const String& path) {
	idx.Clear();
	this->path = path;
	String content = LoadFile(path);
	content.Replace("\r","");
	Vector<String> lines = Split(content, "\n", false);
	for (String& l : lines)
		idx.Add(l);
}

void IndexFile::Store() {
	String content = Join(idx.GetKeys(), "\n", false);
	String old_content = LoadFile(path);
	if (content.GetHashValue() != old_content.GetHashValue()) {
		FileOut fout(path);
		fout << content;
		fout.Close();
	}
}
