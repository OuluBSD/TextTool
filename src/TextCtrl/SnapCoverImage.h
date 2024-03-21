#ifndef _TextTool_SnapCoverImage_h_
#define _TextTool_SnapCoverImage_h_


BEGIN_TEXTLIB_NAMESPACE


class SnapCoverImage : public ImagePlayerBase {
	Button create_suggestions, make_single_image, make_all_images;
	String user_natural_english_key;
	
public:
	typedef SnapCoverImage CLASSNAME;
	SnapCoverImage();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void CreateSuggestionsForPrompts();
	void MakeSingleImage();
	void MakeAllImages();
	void PostOnResult(String res, String part) {PostCallback(THISBACK2(OnResult, res, part));}
	void OnResult(String res, String part);
	
};


END_TEXTLIB_NAMESPACE


#endif
