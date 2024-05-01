#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


Script& Content::GetAddScript(String name) {
	String file_title = MakeTitle(name);
	for(Script& s : scripts) {
		if (s.file_title == file_title)
			return s;
	}
	Script& s = scripts.Add();
	s.file_title = file_title;
	return s;
}


END_TEXTLIB_NAMESPACE

