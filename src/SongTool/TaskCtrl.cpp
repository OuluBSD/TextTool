#include "SongTool.h"


Tasks::Tasks() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(lbl.LeftPos(5, 200-5).BottomPos(0,30));
	Add(prog.HSizePos(200,0).BottomPos(0,30));
	prog.Set(0,1);
	lbl.SetLabel(t_("Idle"));
	
	hsplit.Horz() << list << vsplit;
	vsplit.Vert() << input << output;
	
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Description"));
	list.AddColumn(t_("Depends on"));
	list.AddColumn(t_("Dep rules"));
	list.AddColumn(t_("Deps waiting"));
	list.AddColumn(t_("Status"));
	list.AddColumn(t_("Context"));
	list.AddColumn(t_("Mode"));
	list.AddColumn(t_("Reverse"));
	list.ColumnWidths("1 4 2 4 2 2 1 1 1");
	list <<= THISBACK(DataTask);
	
	output <<= THISBACK(ValueChange);
	
	list.WhenBar	<< THISBACK(OutputMenu);
	
}

void Tasks::Data() {
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	TODO
	#if 0
	for(int i = 0; i < m.tasks.GetCount(); i++) {
		Task& t = m.tasks[i];
		list.Set(i, 0, i);
		list.Set(i, 1, t.GetDescription());
		list.Set(i, 2, t.GetTaskDependencyString(true, false));
		list.Set(i, 3, t.GetTaskDependencyString(true, true));
		list.Set(i, 4, t.GetTaskDepsWaitingString());
		String s;
		if (t.failed)
			s = t_("Error") + String(": ") + t.error;
		else if (t.processing)
			s = t_("Processing");
		else if (t.ready)
			s = t_("Ready");
		else if (t.is_waiting_deps)
			s = t_("Waiting dependencies");
		if (t.tries > 0)
			s << " (" << t_("tries") << " " << t.tries << ")";
		list.Set(i, 5, s);
		list.Set(i, 6, GetGroupContextString(t.p.a.ctx));
		list.Set(i, 7, GetModeString(t.p.a.mode));
		list.Set(i, 8, GetDirectionString(t.p.a.dir));
	}
	list.SetCount(m.tasks.GetCount());
	
	int cursor = 0;
	if (!list.IsCursor() && cursor >= 0 && cursor < list.GetCount())
		list.SetCursor(cursor);
	
	prog.Set(m.actual, max(1,m.total));
	lbl.SetLabel(m.status.IsEmpty() ? String(t_("Idle")) : m.status);
	
	DataTask();
	#endif
}

void Tasks::DataTask() {
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	TODO
	#if 0
	if (!list.IsCursor())
		return;
	
	int cursor = list.GetCursor();
	Task& t = m.tasks[cursor];
	m.active_task = &t;
	
	if (cursor != data_cursor || (output.GetLength() == 0 && t.output.GetCount())) {
		input.SetData(t.input.ToWString());
		output.SetData(t.output.ToWString());
		data_cursor = cursor;
	}
	#endif
}

void Tasks::ValueChange() {
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	TODO
	#if 0
	if (!m.active_task)
		return;
	
	Task& t = *m.active_task;
	t.output = TrimBoth(output.GetData());
	t.changed = true;
	t.output.Replace("\r", "");
	
	t.Store();
	#endif
}

void Tasks::ProcessItem() {
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	TODO
	#if 0
	if (!list.IsCursor())
		return;
	int cursor = list.GetCursor();
	Task& t = m.tasks[cursor];
	t.failed = false;
	m.StartSingle(cursor);
	#endif
}

void Tasks::RetryItem() {
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	TODO
	#if 0
	if (!list.IsCursor())
		return;
	int cursor = list.GetCursor();
	Task& t = m.tasks[cursor];
	t.Retry();
	m.StartSingle(cursor);
	this->output.Clear();
	#endif
}

void Tasks::OutputMenu(Bar& bar) {
	bar.Add(t_("Process output"), THISBACK(ProcessItem));
	bar.Add(t_("Retry"), THISBACK(RetryItem));
	
}
