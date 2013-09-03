#ifndef _SimplifiedCtrl_ProductionIdea_h_
#define _SimplifiedCtrl_ProductionIdea_h_



class ProductionIdeaPage : public SongToolCtrl {
	Splitter mainsplit, hsplit, vsplit;
	
	ArrayCtrl list, positive, negative, active_list;
	
public:
	typedef ProductionIdeaPage CLASSNAME;
	ProductionIdeaPage();
	
	void Data() override;
	void DataTopic();
	void DataActive();
	void AddActive(bool positive);
	void RemoveActive();
	void RandomizeActive();
	void RandomizeAll(bool skip_existing_topic);
	void FocusActiveIdea(String topic, String value);
	void ToolMenu(Bar& bar) override;
	String GetStatusText() override;
	/*void GetProductionIdea();
	void CopyAttributes();
	void GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v);
	void OnProductionIdea(String result, Song* song);
	*/
};


#endif
