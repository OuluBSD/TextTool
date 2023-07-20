#include "SongTool.h"


AI_Tasks::AI_Tasks() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(lbl.LeftPos(5, 200-5).BottomPos(0,30));
	Add(prog.HSizePos(200,0).BottomPos(0,30));
	prog.Set(0,1);
	lbl.SetLabel(t_("Idle"));
	
	hsplit.Horz() << list << vsplit;
	vsplit.Vert() << input << output;
	
	list.AddColumn(t_("Description"));
	list.AddColumn(t_("Status"));
	list.AddColumn(t_("Mode"));
	list.ColumnWidths("8 2 1");
	list <<= THISBACK(DataTask);
	
	output <<= THISBACK(ValueChange);
	
	list.WhenBar	<< THISBACK(OutputMenu);
	
}

void AI_Tasks::Data() {
	TaskMgr& m = TaskMgr::Single();
	
	for(int i = 0; i < m.tasks.GetCount(); i++) {
		AI_Task& t = m.tasks[i];
		list.Set(i, 0, t.GetDescription());
		String s;
		if (t.failed)
			s = t_("Error") + String(": ") + t.error;
		else if (t.processing)
			s = t_("Processing");
		else if (t.ready)
			s = t_("Ready");
		if (t.tries > 0)
			s << " (" << t_("tries") << " " << t.tries << ")";
		list.Set(i, 1, s);
		list.Set(i, 2, GetModeString(t.p.mode));
	}
	list.SetCount(m.tasks.GetCount());
	
	int cursor = 0;
	if (!list.IsCursor() && cursor >= 0 && cursor < list.GetCount())
		list.SetCursor(cursor);
	
	prog.Set(m.actual, max(1,m.total));
	lbl.SetLabel(m.status.IsEmpty() ? String(t_("Idle")) : m.status);
	
	DataTask();
}

void AI_Tasks::DataTask() {
	TaskMgr& m = TaskMgr::Single();
	if (!list.IsCursor())
		return;
	
	int cursor = list.GetCursor();
	AI_Task& t = m.tasks[cursor];
	m.active_task = &t;
	
	if (cursor != data_cursor || (output.GetLength() == 0 && t.output.GetCount())) {
		input.SetData(t.input.ToWString());
		output.SetData(t.output.ToWString());
		data_cursor = cursor;
	}
}

void AI_Tasks::ValueChange() {
	TaskMgr& m = TaskMgr::Single();
	if (!m.active_task)
		return;
	
	AI_Task& t = *m.active_task;
	t.output = TrimBoth(output.GetData());
	t.changed = true;
	t.output.Replace("\r", "");
	
	t.Store();
}

void AI_Tasks::ProcessItem() {
	TaskMgr& m = TaskMgr::Single();
	if (!list.IsCursor())
		return;
	int cursor = list.GetCursor();
	AI_Task& t = m.tasks[cursor];
	t.failed = false;
	m.StartSingle(cursor);
}

void AI_Tasks::RetryItem() {
	TaskMgr& m = TaskMgr::Single();
	if (!list.IsCursor())
		return;
	int cursor = list.GetCursor();
	AI_Task& t = m.tasks[cursor];
	t.Retry();
	m.StartSingle(cursor);
	this->output.Clear();
}

void AI_Tasks::OutputMenu(Bar& bar) {
	bar.Add(t_("Process output"), THISBACK(ProcessItem));
	bar.Add(t_("Retry"), THISBACK(RetryItem));
	
}
