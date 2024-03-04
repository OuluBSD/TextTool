#ifndef _SongCtrl_PublicRelations_h_
#define _SongCtrl_PublicRelations_h_


class PublicRelationsCtrl : public ToolAppCtrl {
	
	
public:
	typedef PublicRelationsCtrl CLASSNAME;
	PublicRelationsCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
