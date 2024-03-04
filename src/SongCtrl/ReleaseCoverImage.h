#ifndef _SongTool_CoverImage_h_
#define _SongTool_CoverImage_h_


class ReleaseCoverImage : public ImagePlayerBase {
	Button create_suggestions, make_single_image, make_all_images;
	String user_natural_english_key;
	
public:
	typedef ReleaseCoverImage CLASSNAME;
	ReleaseCoverImage();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	void CreateSuggestionsForPrompts();
	void MakeSingleImage();
	void MakeAllImages();
	void PostOnResult(String res, String part) {PostCallback(THISBACK2(OnResult, res, part));}
	void OnResult(String res, String part);
	
};


#endif
