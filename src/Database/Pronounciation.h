#ifndef _Database_Pronounciation_h_
#define _Database_Pronounciation_h_


class EnglishPronounciation {
	Vector<WString> parts;
	Vector<int> word_is;
	Vector<WString> pron_parts;
	WString pronounciation;
	Color base_clr, blended_clr;
	
	/*struct ColorWeight : Moveable<ColorWeight> {
		Color clr;
		int syllables;
	};
	Vector<ColorWeight> tmp_clrs;*/
	
public:
	typedef EnglishPronounciation CLASSNAME;
	EnglishPronounciation();
	
	void Clear();
	void SetBaseColor(const Color& clr) {base_clr = clr;}
	bool Parse(const String& s);
	bool Parse(const String& s, DatasetAnalysis& ds);
	bool ParseMore(const String& s, DatasetAnalysis& ds);
	bool ParseFinish(DatasetAnalysis& ds);
	const WString& GetPronounciation() const {return pronounciation;}
	double GetInternalRhyming() const;
	int GetSyllableCount() const;
	const Color& GetColor() const {return blended_clr;}
	
	static bool IsPunct(int chr);
	
};

#endif
