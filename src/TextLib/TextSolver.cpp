#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE

TextSolver::TextSolver() {
	
}

int TextSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void TextSolver::DoPhase() {
	ASSERT(appmode >= 0);
	MetaDatabase& mdb = MetaDatabase::Single();
	TextDatabase& db = mdb.db[appmode];
	SourceData& sd = db.src_data;
	DatasetAnalysis& da = sd.a.dataset;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& ds = sda.dataset;
	
	if (phase == PHASE_TOKENIZE) {
		error.Clear();
		tokens.Clear();
		
		String str = text;
		
		NaturalTokenizer tk;
		HotfixReplaceWord(str);
			
		if (!tk.Parse(str)) {
			SetError("Parse failed");
			return;
		}
		
		for (const auto& line : tk.GetLines()) {
			for (const WString& line : line) {
				String s = line.ToString();
				s = ToLower(s.ToWString()).ToString();
				int tk_i = -1;
				Token& tk = da.tokens.GetAdd(s, tk_i);
				tokens << tk_i;
			}
		}
		
		NextPhase();
	}
	else if (phase == PHASE_SOLVE_WORDS) {
		TokenArgs args;
		args.fn = 0;
		args.words.Clear();
		
		Index<String> added;
		for(int i = 0; i < tokens.GetCount(); i++) {
			int t_i = tokens[i];
			const String& tk = da.tokens.GetKey(t_i);
			if (added.FindAdd(tk) >= 0) continue;
			int j = da.words.Find(tk);
			if (j >= 0) {
				ExportWord& wrd = da.words[j];
				if (wrd.class_count > 0)
					continue;
			}
			args.words << tk;
		}
		
		if (args.words.IsEmpty()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetTokenData(appmode, args, [this](String result) {
			MetaDatabase& mdb = MetaDatabase::Single();
			TextDatabase& db = mdb.db[appmode];
			SourceData& sd = db.src_data;
			DatasetAnalysis& da = sd.a.dataset;
			
			result.Replace("\r", "");
			Vector<String> lines = Split(result, "\n");
			
			for (String& line : lines) {
				line = TrimBoth(line);
				
				if (line.IsEmpty() ||!IsDigit(line[0]))
					continue;
				
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
				
				String result_word = TrimBoth(line.Left(a));
				
				int orig_word_i = -1;
				ExportWord& wrd = da.words.GetAdd(result_word, orig_word_i);
				
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
			}
			
			SetWaiting(0);
			NextPhase();
		});
	}
	else if (phase == PHASE_WORD_SPELLING) {
		SourceDataAnalysisArgs args; // 4
		args.fn = 4;
		
		int iter = 0;
		
		words.Clear();
		
		Index<String> added;
		for(int i = 0; i < tokens.GetCount(); i++) {
			int t_i = tokens[i];
			const String& wrd = da.tokens.GetKey(t_i);
			if (wrd.IsEmpty())
				continue;
			if (added.Find(wrd) >= 0)
				continue;
			added.Add(wrd);
			const ExportWord& wa = ds.words.GetAdd(wrd);
			
			if (wa.spelling.GetCount() && wa.phonetic.GetCount())
				continue;
			
			// HASH BREAKING FEATURE
			if (wrd[0] == '{' || wrd[wrd.GetCount()-1] == '}')
				continue;
			if (wa.link >= 0) {
				LOG("warning: no link in " + wrd);
				continue;
			}
			
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
			
			args.words << wrd;
			words << wrd;
		}
		
		
		if (args.words.IsEmpty()) {
			NextPhase();
			return;
		}
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetSourceDataAnalysis(appmode, args, [this](String res) {
			MetaDatabase& mdb = MetaDatabase::Single();
			TextDatabase& db = mdb.db[appmode];
			SourceData& sd = db.src_data;
			SourceDataAnalysis& sda = db.src_data.a;
			DatasetAnalysis& da = sda.dataset;
			//-hey: hey [heɪ]
			//- hello: hel-lo [hɛˈloʊ]
			
			DatasetAnalysis& ds = sda.dataset;
			
			res.Replace("\r", "");
			RemoveEmptyLines(res);
			Vector<String> lines = Split(res, "\n");
			bool line_match = this->words.GetCount() == lines.GetCount();
			
			int line_i = -1;
			for (String& line : lines) {
				line_i++;
				
				RemoveLineChar(line);
				int a = line.Find(":");
				if (a < 0) continue;
				String wrd = TrimBoth(line.Left(a));
				line = line.Mid(a+1);
				
				if (line_match)
					wrd = this->words[line_i];
				
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
			}
			
			SetWaiting(0);
			NextPhase();
		});
	}
	else if (phase == PHASE_MAKE_RHYME_CONTAINER) {
		rc.Clear();
		RhymeContainer::Line& l = rc.Get().Add();
		l.text = this->text;
		
		for(int i = 0; i < tokens.GetCount(); i++) {
			int t_i = tokens[i];
			const String& wrd = da.tokens.GetKey(t_i);
			if (wrd.IsEmpty())
				continue;
			const ExportWord& wa = ds.words.GetAdd(wrd);
			
			LOG(wrd + ": \"" + wa.spelling.ToString() + "\", \"" + wa.phonetic.ToString() + "\"");
			
			RhymeContainer::Word& rcw = l.words.Add();
			Vector<WString> syllables = Split(wa.phonetic, String(".").ToWString());
			for(int j = 0; j < syllables.GetCount(); j++) {
				RhymeContainer::Syllable& syl = rcw.syllables.Add();
				syl.phonetic = syllables[j];
			}
		}
		LOG("");
		
		NextPhase();
	}
}

TextSolver& TextSolver::Get(int appmode, String line) {
	static ArrayMap<String, TextSolver> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + "): " + line;
	TextSolver& ts = arr.GetAdd(key);
	ts.text = line;
	ts.appmode = appmode;
	return ts;
}

	
END_TEXTLIB_NAMESPACE