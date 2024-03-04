#ifndef _SocialTool_CoverImage_h_
#define _SocialTool_CoverImage_h_


class CoverImage : public ImagePlayerBase {
	Button create_suggestions, make_single_image, make_all_images;
	String user_natural_english_key;
	
public:
	typedef CoverImage CLASSNAME;
	CoverImage();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void CreateSuggestionsForPrompts();
	void MakeSingleImage();
	void MakeAllImages();
	void PostOnResult(String res, String part) {PostCallback(THISBACK2(OnResult, res, part));}
	void OnResult(String res, String part);
	
};


#endif
