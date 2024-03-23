#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TaskManager::OnTokenData(String result, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
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
		
		Vector<String> parts = Split(line, "|");
		for (String& p : parts) {
			p = TrimBoth(p);
			int wc_i = da.word_classes.FindAdd(p);
			if (wrd.class_count < wrd.MAX_CLASS_COUNT)
				FixedIndexFindAdd(wrd.classes, wrd.MAX_CLASS_COUNT, wrd.class_count, wc_i);
		}
		
		t->actual++;
	}
	
	
	da.diagnostics.GetAdd("tokens: total") = IntStr(t->total);
	da.diagnostics.GetAdd("tokens: actual") =  IntStr(t->actual);
	da.diagnostics.GetAdd("tokens: percentage") =  DblStr((double)t->actual / (double) t->total * 100);
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnAmbiguousWordPairs(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. is something : verb, noun
	
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
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
			wpp = (WordPairType*)t->tmp_ptrs[line_i];
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
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhrases(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	t->actual = 0;
	t->total = 0;
	
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
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
			vpp_p = (VirtualPhrasePart*)t->tmp_ptrs[line_i];
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
	
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhraseTypes(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 61. compound-complex sentence + complex sentence: compound-complex sentence
	
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp_ptrs.GetCount())
			continue;
		
		VirtualPhraseStruct& vps = *(VirtualPhraseStruct*)t->tmp_ptrs[line_i];
		
		line = TrimBoth(line.Mid(a+1));
		
		a = line.ReverseFind(":");
		if (a < 0)
			continue;
		
		Vector<String> classes = Split(TrimBoth(line.Left(a)), "+", false);
		//sp_is.SetCount(0);
		bool fail = false;
		CombineHash ch;
		for (String& c : classes) {
			c = TrimBoth(c);
			if (c.IsEmpty()) {
				fail = true;
				break;
			}
			int sp_i = da.struct_part_types.Find(c);
			if (sp_i < 0) {
				fail = true;
				break;
			}
			//sp_is << sp_i;
			ch.Do(sp_i).Put(1);
		}
		if (fail)
			continue;
		/*hash_t h = ch;
		
		int vps_i = da.virtual_phrase_structs.Find(h);
		if (vps_i < 0)
			continue;
		VirtualPhraseStruct& vps = da.virtual_phrase_structs[vps_i];*/
		/*int vps_i = -1;
		VirtualPhraseStruct& vps = da.virtual_phrase_structs.GetAdd(h, vps_i);
		if (vps.parts.IsEmpty())
			vps.parts <<= sp_is;*/
		vps.count++;
		
		String struct_type = TrimBoth(line.Mid(a+1));
		
		vps.struct_type = da.struct_types.FindAdd(struct_type);
	}
	
	int a = 0;
	for (const VirtualPhraseStruct& vps : da.virtual_phrase_structs.GetValues())
		if (vps.struct_type >= 0)
			a++;
	da.diagnostics.GetAdd("virtual phrase structs: total") = IntStr(da.virtual_phrase_structs.GetCount());
	da.diagnostics.GetAdd("virtual phrase structs: actual") =  IntStr(a);
	da.diagnostics.GetAdd("virtual phrase structs: percentage") =  DblStr((double)a / (double) da.virtual_phrase_structs.GetCount() * 100);
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnPhraseColors(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 12. RGB(255, 102, 0)
	
	Color black(0,0,0);
	Color non_black(1,1,1);
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			PhrasePart& pp = da.phrase_parts[pp_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		
		a = line.Find("RGB(", a+1);
		if (a < 0) continue;
		a += 4;
		int b = line.Find(")");
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		if (clr == black)
			clr = non_black;
		
		pp.clr = clr;
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.clr != black)
			a++;
	da.diagnostics.GetAdd("phrase part color: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part color: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part color: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseAttrs(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 1. Belief communities: acceptance
	
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		ASSERT(pp.attr < 0);
		
		line = TrimBoth(line.Mid(a+1));
		a = line.Find(":");
		if (a < 0) continue;
		
		AttrHeader ah;
		ah.group = ToLower(TrimBoth(line.Left(a)));
		ah.value = ToLower(TrimBoth(line.Mid(a+1)));
		da.attrs.GetAdd(ah, pp.attr);
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.attr >= 0)
			a++;
	da.diagnostics.GetAdd("phrase part attrs: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part attrs: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part attrs: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseActions(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 5. tone(admiring) + msg(expressing attraction) + bias(physical appearance) + attention-attribute(referencing arms) + attention-physical_state(strength)
	
	Vector<int> actions;
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at '+' character and parse single actions
		Vector<String> parts = Split(line, "+");
		CombineHash ch;
		actions.SetCount(0);
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			s = TrimBoth(s);
			int a = s.Find("(");
			int b = s.Find(")");
			if (a < 0 || b < 0 || a > b) {
				parts.Remove(i--);
				continue;
			}
			ActionHeader aa;
			aa.action = TrimBoth(s.Left(a));
			a++;
			aa.arg = TrimBoth(s.Mid(a,b-a));
			
			da.actions.GetAdd(aa, actions.Add());
		}
		Sort(actions, StdLess<int>());
		pp.actions <<= actions;
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (!pp.actions.IsEmpty())
			a++;
	da.diagnostics.GetAdd("phrase part actions: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part actions: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part actions: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseScores(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 8. 4 5 7 9 6 7 9 8 6 3
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		// Expect x values
		if (parts.GetCount() != SCORE_COUNT)
			continue;
		
		int i = 0;
		for (const String& part : parts)
			pp.scores[i++] = ScanInt(part);
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.HasScores())
			a++;
	da.diagnostics.GetAdd("phrase part scores: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part scores: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part scores: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseTypeclasses(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2." + res;
	
	int opt_count = GetTypeclassCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.typecasts.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				pp.typecasts.Clear();
				break;
			}
			opt--; // convert to 0-based index
			pp.typecasts.Add(opt);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.typecasts.GetCount())
			a++;
	da.diagnostics.GetAdd("typecasts: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("typecasts: actual") = IntStr(a);
	da.diagnostics.GetAdd("typecasts: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseContrast(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2. " + res;
	
	int opt_count = GetContentCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.contrasts.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				//pp.contrasts.Clear();
				//break;
				continue;
			}
			int mod = -1;
			if      (part.Find("A") >= 0 || part.Find("a") >= 0) mod = 0;
			else if (part.Find("B") >= 0 || part.Find("b") >= 0) mod = 1;
			else if (part.Find("C") >= 0 || part.Find("c") >= 0) mod = 2;
			else continue;
			opt--; // convert to 0-based index
			int code = opt * ContentType::PART_COUNT + mod;
			pp.contrasts.Add(code);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.contrasts.GetCount())
			a++;
	da.diagnostics.GetAdd("contrast: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("contrast: actual") = IntStr(a);
	da.diagnostics.GetAdd("contrast: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

#if 0
void TaskManager::OnPhraseProfile(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2." + res;
	
	int opt_count = GetProfileCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.profiles.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				pp.profiles.Clear();
				break;
			}
			opt--; // convert to 0-based index
			pp.profiles.Add(opt);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.profiles.GetCount())
			a++;
	da.diagnostics.GetAdd("singer profiles: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("singer profiles: actual") = IntStr(a);
	da.diagnostics.GetAdd("singer profiles: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseContent(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2." + res;
	
	int opt_count = GetContentCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.contents.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				pp.contents.Clear();
				break;
			}
			opt--; // convert to 0-based index
			pp.contents.Add(opt);
		}
	}
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.contents.GetCount())
			a++;
	da.diagnostics.GetAdd("contents: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("contents: actual") = IntStr(a);
	da.diagnostics.GetAdd("contents: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhrasePrimary(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2. " + res;
	
	int opt_count = GetPrimaryCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.primary.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				pp.primary.Clear();
				break;
			}
			opt--; // convert to 0-based index
			pp.primary.Add(opt);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.primary.GetCount())
			a++;
	da.diagnostics.GetAdd("primary focus: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("primary focus: actual") = IntStr(a);
	da.diagnostics.GetAdd("primary focus: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseSecondary(String res, Task* t) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2." + res;
	
	int opt_count = GetSecondaryCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)t->tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= t->tmp.GetCount())
				continue;
			int pp_i = t->tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.secondary.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				pp.secondary.Clear();
				break;
			}
			opt--; // convert to 0-based index
			pp.secondary.Add(opt);
		}
	}
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.secondary.GetCount())
			a++;
	da.diagnostics.GetAdd("secondary focus: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("secondary focus: actual") = IntStr(a);
	da.diagnostics.GetAdd("secondary focus: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}
#endif

void TaskManager::OnActionlistColors(String result, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
	
	Color black(0,0,0);
	Color non_black(1,1,1);
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.ReverseFind("\"");
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		
		a = line.Find("RGB(", b);
		if (a < 0) continue;
		a += 4;
		b = line.Find(")", a);
		if (b < 0) continue;
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = ScanInt(TrimLeft(clr_parts[0]));
		int G = ScanInt(TrimLeft(clr_parts[1]));
		int B = ScanInt(TrimLeft(clr_parts[2]));
		Color clr(R,G,B);
		a = full_action.Find("(");
		if (a < 0) continue;
		
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.ReverseFind(")");
		ah.arg = full_action.Mid(a,b-a);
		
		if (clr == black)
			clr = non_black;
		
		ExportAction& aa = da.actions.GetAdd(ah);
		aa.clr = clr;
	}
	
	int a = 0;
	for (const ExportAction& ea : da.actions.GetValues())
		if (ea.clr != black)
			a++;
	da.diagnostics.GetAdd("actionlist colors: total") = IntStr(da.virtual_phrase_parts.GetCount());
	da.diagnostics.GetAdd("actionlist colors: actual") =  IntStr(a);
	da.diagnostics.GetAdd("actionlist colors: percentage") =  DblStr((double)a / (double) da.virtual_phrase_parts.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-procedures(planning)" problem solving strategy / shortcut taking
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.ReverseFind("\"");
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		
		a = b+1;
		b = line.Find("/", b);
		if (a < 0) continue;
		AttrHeader ath;
		ath.group = TrimBoth(line.Mid(a,b-a));
		a = b+1;
		ath.value = TrimBoth(line.Mid(a));
		
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.ReverseFind(")");
		ah.arg = full_action.Mid(a,b-a);
		
		ExportAction& aa = da.actions.GetAdd(ah);
		da.attrs.GetAdd(ath, aa.attr);
	}
	
	int a = 0;
	for (const ExportAction& ea : da.actions.GetValues())
		if (ea.attr >= 0)
			a++;
	da.diagnostics.GetAdd("actionlist attrs: total") = IntStr(da.virtual_phrase_parts.GetCount());
	da.diagnostics.GetAdd("actionlist attrs: actual") =  IntStr(a);
	da.diagnostics.GetAdd("actionlist attrs: percentage") =  DblStr((double)a / (double) da.virtual_phrase_parts.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnLineActions(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	Vector<String> txt_lines = Split(batch.txt, "\n");
	ASSERT(txt_lines.GetCount());
	
	// e.g. tone(desperate) + msg(distracting oneself) + bias(impulsiveness)
	Vector<ActionHeader> actions;
	
	if (lines.GetCount() == txt_lines.GetCount()) {
		int line_idx = -1;
		Vector<int> ap_is;
		for (String& l : lines) {
			line_idx++;
			l = TrimBoth(l);
			if (l.IsEmpty())
				continue;
			
			const String& txt = txt_lines[line_idx];
			actions.SetCount(0);
			
			Vector<String> parts = Split(l, "+");
			CombineHash ch;
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				s = TrimBoth(s);
				int a = s.Find("(");
				int b = s.Find(")");
				if (a < 0 || b < 0 || a > b) {
					parts.Remove(i--);
					continue;
				}
				String action = TrimBoth(s.Left(a));
				a++;
				String arg = TrimBoth(s.Mid(a,b-a));
				
				ActionHeader& aa = actions.Add();
				aa.action = action;
				aa.arg = arg;
				
			}
			/*Sort(actions, ActionHeader());
			for (ActionHeader& aa : actions) {
				ch.Do(aa.action);
				ch.Do(aa.arg);
			}
			hash_t h = ch;
			
			bool found = false;
			int i = 0;
			for (ExportDepActionPhrase& ap : da.action_phrases.GetValues()) {
				if (ap.hash == h) {
					found = true;
					ap_is << i;
					if (line_idx == 0 && batch.song_begins)
						ap.first_lines++;
				}
				i++;
			}
			if (!found) {*/
			int ap_i = -1;
			ExportDepActionPhrase& ap = da.action_phrases.GetAdd(txt, ap_i);
			ap_is << ap_i;
			if (ap.actions.IsEmpty()) {
				//ap.hash = h;
				//ap.txt = txt;
				ap.first_lines = line_idx == 0 && batch.song_begins ? 1 : 0;
				//Swap(ap.actions, actions);
				for (const ActionHeader& ah : actions)
					da.actions.GetAdd(ah, ap.actions.Add());
			}
		}
		for(int i = 1; i < ap_is.GetCount(); i++) {
			int ap_i0 = ap_is[i-1];
			int ap_i1 = ap_is[i];
			ExportDepActionPhrase& ap0 = da.action_phrases[ap_i0];
			VectorFindAdd(ap0.next_phrases, ap_i1);
		}
	}
	
	t->update(t->batch_i, t->batches.GetCount());
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnSyllables(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	//-hey: hey [heɪ]
	//- hello: hel-lo [hɛˈloʊ]
	
	int ds_i = t->ds_i;
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	res.Replace("\r", "");
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = t->tmp_words.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		
		RemoveLineChar(line);
		int a = line.Find(":");
		if (a < 0) continue;
		String wrd = TrimBoth(line.Left(a));
		line = line.Mid(a+1);
		
		if (line_match)
			wrd = t->tmp_words[line_i];
		
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
		
		t->actual++;
	}
	
	//da.diagnostics.GetAdd("syllables: total") = IntStr(t->total);
	//da.diagnostics.GetAdd("syllables: actual") =  IntStr(t->actual);
	//da.diagnostics.GetAdd("syllables: percentage") =  DblStr((double)t->actual / (double) t->total * 100);
	
	int a = 0;
	for(const ExportWord& wa : da.words.GetValues())
		if (!wa.phonetic.IsEmpty())
			a++;
	da.diagnostics.GetAdd("syllables: total") = IntStr(da.words.GetCount());
	da.diagnostics.GetAdd("syllables: actual") = IntStr(a);
	da.diagnostics.GetAdd("syllables: percentage") =  DblStr((double)a / (double)da.words.GetCount() * 100);
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnDetails(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	res.Replace("\r", "");
	
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	
	//DUMPC(lines);
	int ds_i = t->ds_i;
	
	Color black(0,0,0);
	Color non_black(1,1,1);
	
	DatasetAnalysis& ds = sda.datasets[ds_i];
	
	bool line_match = t->tmp_words.GetCount() == lines.GetCount();
	
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
			orig = t->tmp_words[i];
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
		
		String& trans_dst = ds.translations.GetAdd(orig);
		if (trans_dst.IsEmpty())
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
		t->actual++;
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
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnLineChangeScores(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	res = "1. Stop line 1 & start line 2: S0:" + res;
	res.Replace("not applicable", "0");
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	
	Vector<int>& ap_is = t->tmp;
	
	for (String& l : lines) {
		int a = l.Find("line");
		if (a < 0) continue;
		int b = l.Find("&", a);
		if (b < 0) continue;
		a += 4;
		int line0 = StrInt(TrimBoth(l.Mid(a,b-a)));
		a = l.Find("line",b);
		if (a < 0) continue;
		b = l.Find(":", a);
		if (b < 0) continue;
		a += 4;
		int line1 = StrInt(TrimBoth(l.Mid(a,b-a)));
		
		Vector<String> parts = Split(l.Mid(b+1), ",");
		if (parts.GetCount() != SCORE_COUNT)
			continue;
		int score[SCORE_COUNT] = {0,0,0,0,0};
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			int a = s.Find(":");
			if (a < 0) continue;
			score[i] = StrInt(TrimBoth(s.Mid(a+1)));
		}
		
		line0--;
		line1--;
		if (line0 >= ap_is.GetCount() || line1 >= ap_is.GetCount()) continue;
		
		int ap_i0 = ap_is[line0];
		int ap_i1 = ap_is[line1];
		if (ap_i0 < 0 || ap_i1 < 0)
			continue;
		
		ExportDepActionPhrase& ap0 = da.action_phrases[ap_i0];
		
		int j = VectorFind(ap0.next_phrases, ap_i1);
		if (j >= 0) {
			if (ap0.next_scores.GetCount() <= j)
				ap0.next_scores.SetCount(j+1);
			Vector<int>& tgt = ap0.next_scores[j];
			tgt.SetCount(SCORE_COUNT);
			for(int j = 0; j < SCORE_COUNT; j++)
				tgt[j] = score[j];
		}
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnColorAlternatives(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	VectorMap<String, Color>& word_clr = t->word_clr;
	
	res = t->tmp_str + res;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	VectorMap<int,Vector<String>> iter_phrase_parts;
	VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
	
	int ds_i = t->ds_i;
	
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		
		int a = l.Find(":");
		if (a < 0) continue;
		String main_class = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find("->");
		if (a < 0) continue;
		String src_word = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+2));
		
		a = l.Find(":");
		if (a < 0) continue;
		String dst_clr_str = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		String dst_word = l;
		
		
		dst_clr_str = dst_clr_str.Mid(dst_clr_str.Find("(") + 1);
		dst_clr_str = dst_clr_str.Left(dst_clr_str.GetCount()-1);
		
		int clr_i[3];
		Vector<String> clr_parts = Split(dst_clr_str, ",");
		if (clr_parts.GetCount() != 3)
			continue;
		for(int i = 0; i < clr_parts.GetCount(); i++)
			clr_i[i] = StrInt(clr_parts[i]);
		Color dst_clr(clr_i[0], clr_i[1], clr_i[2]);
		
		Color src_clr = word_clr.Get(src_word, Black());
		
		
		
		
		CombineHash ch;
		ch.Do(main_class);
		ch.Do(src_word);
		ch.Do(src_clr);
		hash_t h = ch;
		
		break; // difficult problem: hash is calculated differently & matching is done differently
		#if 0
		DatasetAnalysis& ds = sda.datasets[ds_i];
		bool found = false;
		for (ColorWordnet& wn : ds.clr_wordnets) {
			if (wn.hash == h) {
				int j = VectorFindAdd(wn.words, dst_word);
				if (j == wn.colors.GetCount())
					wn.colors.Add(dst_clr);
				found = true;
			}
		}
		if (!found) {
			ColorWordnet& wn = ds.clr_wordnets.Add();
			wn.hash = h;
			wn.words << dst_word;
			wn.colors << dst_clr;
			wn.main_class = main_class;
			wn.src_word = src_word;
			wn.clr = src_clr;
			wn.clr_group = GetColorGroup(src_clr);
		}
		#endif
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnWordData(String res, Task* t) {
	
	
	
	t->batch_i++;
	t->running = false;
}

#if 0
void TaskManager::OnSongStory(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	Component& song = *t->song;
	
	
	RemoveEmptyLines3(res);
	
	Vector<String> lines = Split(res, "\n");
	VectorMap<String, Vector<int>> results;
	
	//StaticPart& sp = song.parts[t->part_i];
	
	for(String& line : lines) {
		int a = line.Find(":");
		if (a < 0) continue;
		
		String part_name = TrimBoth(line.Left(a));
		
		a = line.Find(":", a+1);
		if (a < 0) continue;
		
		auto& part_results = results.GetAdd(ToLower(part_name));
		
		String parts_str = TrimBoth(line.Mid(a+1));
		Vector<String> parts = Split(parts_str, ",");
		for (String& p : parts) {
			p = TrimBoth(p);
			int c = ScanInt(p);
			c--;
			part_results << c;
		}
	}
	
	for(int i = 0; i < results.GetCount(); i++) {
		String res_part = results.GetKey(i);
		const auto& part_lines = results[i];
		#if 1
		StaticPart* sp = song.FindPartByName(res_part);
		if (!sp)
			continue;
		const auto& nana_lines = sp->nana.Get();
		int c = min(part_lines.GetCount(), nana_lines.GetCount());
		for(int j = 0; j < c; j++) {
			int row = part_lines[j];
			int pp_i = -1;
			if (row < song.picked_phrase_parts.GetCount())
				pp_i = song.picked_phrase_parts[row];
			sp->nana.SetPhrasePart(j, pp_i);
		}
		#else
		if (ToLower(res_part) != ToLower(sp.name))
			continue;
		const auto& nana_lines = sp.nana.Get();
		int c = min(part_lines.GetCount(), nana_lines.GetCount());
		for(int j = 0; j < c; j++) {
			int row = part_lines[j];
			int pp_i = -1;
			if (row < sp.picked_phrase_parts.GetCount())
				pp_i = sp.picked_phrase_parts[row];
			sp.nana.SetPhrasePart(j, pp_i);
		}
		#endif
		
	}
	
	//t->batch_i++;
	//t->running = false;
	t->on_ready();
	
	RemoveTask(*t);
}
#endif

void TaskManager::OnSubPicked(String result, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	Component& song = *t->component;
	
	TODO
	#if 0
	
	result = TrimBoth(result);
	if (result.GetCount()) {
		int sub_i = -1;
		if (IsDigit(result[0]))
			sub_i = ScanInt(result) - 1;
		else
			sub_i = t->tmp_words.Find(result);
		
		if (sub_i >= 0) {
			ASSERT(t->part_i >= 0 && t->line_i >= 0);
			StaticPart& sp = song.parts[t->part_i];
			auto& line = sp.nana.Get()[t->line_i];
			
			if (sub_i >= 0 && sub_i < line.sub_pp_i.GetCount()) {
				int pp_i = line.sub_pp_i[sub_i];
				line.end_pp_i = pp_i;
			}
			else {
				line.end_pp_i = -1;
			}
		}
	}
	
	t->on_ready();
	
	RemoveTask(*t);
	#endif
}

void TaskManager::OnNanaFit(String result, Task* t) {
	
	
	
	
	t->on_ready();
	
	RemoveTask(*t);
}

void TaskManager::OnAttributes(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	
	RemoveEmptyLines2(res);
	Vector<String> parts = Split(res, "\n");
	
	if (parts.GetCount() != 2) {
		if (parts.GetCount() == 1) {
			if (parts[0].Find(" vs. ") >= 0)	parts = Split(parts[0], " vs. ");
			if (parts[0].Find("/") >= 0)		parts = Split(parts[0], "/");
			if (parts[0].Find(" vs ") >= 0)		parts = Split(parts[0], " vs ");
			if (parts[0].Find(" - ") >= 0)		parts = Split(parts[0], " - ");
			if (parts[0].Find(" and ") >= 0)	parts = Split(parts[0], " and ");
		}
	}
	if (parts.GetCount() == 2) {
		if (parts[0].Find(" vs. ") >= 0)
			parts = Split(parts[0], " vs. ");
		String& f = parts[0];
		String& l = parts[1];
		RemoveLineChar(f);
		RemoveLineChar(l);
		int a = f.Find("1.");
		if (a >= 0) {
			f = f.Mid(a+2);
		}
		f = TrimBoth(f);
		l = TrimBoth(l);
		
		Vector<String> keys;
		keys.SetCount(2);
		for(int i = 0; i < parts.GetCount(); i++) {
			String& part = parts[i];
			int a0 = part.Find(":");
			int a1 = part.Find("-");
			int a2 = part.Find("/");
			int a3 = part.Find("\n");
			int a4 = part.Find(",");
			int a5 = part.Find("(");
			int a = INT_MAX;
			if (a0 >= 0 && a0 < a) a = a0;
			if (a1 >= 0 && a1 < a) a = a1;
			if (a2 >= 0 && a2 < a) a = a2;
			if (a3 >= 0 && a3 < a) a = a3;
			if (a4 >= 0 && a4 < a) a = a4;
			if (a5 >= 0 && a5 < a) a = a5;
			if (a == INT_MAX) a = part.GetCount();
			/*if (a == INT_MAX && part.GetCount() < 100) a = part.GetCount();
			if (a == INT_MAX) {
				WString ws = part.ToWString();
				LOG(ws);
				DUMPC(ws);
			}*/
			if (a == INT_MAX)
				continue;
			String& key = keys[i];
			key = TrimBoth(part.Left(a));
		}
		
		if (keys[0].GetCount() && keys[1].GetCount()) {
			String group = ToLower(t->tmp_str);
			int attr_i[2] = {-1,-1};
			for(int i = 0; i < keys.GetCount(); i++) {
				AttrHeader ah;
				ah.group = group;
				ah.value = ToLower(keys[i]);
				
				da.attrs.GetAdd(ah, attr_i[i]);
			}
			
			
			ExportSimpleAttr& sat = da.simple_attrs.GetAdd(t->tmp_str);
			sat.attr_i0 = attr_i[0];
			sat.attr_i1 = attr_i[1];
			
			const Index<String>& v = t->uniq_attrs.Get(group);
		}
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnAttributePolars(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	
	if (lines.GetCount() == t->tmp_words.GetCount()) {
		String group = t->tmp_str;
		int i = da.simple_attrs.Find(group);
		String pos_value, neg_value;
		if (i >= 0) {
			const ExportSimpleAttr& esa = da.simple_attrs[i];
			pos_value = da.attrs.GetKey(esa.attr_i0).value;
			neg_value = da.attrs.GetKey(esa.attr_i1).value;
		}
		for(int i = 0; i < lines.GetCount(); i++) {
			String key = t->tmp_words[i];
			String value = TrimBoth(ToLower(lines[i]));
			bool is_negative = value.Find("negative") >= 0;
			
			// Force the value, if the key is the extreme (and AI got it wrong somehow)
			if (key == pos_value)
				is_negative = false;
			else if (key == neg_value)
				is_negative = true;
			
			AttrHeader ah;
			ah.group = group;
			ah.value = key;
			int j = da.attrs.Find(ah);
			if (j < 0)
				continue;
			
			ExportAttr& ea = da.attrs[j];
			ea.positive = !is_negative;
			ea.simple_attr = i;
		}
		
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnAttributeJoins(String res, Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	
	if (lines.GetCount() == t->tmp_words.GetCount()) {
		
		for(int i = 0; i < lines.GetCount(); i++) {
			String& l = lines[i];
			Vector<String> ah_parts = Split(t->tmp_words[i], ": ");
			if (ah_parts.GetCount() != 2)
				continue;
			AttrHeader ah;
			ah.group = ah_parts[0];
			ah.value = ah_parts[1];
			
			int attr_i = da.attrs.Find(ah);
			if (attr_i < 0)
				continue;
			ExportAttr& ea = da.attrs[attr_i];
			
			String digit, sign;
			for(int i = 0; i < l.GetCount(); i++) {
				int chr = l[i];
				if (IsDigit(chr))
					digit.Cat(chr);
				else if (chr == '+' || chr == '-') {
					sign.Cat(chr);
					break;
				}
				else if (chr == ',')
					break;
			}
			if (digit.IsEmpty() || sign.IsEmpty())
				continue;
			
			int group_i = ScanInt(digit);
			bool is_positive = sign == "+";
			
			if (group_i < 0 || group_i >= t->tmp_words2.GetCount())
				continue;
			String group = t->tmp_words2[group_i];
			
			AttrHeader link_ah;
			link_ah.group = group;
			link_ah.value = ah.value;
			int link_i = -1;
			da.attrs.GetAdd(link_ah, link_i);
			
			ea.link = link_i;
		}
		
	}
	
	
	t->batch_i++;
	t->running = false;
}


END_TEXTLIB_NAMESPACE

