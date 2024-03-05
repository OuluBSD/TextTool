#include "SongDatabase.h"


BEGIN_SONGLIB_NAMESPACE


int Typecast::GetLyricsCount() const {
	int total = 0;
	for (const auto& a : archetypes)
		total += a.lyrics.GetCount();
	return total;
}


END_SONGLIB_NAMESPACE

