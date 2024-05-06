#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String KeyToName(String s) {
	s = ToLower(s);
	String o;
	bool upper = true;
	for(int i = 0; i < s.GetCount(); i++) {
		int chr = s[i];
		if (chr == '_') {
			upper = true;
			o.Cat(' ');
			continue;
		}
		if (upper && chr >= 'a' && chr <= 'z')
			chr = ToUpper(chr);
		upper = false;
		o.Cat(chr);
	}
	return o;
}

int BiographyCategory::GetFilledCount() const {
	int c = 0;
	for (const BioYear& by : years)
		if (by.text.GetCount() || by.keywords.GetCount())
			c++;
	return c;
}

int BiographyCategory::GetFilledImagesCount() const {
	int c = 0;
	for (const BioYear& by : years)
		c += by.images.GetCount();
	return c;
}

String GetBiographyCategoryEnum(int i) {
	switch (i) {
		#define BIOCATEGORY(x) case BIOCATEGORY_##x: return #x;
		BIOCATEGORY_LIST
		#undef BIOCATEGORY
		default: return String();
	}
}

String GetBiographyCategoryKey(int i) {
	return KeyToName(GetBiographyCategoryEnum(i));
}


BioYear& BiographyCategory::GetAdd(int year) {
	for (BioYear& by : years)
		if (by.year == year)
			return by;
	BioYear& by = years.Add();
	by.year = year;
	return by;
}

BiographyCategory& Biography::GetAdd(Owner& o, int enum_) {
	String s = GetBiographyCategoryEnum(enum_);
	BiographyCategory& bc = categories.GetAdd(s);
	int now = GetSysTime().year;
	if (o.year_of_birth > 0 && o.year_of_birth <= now) {
		for(int i = o.year_of_birth; i <= now; i++) {
			BioYear& by = bc.GetAdd(i);
		}
	}
	return bc;
}


END_TEXTLIB_NAMESPACE

