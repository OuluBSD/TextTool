#ifndef _TextOS_Shell_h_
#define _TextOS_Shell_h_


BEGIN_TEXTLIB_NAMESPACE


class Shell {
	bool running = false, stopped = true;
	
	String current_vfs_dir;
	
public:
	VM& vm;
	EscInterpreter esc;
	
public:
	typedef Shell CLASSNAME;
	Shell(VM& vm);
	~Shell() {Stop();}
	
	void SetCurrentDir(String dir) {this->current_vfs_dir = dir;}
	
	String GetCurrentDir() const {return current_vfs_dir;}
	
	void Init();
	void Process();
	void Start() {if (running) return; running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(10);}
	
};


END_TEXTLIB_NAMESPACE


#endif
