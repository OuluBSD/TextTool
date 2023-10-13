#include "SimplifiedCtrl.h"


PartIdea::PartIdea() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << hsplit0 << hsplit1;
	
	hsplit0.Horz() << active << parts;
	hsplit0.SetPos(2500);
	
	hsplit1.Horz() << contents << imageries << symbolisms;
	
	active.AddColumn(t_("Active idea"));
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Content suggestion"));
	parts.AddColumn(t_("Specific imagery"));
	parts.AddColumn(t_("Symbolism"));
	
	contents.AddColumn(t_("Content suggestion"));
	imageries.AddColumn(t_("Specific imagery"));
	symbolisms.AddColumn(t_("Symbolism"));
	
}

void PartIdea::Data() {
	Song& song = GetSong();
	
	
	for(int i = 0; i < IDEAPATH_COUNT; i++) {
		active.Set(i, 0,
			AttrText(song.active_idea[i])
				.NormalPaper(Blend(song.active_idea_clr[i], White(), 128+64))
				.Paper(Blend(song.active_idea_clr[i], GrayColor(), 128+64))
				.Ink(White()).NormalInk(Black()));
	}
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		String k;
		Color clr = White();
		{
			StaticPart& p = song.parts[i];
			k = p.name;
			clr = GetSongPartPaperColor(p.type);
		}
		parts.Set(i, 0,
			AttrText(k)
				.NormalPaper(clr)
				.Paper(Blend(clr, GrayColor(64)))
				.Ink(White()).NormalInk(Black())
			);
	}
	parts.SetCount(song.parts.GetCount());
	
	
}

void PartIdea::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

String PartIdea::GetStatusText() {
	return "";
}
