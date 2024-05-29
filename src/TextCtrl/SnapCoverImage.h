#ifndef _TextTool_SnapCoverImage_h_
#define _TextTool_SnapCoverImage_h_


BEGIN_TEXTLIB_NAMESPACE


class SnapCoverImage : public ToolAppCtrl {
	Splitter vsplit, hsplit[3];
	ArrayCtrl attr_list, sugg_list;
	DocEdit attr_text, sugg_text;
	ImageViewerCtrl suggestion[4];
	
	
public:
	typedef SnapCoverImage CLASSNAME;
	SnapCoverImage();
	
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void CreateSuggestionsForPrompts();
	void DataAttribute();
	void DataSuggestion();
	void DataSuggestionImage();
	void OnAttributeChange();
	void OnSuggestionChange();
	void Do(int fn);
	void SuggestionMenu(Bar& bar);
	
};


END_TEXTLIB_NAMESPACE


#endif
