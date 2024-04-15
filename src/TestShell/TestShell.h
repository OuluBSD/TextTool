#ifndef _TestShell_TestShell_h
#define _TestShell_TestShell_h

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

#include <TextOS/TextOS.h>


class TestShell : public TopWindow {
	DocEdit log;
	EditString cmd;
	Shell* shell = 0;
	
	void OnError(String msg);
	void Print(String line);
	
public:
	typedef TestShell CLASSNAME;
	TestShell();
	~TestShell();
	
	void Execute();
	
	static Vector<String> ParseArgs(String cmd_str);
	
};

#endif
