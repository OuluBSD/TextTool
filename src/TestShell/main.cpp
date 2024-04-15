#include "TestShell.h"

TestShell::TestShell()
{
	Add(log.HSizePos().VSizePos(0,30));
	Add(cmd.BottomPos(0,30).HSizePos());
	
	log.WantFocus(false);
	
	log.SetFont(Monospace(15));
	cmd.SetFont(Monospace(15));
	
	
	cmd.WhenEnter << THISBACK(Execute);
	cmd.SetFocus();
	
	VM& vm = VM::Single();
	vm.InitDefault();
	
	Print("Starting shell");
	shell = &vm.AddShell();
	shell->Init();
	
	EscInterpreter& esc = shell->esc;
	esc.OnPrint << THISBACK(Print);
	esc.OnError << THISBACK(OnError);
	vm.vfs.OnError << THISBACK(OnError);
	
}

TestShell::~TestShell() {
	
	shell->Stop();
	
}

Vector<String> TestShell::ParseArgs(String cmd_str0) {
	WString cmd_str = cmd_str0.ToWString();
	
	
	enum {
		MODE_RAW,
		MODE_STRING_QUOTE,
		MODE_CHAR_QUOTE,
	};
	int mode = 0;
	
	Vector<WString> args;
	WString* arg = 0;
	for(int i = 0; i < cmd_str.GetCount(); i++) {
		int chr0 = cmd_str[i];
		int chr1 = i+1 < cmd_str.GetCount() ? cmd_str[i+1] : 0;
		
		if (mode == MODE_RAW) {
			if (chr0 == '\"') {
				mode = MODE_STRING_QUOTE;
				arg = &args.Add();
			}
			else if (chr0 == '\'') {
				mode = MODE_CHAR_QUOTE;
				arg = &args.Add();
			}
			else if (chr0 == ' ' || chr0 == '\t')
				arg = 0;
			else {
				if (arg == 0)
					arg = &args.Add();
				arg->Cat(chr0);
			}
		}
		else if (mode == MODE_STRING_QUOTE) {
			if (chr0 == '\\' && chr1 == '\"') {
				arg->Cat('\"');
				i++;
			}
			else if (chr0 == '\"') {
				mode = MODE_RAW;
				arg = 0;
			}
			else {
				arg->Cat(chr0);
			}
		}
		else if (mode == MODE_CHAR_QUOTE) {
			if (chr0 == '\\' && chr1 == '\'') {
				arg->Cat('\'');
				i++;
			}
			else if (chr0 == '\'') {
				mode = MODE_RAW;
				arg = 0;
			}
			else {
				arg->Cat(chr0);
			}
		}
		else Panic("error");
	}
	
	Vector<String> cargs;
	for (WString& ws : args)
		cargs << ws.ToString();
	
	return cargs;
}

void TestShell::Execute() {
	String cmd_str = this->cmd.GetData();
	cmd.Clear();
	
	//Print("Parsing: " + cmd_str);
	Vector<String> args = ParseArgs(cmd_str);
	
	if (args.IsEmpty())
		return;
	
	Print(cmd_str);
	
	String path = args[0];
	args.Remove(0);
	
	Vector<String> exts;
	exts << "";
	exts << ".esc";
	
	VM& vm = shell->vm;
	String filepath;
	for (String& ext : exts) {
		filepath = vm.FindInPaths(path + ext);
		if (!filepath.IsEmpty())
			break;
	}
	if (filepath.IsEmpty()) {
		OnError("path not found: " + path);
		return;
	}
	
	String esc_content = vm.vfs.LoadFile(filepath);
	if (esc_content.IsEmpty()) {
		OnError("empty file: " + filepath);
		return;
	}
	
	if (!shell->esc.Load(esc_content)) {
		OnError("esc loading failed");
	}
	
	shell->Process();
}

void TestShell::OnError(String msg) {
	Print("ERROR: " + msg);
}

void TestShell::Print(String line) {
	String s = log.GetData();
	s << line << "\n";
	log.SetData(s);
	log.SetCursor(log.GetLength());
}


GUI_APP_MAIN
{
	TestShell().Run();
}
