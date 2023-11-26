#ifndef _Database_Pronounciation_h_
#define _Database_Pronounciation_h_


class EnglishPronounciation {
	Vector<WString> parts;
	Vector<int> word_is;
	Vector<WString> pron_parts;
	WString pronounciation;
	
public:
	typedef EnglishPronounciation CLASSNAME;
	EnglishPronounciation();
	
	
	bool Parse(const String& s);
	WString GetPronounciation() const {return pronounciation;}
	double GetInternalRhyming() const;
	
	static bool IsPunct(int chr);
	
};

#endif
