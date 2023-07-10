#include "SongTool.h"


AI_Tasks::AI_Tasks() {
	Add(hsplit.SizePos());
	hsplit.Horz() << list << vsplit;
	vsplit.Vert() << input << output;
	
	list.AddColumn(t_("Description"));
	list.AddColumn(t_("Status"));
	list.ColumnWidths("4 1");
	list <<= THISBACK(DataTask);
	
	output <<= THISBACK(ValueChange);
	
	list.WhenBar	<< THISBACK(OutputMenu);
	
}

void AI_Tasks::Data() {
	TaskMgr& m = TaskMgr::Single();
	
	for(int i = 0; i < m.tasks.GetCount(); i++) {
		AI_Task& t = m.tasks[i];
		list.Set(i, 0, t.GetDescription());
		if (t.failed)
			list.Set(i, 1, t_("Error") + String(": ") + t.error);
		else
			list.Set(i, 1, t.ready ? t_("Ready") : "");
	}
	list.SetCount(m.tasks.GetCount());
	
	int cursor = 0;
	if (cursor >= 0 && cursor < list.GetCount())
		list.SetCursor(cursor);
	
	DataTask();
}

void AI_Tasks::DataTask() {
	TaskMgr& m = TaskMgr::Single();
	if (!list.IsCursor())
		return;
	
	int cursor = list.GetCursor();
	AI_Task& t = m.tasks[cursor];
	m.active_task = &t;
	
	input.SetData(t.input);
	output.SetData(t.output);
}

void AI_Tasks::ValueChange() {
	TaskMgr& m = TaskMgr::Single();
	if (!m.active_task)
		return;
	
	AI_Task& t = *m.active_task;
	t.output = output.GetData();
	
	t.Store();
}

void AI_Tasks::ProcessItem() {
	TaskMgr& m = TaskMgr::Single();
	if (!list.IsCursor())
		return;
	int cursor = list.GetCursor();
	AI_Task& t = m.tasks[cursor];
	t.Process();
}

void AI_Tasks::OutputMenu(Bar& bar) {
	bar.Add(t_("Process output"), THISBACK(ProcessItem));
	
}
