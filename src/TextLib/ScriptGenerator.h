#ifndef _TextLib_ScriptGenerator_h_
#define _TextLib_ScriptGenerator_h_


BEGIN_TEXTLIB_NAMESPACE


template <class T>
void MakeBelief(Belief& b, T& args, int fn) {
	for(int i = 0; i < b.attrs.GetCount(); i++) {
		Belief::Attr& a = b.attrs[i];
		String s;
		if (fn == 0)
			s = "S" + IntStr(i) + ": High " + ToLower(a.positive) + " score. Low score means that the phrase is " + ToLower(a.negative) + ".";
		else if (fn == 1)
			s = "high " + ToLower(a.positive) + " value, low " + ToLower(a.negative) + " value";
		args.scores << s;
	}
}

template <class T>
void MakeBelief(Script& song, T& args, int fn) {
	MetaDatabase& mdb = MetaDatabase::Single();
	if (song.belief_uniq != 0) {
		int b_i = mdb.FindBelief(song.belief_uniq);
		ASSERT(b_i >= 0);
		Belief& b = mdb.beliefs[b_i];
		MakeBelief(b, args, fn);
	}
}


class ScriptGenerator : public SolverBase {
	enum {
		PHASE_COLOR,
		PHASE_ATTR,
		PHASE_ACTION,
		PHASE_MAKE_SOURCE_POOL,
		PHASE_TRANSLATE,
		PHASE_MAKE_PHRASE_PAIRS,
		PHASE_MAKE_RHYMES,
		PHASE_GET_AI_SCORES,
		PHASE_COLLECT,
		
		PHASE_COUNT
	};
	Entity* artist = 0;
	Script* script = 0;
	
	// Params
	int per_attr_batch = 50;
	int per_translate_batch =  20;
	int per_rhymes_batch = 15;
	int per_scores_batch = 15;
	int max_phrase_parts = 100;
	int per_pair_phrases_batch =  50;
	int pair_limit = 300;
	int phrase_limit = 150;
	
	// temp
	Vector<VectorMap<int,double>> phrases;
	Vector<int> tmp;
	int iter = 0;
	VectorMap<String, int> pp_is;
	int estimated_sub_batches = 1;
	
	void ProcessSourcePool();
	void ProcessTranslate();
	void ProcessPairPhrases();
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void ProcessRhymes();
	void ProcessScores();
	void ProcessCollect();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	void OnProcessPairPhrases(String result);
	void OnProcessRhymes(String result);
	void OnProcessScores(String result);
	void OnProcessTranslate(String result);
	
	TextDatabase& GetDatabase() const {return GetAppModeDatabase(appmode);}
	int GetTypeclassCount() const {return TextLib::GetTypeclassCount(appmode);}
	int GetContentCount() const {return TextLib::GetContentCount(appmode);}
	
public:
	typedef ScriptGenerator CLASSNAME;
	ScriptGenerator();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	static ScriptGenerator& Get(int appmode, Entity& a, Script& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
