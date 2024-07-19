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
	Vector<double> map_ab_weights;
	
	double SCALING_FACTOR = 0.1;
	
	static void ReplaceWithWhite(WString& a);
	
public:
	typedef TextComparison CLASSNAME;
	TextComparison();
	double Process(const String& a, const String& b);
	
	double JaroWinklerDistance(const WString& s, const WString& a);
};


struct SoftMatchString : Moveable<SoftMatchString> {
	Vector<String> lines;
	SoftMatchString() {}
	SoftMatchString(const Vector<String>& s) {lines <<= s;}
	SoftMatchString(const SoftMatchString& s) {lines <<= s.lines;}
	SoftMatchString(SoftMatchString&& s) {lines <<= s.lines;}
	void operator=(const SoftMatchString& s) {lines <<= s.lines;}
	void operator=(const Vector<String>& s) {lines <<= s;}
	bool operator==(const SoftMatchString& s) const;
	hash_t GetHashValue() const;
};

END_TEXTLIB_NAMESPACE


#endif
