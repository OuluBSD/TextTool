#include "TextLib.h"

NAMESPACE_UPP

static force_inline uint64_t s_rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k)); // GCC/CLANG/MSC happily optimize this
}

force_inline
static uint64 sNext(uint64 *s)
{
	const uint64_t result_starstar = s_rotl(s[1] * 5, 7) * 9;
	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = s_rotl(s[3], 45);

	return result_starstar;
}

END_UPP_NAMESPACE


BEGIN_TEXTLIB_NAMESPACE




ScriptPostSolver::ScriptPostSolver() {
	//skip_ready = false;
	generation_count = 4;
	appmode = DB_SONG;
}

int ScriptPostSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void ScriptPostSolver::DoPhase() {
	ASSERT(owner);
	
	if (phase == PHASE_ANALYSE_WEAKNESSES) {
		if (generation >= script->postfixes.GetCount()) {
			script->postfixes.SetCount(generation+1);
		}
		
		// Convert source text to lines + names of parts
		ScriptPostFix& spf = script->postfixes[generation];
		if (batch == 0 && sub_batch == 0) {
			if (!skip_ready) {
				spf.weaks.Clear();
				spf.improvements.Clear();
			}
			spf.src_lines.Clear();
			spf.src_line_parts.Clear();
			
			String s;
			if (generation == 0) {
				s = script->GetText(appmode);
			}
			else {
				ScriptPostFix& prev_spf = script->postfixes[generation-1];
				if (prev_spf.variations.IsEmpty()) {
					LOG("error: no best variation");
					SetNotRunning();
					return;
				}
				s = prev_spf.variations[0].text;
			}
			s.Replace("\r", "");
			if (s.IsEmpty()) {
				LOG("error: no source text");
				SetNotRunning();
				return;
			}
			Vector<String> lines = Split(s, "\n");
			String active_key;
			
			// This is kind of a state machine
			for(int i = 0; i < lines.GetCount(); i++) {
				String& l = lines[i];
				l = TrimBoth(l);
				if (l.IsEmpty()) {lines.Remove(i--); continue;}
				if (l.Left(1) == "[") {
					int b = l.Find("]");
					if (b >= 0)
						active_key = TrimBoth(l.Mid(1,b-1));
					else
						active_key = "unnamed part";
					lines.Remove(i--);
					continue;
				}
				else if (l.Right(1) == ":") {
					active_key = TrimBoth(l.Left(l.GetCount()-1));
					lines.Remove(i--);
					continue;
				}
				if (active_key.GetCount()) {
					spf.src_line_parts.Add(i, active_key);
					active_key.Clear();
				}
			}
			Swap(spf.src_lines, lines); // Just swap for fast write: "lines" is discarded
		}
		
		//if (batch >= POSTSCRIPT_COUNT) {
		if (batch >= SCORE_COUNT) {
			NextPhase();
			return;
		}
		
		if (batch >= spf.weaks.GetCount())
			spf.weaks.SetCount(batch+1);
		
		if (skip_ready && spf.weaks[batch].GetCount()) {
			NextBatch();
			return;
		}
		
		ScriptPostArgs args;
		args.fn = 0;
		//args.key = GetPostScriptAnalysisKey(batch);
		//args.desc = GetPostScriptAnalysisDescription(batch);
		args.lines <<= spf.src_lines;
		args.is_female = entity->is_female;
		if (script->belief_uniq == 0) {
			args.key = GetScoreTitle(batch);
			args.desc = GetScoreDescription(batch);
		}
		else {
			auto& mdb = MetaDatabase::Single();
			int belief_i = mdb.FindBelief(script->belief_uniq);
			Belief& b = mdb.beliefs[belief_i - 1];
			if (batch >= b.attrs.GetCount()) {SetNotRunning(); return;}
			args.key = b.attrs[batch].positive;
			args.desc = "Has " + ToLower(b.attrs[batch].positive) + ". Doesn't have " + ToLower(b.attrs[batch].negative) + ".";
		}
		
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptPost(appmode, args, [this](String res) {
			ScriptPostFix& spf = script->postfixes[generation];
			auto& weaks = spf.weaks[batch];
			weaks.Clear();
			
			if (res.Left(6) != "line #")
				res = "line #" + res;
			
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			for (String& l : lines) {
				int a = l.Find(":");
				if (a < 0) continue;
				
				String line_str = l.Left(a);
				String desc = TrimBoth(l.Mid(a+1));
				if (desc.IsEmpty())
					continue;
				
				int line_i = -1;
				int altline_i = -1;
				Vector<String> parts = Split(line_str, " and ");
				for(int i = 0; i < parts.GetCount(); i++) {
					String& p = parts[i];
					a = p.Find("#");
					if (a < 0) continue;
					if (i == 0)
						line_i = ScanInt(p.Mid(a+1));
					else if (i == 1)
						altline_i = ScanInt(p.Mid(a+1));
				}
				
				if (line_i < 0 || line_i >= spf.src_lines.GetCount()) continue;
				if (altline_i < 0 || altline_i >= spf.src_lines.GetCount()) continue;
				
				ScriptPostFix::Weak& w = weaks.Add();
				w.line_i = line_i;
				w.altline_i = altline_i;
				w.description = desc;
			}
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else if (phase == PHASE_IMPROVE_WEAKNESSES) {
		
		//if (batch >= POSTSCRIPTMOD_COUNT) {
		if (batch >= SCORE_COUNT) {
			NextPhase();
			return;
		}
		
		ScriptPostFix& spf = script->postfixes[generation];
		if (batch >= spf.improvements.GetCount())
			spf.improvements.SetCount(batch+1);
		
		if (skip_ready && spf.improvements[batch].GetCount()) {
			NextBatch();
			return;
		}
		
		ScriptPostArgs args;
		args.fn = 1;
		args.is_female = entity->is_female;
		//args.key = GetPostScriptModificationKey(batch);
		args.lines <<= spf.src_lines;
		if (script->belief_uniq == 0) {
			args.key = "Modify text to increase " + ToLower(GetScoreTitle(batch)) + " score";
		}
		else {
			auto& mdb = MetaDatabase::Single();
			int belief_i = mdb.FindBelief(script->belief_uniq);
			Belief& b = mdb.beliefs[belief_i - 1];
			if (batch >= b.attrs.GetCount()) {SetNotRunning(); return;}
			args.key = "Modify text to have more " + ToLower(b.attrs[batch].positive);
		}
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptPost(appmode, args, [this](String res) {
			ScriptPostFix& spf = script->postfixes[generation];
			auto& improvements = spf.improvements[batch];
			improvements.Clear();
			
			if (res.Left(6) != "line #")
				res = "line #" + res;
			
			RemoveEmptyLines3(res);
			Vector<String> lines = Split(res, "\n");
			for (String& l : lines) {
				int a = l.Find(":");
				if (a < 0) continue;
				
				String line_str = l.Left(a);
				String desc = TrimBoth(l.Mid(a+1));
				if (desc.IsEmpty() || desc.Find("blah blah") >= 0)
					continue;
				
				int line_i = -1;
				Vector<String> parts = Split(line_str, " and ");
				for(int i = 0; i < parts.GetCount(); i++) {
					String& p = parts[i];
					a = p.Find("#");
					if (a < 0) continue;
					if (i == 0)
						line_i = ScanInt(p.Mid(a+1));
				}
				if (line_i < 0 || line_i >= spf.src_lines.GetCount()) continue;
				
				a = desc.Find("->");
				if (a < 0) continue;
				a = desc.Find("\"", a);
				if (a < 0) continue;
				a++;
				int b = desc.ReverseFind("\"");
				if (b < 0 || a >= b) continue;
				
				ScriptPostFix::Improvement& w = improvements.Add();
				w.line_i = line_i;
				w.text = desc.Mid(a,b-a);
			}
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else if (phase == PHASE_SCORE_VARIATIONS) {
		ScriptPostFix& spf = script->postfixes[generation];
		
		#define SCORE_COMPARISON 1
		
		if (batch == 0 && sub_batch == 0) {
			// Make probability vector for items to be changes
			Vector<int> line_slots, line_counts;
			Vector<Vector<Tuple2<int,int>>> improvements;
			
			// Get 1-dimensional improvements vector for random improvement selection
			improvements.SetCount(spf.src_lines.GetCount());
			for(int i = 0; i < spf.improvements.GetCount(); i++) {
				auto& v = spf.improvements[i];
				for(int j = 0; j < v.GetCount(); j++) {
					ScriptPostFix::Improvement& w = v[j];
					if (w.line_i < 0 || w.line_i >= spf.src_lines.GetCount()) continue;
					improvements[w.line_i] << Tuple2<int,int>(i, j);
				}
			}
			
			// Deterministic seed
			uint64 state[4] = {1,10,100,1000};
			
			// Make variations
			variations.Clear();
			for (int var_i = 0; var_i < variation_count; var_i++) {
				auto& var = variations.Add();
				var.scores.SetCount(SCORE_COUNT,0);
				Vector<String>& lines = var.lines;
				lines <<= spf.src_lines;
				int max_changes = max(1,(int)(lines.GetCount() * mutation_factor));
				
				Index<int> modified_lines;
				for (int change = 0; change < max_changes; change++) {
					// Get improvement count per line
					line_counts.SetCount(spf.src_lines.GetCount());
					for (int& i : line_counts) i = 0;
					for(int i = 0; i < spf.improvements.GetCount(); i++) {
						auto& v = spf.improvements[i];
						for(int j = 0; j < v.GetCount(); j++) {
							ScriptPostFix::Improvement& w = v[j];
							if (w.line_i < 0 || w.line_i >= spf.src_lines.GetCount()) continue;
							if (modified_lines.Find(w.line_i) >= 0) continue;
							line_counts[w.line_i]++;
						}
					}
					
					// Calculate total improvement count
					int total_slots = 0;
					for (int i : line_counts)
						total_slots += i;
					if (!total_slots)
						break;
					
					// Make line slots vector
					line_slots.SetCount(total_slots, -1);
					int* it = line_slots.Begin();
					for(int i = 0; i < line_counts.GetCount(); i++) {
						int line_count = line_counts[i];
						for(int j = 0; j < line_count; j++)
							*it++ = i;
					}
					ASSERT(it == line_slots.End());
					
					// Get "random" line and improvement
					int slot_i = sNext(state) % (uint64)line_slots.GetCount();
					int line_i = line_slots[slot_i];
					ASSERT(modified_lines.Find(line_i) < 0);
					auto& line_impovements = improvements[line_i];
					int impr_i = sNext(state) % (uint64)line_impovements.GetCount();
					auto& impr = line_impovements[impr_i];
					
					// Change the line for variation
					ScriptPostFix::Improvement& w = spf.improvements[impr.a][impr.b];
					lines[line_i] = w.text;
					modified_lines.Add(line_i);
				}
				
				if (modified_lines.IsEmpty()) {
					SetNotRunning();
					LOG("error: no total slots");
					return;
				}
			}
			
			// Do matches until 1 remaining
			matches.Clear();
			
			#if !SCORE_COMPARISON
			remaining.Clear();
			for(int i = 0; i < variations.GetCount(); i++) {
				remaining.Add(i);
			}
			#endif
		}
		
		
		#if SCORE_COMPARISON
		if (batch >= variations.GetCount())
		#else
		if (remaining.GetCount() <= 1)
		#endif
		{
			// U++ Sort crash bug workaround
			#if 1
			VectorMap<int,double> var_scores;
			for(int i = 0; i < variations.GetCount(); i++)
				var_scores.Add(i, variations[i].ScoreSum());
			SortByValue(var_scores, StdGreater<double>());
			//DUMPM(var_scores);
			Array<Variation> tmp_vars;
			Index<int> rm_list;
			for (int i : var_scores.GetKeys()) {
				int diff = 0;
				for(int j = 0; j < i; j++) {
					if (rm_list.Find(j) >= 0)
						diff++;
				}
				tmp_vars.Add(variations.Detach(i-diff));
				rm_list.Add(i);
			}
			Swap(tmp_vars, variations);
			/*for(int i = 0; i < variations.GetCount(); i++) {
				LOG(i << ": " << variations[i].ScoreSum());
			}*/
			#else
			Sort(variations, Variation());
			#endif
			LOG(Join(variations[0].lines, "\n"));
			spf.variations.Clear();
			spf.variations.SetCount(variations.GetCount());
			for(int i = 0; i < variations.GetCount(); i++) {
				// Join text and add part names also
				String text;
				const auto& lines = variations[i].lines;
				for(int j = 0; j < lines.GetCount(); j++) {
					int k = spf.src_line_parts.Find(j);
					if (k >= 0) {
						if (!text.IsEmpty()) text << "\n";
						text << "[" << spf.src_line_parts[k] << "]\n";
					}
					text << lines[j] << "\n";
				}
				spf.variations[i].text = text;
				spf.variations[i].scores <<= variations[i].scores;
			}
			NextPhase();
			
			// Check if this is very well improving situation
			if (generation == generation_count-1) {
				double max_score = 0;
				int max_i = -1;
				for(int i = 0; i < script->postfixes.GetCount(); i++) {
					double score = script->postfixes[i].variations[0].ScoreAv();
					if (score >= max_score) {
						max_i = i;
						max_score = score;
					}
				}
				// If the latest result is the best, then try once more
				if (max_i == generation) {
					generation_count++;
				}
			}
			
			return;
		}
		
		ScriptPostArgs args;
		args.fn = 2;
		args.is_female = entity->is_female;
		
		if (script->belief_uniq == 0) {
			auto& mdb = MetaDatabase::Single();
			int belief_i = mdb.FindBelief(script->belief_uniq);
			Belief& b = mdb.beliefs[belief_i - 1];
			if (batch >= b.attrs.GetCount()) {SetNotRunning(); return;}
			for(int i = 0; i < b.attrs.GetCount(); i++) {
				Belief::Attr& a = b.attrs[i];
				String s = "S" + IntStr(i) + ": High " + ToLower(a.positive) + " score. Low score means that the phrase is " + ToLower(a.negative) + ".";
				args.scores << s;
			}
		}
		
		#if SCORE_COMPARISON
		args.lines << Join(variations[batch].lines, "\n");
		#else
		for(int i = 0; i < 2; i++) {
			int sugg_i = remaining[i];
			String content = Join(variations[sugg_i].lines, "\n");
			args.lines << content;
		}
		#endif
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptPost(appmode, args, [this](String res) {
			ScriptPostFix& spf = script->postfixes[generation];
			
			#if SCORE_COMPARISON
			res = "S0:" + res;
			Vector<int>& scores = variations[batch].scores;
			scores.SetCount(0);
			{
				Vector<String> parts = Split(res, ",");
				int score_sum = 0, score_count = 0;
				for (String& p : parts) {
					p = TrimBoth(p);
					int a = p.Find(":");
					if (a < 0) continue;
					p = p.Mid(a+1);
					int score = ScanInt(TrimLeft(p));
					score_sum += score;
					score_count++;
					scores << score;
				}
			}
			#else
			int loser = 0;
			
			res = "entry #1: S0:" + res;
			
			RemoveEmptyLines3(res);
			Vector<String> lines = Split(res, "\n");
			Vector<double> total_scores;
			Vector<Vector<int>> all_scores;
			for (String& line : lines) {
				int a = line.Find(":");
				if (a < 0) continue;
				line = TrimBoth(line.Mid(a+1));
				Vector<String> parts = Split(line, ",");
				int score_sum = 0, score_count = 0;
				Vector<int>& scores = all_scores.Add();
				for (String& p : parts) {
					p = TrimBoth(p);
					a = p.Find(":");
					if (a < 0) continue;
					p = p.Mid(a+1);
					int score = ScanInt(TrimLeft(p));
					score_sum += score;
					score_count++;
					scores << score;
				}
				double score = (double)score_sum / score_count;
				total_scores << score;
			}
			if (total_scores.GetCount() >= 2)
				loser = total_scores[0] > total_scores[1] ? 1:0;
			else
				loser = 0; // error
			
			// Don't overwrite scores, but maximize them (kinda irrelevant anyway)
			for(int i = 0; i < all_scores.GetCount() && i < 2; i++) {
				const Vector<int>& from = all_scores[i];
				Vector<int>& to = variations[remaining[i]].scores;
				to.SetCount(SCORE_COUNT,0);
				for(int j = 0; j < from.GetCount() && j < to.GetCount(); j++)
					to[j] = max(to[j], from[j]);
			}
			
			int loser_sugg_i = remaining[loser];
			variations[loser_sugg_i].rank = remaining.GetCount()-1;
			remaining.Remove(loser);
			
			if (remaining.GetCount() == 1) {
				int sugg_i = remaining[0];
				variations[sugg_i].rank = 0;
			}
			#endif
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else TODO
	
}

ArrayMap<hash_t, ScriptPostSolver>& __ScriptPostSolvers() {
	static ArrayMap<hash_t, ScriptPostSolver> map;
	return map;
}

ScriptPostSolver& ScriptPostSolver::Get(Profile& p, Entity& e, Script& s, int appmode) {
	String t = p.owner->name + ": " + p.name + ": " + s.native_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, ScriptPostSolver>& map = __ScriptPostSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptPostSolver& ls = map.Add(h);
	ls.owner = p.owner;
	ls.profile = &p;
	ls.script = &s;
	ls.entity = &e;
	ls.appmode = appmode;
	return ls;
}


END_TEXTLIB_NAMESPACE

