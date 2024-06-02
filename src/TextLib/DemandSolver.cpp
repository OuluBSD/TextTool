#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


DemandSolver::DemandSolver() {
	//skip_ready = false;
}

int DemandSolver::GetPhaseCount() const {
	return PHASE_COUNT;
}

void DemandSolver::DoPhase() {
	ASSERT(owner);
	
	if (phase == PHASE_NEEDS_PER_ROLE) {
		if (batch >= owner->roles.GetCount()) {
			NextPhase();
			return;
		}
		Role& r = owner->roles[batch];
		
		if (skip_ready && r.needs.GetCount()) {
			NextBatch();
			return;
		}
		
		DemandArgs args;
		args.fn = 0;
		args.role = r.name;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetDemandSolver(args, [this](String res) {
			Role& r = owner->roles[batch];
			RemoveEmptyLines3(res);
			Vector<String> needs = Split(res, "\n");
			
			r.needs.Clear();
			for (String& n : needs)
				r.needs.Add().name = n;
			
			SetWaiting(0);
			NextBatch();
		});
	}
	else if (phase == PHASE_CAUSES_PER_NEED) {
		if (batch >= owner->roles.GetCount()) {
			NextPhase();
			return;
		}
		Role& r = owner->roles[batch];
		if (sub_batch >= r.needs.GetCount()) {
			NextBatch();
			return;
		}
		const Need& n = r.needs[sub_batch];
		
		if (skip_ready && n.causes.GetCount()) {
			NextSubBatch();
			return;
		}
		
		DemandArgs args;
		args.fn = 1;
		args.role = r.name;
		args.need = n.name;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetDemandSolver(args, [this](String res) {
			Role& r = owner->roles[batch];
			Need& n = r.needs[sub_batch];
			
			RemoveEmptyLines3(res);
			n.causes = Split(res, "\n");
			
			SetWaiting(0);
			NextSubBatch();
		});
	}
	else if (phase == PHASE_MESSAGES_PER_NEED) {
		if (batch >= owner->roles.GetCount()) {
			NextPhase();
			return;
		}
		Role& r = owner->roles[batch];
		if (sub_batch >= r.needs.GetCount()) {
			NextBatch();
			return;
		}
		const Need& n = r.needs[sub_batch];
		
		if (skip_ready && n.messages.GetCount()) {
			NextSubBatch();
			return;
		}
		
		DemandArgs args;
		args.fn = 2;
		args.role = r.name;
		args.need = n.name;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetDemandSolver(args, [this](String res) {
			Role& r = owner->roles[batch];
			Need& n = r.needs[sub_batch];
			
			RemoveEmptyLines3(res);
			n.messages = Split(res, "\n");
			for (String& s : n.messages)
				RemoveQuotes2(s);
			
			SetWaiting(0);
			NextSubBatch();
		});
	}
	else if (phase == PHASE_PLATFORMS_PER_CAUSE) {
		const auto& plats = GetPlatforms();
		if (batch >= plats.GetCount()) {
			NextPhase();
			return;
		}
		const Platform& plat = plats[batch];
		
		if (!sub_batch) {
			need_tasks.Clear();
			for (Role& r : owner->roles) {
				for (Need& n : r.needs) {
					if (skip_ready && batch < n.platforms.GetCount())
						continue;
					NeedTask& t = need_tasks.Add();
					t.r = &r;
					t.n = &n;
				}
			}
		}
		if (sub_batch >= need_tasks.GetCount()) {
			NextBatch();
			return;
		}
		NeedTask& t = need_tasks[sub_batch];
		
		
		DemandArgs args;
		args.fn = 3;
		args.role = t.r->name;
		args.need = t.n->name;
		args.platform = batch;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetDemandSolver(args, [this](String res) {
			NeedTask& t = need_tasks[sub_batch];
			
			res = TrimBoth(ToLower(res));
			bool yes = res.Left(3) == "yes";
			if (batch >= t.n->platforms.GetCount())
				t.n->platforms.SetCount(batch+1);
			t.n->platforms[batch].enabled = yes;
			
			SetWaiting(0);
			NextSubBatch();
		});
	}
	else if (phase == PHASE_ACTION_CAUSES) {
		if (batch >= owner->roles.GetCount()) {
			NextPhase();
			return;
		}
		Role& r = owner->roles[batch];
		if (sub_batch >= r.actions.GetCount()) {
			NextBatch();
			return;
		}
		const RoleAction& ra = r.actions[sub_batch];
		
		if (skip_ready && ra.need_causes.GetCount()) {
			NextSubBatch();
			return;
		}
		
		DemandArgs args;
		args.fn = 4;
		args.role = r.name;
		args.action = ra.name;
		
		for(int i = 0; i < r.needs.GetCount(); i++) {
			Need& n = r.needs[i];
			auto& v = args.need_causes.GetAdd(n.name);
			v <<= n.causes;
		}
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetDemandSolver(args, [this](String res) {
			Role& r = owner->roles[batch];
			RoleAction& ra = r.actions[sub_batch];
			ra.need_causes.Clear();
			
			RemoveEmptyLines3(res);
			Vector<String> lines = Split(res, "\n");
			for (String& s : lines) {
				int a = s.Find(".");
				if (a < 0) continue;
				String need_str = TrimLeft(s.Left(a));
				String cause_str = TrimLeft(s.Mid(a+1));
				int need_i = ScanInt(need_str) - 1;
				int cause_i = ScanInt(cause_str) - 1;
				if (need_i < 0 || need_i >= r.needs.GetCount()) continue;
				Need& n = r.needs[need_i];
				if (cause_i < 0 || cause_i >= n.causes.GetCount()) continue;
				auto& nc = ra.need_causes.Add();
				nc.need_i = need_i;
				nc.cause_i = cause_i;
			}
			
			SetWaiting(0);
			NextSubBatch();
		});
	}
	else if (phase == PHASE_USER_NEEDS) {
		
		SetNotRunning(); return;
		TODO
		
	}
	else if (phase == PHASE_USER_CAUSES) {
		
		SetNotRunning(); return;
		TODO
		
	}
	else if (phase == PHASE_USER_ACTIONS) {
		
		SetNotRunning(); return;
		TODO
		
	}
	else TODO
	
}

ArrayMap<hash_t, DemandSolver>& __DemandSolvers() {
	static ArrayMap<hash_t, DemandSolver> map;
	return map;
}

DemandSolver& DemandSolver::Get(Owner& o) {
	String t = o.name;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, DemandSolver>& map = __DemandSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	DemandSolver& ls = map.Add(h);
	ls.owner = &o;
	return ls;
}


END_TEXTLIB_NAMESPACE

