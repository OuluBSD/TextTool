#ifndef _SongCtrl_ContextAttributes_h_
#define _SongCtrl_ContextAttributes_h_

#if 0

class ContextAttributesCtrl : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl artist, release, song;
	int ds_i = 0;
	
	void InitList(ArrayCtrl& a);
	
public:
	typedef ContextAttributesCtrl CLASSNAME;
	ContextAttributesCtrl();
	
	void Init();
	void ToolMenu(Bar& bar) override;
	void Data() override;
	void SetData(VectorMap<String,String>& data, ArrayCtrl& a);
	void OnAttrChange(VectorMap<String,String>* data, const char* s, DropList* dl);
	
	
	
};


#endif
#endif
