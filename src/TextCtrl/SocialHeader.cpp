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
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	for(int i = 0; i < PLATFORM_COUNT; i++) {
		platforms.Set(i, 0, GetPlatforms()[i].name);
		platforms.Set(i, 1, 0);
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
	attr_keys.Add(t_("Original description"));
	attr_keys.Add(t_("Polished description"));
	attr_keys.Add(t_("Polished short description"));
	attr_keys.Add(t_("Polished short description in dialect"));
	attr_keys.Add(t_("Translated description"));
	attr_keys.Add(t_("Translated short description"));
	attr_keys.Add(t_("Translated short description in dialect"));
	attr_keys.SetCursor(0);
	attr_keys.WhenCursor << THISBACK(DataPlatform);
	attr_value.WhenAction << THISBACK(OnValueChange);
	
}

void SocialHeaderCtrl::Data() {
	
	
	DataPlatform();
}

void SocialHeaderCtrl::DataPlatform() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	BiographyAnalysis& analysis = mp.owner->biography_analysis;
	
	if (!platforms.IsCursor())
		return;
	
	int plat_i = platforms.GetCursor();
	
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	
	if (attr_keys.IsCursor()) {
		int key_i = attr_keys.GetCursor();
		
		switch (key_i) {
			case 0: attr_value.SetData(plat_anal.profile_description_from_biography); break;
			case 1: attr_value.SetData(plat_anal.polished_description); break;
			case 2: attr_value.SetData(plat_anal.short_polished_description); break;
			case 3: attr_value.SetData(plat_anal.short_polished_description_slightly_dialect); break;
			case 4: attr_value.SetData(plat_anal.translated_polished_description); break;
			case 5: attr_value.SetData(plat_anal.translated_short_polished_description); break;
			case 6: attr_value.SetData(plat_anal.translated_short_polished_description_slightly_dialect); break;
			default:
				attr_value.SetData("");
				break;
		}
	}
}

void SocialHeaderCtrl::OnValueChange() {
	MetaDatabase& mdb = MetaDatabase::Single();
	MetaPtrs& mp = MetaPtrs::Single();
	Owner& owner = *mp.owner;
	Biography& biography = mp.owner->biography_detailed;
	BiographyAnalysis& analysis = mp.owner->biography_analysis;
	
	if (!platforms.IsCursor() || !attr_keys.IsCursor())
		return;
	
	int plat_i = platforms.GetCursor();
	int key_i = attr_keys.GetCursor();
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	
	switch (key_i) {
		case 0: attr_value.SetData(plat_anal.profile_description_from_biography); break;
		case 1: attr_value.SetData(plat_anal.polished_description); break;
		case 2: attr_value.SetData(plat_anal.short_polished_description); break;
		case 3: attr_value.SetData(plat_anal.short_polished_description_slightly_dialect); break;
		case 4: attr_value.SetData(plat_anal.translated_polished_description); break;
		case 5: attr_value.SetData(plat_anal.translated_short_polished_description); break;
		case 6: attr_value.SetData(plat_anal.translated_short_polished_description_slightly_dialect); break;
		default:
			attr_value.SetData("");
			break;
	}
}

void SocialHeaderCtrl::ToolMenu(Bar& bar) {
	ToolAppCtrl::ToolMenu(bar);
}

void SocialHeaderCtrl::EntryListMenu(Bar& bar) {
	
}


END_TEXTLIB_NAMESPACE

