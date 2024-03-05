#include "SongLib.h"


BEGIN_SONGLIB_NAMESPACE


void Task::UpdateBatches(int per_batch) {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	
	batches.SetCount(0);
	batches.Reserve(1000);
	
	Vector<hash_t> line_hashes;
	Vector<String> added_lines;
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		Vector<ArtistDataset>& artists = sd[ds_i];
		for(int i = 0; i < artists.GetCount(); i++) {
			ArtistDataset& artist = artists[i];
			for(int j = 0; j < artist.lyrics.GetCount(); j++) {
				LyricsDataset& lyrics = artist.lyrics[j];
				Vector<String> lines = Split(lyrics.text, "\n");
				for(int k = 0; k < lines.GetCount(); k++) {
					String& l = lines[k];
					l = TrimBoth(l);
					if (l.IsEmpty()) lines.Remove(k--);
				}
				if (lines.IsEmpty()) continue;
				added_lines.SetCount(0);
				line_hashes.SetCount(0);
				bool song_begins = true;
				for(int k = 0; k < lines.GetCount(); k++) {
					String l = TrimBoth(lines[k]);
					hash_t h = l.GetHashValue();
					if (VectorFind<hash_t>(line_hashes, h) >= 0)
						continue;
					line_hashes << h;
					added_lines << l;
					if (added_lines.GetCount() >= per_batch) {
						Batch& b = batches.Add();
						b.song_begins = song_begins;
						b.artist = &artist;
						b.lyrics = &lyrics;
						b.ds_i = ds_i;
						b.txt = Join(added_lines, "\n");
						ASSERT(b.txt.GetCount());
						added_lines.SetCount(0);
						song_begins = false;
					}
				}
				if (added_lines.GetCount()) {
					Batch& b = batches.Add();
					b.song_begins = song_begins;
					b.artist = &artist;
					b.lyrics = &lyrics;
					b.ds_i = ds_i;
					b.txt = Join(added_lines, "\n");
					ASSERT(b.txt.GetCount());
				}
			}
		}
	}
}


END_SONGLIB_NAMESPACE

