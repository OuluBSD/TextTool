#ifndef _Database_Typecast_h_
#define _Database_Typecast_h_


struct Typecast : DataFile {
	Array<Archetype>	archetypes;
	
	
	int GetLyricsCount() const;
};


#endif
