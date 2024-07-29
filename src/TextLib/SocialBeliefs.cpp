#include "TextLib.h"

BEGIN_TEXTLIB_NAMESPACE

SocialBeliefsProcess::SocialBeliefsProcess() {
	
}

int SocialBeliefsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

void SocialBeliefsProcess::DoPhase() {
	MetaDatabase& mdb = MetaDatabase::Single();
	if (phase == PHASE_GET_POSITIVE_ATTRS) {
		if (batch >= mdb.beliefs.GetCount()){
			NextPhase();
			return;
		}
		Belief& b = mdb.beliefs[batch];
		if (b.attrs.GetCount()) {
			NextBatch();
			return;
		}
		
		BeliefArgs args;
		args.fn = 0;
		args.user <<= b.user;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetSocialBeliefsProcess(args, [this,&b](String res) {
			res = TrimBoth(res);
			if (res.Left(2) != "1.")
				res = "1." + res;
			
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			b.attrs.Clear();
			for(int i = 0; i < lines.GetCount(); i++) {
				b.attrs.Add().positive = lines[i];
			}
			SetWaiting(0);
			NextBatch();
			
			WhenReady();
		});
	}
	else if (phase == PHASE_GET_NEGATIVE_ATTRS) {
		if (batch >= mdb.beliefs.GetCount()){
			NextPhase();
			return;
		}
		Belief& b = mdb.beliefs[batch];
		if (!b.attrs.GetCount() || b.attrs[0].negative.GetCount()) {
			NextBatch();
			return;
		}
		
		BeliefArgs args;
		args.fn = 1;
		for(int i = 0; i < b.attrs.GetCount(); i++)
			args.pos << b.attrs[i].positive;
		
		SetWaiting(1);
		TaskMgr& m = TaskMgr::Single();
		m.GetSocialBeliefsProcess(args, [this,&b](String res) {
			res = TrimBoth(res);
			if (res.Left(2) != "1.")
				res = "1." + res;
			
			RemoveEmptyLines2(res);
			Vector<String> lines = Split(res, "\n");
			
			for(int i = 0; i < lines.GetCount(); i++) {
				if (i >= b.attrs.GetCount()) break;
				b.attrs[i].negative = lines[i];
			}
			
			SetWaiting(0);
			NextBatch();
			
			WhenReady();
		});
	}
}

SocialBeliefsProcess& SocialBeliefsProcess::Get(Profile& e, BiographySnapshot& snap) {
	String t = e.owner->name + ": " + e.name;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, SocialBeliefsProcess> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	SocialBeliefsProcess& ls = map.Add(h);
	ASSERT(e.owner);
	ls.owner = e.owner;
	ls.profile = &e;
	ls.snap = &snap;
	return ls;
}

	
END_TEXTLIB_NAMESPACE