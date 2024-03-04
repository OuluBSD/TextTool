#include "SocialDataCtrl.h"

#if 0


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
	structures.AddIndex("IDX");
	structures.ColumnWidths("3 1");
	structures.WhenCursor << THISBACK(DataStructure);
	
	clauses.AddColumn(t_("Structure args"));
	clauses.AddColumn(t_("Count"));
	clauses.ColumnWidths("3 1");
	clauses.WhenCursor << THISBACK(DataStructureHeader);
	
	phrases.AddColumn(t_("Sentence parts"));
	phrases.AddColumn(t_("Structure type"));
	phrases.AddColumn(t_("Part types"));
	phrases.AddIndex("IDX");
	phrases.ColumnWidths("3 1 3");
	
}

void LineTypesPage::EnableAll() {
	
}

void LineTypesPage::DisableAll() {
	
}

void LineTypesPage::Data() {
	
}

void LineTypesPage::DataMain() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	
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
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	uniq_st.Clear();
	for(int i = 0; i < da.structure_types.GetCount(); i++) {
		const StructureType& st = da.structure_types[i];
		String s = Join(st.part_types, ", ");
		uniq_st.GetAdd(st.struct_type).GetAdd(s, 0)++;
	}
	
	structures.Set(0, 0, "All");
	total_struct_count = 0;
	for(int i = 0; i < uniq_st.GetCount(); i++) {
		const auto& v = uniq_st[i];
		int c = uniq_st[i].GetCount();
		structures.Set(1+i, 0, uniq_st.GetKey(i));
		structures.Set(1+i, 1, c);
		structures.Set(1+i, "IDX", i);
		total_struct_count += c;
	}
	structures.Set(0, 1, total_struct_count);
	INHIBIT_CURSOR(structures);
	structures.SetCount(1+da.structure_types.GetCount());
	structures.SetSortColumn(1, true);
	if (!structures.IsCursor() && structures.GetCount())
		structures.SetCursor(0);
	
	DataStructure();
}

void LineTypesPage::DataStructure() {
	if (!datasets.IsCursor() || !structures.IsCursor())
		return;
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	INHIBIT_CURSOR(clauses);
	int struct_i = structures.GetCursor();
	if (struct_i == 0) {
		clauses.SetCount(1);
		clauses.Set(0, 0, "All");
		clauses.Set(0, 1, total_struct_count);
	}
	else {
		struct_i = structures.Get(struct_i, "IDX");
		const auto& v = uniq_st[struct_i];
		clauses.Set(0, 0, "All");
		int total = 0;
		clauses.Set(0, 1, da.structure_phrases.GetCount());
		for(int i = 0; i < v.GetCount(); i++) {
			int c = v[i];
			clauses.Set(1+i, 0, v.GetKey(i));
			clauses.Set(1+i, 1, c);
			total += c;
		}
		clauses.Set(0,1,total);
		clauses.SetCount(1+v.GetCount());
	}
	if (!clauses.IsCursor() && clauses.GetCount())
		clauses.SetCursor(0);
	
	DataStructureHeader();
}

