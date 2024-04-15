#ifndef _TextOS_EscInterpreter_h_
#define _TextOS_EscInterpreter_h_


BEGIN_TEXTLIB_NAMESPACE


class VM;
class Shell;


class EscInterpreter {
	String esc_content;
	ArrayMap<String, EscValue> global;
	int update_interval = -1;
	
public:
	Shell& shell;
	
public:
	typedef EscInterpreter CLASSNAME;
	EscInterpreter(Shell&);
	
	bool Load(String path);
	bool LoadFile(String path);
	bool RunInit() {return Run("init");}
	bool RunUpdate() {return Run("update");}
	bool RunDeinit() {return Run("deinit");}
	bool Run(String fn);
	
	int GetUpdateInterval() const {return update_interval;}
	
	void Print(EscEscape& e);
	void Set(EscEscape& e);
	void GetCurrentDir(EscEscape& e);
	void GetDirFiles(EscEscape& e);
	
	
	
	
	Event<String> OnPrint;
	Event<String> OnError;
	
};


END_TEXTLIB_NAMESPACE

#endif
