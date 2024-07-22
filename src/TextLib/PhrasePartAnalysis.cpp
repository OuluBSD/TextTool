#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



PhrasePartAnalysisProcess::PhrasePartAnalysisProcess() {
	
}

int PhrasePartAnalysisProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int PhrasePartAnalysisProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int PhrasePartAnalysisProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void PhrasePartAnalysisProcess::DoPhase() {
	ASSERT(phase >= 0 && phase < 6);
	Do(phase);
}

PhrasePartAnalysisProcess& PhrasePartAnalysisProcess::Get(int appmode) {
	static ArrayMap<String, PhrasePartAnalysisProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	PhrasePartAnalysisProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}

void PhrasePartAnalysisProcess::Do(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	PhraseArgs& args = phrase_args;
	args.fn = fn;
	args.phrases.Clear();
	
	int per_action_task = 50;
	if (args.fn == 5)
		per_action_task = 20;
	else if (args.fn >= 2)
		per_action_task = 35;
	
	int begin = batch * per_action_task;
	int end = begin + per_action_task;
	
	Color no_clr(0,0,0);
	tmp_ptrs.SetCount(0);
	tmp.SetCount(0);
	
	if (batch == 0) {
		tmp_iters.SetCount(0);
		int trimmed_by[6] = {0,0,0,0,0,0};
		
		int iter = 0;
		int idx = -1;
		for (const PhrasePart& pp : da.phrase_parts.GetValues()) {
			idx++;
			
			if ((fn == 0 && pp.clr != no_clr) || (fn > 0 && pp.clr == no_clr)) {
				trimmed_by[0]++;
				continue;
			}
			
			if ((fn == 1 && pp.attr >= 0) || (fn > 1 && pp.attr < 0)){
				trimmed_by[1]++;
				continue;
			}
			
			if ((fn == 2 && !pp.actions.IsEmpty()) || (fn > 2 && pp.actions.IsEmpty())){
				trimmed_by[2]++;
				continue;
			}
			
			if ((fn == 3 && pp.HasScores()) || (fn > 3 && !pp.HasScores())){
				trimmed_by[3]++;
				continue;
			}
			
			if ((fn == 4 && !pp.typecasts.IsEmpty()) || (fn > 4 && pp.typecasts.IsEmpty())){
				trimmed_by[4]++;
				continue;
			}
			
			if ((fn == 5 && !pp.contrasts.IsEmpty()) || (fn > 5 && pp.contrasts.IsEmpty())){
				trimmed_by[5]++;
				continue;
			}
			
			tmp_iters << idx;
			iter++;
		}
	}
	
	for(int i = begin; i < end && i < tmp_iters.GetCount(); i++) {
		int idx = tmp_iters[i];
		const PhrasePart& pp = da.phrase_parts[idx];
		String phrase = da.GetWordString(pp.words);
		args.phrases << phrase;
		tmp_ptrs << (void*)&pp;
		tmp << idx;
	}
	
	if (args.phrases.IsEmpty()) {
		NextPhase();
		return;
	}
	
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	if (args.fn == 0)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseColors));
	else if (args.fn == 1)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseAttrs));
	else if (args.fn == 2)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseActions));
	else if (args.fn == 3)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseScores));
	else if (args.fn == 4)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseTypeclasses));
	else if (args.fn == 5)
		m.GetPhraseData(appmode, args, THISBACK(OnPhraseContrast));
	else
		TODO;
}

