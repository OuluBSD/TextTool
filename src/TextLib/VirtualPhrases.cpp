#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhrasesProcess::VirtualPhrasesProcess() {
	
}

int VirtualPhrasesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhrasesProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_IMPORT_TOKEN_TEXTS: return 1;
		case PHASE_GET_PARTS_USING_EXISTING: return DB_COUNT;
		case PHASE_GET_PARTS: return GetDatabase().src_data.a.dataset.virtual_phrase_parts.GetCount() / per_action_task;
		default: TODO; return 1;
	}
}

int VirtualPhrasesProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void VirtualPhrasesProcess::DoPhase() {
	switch (phase) {
		case PHASE_IMPORT_TOKEN_TEXTS: ImportTokenTexts(); return;
		case PHASE_GET_PARTS_USING_EXISTING: GetPartsUsingExisting(); return;
		case PHASE_GET_PARTS: GetParts(); return;
	}
	
	// DoVirtualPhrases(0);
	// DoVirtualPhrasesUsingExisting(1);
	// DoVirtualPhrases(1);
	
}

VirtualPhrasesProcess& VirtualPhrasesProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhrasesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhrasesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


void VirtualPhrasesProcess::ImportTokenTexts() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	Vector<int> word_is, word_classes;
	actual = 0;
	total = 0;
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		TokenText& txt = da.token_texts[i];
		total++;
		
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
		
		int prev_w_i = -1;
		for(int j = 0; j < word_is.GetCount(); j++) {
			int w_i = word_is[j];
			int next_w_i = j+1 < word_is.GetCount() ? word_is[j] : -1;
			succ = succ && GetTypePhrase(word_classes, da, next_w_i, w_i, prev_w_i);
			prev_w_i = w_i;
		}
		
		if (word_classes.IsEmpty())
			succ = false;
		
		if (succ) {
			CombineHash ch;
			for (int wc_i : word_classes)
				ch.Do(wc_i);
			hash_t h = ch;
			
			int vp_i = -1;
			VirtualPhrase& vp = da.virtual_phrases.GetAdd(h, vp_i);
			Swap(word_classes, vp.word_classes);
			
			txt.virtual_phrase = vp_i;
			actual++;
		}
	}
	
	da.diagnostics.GetAdd("token texts to virtual phrases: total") = IntStr(total);
	da.diagnostics.GetAdd("token texts to virtual phrases: actual") =  IntStr(actual);
	da.diagnostics.GetAdd("token texts to virtual phrases: percentage") =  DblStr((double)actual / (double) total * 100);
	
	//int punctuation_mark_i = da.word_classes.FindAdd("punctuation mark");
	//int punctuation_i = da.word_classes.FindAdd("punctuation");
	for(int i = 0; i < da.virtual_phrase_parts.GetCount(); i++)
		da.virtual_phrase_parts[i].count = 0;
	
	for(int i = 0; i < da.virtual_phrases.GetCount(); i++) {
		VirtualPhrase& vp = da.virtual_phrases[i];
		Vector<Vector<int>> tmps;
		Vector<int> tmp;
		
		// NOTE: see duplicate in fn 3
		
		for (int wc_i : vp.word_classes) {
			String wc = da.word_classes[wc_i];
			int a = wc.Find("punctuation");
			int b = wc.Find("conjunction");
			//if (type == punctuation_mark_i || type == punctuation_i) {
			if (a >= 0 || b >= 0) {
				if (tmp.GetCount()) {
					Swap(tmps.Add(), tmp);
					tmp.SetCount(0);
				}
				if (b >= 0)
					tmp << wc_i;
			}
			else
				tmp << wc_i;
		}
		if (tmp.GetCount()) {
			Swap(tmps.Add(), tmp);
			tmp.SetCount(0);
		}
		CombineHash struct_ch;
		Vector<int> vpp_is;
		for (const Vector<int>& tmp : tmps) {
			CombineHash ch;
			for (int type : tmp)
				ch.Do(type).Put(1);
			hash_t h = ch;
			
			int vpp_i = -1;
			VirtualPhrasePart& vpp = da.virtual_phrase_parts.GetAdd(h, vpp_i);
			if (vpp.word_classes.IsEmpty())
				vpp.word_classes <<= tmp;
			vpp.count++;
			vpp_is << vpp_i;
			struct_ch.Do(vpp_i).Put(1);
		}
		hash_t vps_h = struct_ch;
		int vps_i = -1;
		VirtualPhraseStruct& vps = da.virtual_phrase_structs.GetAdd(vps_h, vps_i);
		//if (vps.parts.IsEmpty())
			vps.virtual_phrase_parts <<= vpp_is;
		vp.virtual_phrase_struct = vps_i;
	}
	LOG(da.virtual_phrase_parts.GetCount());
	LOG(da.virtual_phrase_parts.GetCount() * 100.0 / da.virtual_phrases.GetCount());
	NextPhase();
}

