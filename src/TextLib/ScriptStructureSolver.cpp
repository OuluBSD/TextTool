#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


hash_t TextDescriptor::GetHash(int c) const {
	CombineHash ch;
	c = max(0,min(c, ITEM_COUNT));
	for(int i = 0; i < c; i++) {
		const TextDescriptor::Item& it = items[i];
		//if (i >= 3 && it.count <= 1) break;
		if (it.count < 1) break;
		ch.Do(it.from);
		ch.Do(it.to);
	}
	return ch;
}










ScriptStructureSolverBase::ScriptStructureSolverBase() {
	
}

void ScriptStructureSolverBase::Process(String s) {
	input = s;
		
	DefaultProcess();
}

void ScriptStructureSolverBase::SingleIteration() {
	debug_out.Clear();
	MakeLines();
	MakeUniqueLines();
	MakeSections();
	MakeRepeatingSectionLines();
	MakeSingleLineSections();
	MakeMetaSections();
}











MultiScriptStructureSolver::MultiScriptStructureSolver() {
	
}

ScriptStructureSolverBase& MultiScriptStructureSolver::Get() {
	switch (c) {
		case 0: return t1;
		case 1: return t2;
		case 2: return t3;
		case 3: return t4;
		default:
		case 4: return t5;
	}
}


END_TEXTLIB_NAMESPACE

