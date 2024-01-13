#include "SongLib.h"

namespace SongLib {

void TaskManager::OnTokenData(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
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
		
	}
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnAmbiguousWordPairs(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. is something : verb, noun
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	int offset = 1+1;
	
	for (String& line : lines) {
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
		
		int w_i0 = da.words.Find(result_words[0]);
		int w_i1 = da.words.Find(result_words[1]);
		CombineHash ch;
		ch.Do(w_i0).Put(1).Do(w_i1);
		hash_t h = ch;
		
		//ExportWord& wrd0 = da.words[w_i0];
		//ExportWord& wrd1 = da.words[w_i1];
		WordPairType& wp = da.ambiguous_word_pairs.GetAdd(h);
		
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
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhrases(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	Vector<int> word_classes;
	for (String& line : lines) {
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
		
		int vpp_i = -1;
		VirtualPhrasePart& vpp = da.virtual_phrase_parts.GetAdd(h, vpp_i);
		if (vpp.word_classes.IsEmpty())
			vpp.word_classes <<= word_classes;
		
		line = TrimBoth(line.Mid(a+1));
		
		vpp.struct_part_type = da.struct_part_types.FindAdd(line);
	}
	
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhraseTypes(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 61. compound-complex sentence + complex sentence: compound-complex sentence
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int vps_i = t->tmp[line_i];
		VirtualPhraseStruct& vps = da.virtual_phrase_structs[vps_i];
		
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
		hash_t h = ch;
		
		/*int vps_i = da.virtual_phrase_structs.Find(h);
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
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnPhraseColors(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 12. RGB(255, 102, 0)
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
		
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
		
		pp.clr = clr;
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseAttrs(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 1. Belief communities: acceptance
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		line = TrimBoth(line.Mid(a+1));
		a = line.Find(":");
		if (a < 0) continue;
		
		AttrHeader ah;
		ah.group = ToLower(TrimBoth(line.Left(a)));
		ah.value = ToLower(TrimBoth(line.Mid(a+1)));
		da.attrs.GetAdd(ah, pp.attr);
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseActions(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 5. tone(admiring) + msg(expressing attraction) + bias(physical appearance) + attention-attribute(referencing arms) + attention-physical_state(strength)
	
	Vector<int> actions;
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
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
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseScores(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 8. 4 5 7 9 6 7 9 8 6 3
	
	Vector<int> actions;
	int offset = 1+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	for (String& line : lines) {
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
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
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistColors(String result, Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
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
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ExportAction& aa = da.actions.GetAdd(ah);
		aa.clr = clr;
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-procedures(planning)" problem solving strategy / shortcut taking
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
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
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ExportAction& aa = da.actions.GetAdd(ah);
		da.attrs.GetAdd(ath, aa.attr);
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnLineActions(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	
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
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	//-hey: hey [heɪ]
	//- hello: hel-lo [hɛˈloʊ]
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for (String& line : lines) {
		RemoveLineChar(line);
		int a = line.Find(":");
		if (a < 0) continue;
		String wrd = TrimBoth(line.Left(a));
		line = line.Mid(a+1);
		
		a = line.Find("[");
		if (a < 0) continue;
		String spelling = TrimBoth(line.Left(a));
		a++;
		int b = line.Find("]", a);
		if (b < 0) continue;
		WString phonetic = TrimBoth(line.Mid(a,b-a)).ToWString();
		
		int ds_i = t->tmp_map_ds_i.Get(wrd, -1);
		if (ds_i < 0) continue;
		String ds_key = sd.GetKey(ds_i);
		
		// hotfix
		if (1) {
			wrd = ToLower(wrd.ToWString()).ToString();
			wrd.Replace("'", "");
		}
		
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		ExportWord& wa = ds.words.GetAdd(wrd);
		wa.spelling = spelling;
		wa.phonetic = phonetic;
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnDetails(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	res.Replace("\r", "");
	
	Vector<String> lines = Split(res, "\n");
	//DUMPC(lines);
	
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
		
		
		//orig = ToLower(orig.ToWString()).ToString();
		//orig.Replace("'", "");
		
		
		
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
		
		int j = t->tmp_words.Find(orig);
		if (j < 0) continue;
		int ds_i = t->tmp_ds_i[j];
		DatasetAnalysis& ds = sda.datasets[ds_i];
		
		j = ds.words.Find(orig);
		if (j < 0) continue;
		
		ExportWord& wa = ds.words[j];
		if (wa.class_count == 0) {
			int c_i = ds.word_classes.FindAdd(main_class);
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
		}
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnLineChangeScores(String res, Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Task::Batch& batch = t->batches[t->batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	res = "1. Stop line 1 & start line 2: S0:" + res;
	res.Replace("not applicable", "0");
	
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	
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
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	VectorMap<String, int>& word_ds = t->tmp_map_ds_i;
	VectorMap<String, Color>& word_clr = t->word_clr;
	
	res = t->tmp_str + res;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	VectorMap<int,Vector<String>> iter_phrase_parts;
	VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
	
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
		
		int ds_i = word_ds.Get(src_word, -1);
		if (ds_i < 0)
			continue;
		
		Color src_clr = word_clr.Get(src_word, Black());
		
		
		
		
		CombineHash ch;
		ch.Do(main_class);
		ch.Do(src_word);
		ch.Do(src_clr);
		hash_t h = ch;
		
		TODO // difficult problem: hash is calculated differently & matching is done differently
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
	
	
	
}

}
