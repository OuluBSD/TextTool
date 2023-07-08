#ifndef _SongTool_Story_h_
#define _SongTool_Story_h_


struct Story {
	int year = 0;
	String title;
	String meaning;
	String devices;
	String emotion;
	String references;
	String structure;
	String history;
	String storyline;
	String implications;
	
	void Clear() {
		year = 0;
		title.Clear();
		meaning.Clear();
		devices.Clear();
		emotion.Clear();
		references.Clear();
		structure.Clear();
		history.Clear();
		storyline.Clear();
		implications.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("meaning", meaning)
			("devices", devices)
			("emotion", emotion)
			("references", references)
			("structure", structure)
			("history", history)
			("storyline", storyline)
			("implications", implications)
			;
	}
	bool operator()(const Story& a, const Story& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
	}
	
};


#endif
