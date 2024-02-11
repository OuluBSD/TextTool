#include "SongLib.h"

namespace SongLib {

void TaskManager::GetSongs(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	Vector<int> token_is;
	
	
	String sd_key = sd.GetKey(t->ds_i);
	bool filter_foreign = sd_key == "en";
	
	DatasetAnalysis& da = sd.a.datasets[t->ds_i];
	Vector<ArtistDataset>& artists = sd[t->ds_i];
	
	TimeStop ts;
	int total = 0, actual = 0;
	int well_filter_loss = 0, parse_loss = 0, foreign_loss = 0;
	for(int j = 0; j < artists.GetCount(); j++) {
		ArtistDataset& artist = artists[j];
		
		for(int k = 0; k < artist.lyrics.GetCount(); k++) {
			total++;
			LyricsDataset& lyrics = artist.lyrics[k];
			
			NaturalTokenizer tk;
			
			String str = lyrics.text;
			
			// Ignore files with hard ambiguities
			if (str.Find(" well ") >= 0) {
				// well or we'll... too expensive to figure out
				well_filter_loss++;
				continue;
			}
			
			HotfixReplaceWord(str);
			
			if (!tk.Parse(str)) {
				parse_loss++;
				continue;
			}
			
			if (filter_foreign && tk.HasForeign()) {
				foreign_loss++;
				continue;
			}
			
			for (const auto& line : tk.GetLines()) {
				token_is.SetCount(0);
				CombineHash ch;
				for (const WString& line : line) {
					String s = line.ToString();
					int tk_i = -1;
					Token& tk = da.tokens.GetAdd(s, tk_i);
					ch.Do(tk_i);
					token_is << tk_i;
				}
				hash_t h = ch;
				
				TokenText& tt = da.token_texts.GetAdd(h);
				if (tt.tokens.IsEmpty()) {
					Swap(tt.tokens, token_is);
				}
			}
			
			actual++;
			
			if (total % 500 == 0) {
				da.diagnostics.GetAdd("songs: total") = IntStr(total);
				da.diagnostics.GetAdd("songs: actual") =  IntStr(actual);
				da.diagnostics.GetAdd("songs: percentage") =  DblStr((double)actual / (double) total * 100);
				da.diagnostics.GetAdd("songs: filter 'well' loss") =  DblStr((double)well_filter_loss / (double) total * 100);
				da.diagnostics.GetAdd("songs: filter 'parse success' loss") =  DblStr((double)parse_loss / (double) total * 100);
				da.diagnostics.GetAdd("songs: filter 'foreign' loss") =  DblStr((double)foreign_loss / (double) total * 100);
			}
		}
	}
	
	da.diagnostics.GetAdd("songs: total") = IntStr(total);
	da.diagnostics.GetAdd("songs: actual") =  IntStr(actual);
	da.diagnostics.GetAdd("songs: percentage") =  DblStr((double)actual / (double) total * 100);
	da.diagnostics.GetAdd("songs: filter 'well' loss") =  DblStr((double)well_filter_loss / (double) total * 100);
	da.diagnostics.GetAdd("songs: filter 'parse success' loss") =  DblStr((double)parse_loss / (double) total * 100);
	da.diagnostics.GetAdd("songs: filter 'foreign' loss") =  DblStr((double)foreign_loss / (double) total * 100);
	da.diagnostics.GetAdd("songs: duration of song process") =  ts.ToString();
	
}

void TaskManager::GetTokenData(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
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
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 0)
		m.GetTokenData(args, THISBACK1(OnTokenData, t));
	
}

void TaskManager::GetUnknownTokenPairs(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	DatasetAnalysis& da = sd.a.datasets[t->ds_i];
	
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
				const ExportWord& ew = da.words[w_i];
				bool is_unknown = ew.class_count > 1;
				
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
				}
				else*/
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
}

void TaskManager::GetAmbiguousWordPairs(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TokenArgs& args = token_args;
	args.fn = 1;
	args.words.Clear();
	
	int per_action_task = 100;
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.ambiguous_word_pairs.GetCount());
	int iter = 0;
	
	t->tmp_ptrs.Clear();
	
	for (const WordPairType& wp : da.ambiguous_word_pairs.GetValues()) {
		if (wp.from < 0 || wp.to < 0)
			continue;
		if (wp.from_type >= 0 && wp.to_type >= 0)
			continue;
		
		if (iter >= begin && iter < end) {
			const String& from = da.words.GetKey(wp.from);
			const String& to = da.words.GetKey(wp.to);
			args.words << (from + " " + to);
			t->tmp_ptrs << (void*)&wp;
		}
		else if (iter >= end)
			break;
		iter++;
	}
	if (args.words.IsEmpty()) {
		RemoveTask(*t);
		return;
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 1)
		m.GetTokenData(args, THISBACK1(OnAmbiguousWordPairs, t));
	
}

