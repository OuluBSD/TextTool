#include "TextOS.h"


BEGIN_TEXTLIB_NAMESPACE


void VLink::SetBindHost(String host_path) {
	if (host_path.Left(7) == "file://") host_path = host_path.Mid(7);
	bind_uri = "file://" + host_path;
	is_bind = true;
	is_symlink = false;
}

void VLink::SetSymlink(String vfs_path) {
	bind_uri = vfs_path;
	is_bind = false;
	is_symlink = true;
}




VFS::VFS() {
	
}

VFile* VFS::RealizeFile(const String& path) {
	Vector<String> parts = ParsePath(path);
	if (parts.IsEmpty())
		return 0;
	
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount()-1; i++) {
		String name = parts[i];
		dir = &dir->GetAddDir(name);
	}
	
	String filename = parts.Top();
	VFile& f = dir->GetAddFile(filename);
	return &f;
}

VDirectory* VFS::RealizeDirectory(const String& path) {
	Vector<String> parts = ParsePath(path);
	if (parts.IsEmpty())
		return 0;
	
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount(); i++) {
		String name = parts[i];
		dir = &dir->GetAddDir(name);
	}
	return dir;
}

bool VFS::DirectoryExists(const String& path) {
	if (path.IsEmpty()) return false;
	Vector<String> parts = ParsePath(path);
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount(); i++) {
		String name = parts[i];
		dir = dir->FindDir(name);
		if (!dir)
			return false;
	}
	return true;
}

bool VFS::FileExists(const String& path) {
	if (path.IsEmpty()) return false;
	Vector<String> parts = ParsePath(path);
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount()-1; i++) {
		String name = parts[i];
		dir = dir->FindDir(name);
		if (!dir)
			return false;
	}
	return dir->FindFile(parts.Top()) != 0;
}

VDirectory* VFS::FindDirectory(const String& path) {
	if (path.IsEmpty()) return 0;
	Vector<String> parts = ParsePath(path);
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount(); i++) {
		String name = parts[i];
		dir = dir->FindDir(name);
		if (!dir)
			return 0;
	}
	return dir;
}

VFile* VFS::FindFile(const String& path) {
	if (path.IsEmpty()) return 0;
	Vector<String> parts = ParsePath(path);
	VDirectory* dir = &root;
	for(int i = 0; i < parts.GetCount()-1; i++) {
		String name = parts[i];
		dir = dir->FindDir(name);
		if (!dir)
			return 0;
	}
	VFile* f = dir->FindFile(parts.Top());
	if (!f) return 0;
	
	static const int max_links = 100;
	VFile* visited[max_links];
	int visited_count = 0;
	while (f->is_symlink) {
		VFile* link = FindFile(f->bind_uri);
		if (!link)
			return 0;
		
		// Stop circular link loops
		for(int i = 0; i < visited_count; i++)
			if (visited[i] == link)
				return 0;
		if (visited_count >= max_links)
			return 0;
		visited[visited_count++] = link;
	}
	
	return f;
}

String VFS::LoadFile(const String& path) {
	VFile* f = FindFile(path);
	if (!f)
		return String();
	
	if (f->is_bind) {
		String uri_base = f->GetURIBase();
		String addr = f->GetURIAddr();
		
		if (uri_base == "file") {
			return UPP::LoadFile(addr);
		}
		
		return String();
	}
	
	return f->content;
}

bool VFS::BindHost(String vfs_path, String host_path) {
	if (this->FileExists(vfs_path)) {
		Error("VFS path exists already as a file");
		return false;
	}
	
	if (this->DirectoryExists(vfs_path)) {
		Error("VFS path exists already as a directory");
		return false;
	}
	
	if (UPP::DirectoryExists(host_path)) {
		VDirectory* dir = this->RealizeDirectory(vfs_path);
		if (!dir) {OnError("Unknown error while creating directory"); return false;}
		dir->SetBindHost(host_path);
		SyncBind(*dir);
		return true;
	}
	else if (UPP::FileExists(host_path)) {
		VFile* f = this->RealizeFile(vfs_path);
		if (!f) {OnError("Unknown error while creating file"); return false;}
		f->SetBindHost(host_path);
		return true;
	}
	else return false;
}

Vector<String> VFS::ParsePath(const String& path) {
	Vector<String> v = Split(path, "/");
	return v;
}

bool VFS::SyncBind(VDirectory& dir) {
	if (!dir.is_bind)
		return false;
	
	String base = dir.GetURIBase();
	String addr = dir.GetURIAddr();
	
	if (base == "file") {
		if (!SyncBindHostDir(dir, addr))
			return false;
		return true;
	}
	else return false;
}

bool VFS::SyncBindHostDir(VDirectory& vdir, String dir) {
	UPP::FindFile ff;
	if (ff.Search(AppendFileName(dir, "*"))) {
		do {
			String path = ff.GetPath();
			String filename = UPP::GetFileName(path);
			if (filename == "." || filename == "..") continue;
			
			if (ff.IsDirectory()) {
				VDirectory& sub = vdir.GetAddDir(filename);
				sub.SetBindHost(path);
				if (!SyncBindHostDir(sub, path))
					return false;
			}
			else {
				VFile& vfile = vdir.GetAddFile(filename);
				vfile.SetBindHost(path);
			}
		}
		while (ff.Next());
	}
	return true;
}











String VLink::GetURIBase() const {
	int a = bind_uri.Find("://");
	if (a < 0)
		return String();
	return bind_uri.Left(a);
}

String VLink::GetURIAddr() const {
	int a = bind_uri.Find("://");
	if (a < 0)
		return String();
	return bind_uri.Mid(a+3);
}

VFile* VDirectory::FindFile(const String& filename) {
	for (VFile& f : files) {
		if (f.filename == filename)
			return &f;
	}
	return 0;
}

VDirectory* VDirectory::FindDir(const String& filename) {
	for (VDirectory& d : dirs) {
		if (d.filename == filename)
			return &d;
	}
	return 0;
}

VFile& VDirectory::GetAddFile(const String& filename) {
	for (VFile& f : files) {
		if (f.filename == filename)
			return f;
	}
	VFile& f = files.Add();
	f.uniq_id = Random64();
	f.filename = filename;
	return f;
}

VDirectory& VDirectory::GetAddDir(const String& filename) {
	for (VDirectory& d : dirs) {
		if (d.filename == filename)
			return d;
	}
	VDirectory& d = dirs.Add();
	d.uniq_id = Random64();
	d.filename = filename;
	return d;
}



END_TEXTLIB_NAMESPACE
