#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


StructuredScriptEditor::StructuredScriptEditor() {
	
}

void StructuredScriptEditor::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	
	
}


END_TEXTLIB_NAMESPACE
