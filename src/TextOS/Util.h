#ifndef _TextOS_Util_h_
#define _TextOS_Util_h_


BEGIN_TEXTLIB_NAMESPACE


struct CmdArg {
	char key;
	String desc, value_desc;
	bool has_value;
};

struct CmdInput {
	char key;
	String value;
};

class CommandLineArguments {
	static const int max_inputs = 300;
	static const int max_args = 100;
	
	Array<CmdArg>				args;
	Array<CmdInput>				inputs;
	VectorMap<String,Value>		vars;
	
public:
	CommandLineArguments() {}
	
	void AddArg(char key, const char* desc, bool has_value, String value_desc="value");
	bool Parse(const Vector<String>& args);
	void PrintHelp();
	
	int GetInputCount() const {return inputs.GetCount();}
	bool IsArg(char c) const;
	String GetArg(char c) const;
	
	const Array<CmdInput>& GetInputs() const {return inputs;}
	const VectorMap<String,Value>& GetVariables() const {return vars;}
	
};


String AppendUnixFilename(String a, String b);


END_TEXTLIB_NAMESPACE


#endif
