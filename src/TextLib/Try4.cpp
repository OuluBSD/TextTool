#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TryNo4tStructureSolver::MakeSections() {
	
	int max_lines = uniq_lines[0].count;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		UniqueLine& ul = uniq_lines.Get(line.hash);
		line.repeatability = max_lines > 0 ? (double)(ul.count - 1) / (double)(max_lines - 1) : 0;
	}
	for(int i = 1; i < lines.GetCount()-1; i++) {
		Line& line0 = lines[i-1];
		Line& line1 = lines[i];
		Line& line2 = lines[i+1];
		if (line1.repeatability < 0.3 && line0.repeatability > 0.8 && line2.repeatability > 0.8) {
			line1.circumstacial_repeatability = (line0.repeatability + line2.repeatability) * 0.5;
		}
	}
}

void TryNo4tStructureSolver::MakeRepeatingSectionLines() {
	
}


END_TEXTLIB_NAMESPACE

