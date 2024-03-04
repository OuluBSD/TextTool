#ifndef _SongDatabase_Typecast_h_
#define _SongDatabase_Typecast_h_


struct Typecast : DataFile {
	Array<Archetype>	archetypes;
	
	
	int GetLyricsCount() const;
};


#endif
