#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


WordDataProcess::WordDataProcess() {
	
}

int WordDataProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int WordDataProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int WordDataProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void WordDataProcess::DoPhase() {
	lng_i = MetaDatabase::Single().GetOtherLanguageIndex();
	
	if (phase == PHASE_WORD_FIX)
		WordFix();
	else if (phase == PHASE_WORD_PROCESS)
		WordProcess();
	else if (phase == PHASE_DETAILS)
		Details();
	else if (phase == PHASE_SYLLABLES)
		Syllables();
	else if (phase == PHASE_COPY_LINKED_DATA)
		CopyLinkedData();
	else
		TODO
	
}

WordDataProcess& WordDataProcess::Get(int appmode) {
	static ArrayMap<String, WordDataProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	WordDataProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void WordDataProcess::WordFix() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& ds = sda.dataset;
	
	int fails = 0;
	for(int i = 0; i < ds.words.GetCount(); i++) {
		WString wrd = ds.words.GetKey(i).ToWString();
		ExportWord& wa = ds.words[i];
		
		bool search_alt = false;
		bool needs_alt = false;
		for(int j = 0; j < wrd.GetCount(); j++) {
			int chr = wrd[j];
			if (NaturalTokenizer::IsToken(chr) && chr != '\'') {
				search_alt = true;
				needs_alt = true;
				break;
			}
			if (IsUpper(chr)) {
				search_alt = true;
				break;
			}
		}
		
		if (search_alt || needs_alt) {
			WString alt;
			for(int j = 0; j < wrd.GetCount(); j++) {
				int chr = wrd[j];
				if (NaturalTokenizer::IsToken(chr))
					continue;
				
				if (IsUpper(chr))
					alt.Cat(ToLower(chr));
				else
					alt.Cat(chr);
			}
			String alt_s = alt.ToString();
			int j = ds.words.Find(alt_s);
			if (j >= 0) {
				ExportWord& link = ds.words[j];
				wa.link = j;
			}
			else if (needs_alt) {
				ExportWord& link = ds.words.GetAdd(alt_s, j);
				ds.words[i].link = j; // re-fetch reference
			}
			else {
				fails++;
			}
		}
		else wa.link = -1;
	}
	
	ds.diagnostics.GetAdd("word fix: fail percentage") = DblStr((double)fails / (double)ds.words.GetCount() * 100);
	
	NextPhase();
}

void WordDataProcess::WordProcess() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	DatasetAnalysis& ds = sda.dataset;
	
	
	const Vector<EntityDataset>& dataset = sd.entities;
	for(int j = 0; j < dataset.GetCount(); j++) {
		const EntityDataset& artist = dataset[j];
		//EntityAnalysis& aa = ds.entities.GetAdd(artist.name);
		//aa.word_counts.Clear();
		
		for(int k = 0; k < artist.scripts.GetCount(); k++) {
			const ScriptDataset& song = artist.scripts[k];
			
			String text = song.text;
			if (GetDefaultCharset() != CHARSET_UTF8)
				text = ToCharset(CHARSET_DEFAULT, text, CHARSET_UTF8);
			
			Vector<String> lines = Split(text, "\n");
			for (String& l : lines) {
				Vector<String> words;
				GetWords(l, words);
				if (words.IsEmpty()) continue;
				
				for (String& w : words) {
					w = ToLower(w.ToWString()).ToString();
				}
				
				//aa.total_words += words.GetCount();
				for (String& w : words) {
					//aa.word_counts.GetAdd(w, 0)++;
					int w_i = -1;
					ExportWord& wa = ds.words.GetAdd(w, w_i);
					wa.count++;
				}
			}
		}
	}
	
	NextPhase();
}

