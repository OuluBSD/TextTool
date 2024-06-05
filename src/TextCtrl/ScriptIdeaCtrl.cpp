#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptIdeaCtrl::ScriptIdeaCtrl() {
	Add(src.HSizePos().TopPos(0,30));
	Add(ideasplit.HSizePos().VSizePos(30,0));
	
	src.Add("Entity");
	src.Add("Script");
	src.SetIndex(0);
	src.WhenAction << THISBACK(Data);
	
	ideasplit.Horz() << typeclasses << contents << ideas << singersplit;
	ideasplit.SetPos(1250, 0);
	ideasplit.SetPos(2500, 1);
	
	typeclasses.AddColumn("Typeclass");
	typeclasses.AddColumn("Best score");
	typeclasses.AddIndex("IDX");
	typeclasses.WhenCursor << THISBACK1(DataTypeclass, true);
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
	
	singersplit.Vert() << idea[0] << idea[1] << idea[2] << lead;
	
	lead.WhenAction << [this]() {
		EditorPtrs& p = GetPointers();
		if (!p.script) return;
		p.script->lead = lead.GetData();
	};
}

ContentVisionOwner& ScriptIdeaCtrl::GetCVO() {
	Script& s = GetScript();
	Entity& e = GetEntity();
	ContentVisionOwner& cvo0 = e;
	ContentVisionOwner& cvo1 = s;
	return src.GetIndex() == 0 ? cvo0 : cvo1;
}

void ScriptIdeaCtrl::Data() {
	Script& s = GetScript();
	Entity& e = GetEntity();
	ContentVisionOwner& cvo = GetCVO();
	
	lead.SetData(s.lead);
	
	{
		const auto& tcs = GetTypeclasses();
		for(int i = 0; i < tcs.GetCount(); i++) {
			typeclasses.Set(i, 0, tcs[i]);
			typeclasses.Set(i, 1, cvo.FindBestScore(i));
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
	
	DataTypeclass(false);
}

void ScriptIdeaCtrl::DataTypeclass(bool keep_content_idx) {
	Script& s = GetScript();
	Entity& e = GetEntity();
	ContentVisionOwner& cvo = GetCVO();
	
	if (!typeclasses.IsCursor()) {
		contents.Clear();
		ideas.Clear();
		idea[0].Clear();
		idea[1].Clear();
		idea[2].Clear();
		return;
	}
	int tc_i = typeclasses.Get("IDX");
	int con_i = 0;
	if (keep_content_idx)
		con_i = contents.IsCursor() ? (int)contents.Get("IDX") : 0;
	else
		con_i = s.content;
	
	{
		const auto& cons = GetContents();
		for(int i = 0; i < cons.GetCount(); i++) {
			contents.Set(i, 0, cons[i].key);
			contents.Set(i, 1, cvo.FindBestScore(tc_i, i));
			contents.Set(i, "IDX", i);
		}
		contents.SetCount(cons.GetCount());
		INHIBIT_CURSOR(contents);
		contents.SetSortColumn(1, true);
		int cur = 0;
		for(int i = 0; i < contents.GetCount(); i++) {
			if (contents.Get(i, "IDX") == con_i) {
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
	ContentVisionOwner& cvo = GetCVO();
	
	if (!typeclasses.IsCursor() || !contents.IsCursor()) {
		ideas.Clear();
		idea[0].Clear();
		idea[1].Clear();
		idea[2].Clear();
		return;
	}
	
	int tc_i = typeclasses.Get("IDX");
	int con_i = contents.Get("IDX");
	
	auto cvis = cvo.FindIdeaIndices(tc_i, con_i);
	for(int i = 0; i < cvis.GetCount(); i++) {
		int cvi_i = cvis[i];
		const ContentVisionIdea& cvi = cvo.ideas[cvi_i];
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
	
	DataIdea();
}

void ScriptIdeaCtrl::DataIdea() {
	if (!ideas.IsCursor()) {
		idea[0].Clear();
		idea[1].Clear();
		idea[2].Clear();
		return;
	}
	
	ContentVisionOwner& cvo = GetCVO();
	int cvi_i = ideas.Get("IDX");
	idea[0].SetData(cvo.ideas[cvi_i].text);
	idea[1].SetData(cvo.ideas[cvi_i].singers_2);
	idea[2].SetData(cvo.ideas[cvi_i].singers_3);
}

void ScriptIdeaCtrl::Clear() {
	
}

void ScriptIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Start Single"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	bar.Add(t_("Start script lead solver"), AppImg::RedRing(), THISBACK1(Do, 5)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Set as typeclass/content"), AppImg::BlueRing(), THISBACK1(Do, 2)).Key(K_CTRL_Q);
	bar.Add(t_("Set as content vision"), AppImg::BlueRing(), THISBACK1(Do, 3)).Key(K_CTRL_W);
	bar.Add(t_("Seek song's pre-set typeclass/content"), AppImg::BlueRing(), THISBACK1(Do, 4)).Key(K_CTRL_E);
}

void ScriptIdeaCtrl::Do(int fn) {
	EditorPtrs& p = GetPointers();
	if (!p.entity || !p.entity->profile || !p.script) return;
	
	if (fn == 0 || fn == 1 || fn == 5) {
		Script* s = fn == 5 ? p.script : 0;
		ScriptIdeaSolver& tm = ScriptIdeaSolver::Get(*p.entity, s, GetAppMode());
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
		int idx = 0;
		if (idea[1].HasFocus()) idx = 1;
		if (idea[2].HasFocus()) idx = 2;
		String s = idea[idx].GetData();
		int a = s.Find("\":");
		if (s.Left(1) == "\"" && a >= 0) {
			String title = s.Mid(1,a-1);
			String desc = TrimBoth(s.Mid(a+2));
			p.script->content_vision = desc;
			if (p.script->english_title.IsEmpty())
				p.script->english_title = title;
			p.script->native_title = "";
		}
		else {
			p.script->content_vision = s;
			p.script->english_title = "";
			p.script->native_title = "";
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
	if (!p.script || !typeclasses.IsCursor() || !contents.IsCursor()) return;
	Script& l = *p.script;
	l.typeclass = typeclasses.Get("IDX");
	l.content = contents.Get("IDX");
}


END_TEXTLIB_NAMESPACE