void TaskManager::GetWordProcess(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String ds_key = sd.GetKey(t->ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	
	const Vector<ArtistDataset>& dataset = sd[t->ds_i];
	for(int j = 0; j < dataset.GetCount(); j++) {
		const ArtistDataset& artist = dataset[j];
		//ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
		//aa.word_counts.Clear();
		
		for(int k = 0; k < artist.lyrics.GetCount(); k++) {
			const LyricsDataset& song = artist.lyrics[k];
			
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
}

void TaskManager::GetWordFix(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String ds_key = sd.GetKey(t->ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	if (t->fn == 0) {
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
	}
	else if (t->fn == 1) {
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
				wa.CopyFrom(from);
			}
			else if (wa.link >= 0)
				wa.link = -1;
		}
	}
}

void TaskManager::GetVirtualPhrases(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	if (t->fn == 0) {
		Vector<int> word_is, word_classes;
		t->actual = 0;
		t->total = 0;
		for(int i = 0; i < da.token_texts.GetCount(); i++) {
			TokenText& txt = da.token_texts[i];
			t->total++;
			
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
				t->actual++;
			}
		}
		
		da.diagnostics.GetAdd("token texts to virtual phrases: total") = IntStr(t->total);
		da.diagnostics.GetAdd("token texts to virtual phrases: actual") =  IntStr(t->actual);
		da.diagnostics.GetAdd("token texts to virtual phrases: percentage") =  DblStr((double)t->actual / (double) t->total * 100);
		
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
			vps.count = 0;
			vp.virtual_phrase_struct = vps_i;
		}
		LOG(da.virtual_phrase_parts.GetCount());
		LOG(da.virtual_phrase_parts.GetCount() * 100.0 / da.virtual_phrases.GetCount());
		RemoveTask(*t);
	}
	else if (t->fn == 1) {
		TokenArgs& args = token_args;
		args.fn = 2;
		args.words.Clear();
		
		int per_action_task = 75;
		int iter = 0;
		int begin = t->batch_i * per_action_task;
		int end = begin + per_action_task;
		end = min(end, da.virtual_phrase_parts.GetCount());
		t->tmp_ptrs.Clear();
		
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
				t->tmp_ptrs << (void*)&vpp;
			}
			else if (iter >= end)
				break;
			
			iter++;
		}
		
		if (args.words.IsEmpty()) {
			RemoveTask(*t);
			return;
		}
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetTokenData(args, THISBACK1(OnVirtualPhrases, t));
	}
	else if (t->fn == 2) {
		TokenArgs& args = token_args;
		args.fn = 3;
		args.words.Clear();
		
		int per_action_task = 75;
		int begin = t->batch_i * per_action_task;
		int end = begin + per_action_task;
		end = min(end, da.virtual_phrase_structs.GetCount());
		
		int iter = 0;
		t->tmp_ptrs.Clear();
		for (const VirtualPhraseStruct& vps : da.virtual_phrase_structs.GetValues()) {
			if (vps.struct_type >= 0)
				continue;
			
			if (iter >= begin && iter < end) {
				String s;
				bool fail = false;
				for(int j = 0; j < vps.virtual_phrase_parts.GetCount(); j++) {
					if (j) s << " + ";
					int vpp_i = vps.virtual_phrase_parts[j];
					
					const VirtualPhrasePart& vpp = da.virtual_phrase_parts[vpp_i];
					if (vpp.struct_part_type < 0) {
						fail = true;
						break;
					}
					
					String type_str = da.struct_part_types[vpp.struct_part_type];
					if (type_str.IsEmpty()) {
						fail = true;
						break;
					}
					s << type_str;
				}
				if (fail)
					continue;
				if (s.IsEmpty())
					continue;
				
				args.words << s;
				t->tmp_ptrs << (void*)&vps;
			}
			else if (iter >= end)
				break;
			
			iter++;
		}
		
		if (args.words.IsEmpty()) {
			RemoveTask(*t);
			return;
		}
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetTokenData(args, THISBACK1(OnVirtualPhraseTypes, t));
	}
	
	// Token texts to phrases
	else if (t->fn == 3) {
		// NOTE: see duplicate in fn 0
		// TODO reduce duplicate code
		Vector<int> word_is, word_classes;
		int a = 0;
		for(int i = 0; i < da.token_texts.GetCount(); i++) {
			const TokenText& txt = da.token_texts[i];
			if (txt.virtual_phrase < 0)
				continue;
			
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
			if (word_is.GetCount() != vp.word_classes.GetCount())
				continue;
			
			a++;
			
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
		
		da.diagnostics.GetAdd("token text to phrase: total") = IntStr(da.token_texts.GetCount());
		da.diagnostics.GetAdd("token text to phrase: actual") = IntStr(a);
		da.diagnostics.GetAdd("token text to phrase: percentage") =  DblStr((double)a / (double)da.token_texts.GetCount() * 100);
		
	}
}

void TaskManager::GetPhrases(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	PhraseArgs& args = phrase_args;
	args.fn = t->fn;
	args.phrases.Clear();
	
	int per_action_task = 100;
	if (args.fn >= 2)
		per_action_task = 35;
	
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.ambiguous_word_pairs.GetCount());
	
	Color no_clr(0,0,0);
	t->tmp_ptrs.SetCount(0);
	
	int iter = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues()) {
		
		if (t->fn == 0 && pp.clr != no_clr)
			continue;
		
		if (t->fn == 1 && pp.attr >= 0)
			continue;
		
		if (t->fn == 2 && !pp.actions.IsEmpty())
			continue;
		
		if (t->fn == 3 && pp.HasScores())
			continue;
		
		if (t->fn == 4 && !pp.typecasts.IsEmpty())
			continue;
		
		if (t->fn == 5 && !pp.profiles.IsEmpty())
			continue;
		
		if (t->fn == 6 && !pp.primary.IsEmpty())
			continue;
		
		#if 0
		if (t->fn == 7 && !pp.secondary.IsEmpty())
			continue;
		#else
		if (t->fn == 7 && !pp.archetypes.IsEmpty())
			continue;
		#endif
		
		if (iter >= begin && iter < end) {
			String phrase = da.GetWordString(pp.words);
			args.phrases << phrase;
			t->tmp_ptrs << (void*)&pp;
		}
		else if (iter >= end)
			break;
		iter++;
	}
	
	if (args.phrases.IsEmpty()) {
		if (t->fn < 7) {
			t->fn++;
			t->batch_i = 0;
			t->running = false;
		}
		else RemoveTask(*t);
		return;
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	
	if (args.fn == 0)
		m.GetPhraseData(args, THISBACK1(OnPhraseColors, t));
	else if (args.fn == 1)
		m.GetPhraseData(args, THISBACK1(OnPhraseAttrs, t));
	else if (args.fn == 2)
		m.GetPhraseData(args, THISBACK1(OnPhraseActions, t));
	else if (args.fn == 3)
		m.GetPhraseData(args, THISBACK1(OnPhraseScores, t));
	else if (args.fn == 4)
		m.GetPhraseData(args, THISBACK1(OnPhraseTypecasts, t));
	else if (args.fn == 5)
		m.GetPhraseData(args, THISBACK1(OnPhraseProfile, t));
	else if (args.fn == 6)
		m.GetPhraseData(args, THISBACK1(OnPhrasePrimary, t));
	else if (args.fn == 7)
		m.GetPhraseData(args, THISBACK1(OnPhraseSecondary, t));
	
}

