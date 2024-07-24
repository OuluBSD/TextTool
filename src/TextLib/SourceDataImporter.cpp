#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE




SourceDataImporter::SourceDataImporter() {
	SetParallel();
}

int SourceDataImporter::GetPhaseCount() const {
	return PHASE_COUNT;
}

int SourceDataImporter::GetBatchCount(int phase) const {
	TextDatabase& db = GetDatabase();
	return db.src_data.entities.GetCount();
}

int SourceDataImporter::GetSubBatchCount(int phase, int batch) const {
	TextDatabase& db = GetDatabase();
	if (batch >= db.src_data.entities.GetCount())
		return 1;
	auto& entity = db.src_data.entities[batch];
	return entity.scripts.GetCount();
}

void SourceDataImporter::DoPhase() {
	switch (phase) {
		case PHASE_TOKENIZE:		Tokenize();		return;
		default: TODO;
	}
}

void SourceDataImporter::Tokenize() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	Vector<int> token_is;
	DatasetAnalysis& da = sd.a.dataset;
	Vector<EntityDataset>& entities = sd.entities;
	
	int well_filter_loss = 0, parse_loss = 0, foreign_loss = 0;
	
	int phase = this->phase, batch = this->batch, sub_batch = this->sub_batch;
	if (parallel) {
		WorkerData& w = Worker();
		phase = w.phase;
		batch = w.batch;
		sub_batch = w.sub_batch;
	}
	
	if (!batch && !sub_batch) {
		total = 0;
		actual = 0;
		ts.Reset();
	}
	
	if (batch >= db.src_data.entities.GetCount()) {
		NextPhase();
		return;
	}
	
	auto& entity = db.src_data.entities[batch];
	if (sub_batch >= entity.scripts.GetCount()) {
		NextBatch();
		return;
	}
	
	int worker_total = total++;
	
	auto& script = entity.scripts[sub_batch];
	
	String str = script.text;
	
	// Ignore files with hard ambiguities
	if (str.Find(" well ") >= 0) {
		// well or we'll... too expensive to figure out
		well_filter_loss++;
		NextSubBatch();
		return;
	}
	
	static thread_local BestStructureSolver solver;
	static thread_local NaturalTokenizer tk;
	
	tk.Clear();
	HotfixReplaceWord(str);
	if (!tk.Parse(str)) {
		parse_loss++;
		NextSubBatch();
		return;
	}
	if (filter_foreign && tk.HasForeign()) {
		foreign_loss++;
		NextSubBatch();
		return;
	}
	
	String script_title = entity.name + " - " + script.name;
	hash_t ss_hash = script_title.GetHashValue();
	
	int ss_i = da.scripts.Find(ss_hash);
	if (skip_ready && ss_i >= 0) {
		if (0) {
			data_lock.Enter();
			ScriptStruct& ss = da.scripts[ss_i];
			LOG(da.GetScriptDump(ss_i));
			data_lock.Leave();
		}
		NextSubBatch();
		return;
	}
	
	// Slow solver process
	solver.Process(script.text);
	
	data_lock.Enter();
	ScriptStruct& ss = da.scripts.GetAdd(ss_hash, ss_i);
	
	int prev_msect = -1, prev_sect = -1, prev_ssect = -1;
	ScriptStruct::Part* part = 0;
	ScriptStruct::SubPart* subpart = 0;
	ScriptStruct::SubSubPart* ssubpart = 0;
	int ssub_line_i = 0;
	for(int i = 0; i < solver.lines.GetCount(); i++) {
		auto& line = solver.lines[i];
		auto& sect = solver.sections[line.section];
		auto& msect = solver.meta_sections[sect.meta_section];
		
		if (prev_msect != sect.meta_section) {
			part = &ss.parts.Add();
			subpart = &part->sub.Add();
			ssubpart = &subpart->sub.Add();
			part->type = msect.type;
			part->num = msect.num;
			subpart->repeat = sect.repeat;
			ssub_line_i = 0;
		}
		else if (prev_sect != line.section) {
			subpart = &part->sub.Add();
			subpart->repeat = sect.repeat;
			ssubpart = &subpart->sub.Add();
			ssub_line_i = 0;
		}
		else if (ssub_line_i > 0 && ssub_line_i % 4 == 0)
			ssubpart = &subpart->sub.Add();
		
		if (!tk.Parse(line.txt))
			continue;
		
		if (tk.GetLines().GetCount() == 1) {
			//data_lock.Enter();
			const auto& line = tk.GetLines()[0];
			
			token_is.SetCount(0);
			CombineHash ch;
			for (const WString& line : line) {
				String s = line.ToString();
				int tk_i = -1;
				Token& tk = da.tokens.GetAdd(s, tk_i);
				ch.Do(tk_i);
				token_is << tk_i;
			}
			hash_t h = ch;
			
			int tt_i = -1;
			TokenText& tt = da.token_texts.GetAdd(h, tt_i);
			if (tt.tokens.IsEmpty()) {
				Swap(tt.tokens, token_is);
			}
			
			if (tt_i >= 0)
				ssubpart->token_texts << tt_i;
			//data_lock.Leave();
		}
		
		prev_msect = sect.meta_section;
		prev_sect = line.section;
		ssub_line_i++;
	}
	data_lock.Leave();
	
	if (0) {
		LOG(da.GetScriptDump(ss_i));
	}
	
	actual++;
	NextSubBatch();
	
	if (worker_total % 500 == 0) {
		da.diagnostics.GetAdd(__comps + ": total") = IntStr(total);
		da.diagnostics.GetAdd(__comps + ": actual") =  IntStr(actual);
		da.diagnostics.GetAdd(__comps + ": percentage") =  DblStr((double)actual / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'well' loss") =  DblStr((double)well_filter_loss / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'parse success' loss") =  DblStr((double)parse_loss / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'foreign' loss") =  DblStr((double)foreign_loss / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": duration of song process") =  ts.ToString();
	}
}

SourceDataImporter& SourceDataImporter::Get(int appmode) {
	static ArrayMap<String, SourceDataImporter> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	SourceDataImporter& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

