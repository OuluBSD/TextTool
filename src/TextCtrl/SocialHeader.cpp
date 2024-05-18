#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialHeaderCtrl::SocialHeaderCtrl() {
	//CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << platforms << vsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << timeline << entries << entry_split;
	vsplit.SetPos(800, 0);
	vsplit.SetPos(3333, 1);
	platforms.AddColumn(t_("Enabled"));
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("3 10 5");
	for(int i = 0; i < PLATFORM_COUNT; i++) {
		Option* o = new Option;
		o->WhenAction << [this,o,i]() {
			MetaPtrs& mp = MetaPtrs::Single();
			BiographyAnalysis& analysis = mp.profile->biography_analysis;
			analysis.Realize();
			analysis.platforms[i].platform_enabled = o->Get();
		};
		platforms.SetCtrl(i, 0, o);
		platforms.Set(i, 1, GetPlatforms()[i].name);
		platforms.Set(i, 2, Value());
		platforms.Set(i, "IDX", i);
	}
	platforms.SetCursor(0);
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	entries.AddColumn(t_("Published"));
	entries.AddColumn(t_("Title"));
	entries.AddColumn(t_("Message"));
	entries.AddColumn(t_("Hashtags"));
	entries.AddColumn(t_("Comments"));
	entries.AddColumn(t_("Score"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("3 5 10 4 1 1");
	entries.WhenBar << THISBACK(EntryListMenu);
	
	entry_split.Horz() << attr_keys << attr_value;
	entry_split.SetPos(2500);
	attr_keys.AddColumn(t_("Attribute key"));
	attr_keys.AddIndex("IDX");
	attr_keys.Add(t_("Original description"), -1);
	for(int j = 0; j < PLATDESC_MODE_COUNT; j++) {
		String mode = GetPlatformDescriptionModeKey(j);
		for(int i = 0; i < PLATDESC_LEN_COUNT; i++) {
			String len = GetPlatformDescriptionLengthKey(i);
			int idx = i * PLATDESC_MODE_COUNT + j;
			String t = mode + " " + len;
			attr_keys.Add(t, idx);
		}
	}
	attr_keys.SetCursor(0);
	attr_keys.WhenCursor << THISBACK(DataPlatform);
	attr_value.WhenAction << THISBACK(OnValueChange);
	
}

void SocialHeaderCtrl::Data() {
	MetaPtrs& mp = MetaPtrs::Single();
	BiographyAnalysis& analysis = mp.profile->biography_analysis;
	
	for(int i = 0; i < platforms.GetCount(); i++) {
		Option* o = dynamic_cast<Option*>(platforms.GetCtrl(i, 0));
		
		int plat_i = platforms.Get(i, "IDX");
		if (plat_i >= analysis.platforms.GetCount()) {
			platforms.Set(i, 2, Value());
			o->Set(0);
			continue;
		}
		PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
		o->Set(plat_anal.platform_enabled);
		
		int c = 0;
		for(int j = 0; j < PLATDESC_LEN_COUNT; j++)
			for(int k = 0; k < PLATDESC_MODE_COUNT; k++)
				if (plat_anal.descriptions[j][k].GetCount())
					c++;
		int total = PLATDESC_LEN_COUNT * PLATDESC_MODE_COUNT;
		platforms.Set(i, 2, c > 0 ? IntStr(100 * c / total) + "%" : String());
	}
	
	DataPlatform();
}

void SocialHeaderCtrl::DataPlatform() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	Biography& biography = mp.profile->biography_detailed;
	BiographyAnalysis& analysis = mp.profile->biography_analysis;
	
	if (!mp.profile || !platforms.IsCursor()) {
		attr_value.Clear();
		return;
	}
	
	int plat_i = platforms.GetCursor();
	
	const Platform& plat = GetPlatforms()[plat_i];
	if (plat_i >= analysis.platforms.GetCount()) {
		attr_value.Clear();
		return;
	}
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	
	if (attr_keys.IsCursor()) {
		int key_i = attr_keys.Get("IDX");
		int total_desc = PLATDESC_LEN_COUNT * PLATDESC_MODE_COUNT;
		int len_i = key_i / PLATDESC_MODE_COUNT;
		int mode_i = key_i % PLATDESC_MODE_COUNT;
		if (key_i < 0)
			attr_value.SetData(plat_anal.profile_description_from_biography);
		else if (key_i < total_desc)
			attr_value.SetData(plat_anal.descriptions[len_i][mode_i]);
		else
			attr_value.SetData("");
	}
	else {
		attr_value.Clear();
	}
}

void SocialHeaderCtrl::OnValueChange() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	Biography& biography = mp.profile->biography_detailed;
	BiographyAnalysis& analysis = mp.profile->biography_analysis;
	
	if (!platforms.IsCursor() || !attr_keys.IsCursor())
		return;
	
	int plat_i = platforms.GetCursor();
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	
	
	int key_i = attr_keys.Get("IDX");
	int total_desc = PLATDESC_LEN_COUNT * PLATDESC_MODE_COUNT;
	int len_i = key_i / PLATDESC_MODE_COUNT;
	int mode_i = key_i % PLATDESC_MODE_COUNT;
	if (key_i < 0)
		plat_anal.profile_description_from_biography = attr_value.GetData();
	else if (key_i < total_desc)
		plat_anal.descriptions[len_i][mode_i] = attr_value.GetData();
}

void SocialHeaderCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Solve categories"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
}

void SocialHeaderCtrl::EntryListMenu(Bar& bar) {
	
}

void SocialHeaderCtrl::Do(int fn) {
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.profile)
		return;
	SocialSolver& ss = SocialSolver::Get(*mp.profile);
	if (fn == 0) {
		ss.only_categories = true;
		ss.Start();
	}
	/*else if (fn == 1) {
		ss.Stop();
	}*/
}


END_TEXTLIB_NAMESPACE

