#include "SongDataCtrl.h"


LineTypesPage::LineTypesPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << phrases;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << structures << clauses;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	structures.AddColumn(t_("Structure"));
	structures.AddColumn(t_("Count"));
	structures.ColumnWidths("3 1");
	structures.WhenCursor << THISBACK(DataStructure);
	
	clauses.AddColumn(t_("Structure args"));
	clauses.AddColumn(t_("Count"));
	clauses.ColumnWidths("3 1");
	clauses.WhenCursor << THISBACK(DataStructureHeader);
	
	phrases.AddColumn(t_("Sentence parts"));
	phrases.AddColumn(t_("Part types"));
	phrases.AddColumn(t_("Structure type"));
	phrases.AddIndex("IDX");
	phrases.ColumnWidths("3 3 1");
	
}

void LineTypesPage::EnableAll() {
	
}

void LineTypesPage::DisableAll() {
	
}

void LineTypesPage::Data() {
	
}

void LineTypesPage::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}
	
void LineTypesPage::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	structures.Set(0, 0, "All");
	total_struct_count = 0;
	for(int i = 0; i < da.structure_headers.GetCount(); i++) {
		const StructureHeader& sp = da.structure_headers[i];
		structures.Set(1+i, 0, da.structure_headers.GetKey(i));
		structures.Set(1+i, 1, sp.count);
		total_struct_count += sp.count;
	}
	structures.Set(0, 1, total_struct_count);
	INHIBIT_CURSOR(structures);
	structures.SetCount(1+da.structure_headers.GetCount());
	if (!structures.IsCursor() && structures.GetCount())
		structures.SetCursor(0);
	
	DataStructure();
}

void LineTypesPage::DataStructure() {
	if (!datasets.IsCursor() || !structures.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int struct_i = structures.GetCursor();
	if (struct_i == 0) {
		clauses.SetCount(1);
		clauses.Set(0, 0, "All");
		clauses.Set(0, 1, total_struct_count);
	}
	else {
		struct_i--;
		const StructureHeader& sp = da.structure_headers[struct_i];
		clauses.Set(0, 0, "All");
		clauses.Set(0, 1, sp.count);
		for(int i = 0; i < sp.clauses.GetCount(); i++) {
			clauses.Set(1+i, 0, sp.clauses.GetKey(i));
			clauses.Set(1+i, 1, sp.clauses[i]);
		}
		clauses.SetCount(1+sp.clauses.GetCount());
	}
	if (!clauses.IsCursor() && clauses.GetCount())
		clauses.SetCursor(0);
	
	DataStructureHeader();
}

void LineTypesPage::DataStructureHeader() {
	if (!datasets.IsCursor() || !structures.IsCursor() || !clauses.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String structure = structures.Get(0);
	String clause = clauses.Get(0);
	bool filter_structure = structure != "All";
	bool filter_clause = clause != "All";
	
	int idx = -1;
	int row = 0;
	for (const StructurePhrase& sp : da.structure_phrases) {
		idx++;
		if (filter_structure) {
			if (sp.struct_type != structure)
				continue;
		}
		if (filter_clause) {
			bool found = false;
			for (const auto& a : sp.part_types) {
				if (a == clause) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		
		phrases.Set(row, 0, Join(sp.sent_parts, " + "));
		phrases.Set(row, 1, Join(sp.part_types, " + "));
		phrases.Set(row, 2, sp.struct_type);
		
		phrases.Set(row, "IDX", idx);
		row++;
	}
	INHIBIT_CURSOR(phrases);
	phrases.SetCount(row);
	phrases.SetSortColumn(1+structure_cols, true);
	if (!phrases.IsCursor() && phrases.GetCount())
		phrases.SetCursor(0);
	
}

void LineTypesPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update batches"), AppImg::BlueRing(), THISBACK(UpdateBatches)).Key(K_F5);
	bar.Separator();
	if (running0)
		bar.Add(t_("Stop getting line structures"), AppImg::RedRing(), THISBACK(ToggleGettingLineStructures)).Key(K_F6);
	else
		bar.Add(t_("Start getting line structures"), AppImg::RedRing(), THISBACK(ToggleGettingLineStructures)).Key(K_F6);
	
}

void LineTypesPage::UpdateBatches() {
	Database& db = Database::Single();
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

void LineTypesPage::ToggleGettingLineStructures() {
	running0 = !running0;
	if (running0) {
		if (0) {
			Database& db = Database::Single();
			SongData& sd = db.song_data;
			SongDataAnalysis& sda = db.song_data.a;
			for (DatasetAnalysis& da : sda.datasets) {
				da.structure_headers.Clear();
			}
		}
		UpdateBatches();
		Thread::Start(THISBACK1(GetLineStructures, 0));
	}
}

void LineTypesPage::GetLineStructures(int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	if (batches.IsEmpty()) UpdateBatches();
	if (batch_i < 0 || batch_i >= batches.GetCount()) {
		this->batch = false;
		return;
	}
	Batch& batch = batches[batch_i];
	tmp_batch_i = batch_i;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	SongDataAnalysisArgs args;
	args.fn = 13;
	args.phrases <<= Split(batch.txt, "\n");
	
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	TaskMgr& m = tm.MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnLineStructures, batch_i));
}

void LineTypesPage::OnLineStructures(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Batch& batch = batches[tmp_batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	if (!batch_i)
		da.structure_headers.Clear();
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty() || !IsDigit(l[0])) {lines.Remove(i--); continue;}
		RemoveLineNumber(l);
	}
	
	Vector<String> txt_lines = Split(batch.txt, "\n");
	
	// e.g. 1. "With the birds" + "Ill share this lonely view" == prepositional-phrase + independent-clause == prepositional-clause
	
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty())
			continue;
		
		Vector<String> parts = Split(l, "==");
		if (parts.GetCount() != 3)
			continue;
		Vector<String> sent_parts = Split(parts[0], "+");
		Vector<String> part_types = Split(parts[1], "+");
		if (sent_parts.GetCount() != part_types.GetCount())
			continue;
		
		StructurePhrase& sp = da.structure_phrases.Add();
		sp.struct_type = TrimBoth(parts[2]);
		Swap(sp.sent_parts, sent_parts);
		Swap(sp.part_types, part_types);
		for (String& s : sp.sent_parts) {s = TrimBoth(s); RemoveQuotes(s); s = TrimBoth(s);}
		for (String& s : sp.part_types) s = TrimBoth(s);
		
		StructureHeader& sh = da.structure_headers.GetAdd(sp.struct_type);
		sh.count++;
		for (const auto& s : sp.part_types)
			sh.clauses.GetAdd(s,0)++;
	}
	
	if (running0)
		PostCallback(THISBACK1(GetLineStructures, batch_i+1));
	
}
