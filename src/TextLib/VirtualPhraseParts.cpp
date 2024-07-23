#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhrasePartsProcess::VirtualPhrasePartsProcess() {
	
}

int VirtualPhrasePartsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhrasePartsProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_GET_USING_EXISTING: return DB_COUNT;
		case PHASE_GET: return GetDatabase().src_data.a.dataset.virtual_phrase_structs.GetCount() / per_action_task;
		default: TODO; return 1;
	}
}

int VirtualPhrasePartsProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_GET_USING_EXISTING: return 1;
		case PHASE_GET: return 1;
		default: TODO; return 1;
	}
}

void VirtualPhrasePartsProcess::DoPhase() {
	switch (phase) {
		case PHASE_GET_USING_EXISTING: GetUsingExisting(); return;
		case PHASE_GET: Get(); return;
		default: TODO; return;
	}
}

VirtualPhrasePartsProcess& VirtualPhrasePartsProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhrasePartsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhrasePartsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void VirtualPhrasePartsProcess::GetUsingExisting() {
	TextDatabase& db0 = GetDatabase();
	SourceData& sd0 = db0.src_data;
	SourceDataAnalysis& sda0 = db0.src_data.a;
	DatasetAnalysis& da0 = sda0.dataset;
	
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
	
	for(int j = 0; j < da0.virtual_phrase_structs.GetCount(); j++) {
		// If virtual phrase part has no known type
		VirtualPhraseStruct& vps0 = da0.virtual_phrase_structs[j];
		if (vps0.struct_type < 0) {
			if (vps0.virtual_phrase_parts.IsEmpty())
				continue;
			
			// Calculate da1 hash (since key hash is local to da0)
			CombineHash vps1_ch;
			bool hash_fail = false;
			for (int vpp_i0 : vps0.virtual_phrase_parts) {
				// Calculate sub-hash for VirtualPhrasePart
				const VirtualPhrasePart& vpp0 = da0.virtual_phrase_parts[vpp_i0];
				CombineHash vpp1_ch;
				for (int wc0: vpp0.word_classes) {
					const String& common_key = da0.word_classes[wc0];
					int wc_i1 = da1.word_classes.Find(common_key);
					if (wc_i1 < 0) {
						hash_fail = true; // cannot make common hash (it's not in the list anyway then)
						break;
					}
					vpp1_ch.Do(wc_i1).Put(1);
				}
				if (hash_fail)
					break;
				hash_t vpp1_h = vpp1_ch;
				
				// Use hash to find the index
				int vpp_i1 = da1.virtual_phrase_parts.Find(vpp1_h);
				if (vpp_i1 < 0) {
					hash_fail = true;
					break;
				}
				
				// Collect VirtualPhrasePart indices to the hash
				vps1_ch.Do(vpp_i1).Put(1);
			}
			if (hash_fail)
				continue;
			hash_t vps1_h = vps1_ch;
			
			// Then find it in other
			int k = da1.virtual_phrase_structs.Find(vps1_h);
			if (k < 0)
				continue;
			
			// ...if the other has type
			const VirtualPhraseStruct& vps1 = da1.virtual_phrase_structs[k];
			if (vps1.struct_type < 0)
				continue;
			
			// Copy type to avoid useless AI usage
			const String& struct_type = da1.struct_types[vps1.struct_type];
			vps0.struct_type = da0.struct_types.FindAdd(struct_type);
		}
	}
	
	NextBatch();
}

void VirtualPhrasePartsProcess::Get() {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	args.fn = 3;
	args.words.Clear();
	
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	end = min(end, da.virtual_phrase_structs.GetCount());
	
	int iter = 0;
	tmp_ptrs.Clear();
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
			tmp_ptrs << (void*)&vps;
		}
		else if (iter >= end)
			break;
		
		iter++;
	}
	
	if (args.words.IsEmpty()) {
		NextPhase();
		return;
	}
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetTokenData(appmode, args, [this](String res) {
		TokenArgs& args = token_args;
		TextDatabase& db = GetDatabase();
		SourceData& sd = db.src_data;
		SourceDataAnalysis& sda = db.src_data.a;
		DatasetAnalysis& da = sda.dataset;
		
		// 61. compound-complex sentence + complex sentence: compound-complex sentence
		
		int offset = 3+1;
		RemoveEmptyLines(res);
		Vector<String> lines = Split(res, "\n");
		bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
		
		for (String& line : lines) {
			line = TrimBoth(line);
			
			if (line.IsEmpty() ||!IsDigit(line[0]))
				continue;
			
			int a = line.Find(".");
			if (a < 0) continue;
			
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp_ptrs.GetCount())
				continue;
			
			VirtualPhraseStruct& vps = *(VirtualPhraseStruct*)tmp_ptrs[line_i];
			
			line = TrimBoth(line.Mid(a+1));
			
			a = line.ReverseFind(":");
			if (a < 0)
				continue;
			
			Vector<String> classes = Split(TrimBoth(line.Left(a)), "+", false);
			//sp_is.SetCount(0);
			bool fail = false;
			CombineHash ch;
			for (String& c : classes) {
				c = TrimBoth(c);
				if (c.IsEmpty()) {
					fail = true;
					break;
				}
				int sp_i = da.struct_part_types.Find(c);
				if (sp_i < 0) {
					fail = true;
					break;
				}
				//sp_is << sp_i;
				ch.Do(sp_i).Put(1);
			}
			if (fail)
				continue;
			
			String struct_type = TrimBoth(line.Mid(a+1));
			
			vps.struct_type = da.struct_types.FindAdd(struct_type);
		}
		
		int a = 0;
		for (const VirtualPhraseStruct& vps : da.virtual_phrase_structs.GetValues())
			if (vps.struct_type >= 0)
				a++;
		da.diagnostics.GetAdd("virtual phrase structs: total") = IntStr(da.virtual_phrase_structs.GetCount());
		da.diagnostics.GetAdd("virtual phrase structs: actual") =  IntStr(a);
		da.diagnostics.GetAdd("virtual phrase structs: percentage") =  DblStr((double)a / (double) da.virtual_phrase_structs.GetCount() * 100);
		
		NextBatch();
		SetWaiting(false);
	});
}



END_TEXTLIB_NAMESPACE

