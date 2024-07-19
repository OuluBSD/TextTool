#ifndef _ToolCore_TextComparison_h_
#define _ToolCore_TextComparison_h_


BEGIN_TEXTLIB_NAMESPACE


class TextComparison {
	WString a, b;
    Vector<int> sflags, aflags;
	Vector<WString> aw, bw;
	Vector<bool> a_in_use, b_in_use;
	VectorMap<int, double> link_weights;
	Vector<int> map_ab, unlinked_a, unlinked_b;
	
	double SCALING_FACTOR = 0.1;
	
public:
	typedef TextComparison CLASSNAME;
	TextComparison();
	double Process(const String& a, const String& b);
	
	double JaroWinklerDistance(const WString& s, const WString& a);
};


END_TEXTLIB_NAMESPACE


#endif