void TaskManager::GetNana(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	Song& song = *t->song;
	
	t->tmp.Clear();
	
	NanaArgs args;
	args.fn = t->fn;
	
	if (args.fn == 0) {
		for(int i = 0; i < song.parts.GetCount(); i++) {
			const StaticPart& sp = song.parts[i];
			args.parts << sp.name;
			args.counts << sp.nana.Get().GetCount();
		}
		#if !PRIMARY_STATIC_PART
		for(int i = 0; i < song.picked_phrase_parts.GetCount(); i++) {
			int pp_i = song.picked_phrase_parts[i];
			const PhrasePart& pp = da.phrase_parts[pp_i];
			String phrase = da.GetWordString(pp.words);
			args.phrases << phrase;
		}
		#else
		ASSERT(t->part_i >= 0);
		const StaticPart& sp = song.parts[t->part_i];
		args.part = sp.name;
		for(int i = 0; i < sp.picked_phrase_parts.GetCount(); i++) {
			int pp_i = sp.picked_phrase_parts[i];
			const PhrasePart& pp = da.phrase_parts[pp_i];
			String phrase = da.GetWordString(pp.words);
			args.phrases << phrase;
		}
		#endif
	}
	if (args.fn == 1) {
		ASSERT(t->part_i >= 0 && t->line_i >= 0);
		const StaticPart& sp = song.parts[t->part_i];
		const auto& line = sp.nana.Get()[t->line_i];
		
		if (line.pp_i < 0 || line.sub_pp_i.IsEmpty()) {
			RemoveTask(*t);
			return;
		}
		
		const PhrasePart& pp = da.phrase_parts[line.pp_i];
		args.phrase = da.GetWordString(pp.words);
		for(int i = 0; i < line.sub_pp_i.GetCount(); i++) {
			const PhrasePart& pp0 = da.phrase_parts[line.sub_pp_i[i]];
			String p = da.GetWordString(pp0.words);
			args.phrases << p;
			t->tmp_words.Add(p);
		}
	}
	
	if (t->part_i >= 0) {
		const StaticPart& sp = song.parts[t->part_i];
		const RhymeContainer::Line* line = t->line_i >= 0 ? &sp.nana.Get()[t->line_i] : 0;
		String pre_text;
		for(int i = 0; i < song.parts.GetCount(); i++) {
			const auto& lines = song.parts[i].nana.Get();
			bool end = false;
			if (i == t->part_i && t->line_i < 0) break;
			for(int j = 0; j < lines.GetCount(); j++) {
				const auto& line0 = lines[j];
				if (i == t->line_i && i == t->part_i) {
					if (&line0 == line) {
						end = true;
						break;
					}
				}
				if (!pre_text.IsEmpty())
					pre_text << "\n";
				if (line0.pp_i >= 0) {
					const PhrasePart& pp0 = da.phrase_parts[line0.pp_i];
					pre_text << da.GetWordString(pp0.words);
				}
				if (line0.end_pp_i >= 0) {
					const PhrasePart& pp1 = da.phrase_parts[line0.end_pp_i];
					if (line0.pp_i >= 0)
						pre_text << ", ";
					pre_text << da.GetWordString(pp1.words);
				}
			}
			if (end)
				break;
		}
		args.pre_text = pre_text;
	}
	/*if (args.fn == 1) {
		ASSERT(t->part_i >= 0 && t->line_i >= 0);
		const StaticPart& sp = song.parts[t->part_i];
		const auto& line = sp.nana.Get()[t->line_i];
		
		//const StaticPhrase& spa = sp.phrases[t->line_i];
		if (line.pp_i < 0) {
			RemoveTask(*t);
			return;
		}
		const PhrasePart& pp = da.phrase_parts[line.pp_i];
		
		args.nana = line.AsNana();
		args.phrase = da.GetWordString(pp.words);
		args.pron = da.GetWordPronounciation(pp.words);
		
	}*/
	
	
	RealizePipe();
	TaskMgr& m = *pipe;
	
	if (args.fn == 0)
		m.GetNanaData(args, THISBACK1(OnSongStory, t));
	if (args.fn == 1)
		//m.GetNanaData(args, THISBACK1(OnNanaFit, t));
		m.GetNanaData(args, THISBACK1(OnSubPicked, t));
}

void TaskManager::GetActionlist(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	ActionAnalysisArgs args;
	args.fn = t->fn;
	
	int per_action_task = 0;
	if (t->fn == 0)	per_action_task = per_action_clrs;
	if (t->fn == 1)	per_action_task = per_action_attrs;
	
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	
	Color black(0,0,0);
	int iter = 0;
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		ActionHeader ah;
		ah.action = uniq_acts.GetKey(i);
		
		const VectorMap<String,int>& idx = uniq_acts[i];
		for(int j = 0; j < idx.GetCount(); j++) {
			ah.arg = idx.GetKey(j);
			
			if (iter >= begin && iter < end) {
				ExportAction& aa = da.actions.GetAdd(ah);
				
				if (t->fn == 0 && aa.clr != black)
					continue;
				
				if (t->fn == 1 && aa.attr >= 0)
					continue;
				
				String s = uniq_acts.GetKey(i) + "(" + idx.GetKey(j) + ")";
				args.actions << s;
			}
			iter++;
		}
	}
	if (args.actions.IsEmpty()) {
		RemoveTask(*t);
		return; // ready
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 0)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistColors, t));
	else if (t->fn == 1)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistAttrs, t));
}