void WordDataProcess::Details() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	int per_batch = 30;
	
	ASSERT(batch >= 0);
	int begin = batch * per_batch;
	int end = (batch+1) * per_batch;
	
	tmp_words.Clear();
	
	SourceDataAnalysisArgs args; // 5
	
	int iter = 0;
	DatasetAnalysis& ds = sda.dataset;
	Color black(0,0,0);
	
	for(int i = 0; i < ds.words.GetCount(); i++) {
		ExportWord& wa = ds.words[i];
		const String& wrd = ds.words.GetKey(i);
		
		if (wrd.IsEmpty())
			continue;
		
		#if 1
		// HASH BREAKING FEATURE
		if (wa.clr != black || wrd[0] == '{' || wrd[wrd.GetCount()-1] == '}') {
			total++;
			actual++;
			continue;
		}
		if (wa.link >= 0)
			continue;
		#endif
		
		
		#if 0
		int translation_i = ds.translations.Find(wrd);
		if (translation_i < 0)
			continue;
		#endif
		
		// Skip cyrillic words
		bool is_cyrillic = false;
		WString ws = wrd.ToWString();
		for(int j = 0; j < ws.GetCount(); j++) {
			int chr = ws[j];
			if (chr >= 0x0400 && chr < 0x052F) {
				is_cyrillic = true;
				break;
			}
		}
		if (is_cyrillic)
			continue;
		
		
		if (iter >= begin) {
			args.words << wrd;
			tmp_words.Add(wrd);
		}
		iter++;
		if (iter >= end) break;
	}
	
	if (args.words.IsEmpty()) {
		NextPhase();
		return;
	}
	
	total += args.words.GetCount();
	
	args.fn = 5;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, [this](String res) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		res.Replace("\r", "");
		
		RemoveEmptyLines(res);
		Vector<String> lines = Split(res, "\n");
		
		//DUMPC(lines);
		
		Color black(0,0,0);
		Color non_black(1,1,1);
		
		DatasetAnalysis& ds = sda.dataset;
		
		bool line_match = tmp_words.GetCount() == lines.GetCount();
		ASSERT(lng_i >= 0);
		
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			
			RemoveLineChar(l);
			
			int a = l.Find(":");
			if (a < 0) continue;
			String orig = l.Left(a);
			l = TrimBoth(l.Mid(a+1));
			
			a = l.Find(" or ");
			if (a > 0) {
				l = TrimBoth(l.Mid(a+4));
			}
			
			
			if (line_match)
				orig = tmp_words[i];
			else {
				orig = TrimBoth(orig);
				orig = orig.ToWString().ToString();
				//orig = ToLower(orig.ToWString()).ToString();
				//orig.Replace("'", "");
			}
			
			
			a = l.Find(",");
			if (a < 0) continue;
			String main_class = TrimBoth(l.Left(a));
			l = TrimBoth(l.Mid(a+1));
			
			a = l.Find("),");
			if (a < 0) continue;
			String rgb = TrimBoth(l.Left(a));
			l = TrimBoth(l.Mid(a+2));
			a = rgb.Find("(");
			if (a < 0) continue;
			rgb = TrimBoth(rgb.Mid(a+1));
			
			String translation = TrimBoth(l);
			
			//int j = t->tmp_words.Find(orig);
			//if (j < 0) continue;
			
			int j = ds.words.Find(orig);
			if (j < 0)
				continue;
			
			ExportWord& wa = ds.words[j];
			int c_i = ds.word_classes.FindAdd(main_class);
			if (wa.class_count == 0) {
				wa.classes[wa.class_count++] = c_i;
			}
			else {
				bool found = false;
				for(int i = 0; i < wa.class_count && i < ExportWord::MAX_CLASS_COUNT; i++) {
					if (wa.classes[i] == c_i) {
						found = true;
						break;
					}
				}
				
				if (!found && wa.class_count < ExportWord::MAX_CLASS_COUNT)
					wa.classes[wa.class_count++] = c_i;
			}
			
			String& trans_dst = ds.translations[lng_i].GetAdd(orig);
			if (!translation.IsEmpty())
				trans_dst = translation;
			
			Vector<String> clr_str = Split(rgb, ",");
			if (clr_str.GetCount() == 3) {
				wa.clr = Color(
					ScanInt(TrimBoth(clr_str[0])),
					ScanInt(TrimBoth(clr_str[1])),
					ScanInt(TrimBoth(clr_str[2])));
				if (wa.clr == black)
					wa.clr = non_black;
			}
			actual++;
		}
		
		//da.diagnostics.GetAdd("words: total") = IntStr(t->total);
		//da.diagnostics.GetAdd("words: total") = IntStr(da.words.GetCount());
		//da.diagnostics.GetAdd("words: actual") =  IntStr(t->actual);
		//da.diagnostics.GetAdd("words: percentage") =  DblStr((double)t->actual / (double) t->total * 100);
		
		int a = 0;
		for(const ExportWord& wa : da.words.GetValues())
			if (wa.clr != black)
				a++;
		da.diagnostics.GetAdd("words: actual") =  IntStr(a);
		da.diagnostics.GetAdd("words: total") = IntStr(da.words.GetCount());
		da.diagnostics.GetAdd("words: percentage") =  DblStr((double)a / (double)da.words.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}

