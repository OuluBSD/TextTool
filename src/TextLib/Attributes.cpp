#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



AttributesProcess::AttributesProcess() {
	
}

int AttributesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int AttributesProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_MAIN_GROUPS:		return attr_extremes_batches.GetCount();
		case PHASE_SIMPLIFY_ATTRS:	return attr_polar_batches.GetCount();
		case PHASE_JOIN_ORPHANED:	return attr_join_batches.GetCount();
		case PHASE_FIX_DATA:		return 1;
		
		default: return 1;
	};
}

int AttributesProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_MAIN_GROUPS:		return 1;
		case PHASE_SIMPLIFY_ATTRS:	return 1;
		case PHASE_JOIN_ORPHANED:	return 1;
		case PHASE_FIX_DATA:		return 1;
		default: return 1;
	};
}

void AttributesProcess::DoPhase() {
	switch (phase) {
		case PHASE_MAIN_GROUPS:		MainGroups(); return;
		case PHASE_SIMPLIFY_ATTRS:	SimplifyAttrs(); return;
		case PHASE_JOIN_ORPHANED:	JoinOrphaned(); return;
		case PHASE_FIX_DATA:		FixData(); return;
		default: TODO;
	};
}

void AttributesProcess::MainGroups() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	RealizeBatch_AttrExtremesBatch();
	Vector<AttrExtremesBatch>& batches = attr_extremes_batches;
	
	if (batch >= batches.GetCount()) {
		NextPhase();
		return;
	}
	AttrExtremesBatch& batch = batches[this->batch];
	
	const Index<String>& values = uniq_attrs.Get(batch.group);
	if (values.GetCount() < 2) {
		NextPhase();
		return;
	}
	
	
	AttrArgs args;
	args.fn = 0;
	args.group = batch.group;
	args.values <<= values.GetKeys();
	
	if (args.group.IsEmpty()) {
		NextBatch();
		return;
	}
	
	tmp_str = args.group;
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetAttributes(appmode, args, [this](String res) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		
		RemoveEmptyLines2(res);
		Vector<String> parts = Split(res, "\n");
		
		if (parts.GetCount() != 2) {
			if (parts.GetCount() == 1) {
				if (parts[0].Find(" vs. ") >= 0)	parts = Split(parts[0], " vs. ");
				if (parts[0].Find("/") >= 0)		parts = Split(parts[0], "/");
				if (parts[0].Find(" vs ") >= 0)		parts = Split(parts[0], " vs ");
				if (parts[0].Find(" - ") >= 0)		parts = Split(parts[0], " - ");
				if (parts[0].Find(" and ") >= 0)	parts = Split(parts[0], " and ");
			}
		}
		if (parts.GetCount() == 2) {
			if (parts[0].Find(" vs. ") >= 0)
				parts = Split(parts[0], " vs. ");
			String& f = parts[0];
			String& l = parts[1];
			RemoveLineChar(f);
			RemoveLineChar(l);
			int a = f.Find("1.");
			if (a >= 0) {
				f = f.Mid(a+2);
			}
			f = TrimBoth(f);
			l = TrimBoth(l);
			
			Vector<String> keys;
			keys.SetCount(2);
			for(int i = 0; i < parts.GetCount(); i++) {
				String& part = parts[i];
				int a0 = part.Find(":");
				int a1 = part.Find("-");
				int a2 = part.Find("/");
				int a3 = part.Find("\n");
				int a4 = part.Find(",");
				int a5 = part.Find("(");
				int a = INT_MAX;
				if (a0 >= 0 && a0 < a) a = a0;
				if (a1 >= 0 && a1 < a) a = a1;
				if (a2 >= 0 && a2 < a) a = a2;
				if (a3 >= 0 && a3 < a) a = a3;
				if (a4 >= 0 && a4 < a) a = a4;
				if (a5 >= 0 && a5 < a) a = a5;
				if (a == INT_MAX) a = part.GetCount();
				/*if (a == INT_MAX && part.GetCount() < 100) a = part.GetCount();
				if (a == INT_MAX) {
					WString ws = part.ToWString();
					LOG(ws);
					DUMPC(ws);
				}*/
				if (a == INT_MAX)
					continue;
				String& key = keys[i];
				key = TrimBoth(part.Left(a));
			}
			
			if (keys[0].GetCount() && keys[1].GetCount()) {
				String group = ToLower(tmp_str);
				int attr_i[2] = {-1,-1};
				for(int i = 0; i < keys.GetCount(); i++) {
					AttrHeader ah;
					ah.group = group;
					ah.value = ToLower(keys[i]);
					
					da.attrs.GetAdd(ah, attr_i[i]);
				}
				
				
				ExportSimpleAttr& sat = da.simple_attrs.GetAdd(tmp_str);
				sat.attr_i0 = attr_i[0];
				sat.attr_i1 = attr_i[1];
				
				//const Index<String>& v = uniq_attrs.Get(group);
			}
		}
		
		
		NextBatch();
		SetWaiting(false);
	});
}