void TaskManager::GetActionParallels(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	
	// Loop from previous to 'current' to get score at the 'current'
	// (Instead of 'current' to next)
	int next_t = 0, next_a = 0;
	int score_t = 0, score_a = 0;
	
	for(const ExportDepActionPhrase& prev : da.action_phrases.GetValues()) {
		for(int k = 0; k < prev.next_phrases.GetCount(); k++) {
			int next_i = prev.next_phrases[k];
			const ExportDepActionPhrase& ap = da.action_phrases[next_i];
			
			for(int i = 0; i < ap.actions.GetCount(); i++) {
				int aa0_i = ap.actions[i];
				const ActionHeader& aa0 = da.actions.GetKey(aa0_i);
				for(int j = 0; j < ap.actions.GetCount(); j++) {
					if (i == j) continue;
					
					int aa1_i = ap.actions[j];
					const ActionHeader& aa1 = da.actions.GetKey(aa1_i);
					
					// we have a transition
					ActionHeader ah0, ah1;
					ah0.action = aa0.action;
					ah0.arg = aa0.arg;
					ah1.action = aa1.action;
					ah1.arg = aa1.arg;
					
					ExportParallel& ap = da.parallel.GetAdd3(aa0_i, aa1_i);
					ap.count++; // increase count
					
					if (k < prev.next_scores.GetCount()) {
						const Vector<int>& sc = prev.next_scores[k];
						int total_score = 0;
						for(int j = 0; j < sc.GetCount(); j++) {
							int v = sc[j];
							total_score += v;
						}
						if (!sc.IsEmpty()) score_a++;
						score_t++;
						
						ap.score_sum += max(0, min(
							(int)SCORE_COUNT,
							total_score));
						
						next_a++;
					}
					next_t++;
				}
			}
		}
	}
	
	da.diagnostics.GetAdd("action parallels: next percentage") =  DblStr((double)next_a / (double) next_t * 100);
	da.diagnostics.GetAdd("action parallels: score percentage") =  DblStr((double)score_a / (double) score_t * 100);
	
	LOG("TaskManager::GetActionParallels took " << ts.ToString());
}

void TaskManager::GetActionTransitions(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	
	int score_a = 0, score_t = 0;
	int next_a = 0, next_t = 0;
	for(const ExportDepActionPhrase& ap : da.action_phrases.GetValues()) {
		int ap_i0 = 0;
		for(int next_i : ap.next_phrases) {
			next_t++;
			if (ap_i0 < ap.next_scores.GetCount()) {
				next_a++;
				const ExportDepActionPhrase& next = da.action_phrases[next_i];
				
				for (const auto& aa0 : ap.actions) {
					for (const auto& aa1 : next.actions) {
						if (aa0 == aa1) {
							// we have a transition
							#if 0
							const ActionHeader& ah0 = da.actions.GetKey(aa0);
							const ActionHeader& ah1 = da.actions.GetKey(aa1);
							#endif
							
							ExportTransition& at = da.trans.GetAdd3(aa0, aa1);
							at.count++; // increase count
							
							const auto& sc = ap.next_scores[ap_i0];
							int total_score = 0;
							for(int v : sc)
								total_score += v;
							at.score_sum += max(0, min(
								(int)SCORE_COUNT,
								total_score));
							
							if (sc.GetCount()) score_a++;
							score_t++;
						}
					}
				}
			}
			ap_i0++;
		}
	}
	da.diagnostics.GetAdd("action transition: next percentage") =  DblStr((double)next_a / (double) next_t * 100);
	da.diagnostics.GetAdd("action transition: score percentage") =  DblStr((double)score_a / (double) score_t * 100);
	
	LOG("TaskManager::GetActionTransitions took " << ts.ToString());
}

void TaskManager::GetContainer(Task* t) {
	if (t->fn == 0) {
		GetRhymeContainers(t);
	}
	else if (t->fn == 1) {
		MakeNana(t);
	}
}

void TaskManager::MakeNana(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	int ds_i = t->ds_i;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	PhoneticNanaAnalyser anal;
	
	t->total = 0;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++)
		t->total += da.packed_rhymes[i].GetCount();
	
	t->actual = 0;
	for(int i = 0; i < da.packed_rhymes.GetCount(); i++) {
		const PackedRhymeHeader& h = da.packed_rhymes.GetKey(i);
		Vector<PackedRhymeContainer>& v = da.packed_rhymes[i];
		
		for(int j = 0; j < v.GetCount(); j++) {
			PackedRhymeContainer& c = v[j];
			c.ZeroNana();
			
			if (!anal.Parse(c.pron, PackedRhymeContainer::MAX_PRON_LEN))
				continue;
			
			int len = anal.WritePackedNana(c.nana, PackedRhymeContainer::MAX_NANA_LEN);
			c.nana_len = len;
			
			t->actual++;
			
			if (t->actual % 1000 == 0)
				t->update(t->actual, t->total);
		}
	}
	
	da.diagnostics.GetAdd("rhyme container nana: total") = IntStr(t->total);
	da.diagnostics.GetAdd("rhyme container nana: actual") = IntStr(t->actual);
	da.diagnostics.GetAdd("rhyme container nana: percentage") =  DblStr((double)t->actual / (double)t->total * 100);
	
	t->update(0, 1);
}

