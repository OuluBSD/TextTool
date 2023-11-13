#include <Core/Core.h>
using namespace Upp;

#define TODO Panic("TODO");


struct ColorRule : Moveable<ColorRule> {
	int color;
	double dbl;
	int mode;
	
	enum {
		MODE_MULTIPLY_COLOR,
		MODE_MULTIPLY_AVERAGE,
		MODE_CONSTANT,
	};
};

CONSOLE_APP_MAIN
{
	Vector<String> lines = Split(LoadFile(GetDataFile("data.txt")), "\n");
	
	VectorMap<String, Vector<ColorRule>> out;
	
	for (String& l : lines) {
		LOG(l);
		
		int a = l.Find(":");
		if (a < 0) continue;
		String key = l.Left(a);
		l = TrimBoth(l.Mid(a+1));
		
		Vector<ColorRule>& v = out.Add(ToLower(key));
		
		CParser p(l);
		try {
			while (!p.IsEof()) {
				if (p.Char('+'))
					;
				
				if (p.Char('(')) {
					if (p.IsDouble()) {
						double d = p.ReadDouble();
						
						if (!p.Char('*'))
							p.PassChar('x');
						
						if (p.Id("RGB")) {
							if (p.Char('(')) {
								int c[3];
								c[0] = p.ReadInt();
								p.PassChar(',');
								c[1] = p.ReadInt();
								p.PassChar(',');
								c[2] = p.ReadInt();
								p.PassChar(')');
								
								for(int i = 0; i < 3; i++) {
									ColorRule& r = v.Add();
									r.color = i;
									r.dbl = c[i] * d;
									r.mode = ColorRule::MODE_CONSTANT;
								}
							}
							else {
								int rgb_i = -1;
								p.PassId("value");
								p.PassId("of");
								p.PassId("the");
								if (p.Id("first"))
									rgb_i = 0;
								else if (p.Id("second"))
									rgb_i = 1;
								else if (p.Id("third"))
									rgb_i = 2;
								else if (p.Id("last"))
									rgb_i = 2;
								else TODO
								if (!p.Id("vowel"))
									p.PassId("letter");
								
								ColorRule& r = v.Add();
								r.color = rgb_i;
								r.dbl = d;
								r.mode = ColorRule::MODE_MULTIPLY_COLOR;
							}
						}
						else if (p.Id("sum")) {
							p.PassId("of");
							p.Id("the");
							p.PassId("RGB");
							p.PassId("values");
							p.PassId("of");
							p.PassId("all");
							if (p.Id("vowels") || p.Id("consonants"))
								;
							else {
								if (!p.Id("numbers"))
									p.PassId("letters");
								
								for(int i = 0; i < 3; i++) {
									ColorRule& r = v.Add();
									r.color = i;
									r.dbl = d;
									r.mode = ColorRule::MODE_MULTIPLY_AVERAGE;
								}
							}
						}
						else TODO
					}
					else if (p.Id("sum")) {
						p.PassId("of");
						p.Id("the");
						p.PassId("RGB");
						p.PassId("values");
						p.PassId("of");
						if (p.Id("first")) {
							p.PassId("three");
						}
						else if (p.Id("last")) {
							p.PassId("three");
						}
						else p.PassId("all");
						if (p.Id("vowels") || p.Id("consonants"))
							;
						else {
							if (!p.Id("numbers") && !p.Id("digits"))
								p.PassId("letters");
							
							for(int i = 0; i < 3; i++) {
								ColorRule& r = v.Add();
								r.color = i;
								r.dbl = 1;
								r.mode = ColorRule::MODE_MULTIPLY_AVERAGE;
							}
						}
					}
					else if (p.Id("RGB")) {
						p.PassId("value");
						p.PassId("of");
						p.Id("the");
						int rgb_i = -1;
						if (p.Id("first"))
							rgb_i = 0;
						else if (p.Id("second"))
							rgb_i = 1;
						else if (p.Id("third"))
							rgb_i = 2;
						else if (p.Id("last"))
							rgb_i = 2;
						else
							TODO
						p.PassId("letter");
						
						{
							ColorRule& r = v.Add();
							r.color = rgb_i;
							r.dbl = 1;
							r.mode = ColorRule::MODE_MULTIPLY_COLOR;
						}
					}
					else TODO
					
					p.PassChar(')');
				}
				else if (p.Id("RGB")) {
					int c[3];
					p.PassChar('(');
					c[0] = p.ReadInt();
					p.PassChar(',');
					c[1] = p.ReadInt();
					p.PassChar(',');
					c[2] = p.ReadInt();
					p.PassChar(')');
					
					for(int i = 0; i < 3; i++) {
						ColorRule& r = v.Add();
						r.color = i;
						r.dbl = c[i];
						r.mode = ColorRule::MODE_CONSTANT;
					}
				}
				else if (p.Id("sum")) {
					p.PassId("of");
					p.Id("the");
					p.PassId("RGB");
					p.PassId("values");
					p.PassId("of");
					p.PassId("all");
					p.PassId("letters");
					
					for(int i = 0; i < 3; i++) {
						ColorRule& r = v.Add();
						r.color = i;
						r.dbl = 1;
						r.mode = ColorRule::MODE_MULTIPLY_AVERAGE;
					}
				}
				else if (p.Char(')')) {
					// error, but pass
				}
				else TODO
			}
		}
		catch (Exc e) {
			LOG(e);
			TODO
		}
	}
	
	String code;
	code << "void PutKeyColor(const char* key, int ir, int ig, int ib, double& r, double& g, double& b, double& sr, double& sg, double& sb) {\n";
	code << "\tint av = (ir + ig + ib) / 3;\n";
	
	Index<String> keys_ready;
	for(int i = 0; i < out.GetCount(); i++) {
		String key = out.GetKey(i);
		if (keys_ready.Find(key) >= 0)
			continue;
		keys_ready.Add(key);
		
		key.Replace("\"", "\\\"");
		
		if (1) {
			Vector<ColorRule>& v = out[i];
			if (v.IsEmpty()) continue;
			String dbg = key + ": Color(";
			for(int j = 0; j < 3; j++) {
				if (j) dbg << ", ";
				int chr;
				switch (j) {
					case 0: chr = 'r'; break;
					case 1: chr = 'g'; break;
					case 2: chr = 'b'; break;
				}
				bool empty = true;
				for(int k = 0, count = 0; k < v.GetCount(); k++) {
					ColorRule& r = v[k];
					if (r.color != j) continue;
					if (count++) dbg << " + ";
					empty = false;
					switch (r.mode) {
						case ColorRule::MODE_MULTIPLY_COLOR: if (r.dbl != 1) dbg << r.dbl << " * "; dbg.Cat(chr); break;
						case ColorRule::MODE_MULTIPLY_AVERAGE:  if (r.dbl != 1) dbg << r.dbl << " * "; dbg << "av"; break;
						case ColorRule::MODE_CONSTANT: dbg << r.dbl; break;
						default: break;
					}
				}
				if (empty) dbg << "0";
			}
			dbg << ")";
			LOG(dbg);
		}
		
		if (1) {
			Vector<ColorRule>& v = out[i];
			if (v.IsEmpty()) continue;
			code << "\tif (strncmp(key, \"" << key << "\", " << key.GetCount() << ") == 0) {\n";
			
			for(int j = 0; j < 3; j++) {
				int chr;
				switch (j) {
					case 0: chr = 'r'; break;
					case 1: chr = 'g'; break;
					case 2: chr = 'b'; break;
				}
				for(int k = 0, count = 0; k < v.GetCount(); k++) {
					ColorRule& r = v[k];
					if (r.color != j)
						continue;
					switch (r.mode) {
					case ColorRule::MODE_MULTIPLY_COLOR:
						code << "\t\t";
						code.Cat(chr);
						code << " += ";
						if (r.dbl != 1)
							code << r.dbl << " * ";
						code << "i";
						code.Cat(chr);
						code << "; s";
						code.Cat(chr);
						code << " += " << r.dbl << " * 255;\n";
						break;
						
					case ColorRule::MODE_MULTIPLY_AVERAGE:
						code << "\t\t";
						code.Cat(chr);
						code << " += ";
						if (r.dbl != 1)
							code << r.dbl << " * ";
						code << "av; s";
						code.Cat(chr);
						code << " += " << r.dbl << " * 255;\n";
						break;
						
					case ColorRule::MODE_CONSTANT:
						code << "\t\t";
						code.Cat(chr);
						code << " += ";
						code << r.dbl;
						code << "; s";
						code.Cat(chr);
						code << " += 255;\n";
						break;
						
					default: break;
					}
				}
			}
			code << "\t\treturn;\n";
			code << "\t}\n";
		}
	}
	code << "}\n";
	
	LOG(code);
	
}
