#include "TextOS.h"

BEGIN_TEXTLIB_NAMESPACE


Shell::Shell(VM& vm) : vm(vm), esc(*this) {
	current_vfs_dir = "/";
	
}

void Shell::Init() {
	VFS& vfs = vm.vfs;
	
	String shell_init_path = vm.FindInPaths("shell-init.esc");
	
	if (shell_init_path.IsEmpty())
		return;
	
	String content = vfs.LoadFile(shell_init_path);
	esc.Load(content);
	
	esc.RunInit();
}


void Shell::Process() {
	VFS& vfs = vm.vfs;
	
	running = true;
	stopped = false;
	
	int iter = 0;
	bool fail = false;
	while (running) {
		if (iter == 0) {
			if (!esc.RunInit()) {
				fail = true;
				break;
			}
		}
		else {
			if (!esc.RunUpdate()) {
				fail = true;
				break;
			}
		}
		
		
		int ival = esc.GetUpdateInterval();
		
		// One time only
		if (ival < 0)
			break;
		Sleep(ival);
	}
	
	if (!fail) {
		if (!esc.RunDeinit())
			fail = true;
	}
	
	
	stopped = true;
	running = false;
}


END_TEXTLIB_NAMESPACE
