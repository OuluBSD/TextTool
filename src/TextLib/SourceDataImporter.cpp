#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE




SourceDataImporter::SourceDataImporter() {
	
}

int SourceDataImporter::GetPhaseCount() const {
	return PHASE_COUNT;
}

int SourceDataImporter::GetBatchCount() const {
	TextDatabase& db = GetDatabase();
	return db.src_data.entities.GetCount();
}

int SourceDataImporter::GetSubBatchCount() const {
	TextDatabase& db = GetDatabase();
	if (batch >= db.src_data.entities.GetCount())
		return 1;
	auto& entity = db.src_data.entities[batch];
	return entity.scripts.GetCount();
}

void SourceDataImporter::DoPhase() {
	switch (phase) {
		case PHASE_TOKENIZE:		Tokenize();		return;
		case PHASE_STRUCTURIZE:		Structurize();	return;
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
	
	total++;
	auto& script = entity.scripts[sub_batch];
	
	NaturalTokenizer tk;
	
	String str = script.text;
	
	// Ignore files with hard ambiguities
	if (str.Find(" well ") >= 0) {
		// well or we'll... too expensive to figure out
		well_filter_loss++;
		NextSubBatch();
		return;
	}
	
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
	
	for (const auto& line : tk.GetLines()) {
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
		
		TokenText& tt = da.token_texts.GetAdd(h);
		if (tt.tokens.IsEmpty()) {
			Swap(tt.tokens, token_is);
		}
	}
	
	actual++;
	NextSubBatch();
	
	if (total % 500 == 0) {
		da.diagnostics.GetAdd(__comps + ": total") = IntStr(total);
		da.diagnostics.GetAdd(__comps + ": actual") =  IntStr(actual);
		da.diagnostics.GetAdd(__comps + ": percentage") =  DblStr((double)actual / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'well' loss") =  DblStr((double)well_filter_loss / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'parse success' loss") =  DblStr((double)parse_loss / (double) total * 100);
		da.diagnostics.GetAdd(__comps + ": filter 'foreign' loss") =  DblStr((double)foreign_loss / (double) total * 100);
	}
	
	da.diagnostics.GetAdd(__comps + ": total") = IntStr(total);
	da.diagnostics.GetAdd(__comps + ": actual") =  IntStr(actual);
	da.diagnostics.GetAdd(__comps + ": percentage") =  DblStr((double)actual / (double) total * 100);
	da.diagnostics.GetAdd(__comps + ": filter 'well' loss") =  DblStr((double)well_filter_loss / (double) total * 100);
	da.diagnostics.GetAdd(__comps + ": filter 'parse success' loss") =  DblStr((double)parse_loss / (double) total * 100);
	da.diagnostics.GetAdd(__comps + ": filter 'foreign' loss") =  DblStr((double)foreign_loss / (double) total * 100);
	da.diagnostics.GetAdd(__comps + ": duration of song process") =  ts.ToString();
}

void SourceDataImporter::Structurize() {
	TextDatabase& db = GetDatabase();
	MultiScriptStructureSolver solver;
	
	if (batch >= db.src_data.entities.GetCount()) {
		NextPhase();
		return;
	}
	
	auto& entity = db.src_data.entities[batch];
	if (sub_batch >= entity.scripts.GetCount()) {
		NextBatch();
		return;
	}
	auto& script = entity.scripts[sub_batch];
	if (TrimBoth(script.text).IsEmpty()) {
		NextSubBatch();
		return;
	}
	
	solver.Get().Process(script.text);
	
	
	// TODO
	
	
	NextSubBatch();
}

SourceDataImporter& SourceDataImporter::Get(int appmode) {
	static ArrayMap<String, SourceDataImporter> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	SourceDataImporter& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

