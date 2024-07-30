#ifndef _TextLib_ConceptualFramework_h_
#define _TextLib_ConceptualFramework_h_


BEGIN_TEXTLIB_NAMESPACE


class ConceptualFrameworkProcess : public SolverBase {
	
public:
	enum {
		PHASE_MAKE_ALL_CONCEPTS,
		PHASE_MAKE_TOPIC_CONCEPTS,
		PHASE_SCORE_CONCEPTS,
		
		PHASE_COUNT,
	};
	
	Entity* entity = 0;
	Owner* owner = 0;
	Profile* profile = 0;
	Concept* cf = 0;
	BiographySnapshot* snap = 0;
	
	// Params
	int elements_per_concept_prompt = 30;
	int max_concepts = 100;
	
	
	
	struct Item : Moveable<Item> {
		int cat_i, year_i, el_i;
		double av;
		
		bool operator()(const Item& a, const Item& b) const {return a.av > b.av;}
	};
	Vector<Item> items;
	
	struct Task : Moveable<Task> {
		int begin, end;
	};
	Vector<Task> tasks;
	
public:
	typedef ConceptualFrameworkProcess CLASSNAME;
	ConceptualFrameworkProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ConceptualFrameworkProcess& Get(Entity& e, Concept& cf, BiographySnapshot& snap);
	
private:
	void MakeAllConcepts();
	void MakeTopicConcepts();
	void ScoreConcepts();
	
};


END_TEXTLIB_NAMESPACE


#endif
