#ifndef _TextLib_ScriptGenerator_h_
#define _TextLib_ScriptGenerator_h_


BEGIN_TEXTLIB_NAMESPACE


template <class T>
void MakeBelief(Script& song, T& args, int fn) {
	MetaDatabase& mdb = MetaDatabase::Single();
	if (song.belief_i > 0) {
		int b_i = song.belief_i -1;
		Belief& b = mdb.beliefs[b_i];
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
}


class ScriptGenerator : public SolverBase {
	enum {
		LG_COLOR,
		LG_ATTR,
		LG_ACTION,
		LG_MAKE_SOURCE_POOL,
		LG_TRANSLATE,
		LG_MAKE_PHRASE_PAIRS,
		LG_MAKE_RHYMES,
		LG_GET_AI_SCORES,
		
		LG_COUNT
	};
	Entity* artist = 0;
	Script* scripts = 0;
	
	// params
	int per_sub_batch =  50;
	int pair_limit = 300;
	int phrase_limit = 150;
	int appmode = -1;
	
	// temp
	Vector<VectorMap<int,double>> phrases;
	Vector<int> tmp;
	int iter = 0;
	VectorMap<String, int> pp_is;
	
	void ProcessSourcePool();
	void ProcessTranslate();
	void ProcessPairPhrases();
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void ProcessRhymes();
	void ProcessScores();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	void OnProcessPairPhrases(String result);
	void OnProcessRhymes(String result);
	void OnProcessScores(String result);
	void OnProcessTranslate(String result);
	
	TextDatabase& GetDatabase() {return GetAppModeDatabase(appmode);}
	int GetTypeclassCount() {return TextLib::GetTypeclassCount(appmode);}
	int GetContentCount() {return TextLib::GetContentCount(appmode);}
	
public:
	typedef ScriptGenerator CLASSNAME;
	ScriptGenerator();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	static ScriptGenerator& Get(int appmode, Entity& a, Script& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
