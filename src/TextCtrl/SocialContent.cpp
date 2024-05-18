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
	
	threadsplit.Horz() << entries << threads << comments;
	threadsplit.SetPos(2000, 0);
	threadsplit.SetPos(2500, 1);
	
	platforms.AddColumn(t_("Platform"));
	platforms.AddColumn(t_("Entries"));
	platforms.AddIndex("IDX");
	platforms.ColumnWidths("4 1");
	platforms.WhenCursor << THISBACK(DataPlatform);
	
	//campaigns.AddColumn(t_("Campaign"));
	//campaigns.AddIndex("IDX");
	//campaigns.WhenCursor << THISBACK(DataCampaign);
	
	entries.AddColumn(t_("Sub-Forum"));
	entries.AddColumn(t_("Entry Title"));
	entries.AddIndex("IDX");
	entries.ColumnWidths("1 3");
	entries.WhenBar << THISBACK(EntryListMenu);
	entries.WhenCursor << THISBACK(DataEntry);
	
	threads.AddColumn(t_("Thread Title"));
	threads.AddIndex("IDX");
	threads.WhenBar << THISBACK(ThreadListMenu);
	threads.WhenCursor << THISBACK(DataThread);
	
	comments.AddColumn(t_("#"));
	comments.AddColumn(t_("Published"));
	comments.AddColumn(t_("User"));
	comments.AddColumn(t_("Message"));
	comments.AddColumn(t_("Hashtags"));
	comments.AddColumn(t_("Comments"));
	comments.AddColumn(t_("Score"));
	comments.AddIndex("IDX");
	comments.ColumnWidths("1 3 2 10 4 1 1");
	comments.WhenBar << THISBACK(CommentListMenu);
	comments.WhenCursor << THISBACK(DataComment);
	
	entry.entry_subforum.WhenAction << THISBACK(OnValueChange);
	entry.entry_title.WhenAction << THISBACK(OnValueChange);
	entry.thread_title.WhenAction << THISBACK(OnValueChange);
	entry.user.WhenAction << THISBACK(OnValueChange);
	entry.generate.WhenAction << THISBACK(OnValueChange);
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
		int c = pld.GetTotalEntryCount();
		platforms.Set(row, 1, c > 0 ? Value(c) : Value());
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
		threads.Clear();
		comments.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	
	int row = 0;
	for (const PlatformEntry& e : pld.entries) {
		int score = 0;
		entries.Set(row, "IDX", row);
		entries.Set(row, 0, e.subforum);
		entries.Set(row, 1, e.title);
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
		threads.Clear();
		comments.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformEntry& e = pld.entries[entry_i];
	
	entry.entry_subforum.SetData(e.subforum);
	entry.entry_title.SetData(e.title);
	
	int row = 0;
	for (const PlatformThread& t : e.threads) {
		int score = 0;
		threads.Set(row, "IDX", row);
		threads.Set(row, 0, t.title);
		row++;
	}
	INHIBIT_CURSOR(threads);
	threads.SetCount(e.threads.GetCount());
	threads.SetSortColumn(0, true);
	if (threads.GetCount() && !threads.IsCursor())
		threads.SetCursor(0);
	
	DataThread();
}

void SocialContent::DataThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor() || !threads.IsCursor()) {
		comments.Clear();
		ClearEntry();
		return;
	}
	
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int entry_i = entries.Get("IDX");
	int thrd_i = threads.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformEntry& e = pld.entries[entry_i];
	PlatformThread& thrd = e.threads[thrd_i];
	
	entry.thread_title.SetData(thrd.title);
	
	int row = 0;
	for (const PlatformComment& c : thrd.comments) {
		int score = 0;
		comments.Set(row, "IDX", row);
		comments.Set(row, 0, 1+row);
		comments.Set(row, 1, c.published);
		comments.Set(row, 2, c.user);
		comments.Set(row, 3, c.message);
		comments.Set(row, 4, c.hashtags);
		comments.Set(row, 5, c.responses.GetCount());
		comments.Set(row, 6, score);
		row++;
	}
	INHIBIT_CURSOR(comments);
	comments.SetCount(thrd.comments.GetCount());
	comments.SetSortColumn(0, true);
	if (comments.GetCount() && !comments.IsCursor())
		comments.SetCursor(0);
	
	DataComment();
}

void SocialContent::DataComment() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor() || !entries.IsCursor() || !comments.IsCursor()) {
		ClearEntry();
		return;
	}
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int entry_i = entries.Get("IDX");
	int thrd_i = threads.Get("IDX");
	int comm_i = comments.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformEntry& e = pld.entries[entry_i];
	PlatformThread& thrd = e.threads[thrd_i];
	PlatformComment& c = thrd.comments[comm_i];
	
	entry.user = c.user;
	entry.generate = c.generate;
	entry.message.SetData(c.message);
	entry.orig_message.SetData(c.orig_message);
	entry.hashtags.SetData(c.hashtags);
	entry.location.SetData(c.location);
	entry.date.SetData(c.published);
	entry.clock.SetData(c.published);
	entry.merged.SetData(c.text_merged_status);
}

void SocialContent::ClearEntry() {
	entry.message.SetData("");
	entry.orig_message.SetData("");
	entry.hashtags.Clear();
	entry.location.Clear();
	entry.generate.Set(0);
	entry.user.Clear();
	entry.date.SetData(GetSysTime());
	entry.clock.SetData(GetSysTime());
	
}

