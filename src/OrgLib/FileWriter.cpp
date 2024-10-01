#include "OrgLib.h"


BEGIN_TEXTLIB_NAMESPACE


CppFileWriter::CppFileWriter() {
	
}

void CppFileWriter::WriteHeader(Node& n, const String& h_path) {
	FileOut s(h_path);
	
	String name = n.name;
	String h_name = "_" + name + "_" + name + "_h_";
	
	s << "#ifndef " << h_name << "\n"
		 << "#define " << h_name << "\n\n\n";
	
	
	if (n.type == NODE_CLASS) {
		s << GetClassString(n);
		s << "\n\n";
	}
	else if (n.type == NODE_PACKAGE) {
		Vector<Node*> classes;
		n.FindChildDeep(classes, NODE_CLASS);
		
		for(int i = 0; i < classes.GetCount(); i++) {
			Node& cls = *classes[i];
			//s << GetClassString(cls);
			//s << "\n\n";
			s << "#include \"" << cls.name << ".h\"\n";
		}
		
		s << "\n\n";
		
		Vector<Node*> funcs;
		n.FindChildDeep(funcs, NODE_FUNCTION);
		
		for(int i = 0; i < funcs.GetCount(); i++) {
			Node& fn = *funcs[i];
			s << GetFunctionString(fn);
			s << "\n";
		}
		
		s << "\n\n";
		
	}
	else TODO
	
	s << "#endif\n";
}

String CppFileWriter::GetFunctionString(Node& n) {
	String s;
	s << n.data.GetAdd("ret") << " ";
	s << n.name;
	
	s << "(";
	ValueArray& params = ValueToArray(n.data.GetAdd("params"));
	for(int i = 0; i < params.GetCount(); i++) {
		ValueMap& p = ValueToMap(params.At(i));
		String name = p.GetAdd("name");
		String type = p.GetAdd("type");
		if (i) s << ", ";
		s << type << " " << name;
	}
	s << ")";
	
	s << ";";
	return s;
}

String CppFileWriter::GetClassString(Node& n) {
	String s;
	
	s << "class " << n.name << " {\n";
	s << "public:\n";
	s << "\n";
	
	Vector<Node*> funcs;
	n.FindChildDeep(funcs, NODE_FUNCTION);
	
	for(int i = 0; i < funcs.GetCount(); i++) {
		Node& fn = *funcs[i];
		
		s << "\t";
		s << GetFunctionString(fn);
		s << "\n";
	}
	
	s << "\n";
	s << "};\n\n";
	
	return s;
}


END_TEXTLIB_NAMESPACE
