#include "SongTool.h"

ImportCtrl::ImportCtrl()
{
	CtrlLayout(*this);

	messages.AddColumn(t_("Line"));
	messages.AddColumn(t_("Severity"));
	messages.AddColumn(t_("Message"));
	messages.ColumnWidths("1 3 15");

	make_tasks <<= THISBACK(MakeTasks);

	vsplit.Vert() << hsplit << messages;
	hsplit.Horz();
	for(int i = 0; i < HUMAN_INPUT_MODE_COUNT; i++) {
		input[i] <<= THISBACK(OnValueChange);
		hsplit << input[i];
	}
	vsplit.SetPos(8000);
}

void ImportCtrl::Data()
{
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if(!p.song)
		return;
	if(!p.a.Is())
		return;
	
	for(int i = 0; i < HUMAN_INPUT_MODE_COUNT; i++) {
		a[i] = p.a;
		a[i].ctx = CTX_TEXT;
		a[i].mode = (SnapMode)i;
		input[i].SetData(p.song->headers[a[i]].content);
	}
}

void ImportCtrl::OnValueChange()
{
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if(!p.song)
		return;

	for(int i = 0; i < HUMAN_INPUT_MODE_COUNT; i++)
		p.song->headers[a[i]].content = input[i].GetData();
}

/*void ImportCtrl::OutputBar(Bar& bar) {
    bar.Add(t_("Remove line"), THISBACK(RemoveLine));
}*/

/*void ImportCtrl::RemoveLine() {
    Database& db = Database::Single();
    if (!p.song || !output.IsCursor())
        return;

    int cursor = output.GetCursor();
    if (cursor >= 0 && cursor < db.active.song->unique_lines.GetCount()) {
        db.active.song->unique_lines.Remove(cursor);
        Data();
    }
}*/

void ImportCtrl::AddMessage(int line, int severity, String msg)
{
	String sev;
	switch(severity) {
	case 0:
		sev = "info";
		break;
	case 1:
		sev = "warning";
		break;
	case 2:
		sev = "error";
		break;
	}
	messages.Add(line, sev, msg);
}

void ImportCtrl::MakeTasks()
{
	TaskMgr& m = TaskMgr::Single();
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	// Ptrs& p_rev = db.ctx[MALE_REVERSED];
	if(!p.song || !p.artist)
		return;
	Song& s = *p.song;
	Artist& a = *p.artist;
	Release& r = *p.release;

	/*Song& rs = r.RealizeReversed(s);
	p_rev.Ptrs::Clear();
	p_rev.artist = &a;
	p_rev.release = &r;
	p_rev.song = &rs;*/

	messages.Clear();

	m.ImportSongAndMakeReversedSong(s);
}
