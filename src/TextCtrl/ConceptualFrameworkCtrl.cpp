#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ConceptualFrameworkCtrl::ConceptualFrameworkCtrl() {
	Add(vsplit.VSizePos(0,20).HSizePos());
	Add(prog.BottomPos(0,20).HSizePos(300));
	Add(remaining.BottomPos(0,20).LeftPos(0,300));
	
	
	vsplit.Vert() << tsplit << bsplit;
	tsplit.Horz() << cfsplit << stories;
	
	cfsplit.Vert() << cfs << cf;
	cfsplit.SetPos(7500);
	
	tsplit.SetPos(2500);
	
	bsplit.Horz() << story << story_struct;
	#if USE_IMPROVED_ELEMENTS
	bsplit << story_improved;
	#endif
	
	CtrlLayout(cf);
	CtrlLayout(story);
	
	
	cfs.AddColumn(t_("Framework"));
	cfs.AddColumn(t_("Snap. rev."));
	cfs.AddColumn(t_("Belief"));
	cfs.ColumnWidths("4 1 2");
	cfs.AddIndex("IDX");
	cfs <<= THISBACK(DataFramework);
	cfs.WhenBar << [this](Bar& bar) {
		bar.Add("Add item", THISBACK1(Do, 2));
		bar.Add(cfs.IsCursor(), "Remove item", THISBACK1(Do, 3));
	};
	cfs.WhenCursor << THISBACK(DataFramework);
	
	cf.name.WhenAction << THISBACK(OnValueChange);
	cf.revision.WhenAction << THISBACK(OnValueChange);
	cf.belief.WhenAction << THISBACK(OnValueChange);
	
	stories.AddColumn(t_("Typeclass"));
	stories.AddColumn(t_("Content"));
	stories.AddColumn(t_("Description"));
	for(int i = 0; i < SCORE_COUNT; i++)
		stories.AddColumn("S" + IntStr(i));
	stories.AddColumn("Total score");
	stories.ColumnWidths("2 2 12 1 1 1 1 1 1 1 1 1 1 1");
	stories.AddIndex("IDX");
	stories.WhenCursor << THISBACK(DataStory);
	stories <<= THISBACK(DataStory);
	stories.WhenBar << [this](Bar& bar) {
		bar.Add("Add item", THISBACK1(Do, 2));
		bar.Add(cfs.IsCursor(), "Remove item", THISBACK1(Do, 3));
	};
	
	cf.belief.WhenAction << THISBACK(OnValueChange);
	
	story.colors.AddColumn("Color");
	
}

void ConceptualFrameworkCtrl::Clear() {
	
}