void SocialContent::Clear() {
	platforms.Clear();
	threads.Clear();
	entries.Clear();
	comments.Clear();
	ClearEntry();
}

void SocialContent::OnValueChange() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	int entry_i = entries.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformEntry& e = pld.entries[entry_i];
	
	e.title = entry.entry_title.GetData();
	e.subforum = entry.entry_subforum.GetData();
	entries.Set(0, e.subforum);
	entries.Set(1, e.title);
	
	if (!threads.IsCursor())
		return;
	
	int thrd_i = threads.Get("IDX");
	if (thrd_i < 0 || thrd_i >= e.threads.GetCount()) return;
	PlatformThread& thrd = e.threads[thrd_i];
	thrd.title = entry.thread_title.GetData();
	threads.Set(0, thrd.title);
	
	if (!comments.IsCursor())
		return;
	
	int comm_i = comments.Get("IDX");
	int comm_cur = comments.GetCursor();
	PlatformComment& c = thrd.comments[comm_i];
	c.message = entry.message.GetData();
	c.orig_message = entry.orig_message.GetData();
	c.hashtags = entry.hashtags.GetData();
	c.location = entry.location.GetData();
	c.user = entry.user.GetData();
	c.generate = entry.generate.Get();
	
	Date date = entry.date.GetDate();
	Time time = entry.clock.GetTime();
	time.year = date.year;
	time.month = date.month;
	time.day = date.day;
	c.published = time;
	
	comments.Set(2, c.user);
	comments.Set(3, c.message);
	comments.Set(4, c.hashtags);
}

void SocialContent::AddEntry() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	PlatformEntry& e = pld.entries.Add();
	e.threads.Add();
	
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

void SocialContent::AddThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	PlatformEntry& e = pld.entries[entry_i];
	PlatformThread& t = e.threads.Add();
	
	DataEntry();
}

void SocialContent::RemoveThread() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	PlatformEntry& e = pld.entries[entry_i];
	int thrd_i = threads.Get("IDX");
	if (thrd_i >= 0 && thrd_i < e.threads.GetCount())
		e.threads.Remove(thrd_i);
	
	DataEntry();
}

void SocialContent::AddComment() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	PlatformEntry& e = pld.entries[entry_i];
	int thrd_i = threads.Get("IDX");
	PlatformThread& t = e.threads[thrd_i];
	PlatformComment& c = t.comments.Add();
	c.published = GetSysTime();
	
	DataThread();
}

void SocialContent::RemoveComment() {
	MetaPtrs& p = MetaPtrs::Single();
	if (!platforms.IsCursor() || !threads.IsCursor() || !entries.IsCursor())
		return;
	Profile& prof = *p.profile;
	ProfileData& pd = ProfileData::Get(prof);
	int plat_i = platforms.Get("IDX");
	PlatformData& pld = pd.platforms[plat_i];
	int entry_i = entries.Get("IDX");
	PlatformEntry& e = pld.entries[entry_i];
	int thrd_i = threads.Get("IDX");
	PlatformThread& t = e.threads[thrd_i];
	int comm_i = comments.Get("IDX");
	if (comm_i >= 0 && comm_i < t.comments.GetCount())
		t.comments.Remove(entry_i);
	
	DataThread();
}

void SocialContent::EntryListMenu(Bar& bar) {
	bar.Add(t_("Add Entry"), AppImg::BlueRing(), THISBACK(AddEntry)).Key(K_CTRL_W);
	if (entries.IsCursor())
		bar.Add(t_("Remove Entry"), AppImg::BlueRing(), THISBACK(RemoveEntry)).Key(K_CTRL_D);
}

void SocialContent::ThreadListMenu(Bar& bar) {
	bar.Add(t_("Add Thread"), AppImg::BlueRing(), THISBACK(AddThread)).Key(K_CTRL_E);
	if (entries.IsCursor())
		bar.Add(t_("Remove Thread"), AppImg::BlueRing(), THISBACK(RemoveThread)).Key(K_CTRL_F);
}

void SocialContent::CommentListMenu(Bar& bar) {
	bar.Add(t_("Add Comment"), AppImg::BlueRing(), THISBACK(AddComment)).Key(K_CTRL_T);
	if (entries.IsCursor())
		bar.Add(t_("Remove Comment"), AppImg::BlueRing(), THISBACK(RemoveComment)).Key(K_CTRL_H);
}

void SocialContent::ToolMenu(Bar& bar) {
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Clear thread's merged text"), AppImg::BlueRing(), THISBACK1(Do, 2)).Key(K_F7);
}

void SocialContent::Do(int fn) {
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
	else if (fn == 2) {
		if (!platforms.IsCursor() || !threads.IsCursor())
			return;
		Profile& prof = *mp.profile;
		ProfileData& pd = ProfileData::Get(prof);
		int plat_i = platforms.Get("IDX");
		PlatformData& pld = pd.platforms[plat_i];
		int entry_i = entries.Get("IDX");
		PlatformEntry& e = pld.entries[entry_i];
		int thrd_i = threads.Get("IDX");
		PlatformThread& t = e.threads[thrd_i];
		for(int i = 0; i < t.comments.GetCount(); i++)
			t.comments[i].ClearMerged();
	}
}


END_TEXTLIB_NAMESPACE
