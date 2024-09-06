#ifndef _TextCtrl_VideoStoryboardCtrl_h_
#define _TextCtrl_VideoStoryboardCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class VideoStoryboardCtrl : public ToolAppCtrl {
	Splitter split, vsplit0, vsplit1;
	ArrayCtrl list;
	ImageViewerCtrl img[4];
	
public:
	typedef VideoStoryboardCtrl CLASSNAME;
	VideoStoryboardCtrl();
	
	void Data() override;
	void DataLine();
	void OnValueChange();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
