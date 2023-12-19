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
	
	void Clear();
	bool Parse(const String& s);
	bool Parse(const String& s, DatasetAnalysis& ds);
	bool ParseMore(const String& s, DatasetAnalysis& ds);
	bool ParseFinish(DatasetAnalysis& ds);
	const WString& GetPronounciation() const {return pronounciation;}
	double GetInternalRhyming() const;
	
	static bool IsPunct(int chr);
	
};

#endif
