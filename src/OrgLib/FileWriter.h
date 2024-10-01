#ifndef _OrgLib_FileWriter_h_
#define _OrgLib_FileWriter_h_


BEGIN_TEXTLIB_NAMESPACE


class CppFileWriter {
	
	
public:
	typedef CppFileWriter CLASSNAME;
	CppFileWriter();
	
	void WriteHeader(Node& n, const String& h_path);
	
	String GetFunctionString(Node& n);
	String GetClassString(Node& n);
	
};


END_TEXTLIB_NAMESPACE


#endif
