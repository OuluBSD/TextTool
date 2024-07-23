#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



TokenPhrasesProcess::TokenPhrasesProcess() {
	
}

int TokenPhrasesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int TokenPhrasesProcess::GetBatchCount(int phase) const {
	return 1;
}

int TokenPhrasesProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void TokenPhrasesProcess::DoPhase() {
	
	if (phase == PHASE_UNKNOWN_PAIRS) {
		UnknownPairs();
	}
	else TODO;
	
}

TokenPhrasesProcess& TokenPhrasesProcess::Get(int appmode) {
	static ArrayMap<String, TokenPhrasesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	TokenPhrasesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void TokenPhrasesProcess::UnknownPairs() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	DatasetAnalysis& da = sd.a.dataset;
	
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		if (txt.tokens.GetCount() < 2)
			continue;
		int prev_w_i = -1;
		for(int j = 0; j < txt.tokens.GetCount(); j++) {
			int tk_i = txt.tokens[j];
			bool is_first = j == 0;
			bool is_last = j == txt.tokens.GetCount()-1;
			bool prev_unknown = false;
			
			const Token& tk = da.tokens[tk_i];
			int w_i = tk.word_;
			if (w_i < 0) {
				String key = ToLower(da.tokens.GetKey(tk_i));
				w_i = da.words.Find(key);
				tk.word_ = w_i;
			}
			if (w_i >= 0) {
				String prev_ew_str = prev_w_i >= 0 ? da.words.GetKey(prev_w_i) : String();
				const String& ew_str = da.words.GetKey(w_i);
				const ExportWord& ew = da.words[w_i];
				bool is_unknown = ew.class_count > 1;
				
				if (ew_str.GetCount() == 1 || prev_ew_str.GetCount() == 1) {
					// pass
				}
				/*bool next_unknown = false;
				if (!is_last && !prev_unknown && is_unknown) {
					int next_tk_i = txt.tokens[j+1];
					const Token& next_tk = da.tokens[next_tk_i];
					if (next_tk.word_ >= 0) {
						const ExportWord& next_ew = da.words[next_tk.word_];
						next_unknown = next_ew.class_count > 1;
					}
				}
				
				if (!prev_unknown && is_unknown && next_unknown) {
					// do nothing: wait until next
				}*/
				else
				if (prev_unknown || (is_unknown && is_last)) {
					if (prev_w_i >= 0) {
						CombineHash c;
						c.Do(prev_w_i).Put(1).Do(w_i);
						hash_t h = c;
						WordPairType& wp = da.ambiguous_word_pairs.GetAdd(h);
						wp.from = prev_w_i;
						wp.to = w_i;
					}
				}
				
				prev_unknown = is_unknown;
			}
			prev_w_i = w_i;
		}
	}
	
	NextPhase();
}


END_TEXTLIB_NAMESPACE