void TaskManager::GetRhymeContainers(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	da.packed_rhymes.Clear();
	
	Vector<int> comb;
	Vector<Index<String>> words;
	Vector<String> parts;
	
	t->actual = 0;
	
	int pp_i = -1;
	for (const PhrasePart& pp : da.phrase_parts.GetValues()) {
		pp_i++;
		
		if (pp.words.IsEmpty() || pp.attr < 0)
			continue;
		
		
		int act_count = min(PackedRhymeContainer::MAX_ACTIONS, pp.actions.GetCount());
		int acts[PackedRhymeContainer::MAX_ACTIONS];
		for(int i = 0; i < act_count; i++)
			acts[i] = pp.actions[i];
		
		// Make sentences from templates
		String phrase = da.GetWordString(pp.words);
		
		if (ep.Parse(phrase, da)) {
			//const Color& clr = ep.GetColor();
			Color clr = pp.clr;
			
			PackedRhymeHeader prh;
			prh.syllable_count = ep.GetSyllableCount();
			prh.color_group = GetColorGroup(clr);
			prh.attr = pp.attr;
			
			bool sort = da.packed_rhymes.Find(prh) < 0;
			
			if (phrase.GetCount() <= PackedRhymeContainer::MAX_TXT_LEN) {
				//LOG(phrase_count << ": " << phrase);
				PackedRhymeContainer& pcr = da.packed_rhymes.GetAdd(prh).Add();
				pcr.Zero();
				
				strncpy(pcr.txt, phrase.Begin(), PackedRhymeContainer::MAX_TXT_LEN);
				
				const WString& pron = ep.GetPronounciation();
				int pron_len = pron.GetCount();
				int pron_sz = sizeof(wchar) * pron_len;
				memcpy(pcr.pron, pron.Begin(), PackedRhymeContainer::MAX_PRON_SZ);
				
				pcr.clr[0] = clr.GetR();
				pcr.clr[1] = clr.GetG();
				pcr.clr[2] = clr.GetB();
				
				for(int i = 0; i < act_count; i++) {
					pcr.actions[i] = acts[i];
				}
				
				t->actual++;
			}
		}
		
		if ((pp_i % 100) == 0)
			t->update(pp_i, da.phrase_parts.GetCount());
	}
	
	
	da.diagnostics.GetAdd("rhyme container: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("rhyme container: actual") = IntStr(t->actual);
	da.diagnostics.GetAdd("rhyme container: percentage") =  DblStr((double)t->actual / (double)da.phrase_parts.GetCount() * 100);
	
	
	SortByKey(da.packed_rhymes, PackedRhymeHeader());
	for(int i = 0; i < sda.datasets.GetCount(); i++)
		SortByKey(sda.datasets[i].packed_rhymes, PackedRhymeHeader());
	
	t->update(0,1);
	RemoveTask(*t);
}

void TaskManager::GetRhymeContainersFromTemplates(Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	EnglishPronounciation ep;
	
	int ds_i = 0;
	int at_i = 0;
	
	
	Vector<int> comb;
	Vector<Index<String>> words;
	Vector<String> parts;
	int phrase_count = 0;
	
	while (running) {
		
		if (ds_i >= sda.datasets.GetCount())
			break;
		DatasetAnalysis& da = sda.datasets[ds_i];
		if (at_i == 0) {
			da.packed_rhymes.Clear();
			da.dynamic_actions.Clear();
			da.dynamic_attrs.Clear();
		}
		if (at_i >= da.action_tmpls.GetCount()) {
			SortByKey(da.packed_rhymes, PackedRhymeHeader());
			at_i = 0;
			ds_i++;
			continue;
		}
		
		const ActionTemplate& at = da.action_tmpls[at_i];
		const int tmpl_i = at.tp_i;
		
		int act_count = min(PackedRhymeContainer::MAX_ACTIONS, at.actions.GetCount());
		int16 act_groups[PackedRhymeContainer::MAX_ACTIONS];
		int16 act_values[PackedRhymeContainer::MAX_ACTIONS];
		for(int i = 0; i < act_count; i++) {
			const ActionHeader& aa = at.actions[i];
			da.RealizeAction(aa.action, aa.arg, act_groups[i], act_values[i]);
		}
		
		// Make sentences from templates
		TemplatePhrase& tp = da.tmpl_phrases[tmpl_i];
		if (tp.group.IsEmpty() || tp.value.IsEmpty()) {
			at_i++;
			continue;
		}
		int attr_group, attr_value;
		if (da.RealizeAttribute(tp.group, tp.value, attr_group, attr_value)) {
			tp.group = Attr::AttrKeys[attr_group][1]; // backport fix
		}
		
		if (tp.words.GetCount()) {
			words.Clear();
			for (const auto& wv : tp.words) {
				auto& wvd = words.Add();
				for (const auto& s : wv)
					wvd.Add(s);
			}
			
			parts <<= tp.parts;
			MakeSplit(parts, words);
			
			// Make extra sentences using wordnets
			for (Index<String>& part_words : words) {
				int wc = part_words.GetCount();
				for(int i = 0; i < wc; i++) {
					String w = part_words[i];
					String lw = ToLower(w);
					for (const ColorWordnet& wa : da.clr_wordnets) {
						if (wa.src_word == w || wa.src_word == lw) {
							for (const String& s : wa.words)
								part_words.FindAdd(s);
							break;
						}
					}
					int j = mirror_words.Find(lw);
					if (j >= 0)
						part_words.FindAdd(mirror_words[j]);
				}
			}
			
			// Get combination count
			int comb_count = 1;
			for (const auto& v : words)
				comb_count *= v.GetCount();
			int limit = min(8*1024, comb_count);
			
			// Reset combination vector
			comb.SetCount(words.GetCount());
			for (auto& v : comb) v = 0;
			
			for (int iter = 0; iter < limit; iter++) {
				bool roll_over = false;
				for (int i = comb.GetCount()-1; i >= 0; i--) {
					int& j = comb[i];
					j++;
					if (j < words[i].GetCount())
						break;
					else {
						j = 0;
						roll_over = i == 0;
					}
				}
				if (roll_over)
					break;
				
				ep.Clear();
				
				String phrase;
				ep.SetBaseColor(tp.clr); // modify color based on words (which has color value too)
				
				int word_i = 0;
				bool fail = false;
				for (const auto& part : parts) {
					if (part[0] == '{') {
						if (word_i >= comb.GetCount()) {
							fail = true;
							break;
						}
						int comb_i = comb[word_i];
						const auto& v = words[word_i];
						if (comb_i >= v.GetCount()) {
							fail = true;
							break;
						}
						const String& wrd = v[comb_i];
						if (!phrase.IsEmpty())
							phrase.Cat('\n');
						phrase << wrd;
						if (!ep.ParseMore(wrd, da)) {
							fail = true;
							break;
						}
						word_i++;
					}
					else {
						//if (part.GetCount() == 1 && (part[0] == ',' || part[1] == '.' || part[1] == '!' || part[1] == '?' || part[1] == ':'))
						//	;
						//else
						if (!phrase.IsEmpty())
							phrase.Cat('\n');
						phrase << part;
						if (!ep.ParseMore(part, da)) {
							fail = true;
							break;
						}
					}
				}
				
				if (!fail && !ep.ParseFinish(da))
					fail = true;
				
				if (!fail) {
					const Color& clr = ep.GetColor();
					
					PackedRhymeHeader prh;
					prh.syllable_count = ep.GetSyllableCount();
					prh.color_group = GetColorGroup(clr);
					prh.attr_group = attr_group;
					prh.attr_value = attr_value;
					
					bool sort = da.packed_rhymes.Find(prh) < 0;
					
					ASSERT(phrase.GetCount() <= PackedRhymeContainer::MAX_TXT_LEN);
					//LOG(phrase_count << ": " << phrase);
					PackedRhymeContainer& pcr = da.packed_rhymes.GetAdd(prh).Add();
					pcr.Zero();
					
					strncpy(pcr.txt, phrase.Begin(), PackedRhymeContainer::MAX_TXT_LEN);
					
					const WString& pron = ep.GetPronounciation();
					int pron_len = pron.GetCount();
					int pron_sz = sizeof(wchar) * pron_len;
					memcpy(pcr.pron, pron.Begin(), PackedRhymeContainer::MAX_PRON_SZ);
					
					pcr.clr[0] = clr.GetR();
					pcr.clr[1] = clr.GetG();
					pcr.clr[2] = clr.GetB();
					
					for(int i = 0; i < act_count; i++) {
						pcr.action_groups[i] = act_groups[i];
						pcr.action_values[i] = act_values[i];
					}
					
					if (sort)
						SortByKey(da.packed_rhymes, PackedRhymeHeader());
					
					phrase_count++;
				}
			}
		}
		
		//Sleep(1);
		at_i++;
		
		if ((at_i % 100) == 0)
			PostProgress(at_i, da.action_tmpls.GetCount());
	}
	
	PostProgress(0,1);
	running = false;
	stopped = true;
	#endif
}

void TaskManager::GetLineActions(Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	int per_batch = 15;
	
	if (t->batches.IsEmpty())
		t->UpdateBatches(per_batch);
	
	if (t->batch_i < 0 || t->batch_i >= t->batches.GetCount()) {
		RemoveTask(*t);
		return;
	}
	Task::Batch& batch = t->batches[t->batch_i];
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	SongDataAnalysisArgs args;
	args.fn = 10;
	args.phrases <<= Split(batch.txt, "\n");
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnLineActions, t));
}

