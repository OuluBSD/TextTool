#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


PlatformCtrl::PlatformCtrl() {
	Add(hsplit.SizePos());
	hsplit.Horz() << platforms << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << plat << _;
	CtrlLayout(plat);
	
	platforms.AddColumn(t_("Type"));
	platforms.AddColumn(t_("Platform"));
	platforms.AddIndex("IDX");
	for(int i = 0; i < PLATFORM_COUNT; i++) {
		const Platform& plat = GetPlatforms()[i];
		platforms.Set(i, 0, plat.group);
		platforms.Set(i, 1, plat.name);
		platforms.Set(i, "IDX", i);
	}
	platforms.SetSortColumn(0);
	platforms.SetCursor(0);
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	
	plat.attrs.AddColumn(t_("Attribute"));
	plat.roles.AddColumn(t_("Role"));
	plat.roles.AddColumn(t_("Description"));
	plat.roles.ColumnWidths("1 4");
	
}

void PlatformCtrl::Data() {
	
	
	DataPlatform();
}

void PlatformCtrl::DataPlatform() {
	if (!platforms.IsCursor()) {
		plat.name.SetData("");
		plat.group.SetData("");
		plat.description.SetData("");
		plat.attrs.Clear();
		return;
	}
	
	int plat_i = platforms.Get("IDX");
	const Platform& p = GetPlatforms()[plat_i];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(p);
	
	plat.name.SetData(p.name);
	plat.group.SetData(p.group);
	plat.description.SetData(p.description);
	
	int row = 0;
	for(int i = 0; i < PLATFORM_ATTR_COUNT; i++) {
		if (p.attrs[i])
			plat.attrs.Set(row++, 0, GetPlatformAttrKey(i));
	}
	plat.attrs.SetCount(row);
	
	for(int i = 0; i < pa.roles.GetCount(); i++) {
		int role_i = pa.roles[i];
		plat.roles.Set(i, 0, GetSocietyRoleKey(role_i));
		plat.roles.Set(i, 1, GetSocietyRoleDescription(role_i));
	}
	plat.roles.SetCount(pa.roles.GetCount());
}

void PlatformCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void PlatformCtrl::Do(int fn) {
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile)
		return;
	SocialSolver& ss = SocialSolver::Get(*mp.profile);
	if (fn == 0) {
		ss.Start();
	}
	else if (fn == 1) {
		ss.Stop();
	}
}



END_TEXTLIB_NAMESPACE
