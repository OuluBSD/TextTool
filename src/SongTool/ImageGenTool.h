#ifndef _SongTool_ImageGenTool_h_
#define _SongTool_ImageGenTool_h_


class GeneratedImages : public Ctrl {
	
	
public:
	typedef GeneratedImages CLASSNAME;
	GeneratedImages();
	
	void Paint(Draw& d);
	
	Array<Image> images;
	
};

class ImageGenTool : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl recent;
	
	Ctrl main;
	EditString prompt;
	Button upload, generate;
	GeneratedImages images;
	
public:
	typedef ImageGenTool CLASSNAME;
	ImageGenTool();
	
	void Data() override;
	void Generate();
	void Upload();
	
};


#endif
