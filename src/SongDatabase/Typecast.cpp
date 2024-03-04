#include "SongDatabase.h"

int Typecast::GetLyricsCount() const {
	int total = 0;
	for (const auto& a : archetypes)
		total += a.lyrics.GetCount();
	return total;
}
