#include "OrgLib.h"



BEGIN_TEXTLIB_NAMESPACE


void MetaStatementString::Dump() {
	LOG(GetInstruction());
}

String MetaStatementString::GetInstruction() const {
	String s;
	s << meta_instruction << " " << name;
	for (const auto& p : params) {
		s << ", " << p.name << ": " << p.type;
	}
	if (ret.GetCount())
		s << " -> " << ret;
	if (comment.GetCount())
		s << " // " << comment;
	return s;
}


bool Parser::ParseMetaStatementString(const String& in, MetaStatementString& out) {
	CParser p(in);
	
	try {
		out.meta_instruction = p.ReadId();
		Vector<String> parts = Split(p.ReadString(), "::");
		
		out.class_path.Clear();
		for(int i = 0; i < parts.GetCount()-1; i++)
			out.class_path << parts[i];
		out.name = parts.Top();
		
		out.params.Clear();
		while (p.Char(',')) {
			if (p.IsChar2('-','>'))
				break;
			auto& param = out.params.Add();
			param.name = p.ReadId();
			p.PassChar(':');
			param.type = p.ReadId();
			while (p.IsChar('*') || p.IsChar('&')) {
				param.type.Cat(p.GetChar());
			}
		}
		
		out.ret.Clear();
		if (p.Char2('-','>')) {
			out.ret = p.ReadId();
			while (p.IsChar('*') || p.IsChar('&')) {
				out.ret.Cat(p.GetChar());
			}
		}
		
		out.comment.Clear();
		if (p.Char2('/','/')) {
			out.comment = TrimBoth(p.GetPtr());
		}
	}
	catch (Exc e) {
		LOG("error parsing: " + e);
		return false;
	}
	return true;
}

bool Parser::ParseMetaInstructionType(const String& in, MetaInstructionType& out) {
	#define INS(x) if (in == #x) {out = x; return true;}
	META_INSTRUCTION_LIST
	#undef INS
	return false;
}

String Parser::GetMetaInstructionString(MetaInstructionType type) {
	#define INS(x) if (type == x) return #x;
	META_INSTRUCTION_LIST
	#undef INS
	return String("<unknown type " + IntStr((int)type) << ">");
}


END_TEXTLIB_NAMESPACE

