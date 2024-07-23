#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhraseStructsProcess::VirtualPhraseStructsProcess() {
	
}

int VirtualPhraseStructsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhraseStructsProcess::GetBatchCount(int phase) const {
	if (phase == PHASE_GET)
		return GetDatabase().src_data.a.dataset.token_texts.GetCount();
	else
		return 1;
}

int VirtualPhraseStructsProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void VirtualPhraseStructsProcess::DoPhase() {
	if (phase == PHASE_GET) {
		Get();
	}
}

void VirtualPhraseStructsProcess::Get() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	
	if (batch >= da.token_texts.GetCount()) {
		da.diagnostics.GetAdd("token text to phrase: total") = IntStr(da.token_texts.GetCount());
		da.diagnostics.GetAdd("token text to phrase: actual") = IntStr(actual);
		da.diagnostics.GetAdd("token text to phrase: percentage") =  DblStr((double)actual / (double)da.token_texts.GetCount() * 100);
		NextPhase();
		return;
	}
	
	
	// NOTE: see duplicate in fn 0
	// TODO reduce duplicate code
	Vector<int> word_is, word_classes;
	int i = batch;
	{
		const TokenText& txt = da.token_texts[i];
		if (txt.virtual_phrase < 0) {
			NextBatch();
			return;
		}
		
		// NOTE: see duplicate in fn 0
		bool succ = true;
		word_is.SetCount(0);
		word_classes.SetCount(0);
		for(int tk_i : txt.tokens) {
			const Token& tk = da.tokens[tk_i];
			int w_i = tk.word_;
			if (w_i < 0) {
				String key = da.tokens.GetKey(tk_i);
				w_i = da.words.Find(key);
				if (w_i < 0) {
					key = ToLower(da.tokens.GetKey(tk_i));
					w_i = da.words.Find(key);
				}
				tk.word_ = w_i;
			}
			word_is << w_i;
		}
		
		const VirtualPhrase& vp = da.virtual_phrases[txt.virtual_phrase];
		if (word_is.GetCount() != vp.word_classes.GetCount()) {
			NextBatch();
			return;
		}
		
		actual++;
		
		Vector<Vector<int>> w_isv, wc_isv;
		Vector<int> w_is, wc_is;
		
		// NOTE: see duplicate in fn 0
		int c = word_is.GetCount();
		for(int j = 0; j < vp.word_classes.GetCount(); j++) {
			int w_i = word_is[j];
			int wc_i = vp.word_classes[j];
			
			String wc = da.word_classes[wc_i];
			int a = wc.Find("punctuation");
			int b = wc.Find("conjunction");
			//if (type == punctuation_mark_i || type == punctuation_i) {
			if (a >= 0 || b >= 0) {
				if (w_is.GetCount()) {
					Swap(w_isv.Add(), w_is);
					Swap(wc_isv.Add(), wc_is);
					w_is.SetCount(0);
					wc_is.SetCount(0);
				}
				if (b >= 0) {
					wc_is << wc_i;
					w_is << w_i; // NOTE: this is NOT duplicate
				}
			}
			else {
				wc_is << wc_i;
				w_is << w_i;
			}
		}
		if (w_is.GetCount()) {
			Swap(wc_isv.Add(), wc_is);
			Swap(w_isv.Add(), w_is);
			wc_is.SetCount(0);
			w_is.SetCount(0);
		}
		
		
		for(int j = 0; j < w_isv.GetCount(); j++) {
			const Vector<int>& wc_is = wc_isv[j];
			const Vector<int>& w_is = w_isv[j];
			
			hash_t wc_h, w_h;
			{
				CombineHash ch;
				for (int wc_i : wc_is)
					ch.Do(wc_i).Put(1);
				wc_h = ch;
			}
			{
				CombineHash ch;
				for (int w_i : w_is)
					ch.Do(w_i).Put(1);
				w_h = ch;
			}
			
			int pp_i = -1;
			PhrasePart& pp = da.phrase_parts.GetAdd(w_h, pp_i);
			pp.words <<= w_is;
			pp.tt_i = i;
			pp.virtual_phrase_part = da.virtual_phrase_parts.Find(wc_h);
			
		}
	}
	
	NextBatch();
}

VirtualPhraseStructsProcess& VirtualPhraseStructsProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhraseStructsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhraseStructsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

