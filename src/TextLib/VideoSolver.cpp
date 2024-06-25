#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


VideoSolver::VideoSolver() {
	
}

int VideoSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void VideoSolver::DoPhase() {
	ASSERT(snap);
	
	TODO
	#if 0
	if (phase == PHASE_MAKE_STORYBOARD) {
		int script_i = snap->entity->FindScript(comp->scripts_file_title);
		if (script_i < 0) {
			NextPhase();
			LOG("error: no script found for " << comp->scripts_file_title);
			SetNotRunning();
			return;
		}
		
		if (skip_ready && comp->storyboard_parts.GetCount()) {
			NextPhase();
			return;
		}
		
		Script& script = snap->entity->scripts[script_i];
		
		VideoSolverArgs args;
		args.fn = 0;
		args.text = script.GetText();
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetVideoSolver(args, [this](String res) {
			comp->storyboard_parts.Clear();
			
			if (res.Left(1) != "-")
				res = "- " + res;
			
			RemoveEmptyLines3(res);
			Vector<String> lines = Split(res, "\n");
			for (String& l : lines) {
				int a = l.Find(":");
				if (a < 0) continue;
				String key = l.Left(a);
				String value = TrimBoth(l.Mid(a+1));
				if (value == "None" || value == "none")
					continue;
				
				comp->storyboard_parts.Add(key, value);
			}
			
			SetWaiting(0);
			NextPhase();
		});
	}
	else if (phase == PHASE_STORYBOARD_TO_PARTS_AND_DALLE_PROMPTS) {
		int script_i = snap->entity->FindScript(comp->scripts_file_title);
		ASSERT(script_i >= 0);
		Script& script = snap->entity->scripts[script_i];
		
		if (batch >= comp->storyboard_parts.GetCount()) {
			NextPhase();
			return;
		}
		
		String key = comp->storyboard_parts.GetKey(batch);
		auto& prompts = comp->storyboard_prompts.GetAdd(key);
		if (skip_ready && prompts.GetCount()) {
			NextBatch();
			return;
		}
		
		VideoSolverArgs args;
		args.fn = 1;
		args.text = script.GetText();
		args.parts.Add(key);
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetVideoSolver(args, [this](String res) {
			String key = comp->storyboard_parts.GetKey(batch);
			auto& prompts = comp->storyboard_prompts.GetAdd(key);
			prompts.Clear();
			
			if (res.Left(1) != "-")
				res = "- " + res;
			
			RemoveEmptyLines3(res);
			Vector<String> lines = Split(res, "\n");
			for (String& l : lines) {
				l = TrimBoth(l);
				RemoveQuotes(l);
				if (l.Find("List of long descriptions") == 0)
					break;
				
				prompts.Add(l);
			}
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else if (phase == PHASE_TEXT_TO_PARTS) {
		int script_i = snap->entity->FindScript(comp->scripts_file_title);
		ASSERT(script_i >= 0);
		Script& script = snap->entity->scripts[script_i];
		
		VideoSolverArgs args;
		args.fn = 2;
		args.parts <<= comp->storyboard_parts;
		
		String txt = script.GetText();
		txt.Replace("\r", "");
		tmp_lines = Split(txt, "\n");
		for(int i = 0; i < tmp_lines.GetCount(); i++) {
			String& l = tmp_lines[i];
			if (l.Left(1) == "[" || l.IsEmpty())
				tmp_lines.Remove(i--);
		}
		args.text = Join(tmp_lines, "\n");
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetVideoSolver(args, [this](String res) {
			comp->text_storyboard_parts.Clear();
			Vector<String> lines = Split(res, "\n");
			for(int i = 0; i < lines.GetCount(); i++) {
				String& l = lines[i];
				l = TrimBoth(l);
				RemoveQuotes(l);
				
				int la = l.Find("line #");
				int pa = l.Find("part #");
				if (pa < 0)
					continue;
				if (la < 0)
					la = -6;
				
				int line_i = ScanInt(l.Mid(la+6));
				int part_i = ScanInt(l.Mid(pa+6));
				if (line_i < 0 || line_i >= tmp_lines.GetCount())
					continue;
				
				String& tl = tmp_lines[line_i];
				comp->text_storyboard_parts.Add(tl, part_i);
			}
			
			SetWaiting(0);
			NextPhase();
		});
	}
	else if (phase == PHASE_FILL_STORY_PROMPTS) {
		if (batch >= comp->text_storyboard_parts.GetCount()) {
			NextPhase();
			return;
		}
		if (skip_ready && comp->text_storyboard_prompts.GetCount() > batch) {
			NextBatch();
			return;
		}
		
		comp->text_storyboard_prompts.SetCount(batch);
		
		String prompts_so_far = Join(comp->text_storyboard_prompts, "\n");
		
		const String& line = comp->text_storyboard_parts.GetKey(batch);
		int part_i = comp->text_storyboard_parts[batch];
		String key = comp->storyboard_parts.GetKey(part_i);
		const auto& prompts = comp->storyboard_prompts.GetAdd(key);
		if (prompts.IsEmpty()) {
			NextBatch();
			return;
		}
		
		VideoSolverArgs args;
		args.fn = 3;
		args.text = prompts_so_far;
		args.line = line;
		args.prompts <<= prompts;
		
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetVideoSolver(args, [this](String res) {
			res = TrimBoth(res);
			RemoveQuotes(res);
			
			comp->text_storyboard_prompts.Add(res);
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else TODO
	#endif
}

ArrayMap<hash_t, VideoSolver>& __VideoSolvers() {
	static ArrayMap<hash_t, VideoSolver> map;
	return map;
}

VideoSolver& VideoSolver::Get(Component& c) {
	Entity& e = *c.snapshot->entity;
	Profile& p = *e.profile;
	Snapshot& n = *c.snapshot;
	String t = p.owner->name + ": " + e.file_title + ": " + n.file_title + ": " + c.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, VideoSolver>& map = __VideoSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	VideoSolver& ls = map.Add(h);
	ASSERT(c.snapshot);
	ASSERT(c.snapshot->entity);
	ls.comp = &c;
	ls.snap = c.snapshot;
	ls.entity = c.snapshot->entity;
	return ls;
}


END_TEXTLIB_NAMESPACE

