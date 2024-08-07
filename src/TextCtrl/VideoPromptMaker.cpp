#include "TextCtrl.h"

BEGIN_TEXTLIB_NAMESPACE


VideoPromptMakerCtrl::VideoPromptMakerCtrl() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() <<	storyboard_parts <<
						storyboard_prompt_split <<
						text_storyboard_parts;
	vsplit.SetPos(2500,0);
	vsplit.SetPos(5000,1);
	
	storyboard_parts.AddColumn(t_(""));
	storyboard_parts.AddColumn(t_(""));
	
	storyboard_prompt_split.Horz() << storyboard_prompt_keys << storyboard_prompt_values;
	storyboard_prompt_keys.AddColumn(t_(""));
	storyboard_prompt_keys.WhenCursor << THISBACK(DataPrompt);
	storyboard_prompt_values.AddColumn(t_(""));
	
	text_storyboard_parts.AddColumn(t_(""));
	text_storyboard_parts.AddColumn(t_(""));
	text_storyboard_parts.AddColumn(t_("Prompt"));
	text_storyboard_parts.ColumnWidths("2 1 4");
	
	
	
}

void VideoPromptMakerCtrl::Data() {
	EditorPtrs& p = GetPointers();
	if (!p.component)
		return;
	
	#if 0
	// Storyboard parts
	for(int i = 0; i < p.component->storyboard_parts.GetCount(); i++) {
		String key = p.component->storyboard_parts.GetKey(i);
		String value = p.component->storyboard_parts[i];
		storyboard_parts.Set(i, 0, key);
		storyboard_parts.Set(i, 1, value);
	}
	INHIBIT_CURSOR_(storyboard_parts, a);
	storyboard_parts.SetCount(p.component->storyboard_parts.GetCount());
	
	// Storyboard prompts
	for(int i = 0; i < p.component->storyboard_prompts.GetCount(); i++) {
		storyboard_prompt_keys.Set(i, 0, p.component->storyboard_prompts.GetKey(i));
	}
	INHIBIT_CURSOR_(storyboard_prompt_keys, b);
	storyboard_prompt_keys.SetCount(p.component->storyboard_prompts.GetCount());
	if (!storyboard_prompt_keys.IsCursor() && storyboard_prompt_keys.GetCount())
		storyboard_prompt_keys.SetCursor(0);
	
	// Text to storyboard parts
	for(int i = 0; i < p.component->text_storyboard_parts.GetCount(); i++) {
		int part_i = p.component->text_storyboard_parts[i];
		const String& part = p.component->storyboard_parts.GetKey(part_i);
		text_storyboard_parts.Set(i, 0, p.component->text_storyboard_parts.GetKey(i));
		text_storyboard_parts.Set(i, 1, part);
		if (i < p.component->text_storyboard_prompts.GetCount())
			text_storyboard_parts.Set(i, 2, p.component->text_storyboard_prompts[i]);
		else
			text_storyboard_parts.Set(i, 2, "");
	}
	INHIBIT_CURSOR_(text_storyboard_parts, c);
	text_storyboard_parts.SetCount(p.component->text_storyboard_parts.GetCount());
	
	#endif
	
	DataPrompt();
}

void VideoPromptMakerCtrl::DataPrompt() {
	EditorPtrs& p = GetPointers();
	if (!p.component || !storyboard_prompt_keys.IsCursor()) {
		storyboard_prompt_values.Clear();
		return;
	}
	
	#if 0
	int cur = storyboard_prompt_keys.GetCursor();
	const auto& values = p.component->storyboard_prompts[cur];
	for(int j = 0; j < values.GetCount(); j++) {
		storyboard_prompt_values.Set(j, 0, values[j]);
	}
	INHIBIT_CURSOR(storyboard_prompt_values);
	storyboard_prompt_values.SetCount(values.GetCount());
	#endif
}

void VideoPromptMakerCtrl::OnValueChange() {
	
}

void VideoPromptMakerCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Make video prompts"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
}

void VideoPromptMakerCtrl::Do(int fn) {
	EditorPtrs& p = GetPointers();
	if (!p.component) return;
	
	if (fn == 0) {
		VideoSolver& tm = VideoSolver::Get(*p.component);
		tm.Start();
	}
}


END_TEXTLIB_NAMESPACE
