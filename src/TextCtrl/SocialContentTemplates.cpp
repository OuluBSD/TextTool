#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialContentTemplateCtrl::SocialContentTemplateCtrl() {
	CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << vsplit;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << platforms;
	
	vsplit.Vert() << timeline << threadsplit << tabs;
	vsplit.SetPos(800, 0);
	vsplit.SetPos(3333, 1);
	
	threadsplit.Horz() << threads << entries;
	threadsplit.SetPos(2500);
	
	tabs.Add(entry.SizePos(), t_("Entry"));
	tabs.Add(tmpl_params.SizePos(), t_("Template params"));
	tabs.Add(campaign_params.SizePos(), t_("Campaign params"));
	
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	//campaigns.AddColumn(t_("Campaign"));
	//campaigns.AddIndex("IDX");
	//campaigns.WhenCursor << THISBACK(DataCampaign);
	
	campaign_tmpls.AddColumn(t_("Campaign Template"));
	campaign_tmpls.AddIndex("IDX");
	//campaign_tmpls.WhenCursor << THISBACK(DataCampaignTemplate);
	
	threads.AddColumn(t_("Sub-Forum"));
	threads.AddColumn(t_("Thread Title"));
	threads.AddIndex("IDX");
	threads.ColumnWidths("1 3");
	threads.WhenBar << THISBACK(ThreadListMenu);
	threads.WhenCursor << THISBACK(DataThread);
	
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
}

void SocialContentTemplateCtrl::Data() {
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
		platforms.Set(row, 1, pld.GetTotalEntryCount());
		row++;
	}
	INHIBIT_CURSOR(platforms);
	platforms.SetCount(GetPlatforms().GetCount());
	platforms.SetSortColumn(1, true);
	if (platforms.GetCount() && !platforms.IsCursor())
		platforms.SetCursor(0);
	
	DataPlatform();
}

void SocialContentTemplateCtrl::DataPlatform() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor()) {
		threads.Clear();
		entries.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	
	int row = 0;
	for (const PlatformThread& t : pld.threads) {
		int score = 0;
		threads.Set(row, "IDX", row);
		threads.Set(row, 0, t.subforum);
		threads.Set(row, 1, t.title);
		row++;
	}
	INHIBIT_CURSOR(threads);
	threads.SetCount(pld.threads.GetCount());
	threads.SetSortColumn(0, true);
	if (threads.GetCount() && !threads.IsCursor())
		threads.SetCursor(0);
	
	DataThread();
}

void SocialContentTemplateCtrl::DataThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor()) {
		entries.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	
	int row = 0;
	for (const Entry& e : thrd.entries) {
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
	entries.SetCount(thrd.entries.GetCount());
	entries.SetSortColumn(0, true);
	if (entries.GetCount() && !entries.IsCursor())
		entries.SetCursor(0);
	
	DataEntry();
}

void SocialContentTemplateCtrl::DataEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor() || !entries.IsCursor()) {
		ClearEntry();
		return;
	}
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	Entry& e = thrd.entries[entry_i];
	
	entry.title = e.title;
	entry.message.SetData(e.message);
	entry.orig_message.SetData(e.orig_message);
	entry.hashtags.SetData(e.hashtags);
	entry.location.SetData(e.location);
}

void SocialContentTemplateCtrl::ClearEntry() {
	entry.message.SetData("");
	entry.orig_message.SetData("");
	entry.hashtags.Clear();
	entry.location.Clear();
	entry.title.Clear();
	
}

void SocialContentTemplateCtrl::Clear() {
	platforms.Clear();
	threads.Clear();
	entries.Clear();
	ClearEntry();
}

void SocialContentTemplateCtrl::OnValueChange() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	Entry& e = thrd.entries[entry_i];
	
	
	
	e.message = entry.message.GetData();
	e.orig_message = entry.orig_message.GetData();
	e.hashtags = entry.hashtags.GetData();
	e.location = entry.location.GetData();
	e.title = entry.title.GetData();
}

void SocialContentTemplateCtrl::AddThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& t = pld.threads.Add();
	
	DataPlatform();
}

void SocialContentTemplateCtrl::RemoveThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int thrd_i = threads.Get("IDX");
	if (thrd_i >= 0 && thrd_i < pld.threads.GetCount())
		pld.threads.Remove(thrd_i);
	
	DataPlatform();
}

void SocialContentTemplateCtrl::AddEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	Entry& e = thrd.entries.Add();
	e.published = GetSysTime();
	
	DataThread();
}

void SocialContentTemplateCtrl::RemoveEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	if (entry_i >= 0 && entry_i < thrd.entries.GetCount())
		thrd.entries.Remove(entry_i);
	
	DataThread();
}

void SocialContentTemplateCtrl::ThreadListMenu(Bar& bar) {
	bar.Add(t_("Add Thread"), AppImg::BlueRing(), THISBACK(AddThread)).Key(K_CTRL_T);
	if (entries.IsCursor())
		bar.Add(t_("Remove Thread"), AppImg::BlueRing(), THISBACK(RemoveThread)).Key(K_CTRL_H);
}

void SocialContentTemplateCtrl::EntryListMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	if (entries.IsCursor())
		bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL_D);
}

void SocialContentTemplateCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL|K_SHIFT|K_W);
	
}





END_TEXTLIB_NAMESPACE
