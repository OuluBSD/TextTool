#include "SongDataCtrl.h"

#if 0



ExportPage::ExportPage() {
	Add(log.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	SetProgress(0,1);
}

void ExportPage::Data() {
	
}

void ExportPage::ToolMenu(Bar& bar) {
	if (!running)
		bar.Add(t_("Export"), AppImg::BlueRing(), THISBACK(Start)).Key(K_F5);
	else
		bar.Add(t_("Stop"), AppImg::BlueRing(), THISBACK(Stop)).Key(K_F5);
	
}

void ExportPage::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Export));
}

void ExportPage::Stop() {
	running = false;
	while (!stopped) Sleep(10);
	PostProgress(0,1);
}

struct ExportTextPart : Moveable<ExportTextPart> {
	int sentence_type = -1;
	
	String StoreToString() {StringDumper d; d % sentence_type; return d;}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % sentence_type;
	}
};

struct ExportText : Moveable<ExportText> {
	static const int max_part_count = 32;
	int sentence_type = -1;
	int part_count = 0;
	int parts[max_part_count] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	
	void Add(int i) {ASSERT(part_count < max_part_count); parts[part_count++] = i;}
	String StoreToString() {
		StringDumper d;
		d % sentence_type % (int)part_count;
		for(int i = 0; i < min<int>(max_part_count, part_count); i++)
			d % parts[i];
		return d;
	}
	void LoadFromString(const String& s) {
		int pc = 0;
		StringParser p(s);
		p % sentence_type % pc;
		for(int i = 0; i < min<int>(max_part_count, pc); i++)
			p % parts[i];
		part_count = pc;
	}
	
};

struct ExportParallel : Moveable<ExportParallel> {
	int count = 0, score_sum= 0;
	
	String StoreToString() {
		StringDumper d;
		d % count % score_sum;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % count % score_sum;
	}
	
};

struct ExportTransition : Moveable<ExportTransition> {
	int count = 0, score_sum= 0;
	
	String StoreToString() {
		StringDumper d;
		d % count % score_sum;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % count % score_sum;
	}
	
};