void TaskManager::GetSyllables(Task* t) {
	int per_batch = 30;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int begin = t->batch_i * per_batch;
	int end = (t->batch_i+1) * per_batch;
	
	
	SongDataAnalysisArgs args;
	
	int iter = 0;
	int ds_i = t->ds_i;
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	for(int i = 0; i < ds.words.GetCount(); i++) {
		const String& wrd = ds.words.GetKey(i);
		const ExportWord& wa = ds.words[i];
		
		// HASH BREAKING CODE
		#if 1
		if (wa.phonetic.GetCount() && wa.spelling.GetCount()) {
			t->total++;
			t->actual++;
			continue;
		}
		#endif
		
		if (wrd.IsEmpty())
			continue;
		
		#if 1
		// HASH BREAKING FEATURE
		if (wrd[0] == '{' || wrd[wrd.GetCount()-1] == '}') {
			t->total++;
			t->actual++;
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
			t->tmp_words << wrd;
		}
		iter++;
		if (iter >= end) break;
	}
	
	
	if (args.words.IsEmpty()) {
		RemoveTask(*t);
		return;
	}
	
	t->total += args.words.GetCount();
	
	args.fn = 4;
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnSyllables, t));
}

void TaskManager::GetDetails(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int per_batch = 30;
	
	ASSERT(t->batch_i >= 0);
	int begin = t->batch_i * per_batch;
	int end = (t->batch_i+1) * per_batch;
	
	t->tmp_words.Clear();
	
	SongDataAnalysisArgs args;
	
	int ds_i = t->ds_i;
	int iter = 0;
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	Color black(0,0,0);
	
	for(int i = 0; i < ds.words.GetCount(); i++) {
		ExportWord& wa = ds.words[i];
		const String& wrd = ds.words.GetKey(i);
		
		if (wrd.IsEmpty())
			continue;
		
		#if 1
		// HASH BREAKING FEATURE
		if (wa.clr != black || wrd[0] == '{' || wrd[wrd.GetCount()-1] == '}') {
			t->total++;
			t->actual++;
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
			t->tmp_words.Add(wrd);
		}
		iter++;
		if (iter >= end) break;
	}
	
	if (args.words.IsEmpty()) {
		//RemoveTask(*t);
		t->fn++;
		return;
	}
	
	t->total += args.words.GetCount();
	
	args.fn = 5;
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnDetails, t));
}

