#include "TextOS.h"


BEGIN_TEXTLIB_NAMESPACE


bool CommandLineArguments::IsArg(char c) const {
	for (const CmdInput& in : inputs) {
		if (in.key == c)
			return true;
	}
	return false;
}

String CommandLineArguments::GetArg(char c) const {
	for (const CmdInput& in : inputs) {
		if (in.key == c)
			return in.value;
	}
	return String();
}

void CommandLineArguments::AddArg(char key, const char* desc, bool has_value, String value_desc) {
	CmdArg& a = args.Add();
	a.key = key;
	a.desc = desc;
	a.has_value = has_value;
	a.value_desc = value_desc;
}

bool CommandLineArguments::Parse(const Vector<String>& args) {
	//DUMPC(args);
	
	for(int i = 0; i < args.GetCount(); i++) {
		String arg = args[i];
		
		if (arg.GetCount() < 2) return false;
		
		bool found = false;
		
		if (arg[0] == '-') {
			bool is_valid_key = true;
			bool is_var = false;
			int key_size = 0;
			for(int i = 1; i < arg.GetCount(); i++) {
				char c = arg[i];
				if (i > 1 && c == '=') {
					is_var = true;
					break;
				}
				if (!IsAlpha(c) && !IsDigit(c) && c != '_') {
					is_valid_key = false;
					break;
				}
				++key_size;
			}
			if (is_valid_key && is_var) {
				String key = arg.Mid(1, key_size);
				String value = arg.Mid(2+key_size);
				NumberParser p(value, 0);
				if (p.type == NumberParser::FLOAT || p.type == NumberParser::DOUBLE)
					vars.Add(key, p.d);
				else if (p.type != NumberParser::INVALID)
					vars.Add(key, p.i32);
				else
					vars.Add(key, value);
				found = true;
			}
			else {
				char key = arg[1];
				if (key) {
					for(int j = 0; j < this->args.GetCount(); j++) {
						CmdArg& a = this->args[j];
						if (a.key == key) {
							found = true;
							CmdInput& in = inputs.Add();
							in.key = key;
							if (a.has_value) {
								if (i+1 >= args.GetCount()) {
									Cerr() << "No value provided: " << arg << EOL;
									return false;
								}
								in.value = args[i+1];
								i++;
							}
							break;
						}
					}
				}
			}
		}
		
		if (!found) {Cerr() << "Invalid argument: " << arg << EOL; return false;}
	}
	
	return true;
}

void CommandLineArguments::PrintHelp() {
	String exe = GetExeFilePath();
	int i = exe.ReverseFind(DIR_SEPS);
	if (i >= 0)
		exe = exe.Mid(i+1);
	
	Stream& cout = Cout();
	cout << exe << " [-";
	for(int i = 0; i < args.GetCount(); i++)
		if (args[i].has_value)
			cout << args[i].key;
	cout << " value]";
	
	cout << " [-";
	for(int i = 0; i < args.GetCount(); i++)
		if (!args[i].has_value)
			cout << args[i].key;
	cout << "]" << EOL;
	
	for(int i = 0; i < args.GetCount(); i++) {
		CmdArg& a = args[i];
		cout << "\t-" << a.key;
		if (a.has_value)
			cout << " " << (a.value_desc.IsEmpty() ? (String)"value" : a.value_desc);
		cout << ": " << a.desc << EOL;
	}
}


String AppendUnixFilename(String a, String b) {
	while (a.Right(1) == "/")
		a = a.Left(a.GetCount()-1);
	while (b.Left(1) == "/")
		b = b.Mid(1);
	return a + "/" + b;
}


END_TEXTLIB_NAMESPACE
