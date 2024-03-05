#ifndef _SocialCtrl_PublicRelations_h_
#define _SocialCtrl_PublicRelations_h_


BEGIN_SOCIALLIB_NAMESPACE


class PublicRelationsCtrl : public ToolAppCtrl {
	
	
public:
	typedef PublicRelationsCtrl CLASSNAME;
	PublicRelationsCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


END_SOCIALLIB_NAMESPACE


#endif
