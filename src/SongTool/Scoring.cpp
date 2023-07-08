#include "SongTool.h"


void PartScore::Realize() {
	Attributes& g = Database::Single().attrs;
	values.SetCount(g.scorings.GetCount());
	for (auto& v : values)
		v.SetCount(len, 0);
}

