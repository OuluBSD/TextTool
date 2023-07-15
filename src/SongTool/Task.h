#ifndef _SongTool_Task_h_
#define _SongTool_Task_h_


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
	int actual = 0, total = 1;
	double values_min = +DBL_MAX;
	double values_max = -DBL_MAX;
	bool active = false;
	Vector<SnapAttrStr> result_attrs;
	GeneticOptimizer optimizer;
	
	// Hash affecting variables
	String txt;
	Vector<double> scores;
	Index<SnapAttrStr> mask_attrs;
	
	// Temp
	RWMutex lock;
	PatternSnap* snap = 0;
	PatternSnap* rev_snap = 0;
	
	void Store();
	void LoadHash(hash_t h);
	void Serialize(Stream& s) {
		s	% results
			% result_values
			% id
			% actual
			% total
			% values_min
			% values_max
			% active
			% result_attrs
			% optimizer
			% txt
			% scores
			% mask_attrs
			;
	}
	static hash_t HashFn(const String& txt, const Vector<double>& scores, const Index<SnapAttrStr>& mask_attrs) {
		CombineHash c;
		c << txt << scores;
		for (const SnapAttrStr& sa : mask_attrs) c << sa;
		return c;
	}
	hash_t GetHashValue() const {
		return HashFn(txt, scores, mask_attrs);
	}
};

#endif
