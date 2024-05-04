#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialContent::SocialContent() {
	CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << vsplit;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << platforms << campaigns;
	
	vsplit.Vert() << timeline << entries << entry;
	vsplit.SetPos(800, 0);
	vsplit.SetPos(3333, 1);
	
	
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	campaigns.AddColumn(t_("Campaign"));
	campaigns.AddIndex("IDX");
	//campaigns.WhenCursor << THISBACK(DataCampaign);
	
	entries.AddColumn(t_("Published"));
	entries.AddColumn(t_("Title"));
	entries.AddColumn(t_("Message"));
	entries.AddColumn(t_("Hashtags"));
	entries.AddColumn(t_("Comments"));
	entries.AddColumn(t_("Score"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("3 5 10 4 1 1");
	entries.WhenBar << THISBACK(EntryListMenu);
	
	entry.title.WhenAction << THISBACK(OnValueChange);
	entry.message.WhenAction << THISBACK(OnValueChange);
	entry.orig_message.WhenAction << THISBACK(OnValueChange);
	entry.hashtags.WhenAction << THISBACK(OnValueChange);
	entry.location.WhenAction << THISBACK(OnValueChange);
	entry.date.WhenAction << THISBACK(OnValueChange);
	entry.clock.WhenAction << THISBACK(OnValueChange);
	entry.clock.WhenDeactivate << THISBACK(OnValueChange);
	entry.clock.WhenPopDown << THISBACK(OnValueChange);
}

void SocialContent::Data() {
	MetaPtrs& p = MetaPtrs::Single();
	
	if (!p.profile) {
		platforms.Clear();
		entries.Clear();
		ClearEntry();
		return;
	}
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	
	int row = 0;
	for (const Platform& p : GetPlatforms()) {
		const PlatformData& pld = pd.platforms[row];
		platforms.Set(row, "IDX", row);
		platforms.Set(row, 0, p.name);
		platforms.Set(row, 1, pld.entries.GetCount());
		row++;
	}
	INHIBIT_CURSOR(platforms);
	platforms.SetCount(GetPlatforms().GetCount());
	platforms.SetSortColumn(1, true);
	if (platforms.GetCount() && !platforms.IsCursor())
		platforms.SetCursor(0);
	
	DataPlatform();
}

void SocialContent::DataPlatform() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor()) {
		entries.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	
	int row = 0;
	for (const Entry& e : pld.entries) {
		int score = 0;
		entries.Set(row, "IDX", row);
		entries.Set(row, 0, e.published);
		entries.Set(row, 1, e.title);
		entries.Set(row, 2, e.message);
		entries.Set(row, 3, e.hashtags);
		entries.Set(row, 4, e.comments.GetCount());
		entries.Set(row, 5, score);
		row++;
	}
	INHIBIT_CURSOR(entries);
	entries.SetCount(pld.entries.GetCount());
	entries.SetSortColumn(0, true);
	if (entries.GetCount() && !entries.IsCursor())
		entries.SetCursor(0);
	
	DataEntry();
}

void SocialContent::DataEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor()) {
		ClearEntry();
		return;
	}
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	Entry& e = pld.entries[entry_i];
	
	entry.title = e.title;
	entry.message.SetData(e.message);
	entry.orig_message.SetData(e.orig_message);
	entry.hashtags.SetData(e.hashtags);
	entry.location.SetData(e.location);
	entry.date.SetData(e.published);
	entry.clock.SetData(e.published);
	
}

void SocialContent::ClearEntry() {
	entry.message.SetData("");
	entry.orig_message.SetData("");
	entry.hashtags.Clear();
	entry.location.Clear();
	entry.title.Clear();
	entry.date.SetData(GetSysTime());
	entry.clock.SetData(GetSysTime());
	
}

void SocialContent::Clear() {
	platforms.Clear();
	entries.Clear();
	ClearEntry();
}

void SocialContent::OnValueChange() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	Entry& e = pld.entries[entry_i];
	
	
	
	e.message = entry.message.GetData();
	e.orig_message = entry.orig_message.GetData();
	e.hashtags = entry.hashtags.GetData();
	e.location = entry.location.GetData();
	e.title = entry.title.GetData();
	
	Date date = entry.date.GetDate();
	Time time = entry.clock.GetTime();
	time.year = date.year;
	time.month = date.month;
	time.day = date.day;
	e.published = time;
}

void SocialContent::AddEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	Entry& e = pld.entries.Add();
	e.published = GetSysTime();
	
	DataPlatform();
}

void SocialContent::RemoveEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	if (entry_i >= 0 && entry_i < pld.entries.GetCount())
		pld.entries.Remove(entry_i);
	
	DataPlatform();
}

void SocialContent::EntryListMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	if (entries.IsCursor())
		bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL_D);
}

void SocialContent::ToolMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL|K_SHIFT|K_W);
	
}


END_TEXTLIB_NAMESPACE