void LineTypesPage::DataStructureHeader() {
	if (!datasets.IsCursor() || !structures.IsCursor() || !clauses.IsCursor())
		return;
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String structure = structures.Get(0);
	String clause = clauses.Get(0);
	bool filter_structure = structure != "All";
	bool filter_clause = clause != "All";
	
	int idx = -1;
	int row = 0;
	for (const StructurePhrase& sp : da.structure_phrases) {
		const StructureType& st = da.structure_types[sp.type];
		idx++;
		if (filter_structure) {
			if (st.struct_type != structure)
				continue;
		}
		if (filter_clause) {
			bool found = false;
			for (const auto& a : st.part_types) {
				if (a == clause) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		
		phrases.Set(row, 0, Join(sp.sent_parts, " + "));
		
		SetColoredListValue(phrases, row, 1, st.struct_type, st.clr);
		SetColoredListValue(phrases, row, 2, Join(st.part_types, " + "), st.clr);
		
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
	
	if (running1)
		bar.Add(t_("Stop getting structure colors"), AppImg::RedRing(), THISBACK(ToggleGettingStructureColors)).Key(K_F7);
	else
		bar.Add(t_("Start getting structure colors"), AppImg::RedRing(), THISBACK(ToggleGettingStructureColors)).Key(K_F7);
	
}

void LineTypesPage::UpdateBatches() {
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
					if (l.GetCount() > 200)
						l = l.Left(200);
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

void LineTypesPage::ToggleGettingLineStructures() {
	running0 = !running0;
	if (running0) {
		UpdateBatches();
		Thread::Start(THISBACK1(GetLineStructures, 0));
	}
}

void LineTypesPage::ToggleGettingStructureColors() {
	running1 = !running1;
	if (running1) {
		Thread::Start(THISBACK1(GetStructureColors, 0));
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
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	ProgramDataAnalysisArgs args;
	args.fn = 13;
	args.phrases <<= Split(batch.txt, "\n");
	
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	TaskMgr& m = tm.MakePipe();
	m.GetProgramDataAnalysis(args, THISBACK1(OnLineStructures, batch_i), true);
}

void LineTypesPage::OnLineStructures(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	Batch& batch = batches[batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	if (!batch_i) {
		da.structure_types.Clear();
		da.structure_phrases.Clear();
		prev_st_i = -1;
	}
	
	Batch* prev_batch = batch_i > 0 && !batch.program_begins ? &batches[batch_i-1] : 0;
	if (batch.program_begins)
		prev_st_i = -1;
	
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
		String struct_type = TrimBoth(parts[2]);
		
		CombineHash ch;
		ch.Do(struct_type);
		for (auto& s : part_types) {
			s = TrimBoth(s);
			ch.Do(s);
		}
		hash_t type_hash = ch;
		
		int cur_st_i = da.structure_types.Find(type_hash);
		if (cur_st_i < 0) {
			cur_st_i = da.structure_types.GetCount();
			StructureType& st = da.structure_types.Add(type_hash);
			Swap(st.part_types, part_types);
			st.struct_type = struct_type;
		}
		StructureType& st = da.structure_types[cur_st_i];
	
		int sp_i = da.structure_phrases.GetCount();
		StructurePhrase& sp = da.structure_phrases.Add();
		Swap(sp.sent_parts, sent_parts);
		for (String& s : sp.sent_parts) {s = TrimBoth(s); RemoveQuotes(s); s = TrimBoth(s);}
		st.phrases << sp_i;
		sp.type = cur_st_i;
		
		if (prev_st_i >= 0) {
			StructureType& st = da.structure_types[prev_st_i];
			st.transition_to << cur_st_i;
		}
		
		prev_st_i = cur_st_i;
	}
	
	if (running0)
		PostCallback(THISBACK1(GetLineStructures, batch_i+1));
	
}

void LineTypesPage::GetStructureColors(int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	int begin = batch_i * per_color_batch;
	int end = begin + per_color_batch;
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	ProgramDataAnalysisArgs args;
	args.fn = 14;
	
	end = min(end, da.structure_types.GetCount());
	for (int i = begin; i < end; i++) {
		const StructureType& st = da.structure_types[i];
		String s = Join(st.part_types, " + ") + " == " + st.struct_type;
		args.phrases << s;
	}
	
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	TaskMgr& m = tm.MakePipe();
	m.GetProgramDataAnalysis(args, THISBACK1(OnStructureColors, batch_i), true);
}

void LineTypesPage::OnStructureColors(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	Batch& batch = batches[batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty() || !IsDigit(l[0])) {lines.Remove(i--); continue;}
		RemoveLineNumber(l);
	}
	
	// - independent-clause + interjection == interrogative-sentence: RGB(255,254,1)
	
	for (String& s : lines) {
		Vector<String> parts = Split(s, "==");
		if (parts.GetCount() != 2) continue;
		Vector<String> clauses = Split(parts[0], "+");
		for (String& c : clauses) c = TrimBoth(c);
		String& s1 = parts[1];
		int a = s1.Find(":");
		if (a < 0) continue;
		String structure = TrimBoth(s1.Left(a));
		a = s1.Find("RGB(", a+1);
		if (a < 0) continue;
		a += 4;
		int b = s1.Find(")");
		String clr_str = s1.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		CombineHash ch;
		ch.Do(structure);
		for (String& clause : clauses) {
			clause = TrimBoth(clause);
			ch.Do(clause);
		}
		hash_t h = ch;
		
		int i = da.structure_types.Find(h);
		if (i >= 0) {
			StructureType& st = da.structure_types[i];
			st.clr = clr;
		}
	}
	
	if (running1)
		PostCallback(THISBACK1(GetStructureColors, batch_i+1));
	
}

#endif
