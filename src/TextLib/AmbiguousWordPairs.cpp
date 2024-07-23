#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



AmbiguousWordPairsProcess::AmbiguousWordPairsProcess() {
	
}

int AmbiguousWordPairsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int AmbiguousWordPairsProcess::GetBatchCount(int phase) const {
	if (phase == PHASE_GET_USING_EXISTING)
		return DB_COUNT;
	else if (phase == PHASE_GET)
		return GetDatabase().src_data.a.dataset.ambiguous_word_pairs.GetCount() / per_action_task;
	else
		return 1;
}

int AmbiguousWordPairsProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void AmbiguousWordPairsProcess::DoPhase() {
	if (phase == PHASE_GET_USING_EXISTING)
		GetUsingExisting();
	else if (phase == PHASE_GET)
		Get();
}

void AmbiguousWordPairsProcess::GetUsingExisting() {
	TimeStop ts;
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	if (batch >= DB_COUNT) {
		LOG("TaskManager::GetAmbiguousWordPairsUsingExisting: copying values took " << ts.ToString());
		LOG("");
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
	
	for(int j = 0; j < da0.ambiguous_word_pairs.GetCount(); j++) {
		// If token has no connected word in this database
		WordPairType& wpt0 = da0.ambiguous_word_pairs[j];
		if (wpt0.from < 0 || wpt0.to < 0) continue;
		
		if (wpt0.from_type < 0 || wpt0.to_type < 0) {
			const String& from_str0 = da0.words.GetKey(wpt0.from);
			const String& to_str0 = da0.words.GetKey(wpt0.to);
			
			
			// Then find it in other
			int k = -1;
			for(int j = 0; j < da1.ambiguous_word_pairs.GetCount(); j++) {
				const WordPairType& wpt1 = da1.ambiguous_word_pairs[j];
				if (wpt1.from < 0 || wpt1.to < 0)
					continue;
				const String& from_str1 = da1.words.GetKey(wpt1.from);
				const String& to_str1 = da1.words.GetKey(wpt1.to);
				if (from_str0 == from_str1 && to_str0 == to_str1) {
					k = j;
					break;
				}
			}
			if (k < 0)
				continue;
			
			// ...if the other has type
			const WordPairType& wpt1 = da1.ambiguous_word_pairs[k];
			if (wpt1.from_type < 0 || wpt1.to_type < 0)
				continue;
			
			// Copy word to avoid useless AI usage
			const String& from_type_class1 = da1.word_classes[wpt1.from_type];
			const String& to_type_class1 = da1.word_classes[wpt1.to_type];
			wpt0.from_type = da0.word_classes.FindAdd(from_type_class1);
			wpt0.to_type   = da0.word_classes.FindAdd(to_type_class1);
		}
	}
	
	NextBatch();
}

void AmbiguousWordPairsProcess::Get() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	TokenArgs& args = token_args;
	args.fn = 1;
	args.words.Clear();
	
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.ambiguous_word_pairs.GetCount());
	int iter = 0;
	
	tmp_ptrs.Clear();
	
	for (const WordPairType& wp : da.ambiguous_word_pairs.GetValues()) {
		if (wp.from < 0 || wp.to < 0)
			continue;
		if (wp.from_type >= 0 && wp.to_type >= 0)
			continue;
		
		if (iter >= begin && iter < end) {
			const String& from = da.words.GetKey(wp.from);
			const String& to = da.words.GetKey(wp.to);
			args.words << (from + " " + to);
			tmp_ptrs << (void*)&wp;
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
		
		// 9. is something : verb, noun
		
		RemoveEmptyLines(res);
		Vector<String> lines = Split(res, "\n");
		
		bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
		int offset = 1+1;
		
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
			
			Vector<String> result_words = Split(TrimBoth(line.Left(a)), " ");
			if (result_words.GetCount() != 2)
				continue;
			
			WordPairType* wpp;
			if (line_match)
				wpp = (WordPairType*)tmp_ptrs[line_i];
			else {
				int w_i0 = da.words.Find(result_words[0]);
				int w_i1 = da.words.Find(result_words[1]);
				CombineHash ch;
				ch.Do(w_i0).Put(1).Do(w_i1);
				hash_t h = ch;
				
				//ExportWord& wrd0 = da.words[w_i0];
				//ExportWord& wrd1 = da.words[w_i1];
				wpp = &da.ambiguous_word_pairs.GetAdd(h);
			}
			WordPairType& wp = *wpp;
			
			line = TrimBoth(line.Mid(a+1));
			
			Vector<String> parts = Split(line, ",");
			if (parts.GetCount() != 2)
				continue;
			int wc_i_list[2];
			for(int i = 0; i < parts.GetCount(); i++) {
				String& p = parts[i];
				p = TrimBoth(p);
				wc_i_list[i] = da.word_classes.FindAdd(p);
			}
			
			wp.from_type = wc_i_list[0];
			wp.to_type = wc_i_list[1];
		}
		
		int a = 0;
		for (const WordPairType& wp : da.ambiguous_word_pairs.GetValues()) {
			if (wp.from < 0 || wp.to < 0)
				continue;
			if (wp.from_type >= 0 && wp.to_type >= 0)
				a++;
		}
		da.diagnostics.GetAdd("ambiguous word pairs: total") = IntStr(da.ambiguous_word_pairs.GetCount());
		da.diagnostics.GetAdd("ambiguous word pairs: actual") = IntStr(a);
		da.diagnostics.GetAdd("ambiguous word pairs: percentage") =  DblStr((double)a / (double)da.ambiguous_word_pairs.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}


AmbiguousWordPairsProcess& AmbiguousWordPairsProcess::Get(int appmode) {
	static ArrayMap<String, AmbiguousWordPairsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	AmbiguousWordPairsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

