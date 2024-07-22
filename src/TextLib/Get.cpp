#include "TextLib.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


void TaskManager::GetComponents(Task* t) {
	// MOVED
}

void TaskManager::GetTokenDataUsingExisting(Task* t) {
	// MOVED
}

void TaskManager::GetTokenData(Task* t) {
	// MOVED
}

void TaskManager::GetUnknownTokenPairs(Task* t) {
	// MOVED
}

void TaskManager::GetAmbiguousWordPairsUsingExisting(Task* t) {
	// MOVED
}

void TaskManager::GetAmbiguousWordPairs(Task* t) {
	// MOVED
}

void TaskManager::GetWordProcess(Task* t) {
	// MOVED
}

void TaskManager::GetWordFix(Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	DatasetAnalysis& ds = sda.dataset;
	
	if (t->fn == 0) {
		// MOVED
	}
	else if (t->fn == 1) {
		// MOVED
	}
}

void TaskManager::GetVirtualPhrases(Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	if (t->fn == 0) {
		// MOVED
	}
	else if (t->fn == 1) {
		// MOVED
	}
	else if (t->fn == 2) {
		// MOVED
	}
	
	// Token texts to phrases
	else if (t->fn == 3) {
		// MOVED
	}
}

void TaskManager::GetVirtualPhrasesUsingExisting(Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	TimeStop ts;
	if (t->fn == 1) {
		// MOVED
	}
	if (t->fn == 2) {
		// MOVED
	}
	
	LOG("TaskManager::GetVirtualPhrasesUsingExisting #" << t->fn << ": copying values took " << ts.ToString());
	LOG("");
}

void TaskManager::GetPhrases(Task* t) {
	// MOVED
}

void TaskManager::GetActionlist(Task* t) {
	// MOVED
}

void TaskManager::GetActionlistUsingExisting(Task* t) {
	// MOVED
}

void TaskManager::GetActionParallels(Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	EnglishPronounciation ep;
	DatasetAnalysis& da = sda.dataset;
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
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	EnglishPronounciation ep;
	
	
	DatasetAnalysis& da = sda.dataset;
	
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
	SortByKey(sda.dataset.packed_rhymes, PackedRhymeHeader());
	
	t->update(0,1);
	RemoveTask(*t);
}

void TaskManager::GetRhymeContainersFromTemplates(Task* t) {
	TODO
	#if 0
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	EnglishPronounciation ep;
	
	int at_i = 0;
	
	
	Vector<int> comb;
	Vector<Index<String>> words;
	Vector<String> parts;
	int phrase_count = 0;
	
	while (running) {
		DatasetAnalysis& da = sda.dataset;
		if (at_i == 0) {
			da.packed_rhymes.Clear();
			da.dynamic_actions.Clear();
			da.dynamic_attrs.Clear();
		}
		if (at_i >= da.action_tmpls.GetCount()) {
			SortByKey(da.packed_rhymes, PackedRhymeHeader());
			at_i = 0;
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
		t->UpdateBatches(appmode, per_batch);
	
	if (t->batch_i < 0 || t->batch_i >= t->batches.GetCount()) {
		RemoveTask(*t);
		return;
	}
	Task::Batch& batch = t->batches[t->batch_i];
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	SourceDataAnalysisArgs args; // 10
	args.fn = 10;
	args.phrases <<= Split(batch.txt, "\n");
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, THISBACK1(OnLineActions, t));
}

void TaskManager::GetSyllables(Task* t) {
	// MOVED
}

void TaskManager::GetDetails(Task* t) {
	// MOVED
}

void TaskManager::GetLineChangeScores(Task* t) {
	if (Thread::IsShutdownThreads())
		return;
	
	int per_batch = 15;
	
	if (t->batches.IsEmpty())
		t->UpdateBatches(appmode, per_batch);
	
	if (t->batch_i < 0 || t->batch_i >= t->batches.GetCount()) {
		RemoveTask(*t);
		return;
	}
	
	Task::Batch& batch = t->batches[t->batch_i];
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	SourceDataAnalysisArgs args; // 11
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
	
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, THISBACK1(OnLineChangeScores, t));
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
	else if (t->fn == 3)
		GetLineActions(t);
	else if (t->fn == 4)
		GetLineChangeScores(t);
	else
		RemoveTask(*t);
}

void TaskManager::GetWordDataUsingExisting(Task* t) {
	
	if (t->fn == 4) {
		/*
		
		This function is highly unlikely to be using existing data!
		--> won't implement
		
		*/
	}
	
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
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	int per_batch = 25;
	int begin = t->batch_i * per_batch;
	int end = (t->batch_i+1) * per_batch;
	
	if (t->batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	SourceDataAnalysisArgs args; // 7
	
	VectorMap<String, Color>& word_clr = t->word_clr;
	word_clr.Clear();
	
	int iter = 0;
	{
		DatasetAnalysis& ds = sda.dataset;
		
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
	}
	
	if (args.words.IsEmpty()) {
		RemoveTask(*t);
		return;
	}
	
	args.fn = 7;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSourceDataAnalysis(appmode, args, THISBACK1(OnColorAlternatives, t));
}

void TaskManager::MakeWordnetsFromTemplates(Task* t) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	TimeStop ts;
	
	PromptOK("TODO");
	#if 0
	{
		DatasetAnalysis& da = sda.dataset;
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
	// MOVED
}

void TaskManager::GetAttributesUsingExisting(Task* t) {
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	t->actual = 0;
	t->total = 0;
	
	Color black(0,0,0);
	
	if (t->fn == 0) {
		RealizeBatch_AttrExtremesBatch(t);
		
		
		// DOESN'T WORK: ACTION LISTS ARE DIFFERENT IN OTHER DBs
		/*for(int i = 0; i < DB_COUNT; i++) {
			if (i == appmode) continue;
			TextDatabase& db1 = MetaDatabase::Single().db[i];
			SourceData& sd1 = db1.src_data;
			SourceDataAnalysis& sda1 = db1.src_data.a;
			DatasetAnalysis& da1 = sda1.dataset;
			
			for(int j = 0; j < t->uniq_attrs.GetCount(); j++) {
				const String& group = t->uniq_attrs.GetKey(j);
				const Index<String>& values = t->uniq_attrs[j];
				
				ExportSimpleAttr& sat = da0.simple_attrs.GetAdd(group);
				
			}
			for(int j = 0; j < da0.actions.GetCount(); j++) {
				const ActionHeader& ah0 = da0.actions.GetKey(j);
				ExportAction& ea0 = da0.actions[j];
				
				if (ea0.clr == black) {
					int k = da1.actions.Find(ah0);
					if (k < 0)
						continue;
					
					const ExportAction& ea1 = da1.actions[k];
					if (ea1.clr != black)
						ea0.clr = ea1.clr;
				}
			}
		}*/
	}
}


END_TEXTLIB_NAMESPACE
#endif