void TaskManager::GetLineChangeScores(Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	int per_batch = 15;
	
	if (t->batches.IsEmpty())
		t->UpdateBatches(per_batch);
	
	if (t->batch_i < 0 || t->batch_i >= t->batches.GetCount()) {
		RemoveTask(*t);
		return;
	}
	
	Task::Batch& batch = t->batches[t->batch_i];
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	SongDataAnalysisArgs args;
	args.fn = 11;
	//args.score_mode = score_mode;
	args.phrases <<= Split(batch.txt, "\n");
	
	Vector<int>& ap_is = t->tmp;
	ap_is.Clear();
	
	bool fail = false;
	for (String& l : args.phrases) {
		int ap_i = 0;
		bool found = false;
		for(int i = 0; i < da.action_phrases.GetCount(); i++) {
			const String& txt = da.action_phrases.GetKey(i);
			const ExportDepActionPhrase& ap = da.action_phrases[i];
			
			if (txt == l) {
				l.Clear();
				for (int a : ap.actions) {
					const ActionHeader& ah = da.actions.GetKey(a);
					if (!l.IsEmpty()) l << " + ";
					l << ah.action << "(" << ah.arg << ")";
				}
				found = true;
				break;
			}
			ap_i++;
		}
		ap_is << (found ? ap_i : -1);
		if (!found)
			fail = true;
	}
	
	/*if (fail) {
		PromptOK("Failed to finish line change scores");
		return;
	}*/
	
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnLineChangeScores, t));
}

void TaskManager::GetWordData(Task* t) {
	t->actual = 0;
	t->total = 0;
	
	if (t->fn == 0)
		GetWordProcess(t);
	else if (t->fn == 1)
		GetDetails(t);
	else if (t->fn == 2)
		GetSyllables(t);
	else
		RemoveTask(*t);
}

void TaskManager::GetWordnet(Task* t) {
	if (t->fn == 0)
		MakeWordnetsFromTemplates(t);
	else if (t->fn == 1)
		GetColorAlternatives(t);
}

void TaskManager::GetColorAlternatives(Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int per_batch = 25;
	int begin = t->batch_i * per_batch;
	int end = (t->batch_i+1) * per_batch;
	
	if (t->batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	SongDataAnalysisArgs args;
	
	VectorMap<String, Color>& word_clr = t->word_clr;
	word_clr.Clear();
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			if (iter >= begin) {
				const String& txt = ds.words.GetKey(i);
				ExportWord& wa = ds.words[i];
				
				String wc = wa.class_count > 0 ? ds.word_classes[wa.classes[0]] : String();
				String header = wc + ": " + txt;
				String key = header;
				key << ", RGB(" << (int)wa.clr.GetR() << "," << (int)wa.clr.GetG() << "," << (int)wa.clr.GetB() << ")";
				args.words << key;
				
				word_clr.Add(txt, wa.clr);
				
				if (args.words.GetCount() == 1) {
					t->tmp_str = header;
				}
			}
			iter++;
			if (iter >=  end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.words.IsEmpty()) {
		RemoveTask(*t);
		return;
	}
	
	args.fn = 7;
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK1(OnColorAlternatives, t));
}

void TaskManager::MakeWordnetsFromTemplates(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	TimeStop ts;
	
	PromptOK("TODO");
	#if 0
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		DatasetAnalysis& da = sda.datasets[i];
		for(int j = 0; j < da.tmpl_phrases.GetCount(); j++) {
			TemplatePhrase& tp = da.tmpl_phrases[j];
			String group = tp.group;
			String value = tp.value;
			Color clr = tp.clr;
			int clr_group = GetColorGroup(clr);
			Vector<String> main_classes;
			for (const String& part : tp.parts) {
				if (part.IsEmpty()) continue;
				if (part[0] == '{')
					main_classes << part.Mid(1,part.GetCount()-2);
			}
			for(int k = 0; k < tp.words.GetCount(); k++) {
				if (k >= main_classes.GetCount()) break;
				const String& main_class = main_classes[k];
				auto& words = tp.words[k];
				
				// Find wordnet
				bool found = false;
				for (ExportWordnet& wn : da.wordnets) {
					if (wn.clr_group == clr_group &&
						wn.group == group &&
						wn.value == value &&
						wn.main_class == main_class) {
						bool any_match = false;
						for (const String& dst : wn.words) {
							for (const String& src : words) {
								if (dst == src) {
									any_match = true;
									break;
								}
							}
							if (any_match) break;
						}
						if (any_match) {
							
							// Append wordnet
							for (const String& src : words)
								VectorFindAdd(wn.words, src);
							
							found = true;
							break;
						}
					}
				}
				if (!found) {
					// New wordnet
					ExportWordnet& wn = da.wordnets.Add();
					wn.main_class = main_class;
					wn.group = group;
					wn.value = value;
					wn.clr = clr;
					wn.clr_group = clr_group;
					wn.words <<= words;
				}
			}
		}
	}
	
	LOG("MakeWordnetsFromTemplates took: " << ts.ToString());
	#endif
}