void VirtualPhrasesProcess::GetPartsUsingExisting() {
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	if (batch >= DB_COUNT) {
		NextPhase();
		return;
	}
	
	int i = batch;
	if (i == appmode) {NextBatch(); return;}
	TextDatabase& db1 = MetaDatabase::Single().db[i];
	SourceData& sd1 = db1.src_data;
	SourceDataAnalysis& sda1 = db1.src_data.a;
	DatasetAnalysis& da1 = sda1.dataset;
	if (!db1.loaded) {NextBatch(); return;}
	
	for(int j = 0; j < da0.virtual_phrase_parts.GetCount(); j++) {
		// If virtual phrase part has no known type
		VirtualPhrasePart& vpp0 = da0.virtual_phrase_parts[j];
		if (vpp0.struct_part_type < 0) {
			// Calculate da1 hash (since key hash is local to da0)
			CombineHash ch;
			bool hash_fail = false;
			for (int wc0: vpp0.word_classes) {
				const String& common_key = da0.word_classes[wc0];
				int wc_i1 = da1.word_classes.Find(common_key);
				if (wc_i1 < 0) {
					hash_fail = true; // cannot make common hash (it's not in the list anyway then)
					break;
				}
				ch.Do(wc_i1).Put(1);
			}
			if (hash_fail)
				continue;
			hash_t hash1 = ch;
			
			// Then find it in other
			int k = da1.virtual_phrase_parts.Find(hash1);
			if (k < 0)
				continue;
			
			// ...if the other has type
			const VirtualPhrasePart& vpp1 = da1.virtual_phrase_parts[k];
			if (vpp1.struct_part_type < 0)
				continue;
			
			// Copy type to avoid useless AI usage
			const String& struct_part_type = da1.struct_part_types[vpp1.struct_part_type];
			vpp0.struct_part_type = da0.struct_part_types.FindAdd(struct_part_type);
		}
	}
	
	NextBatch();
}

void VirtualPhrasesProcess::GetParts() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	TokenArgs& args = token_args;
	args.fn = 2;
	args.words.Clear();
	
	int iter = 0;
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.virtual_phrase_parts.GetCount());
	tmp_ptrs.Clear();
	
	for (const VirtualPhrasePart& vpp : da.virtual_phrase_parts.GetValues()) {
		
		if (vpp.struct_part_type >= 0)
			continue;
		
		if (iter >= begin && iter < end) {
			String s;
			int punct_count = 0;
			bool fail = false;
			for(int j = 0; j < vpp.word_classes.GetCount(); j++) {
				if (j) s << ",";
				int wc_i = vpp.word_classes[j];
				if (wc_i >= da.word_classes.GetCount()) {fail = true; break;}
				String wc = da.word_classes[wc_i];
				
				int a = wc.Find("(");
				if (a >= 0) wc = wc.Left(a);
				a = wc.Find(",");
				if (a >= 0) wc = wc.Left(a);
				
				if (wc.Find("punctuation") >= 0)
					punct_count++;
				
				s << wc;
			}
			
			if (punct_count > 8 || fail)
				continue;
			
			args.words << s;
			tmp_ptrs << (void*)&vpp;
		}
		else if (iter >= end)
			break;
		
		iter++;
	}
	
	if (args.words.IsEmpty()) {
		NextPhase();
		return;
	}
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetTokenData(appmode, args, [this](String res) {
		TokenArgs& args = token_args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		actual = 0;
		total = 0;
		
		RemoveEmptyLines(res);
		Vector<String> lines = Split(res, "\n");
		bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
		
		Vector<int> word_classes;
		int line_i = -1;
		for (String& line : lines) {
			line_i++;
			line = TrimBoth(line);
			
			if (line.IsEmpty() ||!IsDigit(line[0]))
				continue;
			
			int a = line.Find(".");
			if (a < 0) continue;
			line = TrimBoth(line.Mid(a+1));
			
			a = line.ReverseFind(":");
			if (a < 0)
				continue;
			
			Vector<String> classes = Split(TrimBoth(line.Left(a)), ",");
			word_classes.SetCount(0);
			
			VirtualPhrasePart* vpp_p;
			if (line_match)
				vpp_p = (VirtualPhrasePart*)tmp_ptrs[line_i];
			else {
				bool fail = false;
				CombineHash ch;
				for (String& c : classes) {
					c = TrimBoth(c);
					int wc_i = da.word_classes.FindAdd(c);
					if (wc_i < 0) {
						fail = true;
						break;
					}
					word_classes << wc_i;
					ch.Do(wc_i).Put(1);
				}
				if (fail) continue;
				hash_t h = ch;
				vpp_p = &da.virtual_phrase_parts.GetAdd(h);
			}
			VirtualPhrasePart& vpp = *vpp_p;
			
			if (vpp.word_classes.IsEmpty())
				vpp.word_classes <<= word_classes;
			
			line = TrimBoth(line.Mid(a+1));
			
			vpp.struct_part_type = da.struct_part_types.FindAdd(line);
		}
		
		
		int a = 0;
		for (const VirtualPhrasePart& vpp : da.virtual_phrase_parts.GetValues())
			if (vpp.struct_part_type >= 0)
				a++;
		da.diagnostics.GetAdd("virtual phrases: total") = IntStr(da.virtual_phrase_parts.GetCount());
		da.diagnostics.GetAdd("virtual phrases: actual") =  IntStr(a);
		da.diagnostics.GetAdd("virtual phrases: percentage") =  DblStr((double)a / (double) da.virtual_phrase_parts.GetCount() * 100);
		
		
		NextBatch();
		SetWaiting(false);
	});
}



