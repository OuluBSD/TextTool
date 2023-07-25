#ifndef _SongTool_Task_h_
#define _SongTool_Task_h_


struct SnapContext;

struct TaskResult {
	int id = -1;
	double optimizer_score = 0;
	
	void Serialize(Stream& s) {
		s	% id
			% optimizer_score;
	}
};

struct ReverseTask {
	Array<TaskResult> results;
	Vector<double> result_values;
	int id = -1;
	int type = -1;
	int actual = 0, total = 1;
	double values_min = +DBL_MAX;
	double values_max = -DBL_MAX;
	bool active = false;
	Vector<SnapAttrStr> result_attrs[2];
	GeneticOptimizer optimizer;
	VectorMap<String, Tuple2<String,double>> impact_results;
	
	// Hash affecting variables
	String txt;
	Vector<double> scores;
	VectorMap<SnapAttrStr, int> mask_attrs;
	
	// Temp
	RWMutex lock;
	PatternSnap* snap = 0;
	PatternSnap* rev_snap = 0;
	SnapContext* ctx = 0;
	
	void Store();
	void LoadHash(hash_t h);
	void Serialize(Stream& s) {
		/*if (s.IsLoading()) lock.EnterWrite();
		else lock.EnterRead();*/
		s	% results
			% result_values
			% id
			% type
			% actual
			% total
			% values_min
			% values_max
			% active
			% result_attrs[0]
			% result_attrs[1]
			% optimizer
			% txt
			% scores
			% mask_attrs
			% impact_results
			;
		/*if (s.IsLoading()) lock.LeaveWrite();
		else lock.LeaveRead();*/
	}
	static hash_t HashFn(int type, const String& txt, const Vector<double>& scores, const VectorMap<SnapAttrStr, int>& mask_attrs) {
		CombineHash c;
		c << type << txt << scores;
		for (const SnapAttrStr& sa : mask_attrs.GetKeys()) c << sa;
		return c;
	}
	hash_t GetHashValue() const {
		return HashFn(type, txt, scores, mask_attrs);
	}
};

#endif
