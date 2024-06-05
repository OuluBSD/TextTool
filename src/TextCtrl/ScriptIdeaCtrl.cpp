#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptIdeaCtrl::ScriptIdeaCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << ideasplit;
	
	ideasplit.Horz() << typeclasses << contents << ideas << idea;
	ideasplit.SetPos(1250, 0);
	ideasplit.SetPos(2500, 1);
	
	typeclasses.AddColumn("Typeclass");
	typeclasses.AddColumn("Best score");
	typeclasses.AddIndex("IDX");
	typeclasses.WhenCursor << THISBACK(DataTypeclass);
	typeclasses.ColumnWidths("4 1");
	
	contents.AddColumn("Content");
	contents.AddColumn("Best score");
	contents.AddIndex("IDX");
	contents.WhenCursor << THISBACK(DataContent);
	contents.ColumnWidths("4 1");
	
	ideas.AddColumn("Idea");
	for(int i = 0; i < SCORE_COUNT; i++)
		ideas.AddColumn(GetScoreTitle(i));
	ideas.AddColumn("Av-score");
	ideas.AddIndex("IDX");
	String cw = "4";
	cw.Cat(" 1", SCORE_COUNT+1);
	ideas.ColumnWidths(cw);
	ideas.WhenCursor << THISBACK(DataIdea);
	
}

void ScriptIdeaCtrl::Data() {
	Script& s = GetScript();
	Entity& e = GetEntity();
	
	{
		const auto& tcs = GetTypeclasses();
		for(int i = 0; i < tcs.GetCount(); i++) {
			typeclasses.Set(i, 0, tcs[i]);
			typeclasses.Set(i, 1, e.FindBestScore(i));
			typeclasses.Set(i, "IDX", i);
		}
		typeclasses.SetCount(tcs.GetCount());
		INHIBIT_CURSOR(typeclasses);
		typeclasses.SetSortColumn(1, true);
		int cur = 0;
		for(int i = 0; i < typeclasses.GetCount(); i++) {
			if (typeclasses.Get(i, "IDX") == s.typeclass) {
				cur = i;
				break;
			}
		}
		if (!typeclasses.IsCursor() && cur < typeclasses.GetCount())
			typeclasses.SetCursor(cur);
	}
	
	DataTypeclass();
}

void ScriptIdeaCtrl::DataTypeclass() {
	Script& s = GetScript();
	Entity& e = GetEntity();
	
	if (!typeclasses.IsCursor()) {
		contents.Clear();
		ideas.Clear();
		idea.Clear();
		return;
	}
	int tc_i = typeclasses.Get("IDX");
	
	{
		const auto& cons = GetContents();
		for(int i = 0; i < cons.GetCount(); i++) {
			contents.Set(i, 0, cons[i].key);
			contents.Set(i, 1, e.FindBestScore(tc_i, i));
			contents.Set(i, "IDX", i);
		}
		contents.SetCount(cons.GetCount());
		INHIBIT_CURSOR(contents);
		contents.SetSortColumn(1, true);
		int cur = 0;
		for(int i = 0; i < contents.GetCount(); i++) {
			if (contents.Get(i, "IDX") == s.content) {
				cur = i;
				break;
			}
		}
		if (!contents.IsCursor() && cur < contents.GetCount())
			contents.SetCursor(cur);
	}
	
	DataContent();
}

void ScriptIdeaCtrl::DataContent() {
	Script& s = GetScript();
	Entity& e = GetEntity();
	
	if (!typeclasses.IsCursor() || !contents.IsCursor()) {
		ideas.Clear();
		idea.Clear();
		return;
	}
	
	int tc_i = typeclasses.Get("IDX");
	int con_i = contents.Get("IDX");
	{
		auto cvis = e.FindIdeaIndices(tc_i, con_i);
		for(int i = 0; i < cvis.GetCount(); i++) {
			int cvi_i = cvis[i];
			const ContentVisionIdea& cvi = e.ideas[cvi_i];
			ideas.Set(i, 0, cvi.text);
			ideas.Set(i, "IDX", cvi_i);
			int score_sum = 0;
			for(int j = 0; j < SCORE_COUNT; j++) {
				int score = j < cvi.scores.GetCount() ? cvi.scores[j] : 0;
				ideas.Set(i, 1+j, score);
				score_sum += score;
			}
			ideas.Set(i, 1 + SCORE_COUNT, score_sum / (double)SCORE_COUNT);
		}
		INHIBIT_CURSOR(ideas);
		ideas.SetCount(cvis.GetCount());
		ideas.SetSortColumn(1+SCORE_COUNT, true);
		if (!ideas.IsCursor() && ideas.GetCount())
			ideas.SetCursor(0);
	}
	
	DataIdea();
}

void ScriptIdeaCtrl::DataIdea() {
	if (!ideas.IsCursor()) {
		idea.Clear();
		return;
	}
	
	Entity& e = GetEntity();
	Script& s = GetScript();
	int idea_i = ideas.GetCursor();
	idea.SetData(e.ideas[idea_i].text);
}

void ScriptIdeaCtrl::Clear() {
	
}

void ScriptIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Start Single"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Set as typeclass/content"), AppImg::BlueRing(), THISBACK1(Do, 2)).Key(K_CTRL_Q);
	bar.Add(t_("Set as content vision"), AppImg::BlueRing(), THISBACK1(Do, 3)).Key(K_CTRL_W);
	bar.Add(t_("Seek song's pre-set typeclass/content"), AppImg::BlueRing(), THISBACK1(Do, 4)).Key(K_CTRL_E);
}

void ScriptIdeaCtrl::Do(int fn) {
	EditorPtrs& p = GetPointers();
	if (!p.entity || !p.entity->profile || !p.script) return;
	
	if (fn == 0 || fn == 1) {
		ScriptIdeaSolver& tm = ScriptIdeaSolver::Get(*p.entity, GetAppMode());
		tm.WhenReady << [this]() {PostCallback(THISBACK(Data));};
		tm.only_single = fn == 1;
		tm.only_tc = p.script->typeclass;
		tm.only_con = p.script->content;
		tm.Start();
	}
	else if (fn == 2) {
		SetTypeclassContent();
	}
	else if (fn == 3) {
		SetTypeclassContent();
		String s = idea.GetData();
		int a = s.Find("\":");
		if (s.Left(1) == "\"" && a >= 0) {
			String title = s.Mid(1,a-1);
			String desc = TrimBoth(s.Mid(a+2));
			if (p.script->english_title.IsEmpty())
				p.script->english_title = title;
			p.script->content_vision = desc;
		}
		else {
			p.script->content_vision = s;
		}
	}
	else if (fn == 4) {
		for(int i = 0; i < typeclasses.GetCount(); i++) {
			if (typeclasses.Get(i, "IDX") == p.script->typeclass) {
				typeclasses.SetCursor(i);
				break;
			}
		}
		for(int i = 0; i < contents.GetCount(); i++) {
			if (contents.Get(i, "IDX") == p.script->content) {
				contents.SetCursor(i);
				break;
			}
		}
	}
}

void ScriptIdeaCtrl::SetTypeclassContent() {
	EditorPtrs& p = GetPointers();
	if (!p.script || typeclasses.IsCursor() || contents.IsCursor()) return;
	Script& l = *p.script;
	l.typeclass = typeclasses.GetCursor();
	l.content = contents.GetCursor();
}


END_TEXTLIB_NAMESPACE