void ConceptualFrameworkCtrl::Data() {
	EditorPtrs& ep = GetPointers();
	MetaDatabase& mdb = MetaDatabase::Single();
	
	for(int i = 0; i < ep.entity->concepts.GetCount(); i++) {
		Concept& c = ep.entity->concepts[i];
		cfs.Set(i, 0, c.name);
		cfs.Set(i, 1, c.snap_rev);
		int j = mdb.FindBelief(c.belief_uniq);
		cfs.Set(i, 2, j >= 0 ? mdb.beliefs[j].name : String());
		cfs.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(cfs);
	cfs.SetCount(ep.entity->concepts.GetCount());
	if (!cfs.IsCursor() && cfs.GetCount())
		cfs.SetCursor(0);
	
	
	int appmode = GetAppMode();
	
	story.typeclass.Clear();
	story.typeclass.Add("");
	for (String tc : GetTypeclasses())
		story.typeclass.Add(tc);
	
	story.content.Clear();
	story.content.Add("");
	for (const auto& it : GetContents())
		story.content.Add(it.key);
	
	
	DataFramework();
}

void ConceptualFrameworkCtrl::DataFramework() {
	EditorPtrs& ep = GetPointers();
	MetaDatabase& mdb = MetaDatabase::Single();
	if (!cfs.IsCursor()) {
		stories.Clear();
		return;
	}
	int cf_i = cfs.Get("IDX");
	Concept& con = ep.entity->concepts[cf_i];
	
	
	// Form values
	cf.name.SetData(con.name);
	
	
	// Belief list
	if (cf.belief.GetCount() != mdb.beliefs.GetCount()+1) {
		cf.belief.Clear();
		cf.belief.Add("Default");
		for(int i = 0; i < mdb.beliefs.GetCount(); i++) {
			const Belief& b = mdb.beliefs[i];
			cf.belief.Add(b.name);
		}
	}
	int belief_i = mdb.FindBelief(con.belief_uniq) + 1;
	if (belief_i < cf.belief.GetCount())
		cf.belief.SetIndex(belief_i);
	
	
	// Revision list
	cf.revision.Clear();
	int rev_cur = 0;
	for(int i = 0; i < ep.entity->profile->snapshots.GetCount(); i++) {
		auto& snap = ep.entity->profile->snapshots[i];
		cf.revision.Add(snap.revision);
		if (snap.revision == con.snap_rev)
			rev_cur = i;
	}
	if (rev_cur < cf.revision.GetCount())
		cf.revision.SetIndex(rev_cur);
	
	
	// Stories list
	int row = 0;
	const auto& tcs = GetTypeclasses();
	const auto& cons = GetContents();
	for(int i = 0; i < con.stories.GetCount(); i++) {
		const ConceptStory& st = con.stories[i];
		stories.Set(row, 0, st.typeclass >= 0 ? tcs[st.typeclass] : String());
		stories.Set(row, 1, st.content >= 0 ? cons[st.content].key : String());
		//stories.Set(row, 2, st.desc);
		SetColoredListValue(stories, row, 2, st.desc, st.GetAverageColor(), true);
		double sum = 0;
		for(int j = 0; j < SCORE_COUNT; j++) {
			double sc = st.AvSingleScore(j);
			stories.Set(row, 3+j, sc);
			sum += sc;
		}
		double av = sum / (double)SCORE_COUNT;
		if (av > 10.0) av = 0;
		stories.Set(row, 3+SCORE_COUNT, av);
		stories.Set(row, "IDX", i);
		row++;
	}
	SetCountWithDefaultCursor(stories, row);
	stories.SetSortColumn(3+SCORE_COUNT, true);
	
	DataStory();
}

void ConceptualFrameworkCtrl::DataStory() {
	EditorPtrs& ep = GetPointers();
	MetaDatabase& mdb = MetaDatabase::Single();
	if (!cfs.IsCursor() || !stories.IsCursor()) {
		story.colors.SetCount(0);
		return;
	}
	
	int cf_i = cfs.Get("IDX");
	int story_i = stories.Get("IDX");
	if (cf_i >= ep.entity->concepts.GetCount()) return;
	Concept& con = ep.entity->concepts[cf_i];
	if (story_i >= con.stories.GetCount()) return;
	ConceptStory& st = con.stories[story_i];
	
	story.desc.SetData(st.desc);
	if (st.typeclass >= -1 && st.typeclass < story.typeclass.GetCount())
		story.typeclass.SetIndex(st.typeclass+1);
	if (st.content >= -1 && st.content < story.content.GetCount())
		story.content.SetIndex(st.content+1);
	
	//
	String full;
	for(int i = 0; i < st.elements.GetCount(); i++) {
		const auto& el = st.elements[i];
		full << "[" << el.key << "]\n";
		full << el.value << "\n\n";
	}
	story_struct.SetData(full);
	
	//
	String improved;
	for(int i = 0; i < st.ELEMENTS_VAR.GetCount(); i++) {
		const auto& el = st.ELEMENTS_VAR[i];
		improved << "[" << el.key << "]\n";
		improved << el.value << "\n\n";
		
		story.colors.Set(i, 0, AttrText(el.key).NormalPaper(el.clr).Paper(el.clr));
	}
	story_improved.SetData(improved);
	story.colors.SetCount(st.ELEMENTS_VAR.GetCount());
	
	//
	
}

void ConceptualFrameworkCtrl::OnValueChange() {
	EditorPtrs& ep = GetPointers();
	MetaDatabase& mdb = MetaDatabase::Single();
	if (!cfs.IsCursor())
		return;
	int cf_i = cfs.Get("IDX");
	Concept& con = ep.entity->concepts[cf_i];
	
	con.name = cf.name.GetData();
	
	if (cf.revision.GetCount())
		con.snap_rev = cf.revision.GetValue();
	
	cfs.Set(0, con.name);
	cfs.Set(1, con.snap_rev);
	if (mdb.beliefs.GetCount() && cf.belief.GetCount()) {
		int idx = cf.belief.GetIndex() - 1;
		if (idx >= 0) {
			auto& b = mdb.beliefs[idx];
			cfs.Set(2, b.name);
			con.belief_uniq = b.uniq;
		}
		else {
			cfs.Set(2, Value());
			con.belief_uniq = 0;
		}
	}
}

void ConceptualFrameworkCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void ConceptualFrameworkCtrl::Do(int fn) {
	MetaPtrs& mp = MetaPtrs::Single();
	EditorPtrs& ep = GetPointers();
	int appmode = GetAppMode();
	if (!ep.entity)
		return;
	if (fn == 0 || fn == 1) {
		if (!cfs.IsCursor())
			return;
		int cf_i = cfs.Get("IDX");
		Concept& c = ep.entity->concepts[cf_i];
		if (c.snap_rev < 0) {PromptOK("No snapshot revision set"); return;}
		BiographySnapshot* snap = ep.entity->profile->FindSnapshotRevision(c.snap_rev);
		if (!snap) {PromptOK("No snapshot revision found"); return;}
		
		ConceptualFrameworkProcess& sdi = ConceptualFrameworkProcess::Get(appmode, *ep.entity, c, *snap);
		prog.Attach(sdi);
		sdi.WhenRemaining << [this](String s) {PostCallback([this,s](){remaining.SetLabel(s);});};
		if (fn == 0)
			sdi.Start();
		else
			sdi.Stop();
	}
	else if (fn == 2) {
		Concept& c = ep.entity->concepts.Add();
		c.created = GetSysTime();
		c.name = "Unnamed #" + IntStr(ep.entity->concepts.GetCount());
		c.snap_rev = ep.entity->profile->snapshots.Top().revision;
		//c.snap_rev = ep.entity->profile->snapshots.GetCount()-2; // latest can't be used
		PostCallback(THISBACK(Data));
	}
	else if (fn == 3) {
		if (!cfs.IsCursor())
			return;
		int cf_i = cfs.Get("IDX");
		ep.entity->concepts.Remove(cf_i);
	}
}


END_TEXTLIB_NAMESPACE
