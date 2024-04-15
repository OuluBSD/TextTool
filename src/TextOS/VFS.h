#ifndef _TextOS_VFS_h_
#define _TextOS_VFS_h_


BEGIN_TEXTLIB_NAMESPACE



struct VLink {
	uint64 uniq_id = 0;
	String filename;
	String bind_uri;
	bool is_symlink = false;
	bool is_bind = false;
	
	
	VLink() {}
	void SetBindHost(String host_path);
	void SetSymlink(String vfs_path);
	String GetURIBase() const;
	String GetURIAddr() const;
};


struct VFile : VLink{
	String content;
	
	
	VFile() {}
	void Flush() {TODO}
	
};


struct VDirectory : VLink {
	Array<VDirectory> dirs;
	Array<VFile> files;
	
	
	VDirectory() {}
	VFile&			GetAddFile(const String& filename);
	VDirectory&		GetAddDir(const String& filename);
	VFile*			FindFile(const String& filename);
	VDirectory*		FindDir(const String& filename);
	
};

class VFS {
	
	bool SyncBindHostDir(VDirectory& vdir, String dir);
public:
	VDirectory root;
	
	
public:
	typedef VFS CLASSNAME;
	VFS();
	
	void Error(String s) {OnError(s);}
	bool BindHost(String vfs_path, String host_path);
	bool SyncBind(VDirectory& dir);
	
	VFile* FindFile(const String& path);
	VDirectory* FindDirectory(const String& path);
	VFile* RealizeFile(const String& path);
	VDirectory* RealizeDirectory(const String& path);
	bool DirectoryExists(const String& path);
	bool FileExists(const String& path);
	String LoadFile(const String& path);
	
	static Vector<String> ParsePath(const String& path);
	
	Event<String> OnError;
	
};


END_TEXTLIB_NAMESPACE


#endif