void AttributesProcess::SimplifyAttrs() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	int per_batch = 50;
	
	Vector<AttrPolarBatch>& batches = attr_polar_batches;
	
	if (batches.IsEmpty()) {
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			String group = uniq_attrs.GetKey(i);
			int j = da.simple_attrs.Find(group);
			if (j < 0) continue;
			const auto& gsa = da.simple_attrs[j];
			
			const Index<String>& v = uniq_attrs[i];
			AttrPolarBatch& b = batches.Add();
			b.attr0 = da.attrs.GetKey(gsa.attr_i0).value;
			b.attr1 = da.attrs.GetKey(gsa.attr_i1).value;
			ASSERT(da.attrs.GetKey(gsa.attr_i0).group == group);
			b.group = group;
			for(int j = 0; j < v.GetCount(); j++) {
				if (batches.Top().attrs.GetCount() >= per_batch) {
					AttrPolarBatch& b0 = batches.Add();
					AttrPolarBatch& b = batches[batches.GetCount()-2];
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
	
	
	if (batch >= batches.GetCount()) {
		NextPhase();
		return;
	}
	
	AttrPolarBatch& batch = batches[this->batch];
	AttrArgs args;
	args.fn = 1;
	args.group = batch.group;
	args.values <<= batch.attrs;
	args.attr0 = batch.attr0;
	args.attr1 = batch.attr1;
	
	tmp_words <<= batch.attrs;
	tmp_str = args.group;
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetAttributes(appmode, args, [this](String res) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		RemoveEmptyLines2(res);
		Vector<String> lines = Split(res, "\n");
		
		
		if (lines.GetCount() == tmp_words.GetCount()) {
			String group = tmp_str;
			int i = da.simple_attrs.Find(group);
			String pos_value, neg_value;
			if (i >= 0) {
				const ExportSimpleAttr& esa = da.simple_attrs[i];
				pos_value = da.attrs.GetKey(esa.attr_i0).value;
				neg_value = da.attrs.GetKey(esa.attr_i1).value;
			}
			for(int i = 0; i < lines.GetCount(); i++) {
				String key = tmp_words[i];
				String value = TrimBoth(ToLower(lines[i]));
				bool is_negative = value.Find("negative") >= 0;
				
				// Force the value, if the key is the extreme (and AI got it wrong somehow)
				if (key == pos_value)
					is_negative = false;
				else if (key == neg_value)
					is_negative = true;
				
				AttrHeader ah;
				ah.group = group;
				ah.value = key;
				int j = da.attrs.Find(ah);
				if (j < 0)
					continue;
				
				ExportAttr& ea = da.attrs[j];
				ea.positive = !is_negative;
				ea.simple_attr = i;
			}
			
		}
		
		NextBatch();
		SetWaiting(false);
	});
}

void AttributesProcess::JoinOrphaned() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	int per_batch = 35;
	Vector<AttrJoinBatch>& batches = attr_join_batches;
	
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
			
			AttrJoinBatch& b = batches.Top();
			b.values << (ah.group + ": " + ah.value);
		}
	}
	
	if (batch >= batches.GetCount()) {
		NextPhase();
		return;
	}
	
	AttrJoinBatch& batch = batches[this->batch];
	AttrArgs args;
	args.fn = 2;
	//args.groups <<= batch.groups;
	args.values <<= batch.values;
	int count = min(20, uniq_attrs.GetCount());
	tmp_words2.Clear();
	for(int i = 0; i < count; i++) {
		String group = uniq_attrs.GetKey(i);
		if (!group.IsEmpty()) {
			const ExportSimpleAttr& ea = da.simple_attrs.GetAdd(group);
			String a0 = da.attrs.GetKey(ea.attr_i0).value;
			String a1 = da.attrs.GetKey(ea.attr_i1).value;
			args.groups << (group + ": +(" + a0 + "), -(" + a1 + ")");
			tmp_words2 << group;
		}
	}
	
	tmp_words <<= batch.values;
	tmp_str = args.group;
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetAttributes(appmode, args, [this](String res) {
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		RemoveEmptyLines2(res);
		Vector<String> lines = Split(res, "\n");
		
		
		if (lines.GetCount() == tmp_words.GetCount()) {
			
			for(int i = 0; i < lines.GetCount(); i++) {
				String& l = lines[i];
				Vector<String> ah_parts = Split(tmp_words[i], ": ");
				if (ah_parts.GetCount() != 2)
					continue;
				AttrHeader ah;
				ah.group = ah_parts[0];
				ah.value = ah_parts[1];
				
				int attr_i = da.attrs.Find(ah);
				if (attr_i < 0)
					continue;
				ExportAttr& ea = da.attrs[attr_i];
				
				String digit, sign;
				for(int i = 0; i < l.GetCount(); i++) {
					int chr = l[i];
					if (IsDigit(chr))
						digit.Cat(chr);
					else if (chr == '+' || chr == '-') {
						sign.Cat(chr);
						break;
					}
					else if (chr == ',')
						break;
				}
				if (digit.IsEmpty() || sign.IsEmpty())
					continue;
				
				int group_i = ScanInt(digit);
				bool is_positive = sign == "+";
				
				if (group_i < 0 || group_i >= tmp_words2.GetCount())
					continue;
				String group = tmp_words2[group_i];
				
				AttrHeader link_ah;
				link_ah.group = group;
				link_ah.value = ah.value;
				int link_i = -1;
				da.attrs.GetAdd(link_ah, link_i);
				
				ea.link = link_i;
			}
			
		}
		
		NextBatch();
		SetWaiting(false);
	});
}

void AttributesProcess::FixData() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
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

AttributesProcess& AttributesProcess::Get(int appmode) {
	static ArrayMap<String, AttributesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	AttributesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void AttributesProcess::RealizeBatch_AttrExtremesBatch() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	/*if (uniq_attrs.IsEmpty())*/ {
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
	}
	
	Vector<AttrExtremesBatch>& batches = attr_extremes_batches;
	
	if (batches.IsEmpty()) {
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			String group = uniq_attrs.GetKey(i);
			int j = da.simple_attrs.Find(group);
			if (j >= 0) {
				const ExportSimpleAttr& esa = da.simple_attrs[j];
				if (esa.attr_i0 >= 0 && esa.attr_i1 >= 0)
					continue;
			}
			AttrExtremesBatch& b = batches.Add();
			b.group = group;
		}
	}
}



END_TEXTLIB_NAMESPACE

