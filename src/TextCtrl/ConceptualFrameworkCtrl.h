#ifndef _TextCtrl_ConceptualFramework_h_
#define _TextCtrl_ConceptualFramework_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;


class ConceptualFrameworkNavigator : public ToolAppCtrl {
	Splitter cfsplit, vsplit, tsplit, bsplit;
	ArrayCtrl cfs;
	ArrayCtrl stories;
	WithConceptualFramework<Ctrl> cf;
	WithConceptualFrameworkStory<Ctrl> story;
	DocEdit story_struct, story_improved;
	int story_sort_column = 0;
	
public:
	typedef ConceptualFrameworkNavigator CLASSNAME;
	ConceptualFrameworkNavigator();
	
	void Data() override;
	void DataFramework();
	void DataStory();
	void OnValueChange();
	void Do(int fn);
	void MainLayout();
	void SideLayout();
	void GetElements(ConceptualFrameworkArgs& args);
	void MoveSortColumn(int fn);
	
	void ToolMenu(Bar& bar) override;
	
};

class ConceptualFrameworkCtrl : public ConceptualFrameworkNavigator {
	
public:
	typedef ConceptualFrameworkCtrl CLASSNAME;
	ConceptualFrameworkCtrl();
	
	void ToolMenu(Bar& bar) override;
	
	
};


END_TEXTLIB_NAMESPACE


#endif
