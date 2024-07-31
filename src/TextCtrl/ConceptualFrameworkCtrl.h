#ifndef _TextCtrl_ConceptualFramework_h_
#define _TextCtrl_ConceptualFramework_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;


class ConceptualFrameworkCtrl : public ToolAppCtrl {
	Splitter cfsplit, vsplit, tsplit, bsplit;
	ArrayCtrl cfs;
	ArrayCtrl stories;
	WithConceptualFramework<Ctrl> cf;
	WithConceptualFrameworkStory<Ctrl> story;
	DocEdit story_struct, story_improved;
	int story_sort_column = 0;
	
public:
	typedef ConceptualFrameworkCtrl CLASSNAME;
	ConceptualFrameworkCtrl();
	
	void Data() override;
	void DataFramework();
	void DataStory();
	void Clear();
	void OnValueChange();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void MoveSortColumn(int fn);
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
