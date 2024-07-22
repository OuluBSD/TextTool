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
	
	TODO
	
}

TokenDataProcess& TokenDataProcess::Get(int appmode) {
	static ArrayMap<String, TokenDataProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	TokenDataProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void TokenDataProcess::GetUsingExisting() {
	TODO
	#if 0
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
	#endif
}

void TokenDataProcess::Get() {
	TODO
	#if 0
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	TokenArgs& args = token_args;
	args.fn = 0;
	args.words.Clear();
	
	int per_action_task = 100;
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.tokens.GetCount());
	int count = end - begin;
	if (count <= 0) {
		RemoveTask(*t);
		return;
	}
	
	for(int i = begin; i < end; i++) {
		const String& tk = da.tokens.GetKey(i);
		args.words << tk;
	}
	
	t->total += args.words.GetCount();
	
	
	TaskMgr& m = TaskMgr::Single();
	if (t->fn == 0)
		m.GetTokenData(appmode, args, THISBACK1(OnTokenData, t));
	#endif
}


END_TEXTLIB_NAMESPACE

