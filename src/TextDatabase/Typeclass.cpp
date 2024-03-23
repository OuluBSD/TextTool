#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


int Typeclass::GetScriptCount() const {
	int total = 0;
	for (const auto& a : contents)
		total += a.scripts.GetCount();
	return total;
}


END_TEXTLIB_NAMESPACE

