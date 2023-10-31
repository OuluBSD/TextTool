#ifndef _SimplifiedCtrl_ProductionIdea_h_
#define _SimplifiedCtrl_ProductionIdea_h_



class ProductionIdea : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl attrs;
	DocEdit result;
	
	ArrayCtrl genre, lyrics_type;
	ArrayCtrl pos_traits, neg_traits;
	ArrayCtrl pos_stereotypes, neg_stereotypes;
	
public:
	typedef ProductionIdea CLASSNAME;
	ProductionIdea();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	void GetProductionIdea();
	void CopyAttributes();
	void GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	void OnProductionIdea(String result, Song* song);
	
};


#endif
