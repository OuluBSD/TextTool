#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptPostFixCtrl::ScriptPostFixCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << vsplit;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << generations << src_lines;
	menusplit.SetPos(2000);
	
	src_lines.AddColumn("#");
	src_lines.AddColumn("Text");
	src_lines.ColumnWidths("1 6");
	
	vsplit.Vert() << weaksplit << imprsplit << varsplit;
	
	weaksplit.Horz() << weak_types << weaks << weak_text;
	weaksplit.SetPos(2500, 0);
	weaksplit.SetPos(7500, 1);
	
	imprsplit.Horz() << impr_types << imprs;
	imprsplit.SetPos(2500);
	
	varsplit.Horz() << vars << var_content;
	varsplit.SetPos(2500);
	
	generations.AddColumn(t_("Generation"));
	generations.AddColumn(t_("Best score-av"));
	generations.AddIndex("IDX");
	generations.WhenCursor << THISBACK(DataGeneration);
	
	weak_types.AddColumn(t_("Weak type"));
	weak_types.AddColumn(t_("Count"));
	weak_types.AddIndex("IDX");
	weak_types.ColumnWidths("5 1");
	weak_types.WhenCursor << THISBACK(DataWeak);
	
	weaks.AddColumn(t_("Line"));
	weaks.AddColumn(t_("Alt line"));
	weaks.AddColumn(t_("Txt"));
	weaks.AddColumn(t_("Alt Txt"));
	weaks.AddIndex("IDX");
	weaks.ColumnWidths("1 1 8 4");
	weaks.WhenCursor << THISBACK(DataWeakItem);
	
	impr_types.AddColumn(t_("Improvement type"));
	impr_types.AddColumn(t_("Count"));
	impr_types.AddIndex("IDX");
	impr_types.ColumnWidths("5 1");
	impr_types.WhenCursor << THISBACK(DataImprovement);
	
	imprs.AddColumn(t_("Line"));
	imprs.AddColumn(t_("Old line"));
	imprs.AddColumn(t_("New line"));
	imprs.AddIndex("IDX");
	imprs.ColumnWidths("1 4 4");
	
	vars.AddColumn(t_("Variation #"));
	vars.AddColumn(t_("Like"));
	vars.AddColumn(t_("Comment"));
	vars.AddColumn(t_("Listen"));
	vars.AddColumn(t_("Share"));
	vars.AddColumn(t_("Bookmark"));
	vars.AddColumn(t_("Comedy"));
	vars.AddColumn(t_("Sex"));
	vars.AddColumn(t_("Politics"));
	vars.AddColumn(t_("Love"));
	vars.AddColumn(t_("Social"));
	vars.AddColumn(t_("Total"));
	vars.AddIndex("IDX");
	vars.WhenCursor << THISBACK(DataVariation);
	
}

void ScriptPostFixCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start process"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Start process (don't skip ready)"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F5);
	
}