void WordDataProcess::Syllables() {
	int per_batch = 30;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	int begin = batch * per_batch;
	int end = (batch+1) * per_batch;
	
	
	SourceDataAnalysisArgs args; // 4
	
	int iter = 0;
	DatasetAnalysis& ds = sda.dataset;
	
	for(int i = 0; i < ds.words.GetCount(); i++) {
		const String& wrd = ds.words.GetKey(i);
		const ExportWord& wa = ds.words[i];
		
		// HASH BREAKING CODE
		#if 1
		if (wa.phonetic.GetCount() && wa.spelling.GetCount()) {
			total++;
			actual++;
			continue;
		}
		#endif
		
		if (wrd.IsEmpty())
			continue;
		
		#if 1
		// HASH BREAKING FEATURE
		if (wrd[0] == '{' || wrd[wrd.GetCount()-1] == '}') {
			total++;
			actual++;
			continue;
		}
		if (wa.link >= 0)
			continue;
		#endif
		
		
		// Skip cyrillic words
		bool is_cyrillic = false;
		WString ws = wrd.ToWString();
		for(int j = 0; j < ws.GetCount(); j++) {
			int chr = ws[j];
			if (chr >= 0x0400 && chr < 0x052F) {
				is_cyrillic = true;
				break;
			}
		}
		if (is_cyrillic)
			continue;
		
		
		if (iter >= begin) {
			String wrd = ds.words.GetKey(i);
			
			// hotfix
			//HotfixReplaceWord(wrd);
			args.words << wrd;
			tmp_words << wrd;
		}
		iter++;
		if (iter >= end) break;
	}
	
	
	if (args.words.IsEmpty()) {
		NextPhase();
		return;
	}
	
	total += args.words.GetCount();
	
	args.fn = 4;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, [this](String res) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		//-hey: hey [heɪ]
		//- hello: hel-lo [hɛˈloʊ]
		
		DatasetAnalysis& ds = sda.dataset;
		
		res.Replace("\r", "");
		RemoveEmptyLines(res);
		Vector<String> lines = Split(res, "\n");
		bool line_match = tmp_words.GetCount() == lines.GetCount();
		
		int line_i = -1;
		for (String& line : lines) {
			line_i++;
			
			RemoveLineChar(line);
			int a = line.Find(":");
			if (a < 0) continue;
			String wrd = TrimBoth(line.Left(a));
			line = line.Mid(a+1);
			
			if (line_match)
				wrd = tmp_words[line_i];
			
			a = line.Find("[");
			if (a < 0) continue;
			String spelling = TrimBoth(line.Left(a));
			a++;
			int b = line.Find("]", a);
			if (b < 0) continue;
			WString phonetic = TrimBoth(line.Mid(a,b-a)).ToWString();
			
			
			// hotfix
			/*if (0) {
				wrd = ToLower(wrd.ToWString()).ToString();
				wrd.Replace("'", "");
			}*/
			
			int j = ds.words.Find(wrd);
			if (j < 0)
				continue;
			
			if (spelling.IsEmpty()) spelling = "-";
			if (phonetic.IsEmpty()) phonetic = "-";
			
			ExportWord& wa = ds.words[j];
			wa.spelling = spelling;
			wa.phonetic = phonetic;
			
			actual++;
		}
		
		int a = 0;
		for(const ExportWord& wa : da.words.GetValues())
			if (!wa.phonetic.IsEmpty())
				a++;
		da.diagnostics.GetAdd("syllables: total") = IntStr(da.words.GetCount());
		da.diagnostics.GetAdd("syllables: actual") = IntStr(a);
		da.diagnostics.GetAdd("syllables: percentage") =  DblStr((double)a / (double)da.words.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}

void WordDataProcess::CopyLinkedData() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& ds = sda.dataset;
	
	Color black(0,0,0);
	for(int i = 0; i < ds.words.GetCount(); i++) {
		ExportWord& wa = ds.words[i];
		
		int fail = -1;
		for(int i = 0; i < wa.class_count && i < ExportWord::MAX_CLASS_COUNT; i++) {
			if (wa.classes[i] < 0 || wa.classes[i] >= ds.word_classes.GetCount()) {
				fail = i;
				break;
			}
		}
		if (fail >= 0)
			wa.class_count = fail;
		else if (wa.class_count < 0)
			wa.class_count = 0;
		else if (wa.class_count > ExportWord::MAX_CLASS_COUNT)
			wa.class_count = ExportWord::MAX_CLASS_COUNT;
	}
	
	for(int i = 0; i < ds.words.GetCount(); i++) {
		ExportWord& wa = ds.words[i];
		if (wa.link >= 0 && wa.link < ds.words.GetCount()) {
			ExportWord& from = ds.words[wa.link];
			if (from.spelling.IsEmpty()) {
				from.spelling = wa.spelling;
			}
			if (from.phonetic.IsEmpty()) {
				from.phonetic = wa.phonetic;
			}
			if (from.clr == black) {
				from.clr = wa.clr;
				from.class_count = wa.class_count;
				for(int i = 0; i < wa.class_count; i++) {
					from.classes[i] = wa.classes[i];
				}
			}
			wa.CopyFrom(from, false);
		}
		else if (wa.link >= 0)
			wa.link = -1;
	}
	
	NextPhase();
}


END_TEXTLIB_NAMESPACE

