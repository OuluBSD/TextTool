#include "SimplifiedCtrl.h"

ActionEditor::ActionEditor() {
	Add(hsplit.Horz());
	hsplit.Horz() << vsplit << mainsplit;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << parts << thrds;
	
	mainsplit.Horz() << main_vsplit << song_actions;
	
	main_vsplit.Vert() << thrd_actions << sug_hsplit;
	main_vsplit.SetPos(2000);
	
	sug_hsplit.Horz() << sug_vsplit << args;
	sug_hsplit.SetPos(3000);
	
	sug_vsplit.Vert() << actions << attrs << colors;
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Lines"));
	parts.AddColumn(t_("First part"));
	parts.ColumnWidths("4 1 1");
	parts.WhenCursor << THISBACK(DataPart);
	
	thrds.AddColumn(t_("Thread"));
	thrds.WhenCursor << THISBACK(DataThread);
	
	thrd_actions.AddColumn(t_("Action"));
	thrd_actions.AddColumn(t_("Arg"));
	thrd_actions.AddColumn(t_("Attribute group"));
	thrd_actions.AddColumn(t_("Attribute value"));
	thrd_actions.WhenCursor = THISBACK(DataSuggestions);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("4 1");
	actions.WhenCursor = THISBACK(DataAction);
	
	
	args.AddColumn(t_("Action"));
	args.AddColumn(t_("Arg"));
	args.AddColumn(t_("Group"));
	args.AddColumn(t_("Value"));
	
	song_actions.AddColumn(t_(""));
	
	colors.AddColumn(t_("Color"));
	colors.AddIndex("IDX");
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
		colors.Set(1+i, "IDX", i);
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	colors.WhenCursor << THISBACK(DataAction);
	
	int gi = 0;
	int i = 0;
	attrs.AddColumn(t_("Attribute keys"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.Set(i, 0, "All");
	attrs.Set(i, 1, "All");
	attrs.Set(i, "GROUP", -1);
	attrs.Set(i, "VALUE", -1);
	i++;
	
	#define ATTR_ITEM(e, g, i0, i1) \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i0); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 0); \
		i++; \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i1); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 1); \
		i++, gi++;
	ATTR_LIST
	#undef ATTR_ITEM
	attrs.WhenCursor << THISBACK(DataAction);

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
}

void ActionEditor::Data() {
	Song& song = GetSong();
	
	{
		Database& db = Database::Single();
		SongData& sd = db.song_data;
		SongDataAnalysis& sda = db.song_data.a;
		DatasetAnalysis& da = sda.datasets[ds_i];
		
		uniq_acts.Clear();
		for (const ActionPhrase& ap : da.action_phrases) {
			for (const auto& a : ap.actions)
				uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
		}
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	
	int get_first_part_i = song.GetFirstPartPosition();
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		parts.Set(i, 0,
			AttrText(sp.name).NormalPaper(GetSongPartPaperColor(sp.type)));
		parts.Set(i, 1, sp.nana.Get().GetCount());
		parts.Set(i, 2, i == get_first_part_i ? "X" : "");
	}
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
	DataSong();
}

void ActionEditor::DataPart() {
	if (!parts.IsCursor()) {
		thrds.Clear();
		thrd_actions.Clear();
		actions.Clear();
		args.Clear();
		return;
	}
	
	Song& song = GetSong();
	StaticPart& part = song.parts[parts.GetCursor()];
	
	if (part.thrd_actions.GetCount() < def_thread_count)
		part.thrd_actions.SetCount(def_thread_count);
	
	for(int i = 0; i < part.thrd_actions.GetCount(); i++) {
		thrds.Set(i, 0, IntStr(i));
	}
	thrds.SetCount(part.thrd_actions.GetCount());
	if (!thrds.IsCursor() && thrds.GetCount())
		thrds.SetCursor(0);
	
	DataThread();
}

void ActionEditor::DataThread() {
	if (!parts.IsCursor() || !thrds.IsCursor()) {
		actions.Clear();
		args.Clear();
		return;
	}
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (!parts.IsCursor() || !thrds.IsCursor()) {
		thrd_actions.Clear();
		return;
	}
	
	Song& song = GetSong();
	StaticPart& part = song.parts[parts.GetCursor()];
	int thrd_i = thrds.GetCursor();
	const Vector<ActionHeader>& part_thrd_actions = part.thrd_actions[thrd_i];
	int row = 0;
	
	for(int i = 0; i < part_thrd_actions.GetCount(); i++) {
		const ActionHeader& ah = part_thrd_actions[i];
		int j = da.action_attrs.Find(ah);
		if (j < 0) {
			thrd_actions.Set(row, 0, ah.action);
			thrd_actions.Set(row, 1, ah.arg);
			thrd_actions.Set(row, 2, "");
			thrd_actions.Set(row, 3, "");
		}
		else {
			const ActionAttrs& aa = da.action_attrs[j];
			SetColoredListValue(thrd_actions, row, 0, ah.action, aa.clr);
			SetColoredListValue(thrd_actions, row, 1, ah.arg, aa.clr);
			SetColoredListValue(thrd_actions, row, 2, aa.group, aa.clr);
			SetColoredListValue(thrd_actions, row, 3, aa.value, aa.clr);
		}
		row++;
	}
	thrd_actions.SetCount(part_thrd_actions.GetCount());
	if (thrd_actions.GetCount())
		thrd_actions.SetCursor(thrd_actions.GetCount()-1);
	
	DataSuggestions();
}