void TaskManager::GetAttributes(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// TODO optimize: this is being done every time
	VectorMap<String,Index<String>> uniq_attrs;
	uniq_attrs.Clear();
	for(int i = 0; i < da.attrs.GetCount(); i++) {
		const AttrHeader& ah = da.attrs.GetKey(i);
		uniq_attrs.GetAdd(ah.group).FindAdd(ah.value);
	}
	
	struct Sorter {
		bool operator()(const Index<String>& a, const Index<String>& b) const {
			if (a.GetCount() != b.GetCount())
				return a.GetCount() > b.GetCount();
			if (a.GetCount() && b.GetCount())
				return StdLess<String>()(a[0], b[0]);
			return false;
		}
	};
	SortByValue(uniq_attrs, Sorter());
	
	if (t->fn == 0) {
		Vector<Task::AttrExtremesBatch>& batches = t->attr_extremes_batches;
		
		if (batches.IsEmpty()) {
			for(int i = 0; i < uniq_attrs.GetCount(); i++) {
				String group = uniq_attrs.GetKey(i);
				int j = da.simple_attrs.Find(group);
				if (j >= 0) {
					const ExportSimpleAttr& esa = da.simple_attrs[j];
					if (esa.attr_i0 >= 0 && esa.attr_i1 >= 0)
						continue;
				}
				Task::AttrExtremesBatch& b = batches.Add();
				b.group = group;
			}
		}
		
		if (t->batch_i >= batches.GetCount()) {
			RemoveTask(*t);
			return;
		}
		Task::AttrExtremesBatch& batch = batches[t->batch_i];
		
		if (0) {
			while (1) {
				const Index<String>& values = uniq_attrs[t->batch_i];
				if (values.GetCount() > 2) t->batch_i++;
				else break;
			}
		}
		
		const Index<String>& values = uniq_attrs.Get(batch.group);
		if (values.GetCount() < 2) {
			RemoveTask(*t);
			return;
		}
		
		
		AttrArgs args;
		args.fn = t->fn;
		args.group = batch.group;
		args.values <<= values.GetKeys();
		
		if (args.group.IsEmpty()) {
			t->batch_i++;
			t->running = false;
			return;
		}
		
		t->tmp_str = args.group;
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetAttributes(args, THISBACK1(OnAttributes, t));
	}
	else if (t->fn == 1) {
		int per_batch = 50;
		
		Vector<Task::AttrPolarBatch>& batches = t->attr_polar_batches;
		
		if (batches.IsEmpty()) {
			for(int i = 0; i < uniq_attrs.GetCount(); i++) {
				String group = uniq_attrs.GetKey(i);
				int j = da.simple_attrs.Find(group);
				if (j < 0) continue;
				const auto& gsa = da.simple_attrs[j];
				
				const Index<String>& v = uniq_attrs[i];
				Task::AttrPolarBatch& b = batches.Add();
				b.attr0 = da.attrs.GetKey(gsa.attr_i0).value;
				b.attr1 = da.attrs.GetKey(gsa.attr_i1).value;
				ASSERT(da.attrs.GetKey(gsa.attr_i0).group == group);
				b.group = group;
				for(int j = 0; j < v.GetCount(); j++) {
					if (batches.Top().attrs.GetCount() >= per_batch) {
						Task::AttrPolarBatch& b0 = batches.Add();
						Task::AttrPolarBatch& b = batches[batches.GetCount()-2];
						b0.group = b.group;
						b0.attr0 = b.attr0;
						b0.attr1 = b.attr1;
					}
					
					AttrHeader ah;
					ah.group = group;
					ah.value = v[j];
					int k = da.attrs.Find(ah);
					if (k >= 0) {
						const ExportAttr& ea = da.attrs[k];
						if (ea.positive >= 0)
							continue;
					}
					
					batches.Top().attrs << v[j];
				}
				if (batches.Top().attrs.IsEmpty())
					batches.Remove(batches.GetCount()-1);
			}
		}
		
		
		if (t->batch_i >= batches.GetCount()) {
			RemoveTask(*t);
			return;
		}
		
		Task::AttrPolarBatch& batch = batches[t->batch_i];
		AttrArgs args;
		args.fn = t->fn;
		args.group = batch.group;
		args.values <<= batch.attrs;
		args.attr0 = batch.attr0;
		args.attr1 = batch.attr1;
		
		t->tmp_words <<= batch.attrs;
		t->tmp_str = args.group;
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetAttributes(args, THISBACK1(OnAttributePolars, t));
	}
	else if (t->fn == 2) {
		int per_batch = 35;
		Vector<Task::AttrJoinBatch>& batches = t->attr_join_batches;
		
		if (batches.IsEmpty()) {
			for(int i = 0; i < uniq_attrs.GetCount(); i++) {
				String group = uniq_attrs.GetKey(i);
				const Index<String>& v = uniq_attrs[i];
				if (v.GetCount() > 1) continue;
				if (v.IsEmpty()) break;
				if (batches.IsEmpty() || batches.Top().values.GetCount() >= per_batch) {
					batches.Add();
				}
				AttrHeader ah;
				ah.group = group;
				ah.value = v[0];
				
				int j = da.attrs.Find(ah);
				if (j < 0)
					continue;
				
				const ExportAttr& ea = da.attrs[j];
				if (ea.link >= 0)
					continue; // already linked
				
				Task::AttrJoinBatch& b = batches.Top();
				b.values << (ah.group + ": " + ah.value);
			}
		}
		
		if (t->batch_i >= batches.GetCount()) {
			RemoveTask(*t);
			return;
		}
		
		Task::AttrJoinBatch& batch = batches[t->batch_i];
		AttrArgs args;
		args.fn = t->fn;
		//args.groups <<= batch.groups;
		args.values <<= batch.values;
		int count = min(20, uniq_attrs.GetCount());
		t->tmp_words2.Clear();
		for(int i = 0; i < count; i++) {
			String group = uniq_attrs.GetKey(i);
			if (!group.IsEmpty()) {
				const ExportSimpleAttr& ea = da.simple_attrs.GetAdd(group);
				String a0 = da.attrs.GetKey(ea.attr_i0).value;
				String a1 = da.attrs.GetKey(ea.attr_i1).value;
				args.groups << (group + ": +(" + a0 + "), -(" + a1 + ")");
				t->tmp_words2 << group;
			}
		}
		
		t->tmp_words <<= batch.values;
		t->tmp_str = args.group;
		
		RealizePipe();
		TaskMgr& m = *pipe;
		m.GetAttributes(args, THISBACK1(OnAttributeJoins, t));
	}
	else if (t->fn == 3) {
		for(int i = 0; i < da.attrs.GetCount(); i++) {
			da.attrs[i].simple_attr = -1;
		}
		// Fix: add simple_attr index value to ExportAttr
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			AttrHeader ah;
			ah.group = uniq_attrs.GetKey(i);
			const auto& values = uniq_attrs[i];
			int sa_i = da.simple_attrs.Find(ah.group);
			if (sa_i < 0)
				continue;
			for(int j = 0; j < values.GetCount(); j++) {
				ah.value = values[j];
				int k = da.attrs.Find(ah);
				ASSERT(k >= 0);
				ExportAttr& ea = da.attrs[k];
				ea.simple_attr = sa_i;
			}
		}
	}
}

}
