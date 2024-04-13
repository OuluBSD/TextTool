#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void Task::UpdateBatches(int appmode, int per_batch) {
	TextDatabase& db = GetAppModeDatabase(appmode);
	SourceData& sd = db.src_data;
	
	batches.SetCount(0);
	batches.Reserve(1000);
	
	Vector<hash_t> line_hashes;
	Vector<String> added_lines;
	
	{
		Vector<EntityDataset>& entities = sd.entities;
		for(int i = 0; i < entities.GetCount(); i++) {
			EntityDataset& artist = entities[i];
			for(int j = 0; j < artist.scripts.GetCount(); j++) {
				ScriptDataset& scripts = artist.scripts[j];
				Vector<String> lines = Split(scripts.text, "\n");
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
						b.scripts = &scripts;
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
					b.scripts = &scripts;
					b.txt = Join(added_lines, "\n");
					ASSERT(b.txt.GetCount());
				}
			}
		}
	}
}


END_TEXTLIB_NAMESPACE