struct ExportTemplate : Moveable<ExportTemplate> {
	static const int MAX_ACTIONS = 10;
	static const int MAX_WORDS = 256;
	int action_count = 0;
	int actions[MAX_ACTIONS] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int word_count = 0;
	int words[MAX_WORDS];
	Color clr = Black();
	int attr = -1;
	int scores[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {{0,0,0,0,0},{0,0,0,0,0}};
	
	String StoreToString() {
		StringDumper d;
		action_count = max(0,min(action_count, MAX_ACTIONS));
		d % action_count;
		for(int i = 0; i < action_count; i++)
			d % actions[i];
		word_count = max(0,min(word_count, MAX_WORDS));
		d % word_count;
		for(int i = 0; i < word_count; i++)
			d % words[i];
		d % clr % attr;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				d % scores[i][j];
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % action_count;
		action_count = max(0,min(action_count, MAX_ACTIONS));
		for(int i = 0; i < action_count; i++)
			p % actions[i];
		p % word_count;
		word_count = max(0,min(word_count, MAX_ACTIONS));
		for(int i = 0; i < word_count; i++)
			p % words[i];
		p % clr % attr;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				p % scores[i][j];
	}
	
};

void ExportPage::Export() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "songdata");
	RealizeDirectory(dir);
	
	Vector<int> word_is;
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		String ds_dir = AppendFileName(dir, ds_key);
		RealizeDirectory(ds_dir);
		
		String trans_ds_key;
		if (ds_i == 0)
			trans_ds_key = "fi";
		else
			trans_ds_key = "en";
		
		IndexFile word_classes(ds_dir, "word classes");
		IndexFile struct_part_types(ds_dir, "structure part types");
		IndexFile struct_types(ds_dir, "structure types");
		MapFile<String,ExportTextPart> text_parts(ds_dir, "text parts");
		MapFile<hash_t,ExportText> sents(ds_dir, "texts");
		MapFile<ActionHeader,ExportAction> actions(ds_dir, "action");
		MapFile<AttrHeader,ExportAttr> attrs(ds_dir, "attributes");
		MapMapFile<int,int,ExportParallel> parallel(ds_dir, "action parallel");
		MapMapFile<int,int,ExportTransition> trans(ds_dir, "action transition");
		MapFile<String,ExportWord> words(ds_dir, "words");
		MapFile<hash_t,ExportTemplate> tmpls(ds_dir, "templates");
		MapFile<String,ExportDepActionPhrase> act_phrases(ds_dir, "action phrases");
		MapFile<hash_t,ExportWordnet> wordnets(ds_dir, "wordnets");
		MapFile<String,String> translations(ds_dir, trans_ds_key);
		
		DatasetAnalysis& da = sda.datasets[ds_i];
		
		for (const StructurePhrase& sp : da.structure_phrases) {
			const StructureType& st = da.structure_types[sp.type];
			int pc = st.part_types.GetCount();
			int struct_type_i = struct_part_types.FindAdd(st.struct_type);
			
			if (sp.sent_parts.GetCount() != pc)
				continue;
			if (!pc)
				continue;
			
			CombineHash sent_hash;
			int part_is[ExportText::max_part_count];
			ASSERT(pc <= ExportText::max_part_count);
			for(int i = 0; i < pc; i++) {
				const String& txt = sp.sent_parts[i];
				const String& part_type = st.part_types[i];
				
				// Sentence parts
				int part_i = -1;
				ExportTextPart& part = text_parts.GetAdd(txt, part_i);
				part_is[i] = part_i;
				sent_hash.Do(part_i);
				part.sentence_type = struct_part_types.FindAdd(part_type);
				
				
			}
			
			// Full sentences (links to parts)
			ExportText& sent = sents.GetAdd(sent_hash);
			sent.part_count = pc;
			for(int i = 0; i < pc; i++) sent.parts[i] = part_is[i];
			sent.sentence_type = struct_types.FindAdd(st.struct_type);
			
			
		}
		
		// Action attributes
		for(int i = 0; i < da.action_attrs.GetCount(); i++) {
			ActionHeader ah = da.action_attrs.GetKey(i);
			ActionAttrs& aa = da.action_attrs[i];
			AttrHeader ath;
			ath.group = ToLower(aa.group);
			ath.value = ToLower(aa.value);
			
			ah.Trim();
			ath.Trim();
			
			int eac_i = -1, eat_i = -1;
			ExportAction& eac = actions.GetAdd(ah, eac_i);
			ExportAttr& eat = attrs.GetAdd(ath, eat_i);
			eac.attr = eat_i;
			eac.clr = aa.clr;
		}
		
		// Parallel actions
		for(int i = 0; i < da.action_parallel.GetCount(); i++) {
			ActionHeader ah0 = da.action_parallel.GetKey(i);
			ah0.Trim();
			
			int eac_i0 = -1, eac_i1 = -1, paral_i0 = -1;
			ExportAction& eac0 = actions.GetAdd(ah0, eac_i0);
			auto& et0 = parallel.GetAdd(eac_i0, paral_i0);
			
			const auto& map = da.action_parallel[i];
			for(int j = 0; j < map.GetCount(); j++) {
				ActionHeader ah1 = map.GetKey(j);
				ah1.Trim();
				
				const ActionParallel& ap = map[j];
				int paral_i1 = -1;
				
				ExportAction& eac1 = actions.GetAdd(ah1, eac_i1);
				ExportParallel& e = parallel.GetAdd(paral_i0, eac_i1, paral_i1);
				e.count = ap.count;
				e.score_sum = ap.score_sum;
			}
		}
		
		// Transition actions
		for(int i = 0; i < da.action_trans.GetCount(); i++) {
			ActionHeader ah0 = da.action_trans.GetKey(i);
			ah0.Trim();
			
			int eac_i0 = -1, eac_i1 = -1, trans_i0 = -1;
			ExportAction& eac0 = actions.GetAdd(ah0, eac_i0);
			auto& et0 = trans.GetAdd(eac_i0, trans_i0);
			
			const auto& map = da.action_trans[i];
			for(int j = 0; j < map.GetCount(); j++) {
				ActionHeader ah1 = map.GetKey(j);
				ah1.Trim();
				
				const ActionTransition& at = map[j];
				int trans_i1 = -1;
				
				ExportAction& eac1 = actions.GetAdd(ah1, eac_i1);
				ExportTransition& e = trans.GetAdd(trans_i0, eac_i1, trans_i1);
				e.count = at.count;
				e.score_sum = at.score_sum;
			}
		}
		
		// Template actions
		int idx = -1;
		for (const ActionTemplate& ap : da.action_tmpls) {
			++idx;
			const TemplatePhrase& tp = da.tmpl_phrases[ap.tp_i];
			
			// Get object (hash and words too)
			word_is.SetCount(0);
			CombineHash ch;
			for (const String& part : tp.parts) {
				Vector<String> part_words = Split(part, " ");
				for (String& pw : part_words) {
					int w_i = -1;
					ExportWord& w = words.GetAdd(pw, w_i);
					ch.Do(pw);
					word_is << w_i;
				}
			}
			hash_t h = ch;
			ExportTemplate& et = tmpls.GetAdd(h);
			
			// Copy actions
			et.action_count = ap.actions.GetCount();
			for(int i = 0; i < ap.actions.GetCount(); i++) {
				int eac_i = -1;
				
				ActionHeader ah = ap.actions[i];
				ah.Trim();
				
				ExportAction& eac = actions.GetAdd(ah, eac_i);
				et.actions[i] = eac_i;
			}
			
			// Copy words
			et.word_count = min(ExportTemplate::MAX_WORDS, word_is.GetCount());
			for(int i = 0; i < word_is.GetCount(); i++)
				et.words[i] = word_is[i];
			
			// Copy color
			et.clr = tp.clr;
			
			// Copy scores
			for(int j = 0; j < SCORE_MODE_COUNT; j++) {
				for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
					et.scores[j][k] = tp.scores[j][k];
				}
			}
			
			// Copy attribute
			AttrHeader ath;
			ath.group = ToLower(tp.group);
			ath.value = ToLower(tp.value);
			ExportAttr& eat = attrs.GetAdd(ath, et.attr);
		}
		
		
		// Copy action phrases (deprecated)
		
		// Realize phrases beforehand to avoid vector-reference errors
		for(int i = 0; i < da.action_phrases.GetCount(); i++)
			act_phrases.GetAdd(da.action_phrases[i].txt);
		
		// Set values
		for(int i = 0; i < da.action_phrases.GetCount(); i++) {
			const ActionPhrase& ap = da.action_phrases[i];
			ExportDepActionPhrase& eap = act_phrases.GetAdd(ap.txt);
			
			eap.actions.SetCount(ap.actions.GetCount());
			for(int i = 0; i < ap.actions.GetCount(); i++)
				actions.GetAdd(ap.actions[i], eap.actions[i]);
			
			int c = ap.next_phrases.GetCount();
			eap.next_phrases.SetCount(c, -1);
			eap.next_scores.SetCount(c);
			for(int j = 0; j < c; j++) {
				int next_i = -1;
				const ActionPhrase& ap_next = da.action_phrases[ap.next_phrases[j]];
				act_phrases.GetAdd(ap_next.txt, next_i);
				eap.next_phrases[j] = next_i;
				
				if (j < ap.next_scores.GetCount()) {
					const auto& f = ap.next_scores[j];
					auto& t = eap.next_scores[j];
					t.SetCount(SCORE_MODE_COUNT *SCORE_ATTR_COUNT);
					for(int k0 = 0, l = 0; k0 < SCORE_MODE_COUNT; k0++)
						for(int k1 = 0; k1 < SCORE_ATTR_COUNT; k1++)
							t[l++] = f.scores[k0][k1];
				}
			}
			eap.first_lines = ap.first_lines;
		}
		
		
		// Wordnets
		for(int i = 0; i < da.wordnets.GetCount(); i++) {
			const Wordnet& wn = da.wordnets[i];
			
			// Make hash and get object
			CombineHash ch;
			for (const String& w : wn.words)
				ch.Do(w).Put(1);
			hash_t h = ch;
			ExportWordnet& ewn = wordnets.GetAdd(h);
			
			// Add words
			ewn.word_count = 0;
			for (const String& w : wn.words) {
				if (ewn.word_count >= ExportWordnet::MAX_WORDS)
					break;
				int w_i = -1;
				ExportWord& ew = words.GetAdd(w, w_i);
				ewn.words[ewn.word_count++] = w_i;
			}
			ASSERT(ewn.word_count <= ExportWordnet::MAX_WORDS);
			
			// Word class
			ewn.main_class = word_classes.FindAdd(wn.main_class);
			
			// Copy attribute
			AttrHeader ath;
			ath.group = ToLower(wn.group);
			ath.value = ToLower(wn.value);
			ExportAttr& eat = attrs.GetAdd(ath, ewn.attr);
			
			// Copy wordnet color
			ewn.clr = wn.clr;
			
			// Copy scores
			for(int k0 = 0, l = 0; k0 < SCORE_MODE_COUNT; k0++)
				for(int k1 = 0; k1 < SCORE_ATTR_COUNT; k1++)
					ewn.scores[k0][k1] = wn.scores[k0][k1];
		}
		
		for(int i = 0; i < da.clr_wordnets.GetCount(); i++) {
			const ColorWordnet& wn = da.clr_wordnets[i];
			
			// Make hash and get object
			CombineHash ch;
			ch.Do(wn.src_word).Put(1);
			for (const String& w : wn.words)
				ch.Do(w).Put(1);
			hash_t h = ch;
			ExportWordnet& ewn = wordnets.GetAdd(h);
			
			// Add words
			ewn.word_count = 0;
			int w_i = -1;
			words.GetAdd(wn.src_word, ewn.words[ewn.word_count++]);
			for (const String& w : wn.words) {
				int w_i = -1;
				ExportWord& ew = words.GetAdd(w, w_i);
				ewn.words[ewn.word_count++] = w_i;
			}
			ASSERT(ewn.word_count <= ExportWordnet::MAX_WORDS);
			
			// Add word colors
			ewn.word_clr_count = 0;
			ewn.word_clrs[ewn.word_clr_count++] = Black(); // TODO fix later
			for (const Color& c : wn.colors) {
				ewn.word_clrs[ewn.word_clr_count++] = c;
			}
			ASSERT(ewn.word_clr_count <= ExportWordnet::MAX_WORDS);
			
			// Word class
			ewn.main_class = word_classes.FindAdd(wn.main_class);
			
			// Copy wordnet color
			ewn.clr = wn.clr;
			
			// Copy scores
			for(int k0 = 0, l = 0; k0 < SCORE_MODE_COUNT; k0++)
				for(int k1 = 0; k1 < SCORE_ATTR_COUNT; k1++)
					ewn.scores[k0][k1] = wn.scores[k0][k1];
		}
		
		
		// Words
		for(int i = 0; i < da.words.GetCount(); i++) {
			const WordAnalysis& wa = da.words[i];
			int w_i = -1;
			ExportWord& ew = words.GetAdd(wa.txt, w_i);
			
			// Word class
			ew.main_class = word_classes.FindAdd(wa.main_class);
			
			// Copy fields
			ew.clr = wa.clr;
			ew.spelling = wa.spelling;
			ew.phonetic = wa.phonetic;
			ew.count = wa.count;
			
			if (!wa.translation.IsEmpty())
				translations.GetAdd(wa.txt) = wa.translation;
		}
		
		
		// Phrases (from different location)
		for(int i = 0; i < da.artists.GetCount(); i++) {
			ArtistAnalysis& artist = da.artists[i];
			for(int j = 0; j < artist.songs.GetCount(); j++) {
				LyricsAnalysis& song = artist.songs[j];
				for(int k = 0; k < song.phrases.GetCount(); k++) {
					const LyricsAnalysis::Phrase& phrase = song.phrases[k];
					ExportDepActionPhrase& eap = act_phrases.GetAdd(phrase.phrase);
					
					// Copy attribute
					AttrHeader ath;
					ath.group = ToLower(phrase.group);
					ath.value = ToLower(phrase.value);
					ExportAttr& eat = attrs.GetAdd(ath, eap.attr);
					
					// Copy color
					eap.clr = phrase.clr;
				}
			}
		}
		
	}
	
	running = false;
	stopped = true;
}

#endif
