#ifndef _TextCtrl_BiographyGenerator_h_
#define _TextCtrl_BiographyGenerator_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyGeneratorCtrl : public ToolAppCtrl {
	
public:
	typedef BiographyGeneratorCtrl CLASSNAME;
	BiographyGeneratorCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
