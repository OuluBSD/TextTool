#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


TokenDataProcess::TokenDataProcess() {
	
}

int TokenDataProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int TokenDataProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int TokenDataProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void TokenDataProcess::DoPhase() {
	switch (phase) {
		case PHASE_GET_USING_EXISTING: GetUsingExisting(); return;
		case PHASE_GET: Get(); return;
		default: TODO;
	}
}

TokenDataProcess& TokenDataProcess::Get(int appmode) {
	static ArrayMap<String, TokenDataProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	TokenDataProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void TokenDataProcess::GetUsingExisting() {
	TimeStop ts;
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	int lng_i = MetaDatabase::Single().GetOtherLanguageIndex();
	auto& translations0 = da0.translations[lng_i];
	
	for(int i = 0; i < DB_COUNT; i++) {
		if (i == appmode) continue;
		TextDatabase& db1 = MetaDatabase::Single().db[i];
		SourceData& sd1 = db1.src_data;
		SourceDataAnalysis& sda1 = db1.src_data.a;
		DatasetAnalysis& da1 = sda1.dataset;
		
		for(int j = 0; j < da0.tokens.GetCount(); j++) {
			// If token has no connected word in this database
			Token& tk0 = da0.tokens[j];
			if (tk0.word_ < 0) {
				const String& tk_str0 = da0.tokens.GetKey(j);
				
				// Then find it in other
				int k = da1.tokens.Find(tk_str0);
				if (k < 0)
					continue;
				
				// ...if the other has linked word
				const Token& tk1 = da1.tokens[k];
				if (tk1.word_ < 0) continue;
				
				// Copy word to avoid useless AI usage
				const String& wrd1_str = da1.words.GetKey(tk1.word_);
				const ExportWord* wrd1 = &da1.words[tk1.word_];
				
				while (wrd1->link >= 0) {
					wrd1 = &da1.words[wrd1->link];
				}
				
				// If the word is not yet added
				k = da0.words.Find(wrd1_str);
				if (k < 0) {
					int wrd_i0 = -1;
					ExportWord& wrd0 = da0.words.GetAdd(wrd1_str, wrd_i0);
					wrd0.CopyFrom(*wrd1, true);
					
					// Link word
					tk0.word_ = wrd_i0;
					
					// Copy word class
					for(int j = 0; j < wrd1->class_count && j < ExportWord::MAX_CLASS_COUNT; j++) {
						int class_type1 = wrd1->classes[j];
						const String& word_class1 = da1.word_classes[class_type1];
						int class_type0 = da0.word_classes.FindAdd(word_class1);
						wrd0.classes[j] = class_type0;
					}
				}
				// TODO This is a bit unexpected, but use the existing word
				else {
					// Link word
					tk0.word_ = k;
				}
			}
			// Fill more data
			else {
				ExportWord& wrd0 = da0.words[tk0.word_];
				if (wrd0.phonetic.IsEmpty() || wrd0.spelling.IsEmpty()) {
					
					// Then find it in other
					const String& tk_str0 = da0.tokens.GetKey(j);
					int k = da1.tokens.Find(tk_str0);
					if (k < 0)
						continue;
					
					// ...if the other has linked word
					const Token& tk1 = da1.tokens[k];
					if (tk1.word_ < 0) continue;
					
					// Copy some data to avoid useless AI usage
					const String& wrd1_str = da1.words.GetKey(tk1.word_);
					const ExportWord& wrd1 = da1.words[tk1.word_];
					
					if (wrd0.phonetic.IsEmpty() && !wrd1.phonetic.IsEmpty())
						wrd0.phonetic = wrd1.phonetic;
					if (wrd0.spelling.IsEmpty() && !wrd1.spelling.IsEmpty())
						wrd0.spelling = wrd1.spelling;
				}
					
			}
		}
	}
	
	// Re-count word usage
	for (ExportWord& ew0 : da0.words.GetValues()) {
		ew0.count = 0;
	}
	for (TokenText& tt : da0.token_texts.GetValues()) {
		for (int tk_i : tt.tokens) {
			const Token& tk = da0.tokens[tk_i];
			if (tk.word_ >= 0) {
				ExportWord& wrd0 = da0.words[tk.word_];
				wrd0.count++;
			}
		}
	}
	
	// Copy translation
	for(int i = 0; i < DB_COUNT; i++) {
		if (i == appmode) continue;
		TextDatabase& db1 = MetaDatabase::Single().db[i];
		SourceData& sd1 = db1.src_data;
		SourceDataAnalysis& sda1 = db1.src_data.a;
		DatasetAnalysis& da1 = sda1.dataset;
		auto& translations1 = da1.translations[lng_i];
		
		for(const String& wrd_str0 : da0.words.GetKeys()) {
			if (translations0[lng_i].Find(wrd_str0) < 0) {
				int j = translations1.Find(wrd_str0);
				if (j >= 0) {
					const String& trans1 = translations1[j];
					translations1.GetAdd(wrd_str0) = trans1;
				}
			}
		}
	}
	
	LOG("TaskManager::GetTokenDataUsingExisting: copying values took " << ts.ToString());
	LOG("");
}

void TokenDataProcess::Get() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	TokenArgs& args = token_args;
	args.fn = 0;
	args.words.Clear();
	
	if (batch == 0) total = 0;
	
	int per_action_task = 100;
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.tokens.GetCount());
	int count = end - begin;
	if (count <= 0) {
		NextPhase();
		return;
	}
	
	for(int i = begin; i < end; i++) {
		const String& tk = da.tokens.GetKey(i);
		args.words << tk;
	}
	
	total += count;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetTokenData(appmode, args, [this](String result) {
		TokenArgs& args = token_args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		// 9. suppote: verb | noun
		
		result.Replace("\r", "");
		Vector<String> lines = Split(result, "\n");
		
		int offset = 3+1;
		
		for (String& line : lines) {
			line = TrimBoth(line);
			
			if (line.IsEmpty() ||!IsDigit(line[0]))
				continue;
			
			/*int line_i = ScanInt(line);
			line_i -= offset;
			if (line_i < 0 || line_i >= args.words.GetCount())
				continue;
			
			const String& orig_word = args.words[line_i];*/
			
			int a = line.Find(".");
			if (a < 0) continue;
			line = TrimBoth(line.Mid(a+1));
			
			a = line.Find(":");
			if (a == 0) {
				// Rare case of ":" being asked
				line = ":" + line;
				a = 1;
			}
			else if (a < 0)
				continue;
			
			//int orig_word_i = ;
			
			String result_word = TrimBoth(line.Left(a));
			
			/*ExportWord& wrd =
				orig_word_i >= 0 ?
					da.words[orig_word_i] :
					da.words.GetAdd(result_word, orig_word_i);*/
			int orig_word_i = -1;
			ExportWord& wrd = da.words.GetAdd(result_word, orig_word_i);
			
			//TODO // token to word
			
			line = TrimBoth(line.Mid(a+1));
			
			a = line.Find("(");
			if (a >= 0)
				line = line.Left(a);
			
			Vector<String> parts = Split(line, "|");
			for (String& p : parts) {
				p = TrimBoth(p);
				int wc_i = da.word_classes.FindAdd(p);
				if (wrd.class_count < wrd.MAX_CLASS_COUNT)
					FixedIndexFindAdd(wrd.classes, wrd.MAX_CLASS_COUNT, wrd.class_count, wc_i);
			}
			
			actual++;
		}
		
		
		da.diagnostics.GetAdd("tokens: total") = IntStr(total);
		da.diagnostics.GetAdd("tokens: actual") =  IntStr(actual);
		da.diagnostics.GetAdd("tokens: percentage") =  DblStr((double)actual / (double) total * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}


END_TEXTLIB_NAMESPACE

