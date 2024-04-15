#ifndef _TextOS_VM_h_
#define _TextOS_VM_h_


BEGIN_TEXTLIB_NAMESPACE


class VM {
	
public:
	VFS vfs;
	VectorMap<String, String> env_vars;
	Array<Shell> shells;
	
	
public:
	typedef VM CLASSNAME;
	VM();
	
	void InitDefault();
	
	Shell& AddShell();
	void AddPath(String path);
	
	void SetEnvVar(String key, String var) {env_vars.GetAdd(key) = var;}
	
	String GetEnvVar(String key) const {return env_vars.Get(key, "");}
	Vector<String> GetPath() const;
	String FindInPaths(String filename);
	
	
	static VM& Single() {static VM vm; return vm;}
	
};


END_TEXTLIB_NAMESPACE


#endif
