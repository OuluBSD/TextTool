#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


SocialContent::SocialContent() {
	CtrlLayout(entry);
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << vsplit;
	hsplit.SetPos(1500);
	
	menusplit.Vert() << platforms;
	
	vsplit.Vert() << timeline << threadsplit << entry;
	vsplit.SetPos(800, 0);
	vsplit.SetPos(3333, 1);
	
	threadsplit.Horz() << threads << entries;
	threadsplit.SetPos(2500);
	
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	//campaigns.AddColumn(t_("Campaign"));
	//campaigns.AddIndex("IDX");
	//campaigns.WhenCursor << THISBACK(DataCampaign);
	
	threads.AddColumn(t_("Sub-Forum"));
	threads.AddColumn(t_("Thread Title"));
	threads.AddIndex("IDX");
	threads.ColumnWidths("1 3");
	threads.WhenBar << THISBACK(ThreadListMenu);
	threads.WhenCursor << THISBACK(DataThread);
	
	entries.AddColumn(t_("Published"));
	entries.AddColumn(t_("User"));
	entries.AddColumn(t_("Title"));
	entries.AddColumn(t_("Message"));
	entries.AddColumn(t_("Hashtags"));
	entries.AddColumn(t_("Comments"));
	entries.AddColumn(t_("Score"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("3 2 5 10 4 1 1");
	entries.WhenBar << THISBACK(EntryListMenu);
	entries.WhenCursor << THISBACK(DataEntry);
	
	entry.thread_subforum.WhenAction << THISBACK(OnValueChange);
	entry.thread_title.WhenAction << THISBACK(OnValueChange);
	entry.user.WhenAction << THISBACK(OnValueChange);
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

void SocialContent::DataPlatform() {
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

void SocialContent::DataThread() {
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
	
	entry.thread_subforum.SetData(thrd.subforum);
	entry.thread_title.SetData(thrd.title);
	
	int row = 0;
	for (const Entry& e : thrd.entries) {
		int score = 0;
		entries.Set(row, "IDX", row);
		entries.Set(row, 0, e.published);
		entries.Set(row, 1, e.user);
		entries.Set(row, 2, e.title);
		entries.Set(row, 3, e.message);
		entries.Set(row, 4, e.hashtags);
		entries.Set(row, 5, e.comments.GetCount());
		entries.Set(row, 6, score);
		row++;
	}
	INHIBIT_CURSOR(entries);
	entries.SetCount(thrd.entries.GetCount());
	entries.SetSortColumn(0, true);
	if (entries.GetCount() && !entries.IsCursor())
		entries.SetCursor(0);
	
	DataEntry();
}

void SocialContent::DataEntry() {
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
	
	entry.user = e.user;
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
	entry.user.Clear();
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
	if (!platforms.IsCursor() || !threads.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	
	thrd.title = entry.thread_title.GetData();
	thrd.subforum = entry.thread_subforum.GetData();
	threads.Set(thrd_i, 0, thrd.subforum);
	threads.Set(thrd_i, 1, thrd.title);
	
	if (!entries.IsCursor())
		return;
	
	int entry_i = entries.Get("IDX");
	Entry& e = thrd.entries[entry_i];
	e.message = entry.message.GetData();
	e.orig_message = entry.orig_message.GetData();
	e.hashtags = entry.hashtags.GetData();
	e.location = entry.location.GetData();
	e.title = entry.title.GetData();
	e.user = entry.user.GetData();
	
	Date date = entry.date.GetDate();
	Time time = entry.clock.GetTime();
	time.year = date.year;
	time.month = date.month;
	time.day = date.day;
	e.published = time;
	
	entries.Set(1, e.user);
	entries.Set(2, e.title);
	entries.Set(3, e.message);
	entries.Set(4, e.hashtags);
}

void SocialContent::AddThread() {
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

void SocialContent::RemoveThread() {
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

void SocialContent::AddEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int thrd_i = threads.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformThread& thrd = pld.threads[thrd_i];
	Entry& e = thrd.entries.Add();
	e.published = GetSysTime();
	
	DataThread();
}

void SocialContent::RemoveEntry() {
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

void SocialContent::ThreadListMenu(Bar& bar) {
	bar.Add(t_("Add Thread"), AppImg::BlueRing(), THISBACK(AddThread)).Key(K_CTRL_T);
	if (entries.IsCursor())
		bar.Add(t_("Remove Thread"), AppImg::BlueRing(), THISBACK(RemoveThread)).Key(K_CTRL_H);
}

void SocialContent::EntryListMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	if (entries.IsCursor())
		bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL_D);
}

void SocialContent::ToolMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL|K_SHIFT|K_W);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	
}

void SocialContent::Do(int fn) {
	MetaPtrs& mp = MetaPtrs::Single();
	if (!mp.owner)
		return;
	Owner& owner = *mp.owner;
	SocialSolver& ss = SocialSolver::Get(owner);
	if (fn == 0) {
		ss.Start();
	}
	else if (fn == 1) {
		ss.Stop();
	}
}


END_TEXTLIB_NAMESPACE
