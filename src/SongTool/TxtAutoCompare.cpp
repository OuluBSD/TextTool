#include "SongTool.h"


TxtAutoCompare::TxtAutoCompare() {
	unpacked_struct_key = "ENGLISH_UNPACKED_STRUCTURE";
	auto_poetic_styles_english_key = "POETIC_ENGLISH_STYLES";
	main_key = unpacked_struct_key;
	other_key = auto_poetic_styles_english_key;
	
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	hsplit << main << vsplit;
	vsplit.Vert() << lines << line_results << best;
	
	rhymetype.Add(t_("2 end rhymes in 1 line"));
	rhymetype.Add(t_("2 end rhymes in 2 lines"));
	rhymetype.Add(t_("3 end rhymes in 2 lines"));
	rhymetype.Add(t_("4 end rhymes in 2 lines"));
	rhymetype.Add(t_("2 end rhymes in 4 lines"));
	rhymetype.Add(t_("3 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 end rhymes in 4 lines"));
	rhymetype.Add(t_("2 begin rhymes and 2 end rhymes in 2 lines"));
	rhymetype.Add(t_("2 begin rhymes and 4 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 begin rhymes and 2 end rhymes in 4 lines"));
	rhymetype.Add(t_("4 begin rhymes and 4 end rhymes in 4 lines"));
	rhymetype.SetIndex(1);
	
}

void TxtAutoCompare::Init() {
	
}

void TxtAutoCompare::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist)
		return;
	Song& song = *p.song;
	
	if (!has_init)
		Init();
	
	
	String main_txt = song.data.Get(main_key, "");
	main.SetData(main_txt);
	
}

void TxtAutoCompare::DoMainAction(int i) {
	
}

void TxtAutoCompare::ToolMenu(Bar& bar) {
	Size sz = bar.GetStdSize();
	sz.cx = 300;
	bar.AddTool(rhymetype, sz);
	bar.Add(AppImg::Part(), t_("Evaluate different poetic styles"), THISBACK(EvaluatePoeticStyles));
}

void TxtAutoCompare::EvaluatePoeticStyles() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if(!p.song || !p.artist || main_key.IsEmpty())
		return;
	
	p.RealizePipe();
	
	{
		String style = rhymetype.GetValue();
		
		TaskMgr& m = *p.song->pipe;
		m.EvaluatePoeticStyles(style, main_key, auto_poetic_styles_english_key, THISBACK(PostOnPoeticRecv));
	}
}

void TxtAutoCompare::OnPoeticRecv() {
	
	
	
}
