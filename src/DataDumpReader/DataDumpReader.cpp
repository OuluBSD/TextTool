#include "DataDumpReader.h"

void DoFn(int fn);
void DoTwitter();

CONSOLE_APP_MAIN
{
	const Vector<String>& cmdline = CommandLine();
	if (cmdline.IsEmpty()) return;
	String arg0 = cmdline[0];
	if (!IsDigit(arg0[0])) return;
	int fn = ScanInt(arg0);
	
	DoFn(fn);
}

void DoFn(int fn) {
	// Twitter
	if (fn == 0) DoTwitter();
	
}

void DoTwitter() {
	
}
