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
	EditorPtrs& p = db.ctx.ed;
	if (!p.song)
		return;
	
	for(int i = 0; i < HUMAN_INPUT_MODE_COUNT; i++) {
		a[i] = SnapArg(CTX_TEXT, (SnapMode)i, FORWARD);
		input[i].SetData(p.song->content[a[i]]);
	}
}

void ImportCtrl::OnValueChange()
{
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song)
		return;

	for(int i = 0; i < HUMAN_INPUT_MODE_COUNT; i++)
		p.song->content[a[i]] = input[i].GetData();
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
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	// Ptrs& p_rev = db.ctx[MALE_REVERSED];
	if(!p.song || !p.artist)
		return;
	
	Pipe& pipe = db.pipes.Add();
	
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	Song& song = *p.song;
	song.pipe = &pipe;
	pipe.song = &song;
	
	
	// Copy data from Song to Pipe
	pipe.vocalist_visual = a.vocalist_visual;
	for (const SnapArg& a : HumanInputTextArgs()) {
		pipe.content[a] = song.content[a];
	}
	pipe.Attributes::Realize();
	
	
	messages.Clear();

	TaskMgr& m = pipe;
	m.ImportSongAndMakeReversedSong(pipe);
}
