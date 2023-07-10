#ifndef _SongTool_Analysis_h_
#define _SongTool_Analysis_h_


struct Analysis {
	VectorMap<String,String> data;
	
	void Clear() {
		data.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("data", data)
			;
	}
	
};


#endif
