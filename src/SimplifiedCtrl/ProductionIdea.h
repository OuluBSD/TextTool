#ifndef _SimplifiedCtrl_ProductionIdea_h_
#define _SimplifiedCtrl_ProductionIdea_h_



class ProductionIdea : public SongToolCtrl {
	Splitter hsplit, vsplit;
	
	ArrayCtrl list, positive, negative;
	
public:
	typedef ProductionIdea CLASSNAME;
	ProductionIdea();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	/*void GetProductionIdea();
	void CopyAttributes();
	void GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	void OnProductionIdea(String result, Song* song);
	*/
};


#endif
