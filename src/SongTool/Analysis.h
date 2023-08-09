#ifndef _SongTool_Analysis_h_
#define _SongTool_Analysis_h_

#if 0
struct Analysis {
	VectorMap<String,String> data;
	
	void Clear() {
		data.Clear();
	}
	void Serialize(Stream& s) {
		s	% data;
	}
	/*void Jsonize(JsonIO& json) {
		json
			("data", data)
			;
	}*/
	
};


#endif
#endif
