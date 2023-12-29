#ifndef _SongTool_Block_h_
#define _SongTool_Block_h_

#if 0

struct BeatTime {
	int bar = 0;
	int parts = 0;
	int bar_div = 4;
	
	double GetNumber() const {
		ASSERT(parts >= 0 && parts < bar_div);
		return (double)bar + (double)parts / (double)bar_div;
	}
	void Serialize(Stream& s) {
		s	% bar
			% parts
			% bar_div;
	}
};

struct Block {
	BeatTime	begin, end;
	
	
	
	void Serialize(Stream& s) {
		s	% begin
			% end;
	}
	/*void Jsonize(JsonIO& json) {
		json
			("begin", begin)
			("end", end)
			;
	}*/
};


#endif
#endif