bool GetTypePhrase(Vector<int>& types, const DatasetAnalysis& da, int next_w_i, int w_i, int prev_w_i) {
	if (w_i < 0) {
		return false;
	}
	else {
		const ExportWord& ew = da.words[w_i];
		if (!ew.class_count)
			return false;
		
		int class_i = -1;
		if (ew.class_count > 1) {
			bool found = false;
			
			if (w_i >= 0 && prev_w_i >= 0) {
				CombineHash ch;
				ch.Do(prev_w_i).Put(1).Do(w_i);
				hash_t h = ch;
				int i = da.ambiguous_word_pairs.Find(h);
				if (i >= 0) {
					const WordPairType& wp0 = da.ambiguous_word_pairs[i];
					if (wp0.to_type >= 0) {
						class_i = wp0.to_type;
						found = true;
					}
				}
			}
			if (!found && w_i >= 0 && next_w_i >= 0) {
				CombineHash ch;
				ch.Do(w_i).Put(1).Do(next_w_i);
				hash_t h = ch;
				int i = da.ambiguous_word_pairs.Find(h);
				if (i >= 0) {
					const WordPairType& wp0 = da.ambiguous_word_pairs[i];
					if (wp0.from_type >= 0) {
						class_i = wp0.from_type;
						found = true;
					}
				}
			}
			
			if (!found)
				class_i = ew.classes[0];
		}
		else {
			class_i = ew.classes[0];
		}
		
		String wc = da.word_classes[class_i];
		if (wc.Find("contraction") == 0 && wc.Find("(") >= 0) {
			int a = wc.Find("(");
			if (a < 0)
				return false;
			a++;
			int b = wc.Find(")", a);
			if (b < 0)
				return false;
			String arg = wc.Mid(a,b-a);
			
			a = arg.Find("/");
			if (a >= 0)
				arg = TrimBoth(arg.Left(a));
			
			a = arg.Find(";");
			if (a >= 0)
				arg = TrimBoth(arg.Left(a));
			
			const char* split_str = " ";
			arg.Replace("+", " ");
			Vector<String> words = Split(arg, " ");
			
			int prev_w_j = prev_w_i;
			Vector<int> w_js;
			for (String& w : words) {
				w = TrimBoth(w);
				int w_j = da.words.Find(w);
				if (w_j < 0)
					return false;
				w_js << w_j;
			}
			int c = w_js.GetCount();
			for(int j = 0; j < c; j++) {
				int w_j = w_js[j];
				int next_w_j = j < c-1 ? w_js[j+1] : -1;
				bool succ = GetTypePhrase(types, da, next_w_j, w_j, prev_w_j);
				if (!succ)
					return false;
				prev_w_j = w_j;
			}
		}
		else {
			if (class_i < 0)
				return false;
			
			types << class_i;
		}
		
	}
	return true;
}


END_TEXTLIB_NAMESPACE

