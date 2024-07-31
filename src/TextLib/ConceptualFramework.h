#ifndef _TextLib_ConceptualFramework_h_
#define _TextLib_ConceptualFramework_h_


BEGIN_TEXTLIB_NAMESPACE


class ConceptualFrameworkProcess : public SolverBase {
	
public:
	enum {
		PHASE_MAKE_ALL_CONCEPTS,
		PHASE_MAKE_TOPIC_CONCEPTS,
		PHASE_IMPROVE_STORY,
		PHASE_SCORE_CONCEPTS,
		PHASE_SORT_STORIES,
		PHASE_TYPECLASS,
		PHASE_CONTENT,
		PHASE_COLORS,
		PHASE_ATTRS,
		
		PHASE_COUNT,
	};
	
	Owner* owner = 0;
	Profile* profile = 0;
	Concept* cf = 0;
	BiographySnapshot* snap = 0;
	int appmode = -1;
	ConceptualFrameworkArgs args;
	Vector<int> el_items;
	
	// Params
	int elements_per_story_prompt = 30;
	int max_stories = 30;
	int max_topic_stories = 3;
	int max_detailed_count = 100;
	
	
	
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
	
	static ConceptualFrameworkProcess& Get(Profile& p, Concept& cf, BiographySnapshot& snap);
	
private:
	void MakeAllConcepts();
	void MakeTopicConcepts();
	void ImproveStory();
	void ScoreConcepts();
	void SortStories();
	void GetTypeclass();
	void GetContent();
	void GetColors();
	void GetAttrs();
	void OnMakeConcepts(String res);
};


END_TEXTLIB_NAMESPACE


#endif
