#ifndef _TextCtrl_ConceptualFramework_h_
#define _TextCtrl_ConceptualFramework_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;


class ConceptualFrameworkCtrl : public ToolAppCtrl {
	
	
public:
	typedef ConceptualFrameworkCtrl CLASSNAME;
	ConceptualFrameworkCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