void PhrasePartAnalysisProcess::OnPhraseColors(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 12. RGB(255, 102, 0)
	
	Color black(0,0,0);
	Color non_black(1,1,1);
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			PhrasePart& pp = da.phrase_parts[pp_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		
		a = line.Find("RGB(", a+1);
		if (a < 0) continue;
		a += 4;
		int b = line.Find(")");
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		if (clr == black)
			clr = non_black;
		
		pp.clr = clr;
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.clr != black)
			a++;
	da.diagnostics.GetAdd("phrase part color: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part color: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part color: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	NextBatch();
	SetWaiting(false);
}

void PhrasePartAnalysisProcess::OnPhraseAttrs(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 1. Belief communities: acceptance
	
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		
		// This shouldn't happen
		if (pp.attr >= 0)
			continue;
		
		line = TrimBoth(line.Mid(a+1));
		a = line.Find(":");
		if (a < 0) continue;
		
		AttrHeader ah;
		ah.group = ToLower(TrimBoth(line.Left(a)));
		ah.value = ToLower(TrimBoth(line.Mid(a+1)));
		da.attrs.GetAdd(ah, pp.attr);
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.attr >= 0)
			a++;
	da.diagnostics.GetAdd("phrase part attrs: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part attrs: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part attrs: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	NextBatch();
	SetWaiting(false);
}

void PhrasePartAnalysisProcess::OnPhraseActions(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 5. tone(admiring) + msg(expressing attraction) + bias(physical appearance) + attention-attribute(referencing arms) + attention-physical_state(strength)
	
	res = "4. " + res;
	
	Vector<int> actions;
	int offset = 3+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at '+' character and parse single actions
		Vector<String> parts = Split(line, "+");
		CombineHash ch;
		actions.SetCount(0);
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			s = TrimBoth(s);
			int a = s.Find("(");
			int b = s.Find(")");
			if (a < 0 || b < 0 || a > b) {
				parts.Remove(i--);
				continue;
			}
			ActionHeader aa;
			aa.action = TrimBoth(s.Left(a));
			a++;
			aa.arg = TrimBoth(s.Mid(a,b-a));
			
			if (aa.action.GetCount() >= 2 && aa.action.Left(1) == "\"" && aa.action.Right(1) == "\"")
				aa.action = aa.action.Mid(1, aa.action.GetCount()-2);
			if (aa.arg.GetCount() >= 2 && aa.arg.Left(1) == "\"" && aa.arg.Right(1) == "\"")
				aa.arg = aa.arg.Mid(1, aa.arg.GetCount()-2);
			
			da.actions.GetAdd(aa, actions.Add());
		}
		Sort(actions, StdLess<int>());
		pp.actions <<= actions;
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (!pp.actions.IsEmpty())
			a++;
	da.diagnostics.GetAdd("phrase part actions: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part actions: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part actions: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	NextBatch();
	SetWaiting(false);
}

void PhrasePartAnalysisProcess::OnPhraseScores(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 8. 4 5 7 9 6 7 9 8 6 3
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		// Expect x values
		if (parts.GetCount() != SCORE_COUNT)
			continue;
		
		int i = 0;
		for (const String& part : parts)
			pp.scores[i++] = ScanInt(part);
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.HasScores())
			a++;
	da.diagnostics.GetAdd("phrase part scores: total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd("phrase part scores: actual") = IntStr(a);
	da.diagnostics.GetAdd("phrase part scores: percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	
	
	NextBatch();
	SetWaiting(false);
}

void PhrasePartAnalysisProcess::OnPhraseTypeclasses(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2." + res;
	
	int opt_count = GetTypeclassCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.typecasts.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
			#if 0
				pp.typecasts.Clear();
				break;
			#else
				continue;
			#endif
			}
			opt--; // convert to 0-based index
			pp.typecasts.Add(opt);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.typecasts.GetCount())
			a++;
		
	EnterAppMode(appmode);
	da.diagnostics.GetAdd(__typeclasses + ": total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd(__typeclasses + ": actual") = IntStr(a);
	da.diagnostics.GetAdd(__typeclasses + ": percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	LeaveAppMode();
	
	NextBatch();
	SetWaiting(false);
}

void PhrasePartAnalysisProcess::OnPhraseContrast(String res) {
	TokenArgs& args = token_args;
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	// 14. 2 5 9 11 14 19 22 28 34 44
	
	res = "2. " + res;
	
	int opt_count = GetContentCount();
	
	Vector<int> actions;
	int offset = 1+1;
	RemoveEmptyLines(res);
	Vector<String> lines = Split(res, "\n");
	bool line_match = tmp_ptrs.GetCount() == lines.GetCount();
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		if (l.Find("(") >= 0)
			lines.Remove(i--);
	}
	
	int line_i = -1;
	for (String& line : lines) {
		line_i++;
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		PhrasePart* pp_p;
		if (line_match)
			pp_p = (PhrasePart*)tmp_ptrs[line_i];
		else {
			int line_i = ScanInt(line.Left(a));
			line_i -= offset;
			if (line_i < 0 || line_i >= tmp.GetCount())
				continue;
			int pp_i = tmp[line_i];
			pp_p = &da.phrase_parts[pp_i];
		}
		PhrasePart& pp = *pp_p;
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at space character
		Vector<String> parts = Split(line, " ");
		
		
		if (parts.IsEmpty())
			continue;
		
		pp.contrasts.Clear();
		int i = 0;
		for (const String& part : parts) {
			int opt = ScanInt(part);
			if (opt <= 0 || opt > opt_count) {
				//pp.contrasts.Clear();
				//break;
				continue;
			}
			int mod = -1;
			if      (part.Find("A") >= 0 || part.Find("a") >= 0) mod = 0;
			else if (part.Find("B") >= 0 || part.Find("b") >= 0) mod = 1;
			else if (part.Find("C") >= 0 || part.Find("c") >= 0) mod = 2;
			else continue;
			opt--; // convert to 0-based index
			int code = opt * ContentType::PART_COUNT + mod;
			pp.contrasts.Add(code);
		}
	}
	
	
	int a = 0;
	for (const PhrasePart& pp : da.phrase_parts.GetValues())
		if (pp.contrasts.GetCount())
			a++;
	EnterAppMode(appmode);
	da.diagnostics.GetAdd(__content + ": total") = IntStr(da.phrase_parts.GetCount());
	da.diagnostics.GetAdd(__content + ": actual") = IntStr(a);
	da.diagnostics.GetAdd(__content + ": percentage") =  DblStr((double)a / (double)da.phrase_parts.GetCount() * 100);
	LeaveAppMode();
	
	NextBatch();
	SetWaiting(false);
}



END_TEXTLIB_NAMESPACE

