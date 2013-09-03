#ifndef _SongTool_MusicVideoToPlan_h_
#define _SongTool_MusicVideoToPlan_h_


class MusicVideoToPlan : public ImagePlayerBase {
	Button import_screenplay, make_single_image, make_all_images;
	String english_screenplay_auto;
	
public:
	typedef MusicVideoToPlan CLASSNAME;
	MusicVideoToPlan();
	
	void Init() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void ImportScreenplay();
	void MakeSingleImage();
	void MakeAllImages();
	
};


#endif
