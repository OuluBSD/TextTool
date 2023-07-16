#ifndef _SongTool_Track_h_
#define _SongTool_Track_h_

typedef enum : int {
	TT_NONE,
	TT_VOCAL,
	TT_CHORDS,
	TT_INSTRUMENT,
	TT_DRUMS,
	TT_FX,
} TrackType;



struct Track {
	String			name;
	TrackType		type = TT_NONE;
	Array<Block>	blocks;
	
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("type", (int&)type)
			("blocks", blocks)
			;
	}
};


#endif
