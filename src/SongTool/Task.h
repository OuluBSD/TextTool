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
	Vector<double> scores;
	int id = -1;
	int actual = 0, total = 1;
	double values_min = +DBL_MAX;
	double values_max = -DBL_MAX;
	bool active = false;
	Vector<SnapAttr> attrs;
	String txt;
	
	RWMutex lock;
	PatternSnap* snap = 0;
	
	void Serialize(Stream& s) {
		s	% results
			% result_values
			% scores
			% id
			% actual
			% total
			% values_min
			% values_max
			% active
			% attrs
			% txt
			;
	}
};

#endif
