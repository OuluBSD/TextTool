#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptIdeaSolver::ScriptIdeaSolver() {
	
}

int ScriptIdeaSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void ScriptIdeaSolver::DoPhase() {
	const auto& tcs = GetTypeclasses(appmode);
	const auto& cons = GetContents(appmode);
	
	int tc_i = batch;
	int con_i = sub_batch;
	if (only_single) {
		skip_ready = false;
		tc_i = only_tc;
		con_i = only_con;
	}
	else {
		skip_ready = true;
	}
	
	
	if (phase == PHASE_GET_IDEAS) {
		// Do this phase only for the entity
		if (script) {
			ASSERT(cvo == static_cast<ContentVisionOwner*>(script));
			NextPhase();
			return;
		}
		ASSERT(cvo == static_cast<ContentVisionOwner*>(entity));
		if (!only_single) {
			if (tc_i >= tcs.GetCount()) {
				NextPhase();
				return;
			}
			if (con_i >= cons.GetCount()) {
				NextBatch();
				return;
			}
			if (skip_ready && cvo->FindIdeas(tc_i, con_i).GetCount()) {
				NextSubBatch();
				return;
			}
		}
		else {
			if (skip_ready && cvo->FindIdeas(tc_i, con_i).GetCount()) {
				NextPhase();
				return;
			}
		}
		
		cvo->ClearIdeas(tc_i, con_i);
		
		EnterAppMode(appmode);
		
		String bio = entity->biography;
		if (bio.IsEmpty())
			bio = profile->biography;
		
		ScriptSolverArgs args;
		args.fn = 12;
		args.artist.Add(profile->name, bio);
		args.song.Add("Person visually", entity->speaker_visually);
		args.song.Add(__typeclass, TextLib::GetTypeclasses(appmode)[tc_i]);
		args.song.Add(__content, TextLib::GetContents(appmode)[con_i].key);
		
		LeaveAppMode();
		
		SetWaiting(1);
		
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptSolver(appmode, args, THISBACK3(ParseSuggestions, false, tc_i, con_i));
	}
	else if (phase == PHASE_GET_LEAD_IDEAS) {
		// Do this phase only for the script
		if (!script) {
			ASSERT(cvo == static_cast<ContentVisionOwner*>(entity));
			NextPhase();
			return;
		}
		ASSERT(cvo == static_cast<ContentVisionOwner*>(script));
		
		cvo->ideas.Clear();
		
		String bio = entity->biography;
		if (bio.IsEmpty())
			bio = profile->biography;
		
		ScriptSolverArgs args;
		args.fn = 13;
		args.artist.Add(profile->name, bio);
		args.song.Add("Person visually", entity->speaker_visually);
		args.song.Add("Lead info about the song", script->lead);
		
		LeaveAppMode();
		
		SetWaiting(1);
		
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptSolver(appmode, args, THISBACK3(ParseSuggestions, true, -1, -1));
	}
	else if (phase == PHASE_GET_SCORES_OF_IDEAS) {
		if (batch >= cvo->ideas.GetCount()) {
			NextPhase();
			return;
		}
		
		ScriptPostArgs args;
		args.fn = 2;
		args.lines.Add(cvo->ideas[batch].text);
		
		SetWaiting(1);
		
		TaskMgr& m = TaskMgr::Single();
		m.GetScriptPost(appmode, args, [this](String res) {
			res = "S0:" + res;
			Vector<int>& scores = cvo->ideas[batch].scores;
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
			
			if (batch == cvo->ideas.GetCount()-1) {
				Sort(cvo->ideas, ContentVisionIdea());
			}
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else TODO;
	
}

void ScriptIdeaSolver::ParseSuggestions(String res, bool types, int tc_i, int con_i) {
	bool req_storyline = res.Find("Storyline:") >= 0 || res.Find("storyline:") >= 0;
	if (types)
		res = "1. A #" + res;
	RemoveEmptyLines2(res);
	Vector<String> lines = Split(res, "\n");
	
	int added = 0;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (types) {
			tc_i = 0, con_i = 0;
			int a = l.Find("A #");
			if (a < 0)
				continue;
			tc_i = ScanInt(l.Mid(a+3));
			a = l.Find("B #");
			if (a < 0)
				continue;
			con_i = ScanInt(l.Mid(a+3));
			a = l.Find(":");
			if (a >= 0)
				l = TrimBoth(l.Mid(a+1));
		}
		
		if (!req_storyline && l.Right(1) == ":" && i+1 < lines.GetCount()) {
			lines[i] += " " + lines[i+1];
			lines.Remove(i+1);
			i--;
			continue;
		}
		if (!req_storyline && l.Right(1) == "\"" && l.Find(":") < 0 && i+1 < lines.GetCount()) {
			lines[i] += ": " + lines[i+1];
			lines.Remove(i+1);
			i--;
			continue;
		}
		if (!req_storyline && l.Find("Title:") == 0) {
			lines.Remove(i);
			i--;
			continue;
		}
		if (l.Find("Content vision:") == 0)
			l = TrimBoth(l.Mid(15));
		l.Replace("\" -", "\": ");
		if (req_storyline && l.Find("toryline:") < 0)
			continue;
		ContentVisionIdea& idea = cvo->ideas.Add();
		idea.text = TrimBoth(lines[i]);
		idea.tc_i = tc_i;
		idea.con_i = con_i;
		
		if (req_storyline) {
			int a = idea.text.Find("toryline:");
			idea.text = TrimBoth(idea.text.Mid(a+9));
		}
		
		added++;
		if (added >= 10)
			break;
	}
	
	SetWaiting(0);
	
	if (script || only_single)
		NextPhase();
	else
		NextSubBatch();
}

ArrayMap<hash_t, ScriptIdeaSolver>& __ScriptIdeaSolvers() {
	static ArrayMap<hash_t, ScriptIdeaSolver> map;
	return map;
}

ScriptIdeaSolver& ScriptIdeaSolver::Get(Entity& e, Script* s, int appmode) {
	String t = e.profile->owner->name + ": " + e.profile->name;
	if (s) t += ": " + s->GetAnyTitle();
	
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, ScriptIdeaSolver>& map = __ScriptIdeaSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	ScriptIdeaSolver& ls = map.Add(h);
	ls.owner = e.profile->owner;
	ls.entity = &e;
	ls.profile = e.profile;
	ls.script = s;
	ls.appmode = appmode;
	ls.cvo = s ? static_cast<ContentVisionOwner*>(s) : static_cast<ContentVisionOwner*>(&e);
	return ls;
}


END_TEXTLIB_NAMESPACE
