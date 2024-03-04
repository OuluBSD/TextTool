#include "SocialLib.h"

namespace SocialLib {

void Task::UpdateBatches(int per_batch) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	
	batches.SetCount(0);
	batches.Reserve(1000);
	
	Vector<hash_t> line_hashes;
	Vector<String> added_lines;
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		Vector<CompanyDataset>& companies = sd[ds_i];
		for(int i = 0; i < companies.GetCount(); i++) {
			CompanyDataset& company = companies[i];
			for(int j = 0; j < company.stories.GetCount(); j++) {
				StoryDataset& story = company.stories[j];
				Vector<String> lines = Split(story.text, "\n");
				for(int k = 0; k < lines.GetCount(); k++) {
					String& l = lines[k];
					l = TrimBoth(l);
					if (l.IsEmpty()) lines.Remove(k--);
				}
				if (lines.IsEmpty()) continue;
				added_lines.SetCount(0);
				line_hashes.SetCount(0);
				bool program_begins = true;
				for(int k = 0; k < lines.GetCount(); k++) {
					String l = TrimBoth(lines[k]);
					hash_t h = l.GetHashValue();
					if (VectorFind<hash_t>(line_hashes, h) >= 0)
						continue;
					line_hashes << h;
					added_lines << l;
					if (added_lines.GetCount() >= per_batch) {
						Batch& b = batches.Add();
						b.program_begins = program_begins;
						b.company = &company;
						b.story = &story;
						b.ds_i = ds_i;
						b.txt = Join(added_lines, "\n");
						ASSERT(b.txt.GetCount());
						added_lines.SetCount(0);
						program_begins = false;
					}
				}
				if (added_lines.GetCount()) {
					Batch& b = batches.Add();
					b.program_begins = program_begins;
					b.company = &company;
					b.story = &story;
					b.ds_i = ds_i;
					b.txt = Join(added_lines, "\n");
					ASSERT(b.txt.GetCount());
				}
			}
		}
	}
}

}
