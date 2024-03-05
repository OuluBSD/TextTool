#ifndef _SongDatabase_Typecast_h_
#define _SongDatabase_Typecast_h_


BEGIN_SONGLIB_NAMESPACE


struct Typecast : DataFile {
	Array<Archetype>	archetypes;
	
	
	int GetLyricsCount() const;
};


END_SONGLIB_NAMESPACE


#endif
