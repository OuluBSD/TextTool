#ifndef _SongTool_Analysis_h_
#define _SongTool_Analysis_h_


struct Analysis {
	int year = 0;
	String title;
	VectorMap<String,String> data;
	
	void Clear() {
		year = 0;
		title.Clear();
		data.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("data", data)
			;
	}
	bool operator()(const Analysis& a, const Analysis& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
		return false;
	}
	
};


#endif
