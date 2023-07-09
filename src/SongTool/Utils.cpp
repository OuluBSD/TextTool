#include "SongTool.h"


String Capitalize(String s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}