void ScriptPostFixCtrl::Data() {
	EditorPtrs& p = GetPointers();
	if (!p.script) return;
	
	for(int i = 0; i < p.script->postfixes.GetCount(); i++) {
		ScriptPostFix& spf = p.script->postfixes[i];
		generations.Set(i, 0, IntStr(i));
		if (spf.variations.GetCount())
			generations.Set(i, 1, spf.variations[0].ScoreAv());
		else
			generations.Set(i, 1, Value());
		generations.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(generations);
	generations.SetCount(p.script->postfixes.GetCount());
	if (!generations.IsCursor() && generations.GetCount())
		generations.SetCursor(generations.GetCount()-1);
	
	
	DataGeneration();
}

void ScriptPostFixCtrl::DataGeneration() {
	EditorPtrs& p = GetPointers();
	if (!p.script || !generations.IsCursor()) {
		weak_types.Clear();
		weaks.Clear();
		impr_types.Clear();
		imprs.Clear();
		vars.Clear();
		weak_text.Clear();
		return;
	}
	
	int gen_i = generations.Get("IDX");
	ScriptPostFix& spf = p.script->postfixes[gen_i];
	
	
	for(int i = 0; i < spf.src_lines.GetCount(); i++) {
		src_lines.Set(i, 0, i);
		src_lines.Set(i, 1, spf.src_lines[i]);
	}
	src_lines.SetCount(spf.src_lines.GetCount());
	
	// Weaks
	for(int i = 0; i < spf.weaks.GetCount(); i++) {
		Vector<ScriptPostFix::Weak>& w = spf.weaks[i];
		//weak_types.Set(i, 0, GetPostScriptAnalysisKey(i));
		weak_types.Set(i, 0, GetScoreTitle(i));
		weak_types.Set(i, 1, w.GetCount());
		weak_types.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR_(weak_types, a);
	weak_types.SetCount(spf.weaks.GetCount());
	if (!weak_types.IsCursor() && weak_types.GetCount())
		weak_types.SetCursor(0);
	
	
	// Improvements
	for(int i = 0; i < spf.improvements.GetCount(); i++) {
		Vector<ScriptPostFix::Improvement>& w = spf.improvements[i];
		//impr_types.Set(i, 0, GetPostScriptModificationKey(i));
		impr_types.Set(i, 0, GetScoreTitle(i));
		impr_types.Set(i, 1, w.GetCount());
		impr_types.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR_(impr_types, b);
	impr_types.SetCount(spf.improvements.GetCount());
	if (!impr_types.IsCursor() && impr_types.GetCount())
		impr_types.SetCursor(0);
	
	
	
	// Variations
	for(int i = 0; i < spf.variations.GetCount(); i++) {
		ScriptPostFix::Variation& w = spf.variations[i];
		vars.Set(i, 0, i);
		vars.Set(i, "IDX", i);
		int total = 0;
		for(int j = 0; j < 10; j++) {
			int s = j < w.scores.GetCount() ? w.scores[j] : 0;
			vars.Set(i, 1+j, s);
			total += s;
		}
		vars.Set(i, 11, total / 10.0);
	}
	INHIBIT_CURSOR_(vars, c);
	vars.SetCount(spf.variations.GetCount());
	if (!vars.IsCursor() && vars.GetCount())
		vars.SetCursor(0);
	
	
	
	DataWeak();
	DataImprovement();
	DataVariation();
}

void ScriptPostFixCtrl::DataWeak() {
	EditorPtrs& p = GetPointers();
	if (!p.script || !generations.IsCursor() || !weak_types.IsCursor()) {
		weaks.Clear();
		weak_text.Clear();
		return;
	}
	
	int gen_i = generations.Get("IDX");
	int weak_i = weak_types.Get("IDX");
	ScriptPostFix& spf = p.script->postfixes[gen_i];
	Vector<ScriptPostFix::Weak>& v = spf.weaks[weak_i];
	
	for(int i = 0; i < v.GetCount(); i++) {
		ScriptPostFix::Weak& w = v[i];
		weaks.Set(i, 0, w.line_i);
		weaks.Set(i, 1, w.altline_i >= 0 ? Value(w.altline_i) : Value());
		
		if (w.line_i >= 0 && w.line_i < spf.src_lines.GetCount())
			weaks.Set(i, 2, spf.src_lines[w.line_i]);
		else
			weaks.Set(i, 2, Value());
		
		if (w.altline_i >= 0 && w.altline_i < spf.src_lines.GetCount())
			weaks.Set(i, 3, spf.src_lines[w.altline_i]);
		else
			weaks.Set(i, 3, Value());
		
		weaks.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(weaks);
	weaks.SetCount(v.GetCount());
	if (!weaks.IsCursor() && weaks.GetCount())
		weaks.SetCursor(0);
	
	DataWeakItem();
}

void ScriptPostFixCtrl::DataWeakItem() {
	EditorPtrs& p = GetPointers();
	if (!p.script || !generations.IsCursor() || !weak_types.IsCursor() || !weaks.IsCursor()) {
		weaks.Clear();
		weak_text.Clear();
		return;
	}
	
	int gen_i = generations.Get("IDX");
	int weak_i = weak_types.Get("IDX");
	int wi_i = weaks.Get("IDX");
	ScriptPostFix& spf = p.script->postfixes[gen_i];
	Vector<ScriptPostFix::Weak>& v = spf.weaks[weak_i];
	ScriptPostFix::Weak& w = v[wi_i];
	
	weak_text.SetData(w.description);
	
}

void ScriptPostFixCtrl::DataImprovement() {
	EditorPtrs& p = GetPointers();
	if (!p.script || !generations.IsCursor() || !impr_types.IsCursor()) {
		imprs.Clear();
		return;
	}
	
	int gen_i = generations.Get("IDX");
	int impr_i = impr_types.Get("IDX");
	ScriptPostFix& spf = p.script->postfixes[gen_i];
	Vector<ScriptPostFix::Improvement>& v = spf.improvements[impr_i];
	
	for(int i = 0; i < v.GetCount(); i++) {
		ScriptPostFix::Improvement& w = v[i];
		imprs.Set(i, 0, w.line_i);
		if (w.line_i < spf.src_lines.GetCount())
			imprs.Set(i, 1, spf.src_lines[w.line_i]);
		else
			imprs.Set(i, 1, "<error>");
		imprs.Set(i, 2, w.text);
		imprs.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(imprs);
	imprs.SetCount(v.GetCount());
	if (!imprs.IsCursor() && imprs.GetCount())
		imprs.SetCursor(0);
	
}

void ScriptPostFixCtrl::DataVariation() {
	EditorPtrs& p = GetPointers();
	if (!p.script || !generations.IsCursor() || !vars.IsCursor()) {
		var_content.Clear();
		return;
	}
	
	int gen_i = generations.Get("IDX");
	int var_i = vars.Get("IDX");
	ScriptPostFix& spf = p.script->postfixes[gen_i];
	ScriptPostFix::Variation& v = spf.variations[var_i];
	
	var_content.SetData(v.text);
}

void ScriptPostFixCtrl::Do(int fn) {
	EditorPtrs& p = GetPointers();
	if (!p.entity || !p.entity->profile || !p.script) return;
	
	if (fn == 0 || fn == 1) {
		ScriptPostSolver& tm = ScriptPostSolver::Get(*p.entity->profile, *p.script, GetAppMode());
		tm.SetSkipReady(fn == 1);
		tm.Start();
	}
}


END_TEXTLIB_NAMESPACE

