#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



ActionAttrsProcess::ActionAttrsProcess() {
	
}

int ActionAttrsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ActionAttrsProcess::GetBatchCount(int phase) const {
	int per_action_task = BatchCount(phase);
	int total = 0;
	for (const auto& v : uniq_acts)
		total += v.GetCount();
	
	switch (phase) {
		case PHASE_COLORS_USING_EXISTING: return DB_COUNT;
		case PHASE_COLORS: return (total + per_action_task - 1) / per_action_task;
		case PHASE_ATTRS_USING_EXISTING: return DB_COUNT;
		case PHASE_ATTRS: return (total + per_action_task - 1) / per_action_task;
		default: TODO; return 1;
	}
}

int ActionAttrsProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ActionAttrsProcess::DoPhase() {
	switch (phase) {
		case PHASE_COLORS_USING_EXISTING: ColorsUsingExisting(); break;
		case PHASE_COLORS: Colors(); break;
		case PHASE_ATTRS_USING_EXISTING: AttrsUsingExisting(); break;
		case PHASE_ATTRS: Attrs(); break;
		default: TODO;
	}
	
	/*
	bar.Add(t_("Update action colors using existing"), AppImg::VioletRing(), THISBACK1(DoActionlistUsingExisting, 0)).Key(K_F5);
	bar.Add(t_("Update action colors"), AppImg::RedRing(), THISBACK1(DoActionlist, 0)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Update action attributes using existing"), AppImg::VioletRing(), THISBACK1(DoActionlistUsingExisting, 1)).Key(K_F7);
	bar.Add(t_("Update action attributes"), AppImg::RedRing(), THISBACK1(DoActionlist, 1)).Key(K_F8);
	*/
}

void ActionAttrsProcess::PrepareUsingExisting() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	{
		uniq_acts.Clear();
		for (const ActionHeader& ah : da.actions.GetKeys()) {
			if ((ah.action.GetCount() && ah.action[0] == '\"') || (ah.arg.GetCount() && ah.arg[0] == '\"'))
				continue;
			uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
		}
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
}

int ActionAttrsProcess::BatchCount(int fn) const {
	int per_action_task = 0;
	if (fn == 0)	per_action_task = per_action_clrs;
	if (fn == 1)	per_action_task = per_action_attrs;
	return per_action_task;
}

void ActionAttrsProcess::Prepare(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	args.fn = fn;
	
	int per_action_task = BatchCount(fn);
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	
	Color black(0,0,0);
	int iter = 0;
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		ActionHeader ah;
		ah.action = uniq_acts.GetKey(i);
		
		const VectorMap<String,int>& idx = uniq_acts[i];
		for(int j = 0; j < idx.GetCount(); j++) {
			ah.arg = idx.GetKey(j);
			
			if ((ah.action.GetCount() && ah.action[0] == '\"') || (ah.arg.GetCount() && ah.arg[0] == '\"'))
				continue;
			
			if (iter >= begin && iter < end) {
				ExportAction& aa = da.actions.GetAdd(ah);
				
				if (fn == 0 && aa.clr != black)
					continue;
				
				if (fn == 1 && aa.attr >= 0)
					continue;
				
				String s = uniq_acts.GetKey(i) + "(" + idx.GetKey(j) + ")";
				args.actions << s;
			}
			iter++;
		}
	}
	if (args.actions.IsEmpty()) {
		NextPhase();
		return; // ready
	}
}

void ActionAttrsProcess::ColorsUsingExisting() {
	PrepareUsingExisting();
	
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	actual = 0;
	total = 0;
	
	Color black(0,0,0);
	
	if (batch >= DB_COUNT) {
		NextPhase();
		return;
	}
	
	int i = batch;
	if (i == appmode) {NextBatch(); return;}
	TextDatabase& db1 = MetaDatabase::Single().db[i];
	SourceData& sd1 = db1.src_data;
	SourceDataAnalysis& sda1 = db1.src_data.a;
	DatasetAnalysis& da1 = sda1.dataset;
	if (!db1.loaded) {NextBatch(); return;}
	
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
	
	NextBatch();
}

