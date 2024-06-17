#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


SocialBeliefsCtrl::SocialBeliefsCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Add(beliefs);
	hsplit.Add(info);
	hsplit.Add(user);
	hsplit.Add(attrs);
	hsplit.SetPos(1500,0);
	hsplit.SetPos(1500+8500/3,1);
	hsplit.SetPos(1500+8500*2/3,2);
	
	CtrlLayout(info);
	info.reference.WhenAction << THISBACK(OnValueChange);
	
	beliefs.AddColumn("Belief");
	beliefs.WhenCursor << THISBACK(DataBelief);
	beliefs.WhenBar = [this](Bar& b) {
		b.Add("Add belief", THISBACK(AddBelief));
		//b.Add("Remove belief", THISBACK(RemoveBelief));
	};
	
	user.AddColumn("User input");
	
	attrs.AddColumn("Positive");
	attrs.AddColumn("Negative");
	
	
}

void SocialBeliefsCtrl::Data() {
	MetaDatabase& mdb = MetaDatabase::Single();
	
	
	for(int i = 0; i < mdb.beliefs.GetCount(); i++) {
		Belief& b = mdb.beliefs[i];
		
		beliefs.Set(i, 0, b.name);
	}
	INHIBIT_CURSOR(beliefs);
	beliefs.SetCount(mdb.beliefs.GetCount());
	if (!beliefs.IsCursor() && beliefs.GetCount())
		beliefs.SetCursor(0);
	
	DataBelief();
}

void SocialBeliefsCtrl::DataBelief() {
	MetaDatabase& mdb = MetaDatabase::Single();
	if (!beliefs.IsCursor())
		return;
	
	int b_i = beliefs.GetCursor();
	Belief& b = mdb.beliefs[b_i];
	
	
	info.name.SetData(b.name);
	info.reference.SetData(b.reference);
	
	for(int i = 0; i < b.user.GetCount(); i++) {
		user.Set(i, 0, b.user[i]);
	}
	user.SetCount(b.user.GetCount());
	
	for(int i = 0; i < b.attrs.GetCount(); i++) {
		Belief::Attr& a = b.attrs[i];
		attrs.Set(i, 0, a.positive);
		attrs.Set(i, 1, a.negative);
	}
	attrs.SetCount(b.attrs.GetCount());
	
}

void SocialBeliefsCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Paste user data"), AppImg::BlueRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Separator();
	bar.Add(t_("Process"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void SocialBeliefsCtrl::Do(int fn) {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (!p.owner) return;
	
	if (fn == 0) {
		if (!beliefs.IsCursor())
			return;
		int b_i = beliefs.GetCursor();
		Belief& b = mdb.beliefs[b_i];
		String res = ReadClipboardText();
		RemoveEmptyLines3(res);
		b.user <<= Split(res, "\n");
		DataBelief();
	}
	else if (fn == 1) {
		BeliefSolver& tm = BeliefSolver::Get();
		tm.WhenReady << [this](){PostCallback(THISBACK(DataBelief));};
		tm.Start();
	}
}

void SocialBeliefsCtrl::OnValueChange() {
	MetaDatabase& mdb = MetaDatabase::Single();
	if (!beliefs.IsCursor())
		return;
	
	int b_i = beliefs.GetCursor();
	Belief& b = mdb.beliefs[b_i];
	
	b.reference = info.reference.GetData();
}

void SocialBeliefsCtrl::AddBelief() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Add Belief"),
		t_("Belief's name"),
		0
	);
	if (!b) return;
	
	String t = MakeTitle(name);
	int artist_i = -1;
	for(int i = 0; i < mdb.beliefs.GetCount(); i++) {
		Belief& a = mdb.beliefs[i];
		if (a.name == t) {
			artist_i = i;
			break;
		}
	}
	if (artist_i >= 0) {
		PromptOK(DeQtf(t_("Belief exist already")));
		return;
	}
	
	Belief& a = mdb.beliefs.Add();
	a.name = name;
	
	Data();
}

void SocialBeliefsCtrl::RemoveBelief() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& p = MetaPtrs::Single();
	if (beliefs.IsCursor()) {
		mdb.beliefs.Remove(beliefs.GetCursor());
	}
	Data();
}


END_TEXTLIB_NAMESPACE