void ActionEditor::DataSuggestions() {
	if (!parts.IsCursor() || !thrds.IsCursor()) {
		actions.Clear();
		args.Clear();
		return;
	}
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	Song& song = GetSong();
	StaticPart& sp = song.parts[parts.GetCursor()];
	
	
	//// Check if previous part has been made
	
	// Find previous parts
	bool previous_ready = true;
	Vector<int> part_pos = song.GetPartPositions(sp);
	int reason = -1;
	for (int pos : part_pos) {
		if (pos == 0) continue;
		pos--;
		String prev_type = song.active_struct.parts[pos];
		StaticPart* prev = song.FindPartByType(prev_type);
		ASSERT(prev); if (!prev) continue;
		
		// Check if part is skipped
		if (prev->part_type == StaticPart::SKIP)
			continue;
		
		// Check that nana-code exists
		const auto& nana_lines = prev->nana.Get();
		if (nana_lines.IsEmpty()) {
			previous_ready = false; reason = 0;
			break;
		}
		
		// Check threads exists
		if (prev->thrd_actions.IsEmpty()) {
			previous_ready = false; reason = 1;
			break;
		}
		
		// Check that nana line count matches thread line count
		const auto& thrd0 = prev->thrd_actions[0];
		if (thrd0.GetCount() != nana_lines.GetCount()) {
			previous_ready = false; reason = 2;
			break;
		}
		
		// Check that last nana-code line has at least 1 action
		const auto& last_action = thrd0.Top();
		if (last_action.IsEmpty()) {
			previous_ready = false; reason = 3;
			break;
		}
		
		// No problem. Continue.
		
	}
	
	if (!previous_ready) {
		LOG("warning: ActionEditor::DataSuggestions: previous part not ready: reason " << reason);
		actions.Clear();
		args.Clear();
		return;
	}
	
	Vector<int> prev_parts = song.GetPreviousParts(sp);
	
	if (prev_parts.IsEmpty()) {
		// Add all actions
		mode_all_args = true;
		
		actions.Set(0, 0, t_("All"));
		for(int i = 0; i < uniq_acts.GetCount(); i++) {
			String key = uniq_acts.GetKey(i);
			int count = uniq_acts[i].GetCount();
			actions.Set(1+i, 0, key);
			actions.Set(1+i, 1, count);
		}
		actions.SetCount(uniq_acts.GetCount());
		if (actions.GetCount() && !actions.IsCursor())
			actions.SetCursor(0);
		
	}
	else {
		
	}
	
	DataAction();
}

void ActionEditor::DataAction() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	if (!actions.IsCursor()) {
		args.Clear();
		return;
	}
	
	Vector<int> attr_ids, clr_ids, sc_ids;
	
	// Filter attribute
	bool filter_attr = attrs.GetCursor() > 0;
	String attr_group = attrs.Get(0);
	String attr_value = attrs.Get(1);
	
	// Filter color
	bool filter_color = colors.GetCursor() > 0;
	int color_group = colors.GetCursor() - 1;
	
	// Filter action
	String action = actions.Get(0);
	bool filter_action = actions.GetCursor() != 0;
	
	int row = 0;
	if (mode_all_args) {
		for(int i = 0; i < da.action_attrs.GetCount(); i++) {
			const ActionHeader& ah = da.action_attrs.GetKey(i);
			
			if (filter_action && ah.action != action)
				continue;
			
			const ActionAttrs& aa = da.action_attrs[i];
			
			if (filter_color) {
				int cg = GetColorGroup(aa.clr);
				if (cg != color_group)
					continue;
			}
			
			if (filter_attr) {
				if (aa.group != attr_group || aa.value != attr_value)
					continue;
			}
			
			SetColoredListValue(args, row, 0, ah.action, aa.clr);
			SetColoredListValue(args, row, 1, ah.arg, aa.clr);
			SetColoredListValue(args, row, 2, aa.group, aa.clr);
			SetColoredListValue(args, row, 3, aa.value, aa.clr);
			
			row++;
		}
	}
	
	args.SetCount(row);
	
}

void ActionEditor::DataSong() {
	
}

void ActionEditor::AddAction() {
	if (!args.IsCursor()) return;
	int arg_i = args.GetCursor();
	
	if (!parts.IsCursor() || !thrds.IsCursor()) {
		return;
	}
	Song& song = GetSong();
	StaticPart& sp = song.parts[parts.GetCursor()];
	int thrd_i = thrds.GetCursor();
	Vector<ActionHeader>& part_thrd_actions = sp.thrd_actions[thrd_i];
	
	// Enough actions
	if (part_thrd_actions.GetCount() >= sp.nana.Get().GetCount())
		return;
	
	ActionHeader& ah = part_thrd_actions.Add();
	AttrText action = args.Get(arg_i, 0);
	AttrText arg = args.Get(arg_i, 1);
	ah.action = action.text.ToString();
	ah.arg = arg.text.ToString();
	
	PostCallback(THISBACK(DataThread));
}

void ActionEditor::RemoveAction() {
	Song& song = GetSong();
	StaticPart& sp = song.parts[parts.GetCursor()];
	int thrd_i = thrds.GetCursor();
	Vector<ActionHeader>& part_thrd_actions = sp.thrd_actions[thrd_i];
	
	// Enough actions
	if (part_thrd_actions.IsEmpty())
		return;
	
	part_thrd_actions.Remove(part_thrd_actions.GetCount()-1);
	PostCallback(THISBACK(DataThread));
}

void ActionEditor::ToolMenu(Bar& bar) {
	bar.Add(t_("Add action"), AppImg::BlueRing(), THISBACK(AddAction)).Key(K_CTRL_Q);
	bar.Add(t_("Remove last action"), AppImg::BlueRing(), THISBACK(RemoveAction)).Key(K_CTRL_W);
	
}