void ActionAttrsProcess::Colors() {
	Prepare(0);
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetActionAnalysis(appmode, args, [this](String result) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
		
		Color black(0,0,0);
		Color non_black(1,1,1);
		
		result.Replace("\r", "");
		Vector<String> lines = Split(result, "\n");
		for (String& line : lines) {
			line = TrimBoth(line);
			if (line.IsEmpty()) continue;
			if (line[0] != '\"') continue;
			int a = 1;
			int b = line.ReverseFind("\"");
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
			b = full_action.ReverseFind(")");
			ah.arg = full_action.Mid(a,b-a);
			
			if (clr == black)
				clr = non_black;
			
			if ((ah.action.GetCount() && ah.action[0] == '\"') || (ah.arg.GetCount() && ah.arg[0] == '\"'))
				continue;
			ExportAction& aa = da.actions.GetAdd(ah);
			aa.clr = clr;
		}
		
		int a = 0;
		for (const ExportAction& ea : da.actions.GetValues())
			if (ea.clr != black)
				a++;
		da.diagnostics.GetAdd("actionlist colors: total") = IntStr(da.virtual_phrase_parts.GetCount());
		da.diagnostics.GetAdd("actionlist colors: actual") =  IntStr(a);
		da.diagnostics.GetAdd("actionlist colors: percentage") =  DblStr((double)a / (double) da.virtual_phrase_parts.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}

void ActionAttrsProcess::AttrsUsingExisting() {
	PrepareUsingExisting();
	
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
	if (batch >= DB_COUNT) {
		NextPhase();
		return;
	}
	
	actual = 0;
	total = 0;
	
	Color black(0,0,0);
	
	int i = batch;
	if (i == appmode) {NextBatch(); return;}
	TextDatabase& db1 = MetaDatabase::Single().db[i];
	SourceData& sd1 = db1.src_data;
	SourceDataAnalysis& sda1 = db1.src_data.a;
	DatasetAnalysis& da1 = sda1.dataset;
	if (!db1.loaded) {NextBatch(); return;}
	
	for(int j = 0; j < da0.actions.GetCount(); j++) {
		const ActionHeader& ah0 = da0.actions.GetKey(j);
		ExportAction& ea0 = da0.actions[j];
		
		if (ea0.attr < 0) {
			int k = da1.actions.Find(ah0);
			if (k < 0)
				continue;
			
			const ExportAction& ea1 = da1.actions[k];
			if (ea1.attr < 0)
				continue;
			
			const AttrHeader& ath1 = da1.attrs.GetKey(ea1.attr);
			ExportAttr& eat0 = da0.attrs.GetAdd(ath1, ea0.attr);
		}
	}
	
	NextBatch();
}

void ActionAttrsProcess::Attrs() {
	Prepare(1);
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetActionAnalysis(appmode, args, [this](String result) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		// "attention-procedures(planning)" problem solving strategy / shortcut taking
		
		result.Replace("\r", "");
		Vector<String> lines = Split(result, "\n");
		for (String& line : lines) {
			line = TrimBoth(line);
			if (line.IsEmpty()) continue;
			if (line[0] != '\"') continue;
			int a = 1;
			int b = line.ReverseFind("\"");
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
			b = full_action.ReverseFind(")");
			ah.arg = full_action.Mid(a,b-a);
			
			ASSERT(ah.action.Find("\"") != 0 && ah.arg.Find("\"") != 0);
			ExportAction& ea = da.actions.GetAdd(ah);
			da.attrs.GetAdd(ath, ea.attr);
		}
		
		int a = 0;
		for (const ExportAction& ea : da.actions.GetValues())
			if (ea.attr >= 0)
				a++;
		da.diagnostics.GetAdd("actionlist attrs: total") = IntStr(da.virtual_phrase_parts.GetCount());
		da.diagnostics.GetAdd("actionlist attrs: actual") =  IntStr(a);
		da.diagnostics.GetAdd("actionlist attrs: percentage") =  DblStr((double)a / (double) da.virtual_phrase_parts.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}

ActionAttrsProcess& ActionAttrsProcess::Get(int appmode) {
	static ArrayMap<String, ActionAttrsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	ActionAttrsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

