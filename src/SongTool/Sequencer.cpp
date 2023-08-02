#include "SongTool.h"


Sequencer::Sequencer() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << headers << lines;
	hsplit.SetPos(1500);
	
	AddFrame(vsb.Vert());
	AddFrame(hsb.Horz());
	
	vsb.WhenScroll = [=] { Refresh(); };
	hsb.WhenScroll = [=] { Refresh(); };
	
}

void Sequencer::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	
	if (!p.song)
		return;
	Song& song = *p.song;
	
	RealizeSubCtrls(headers, headers.list, song.tracks);
	for(int i = 0; i < headers.list.GetCount(); i++) {
		SeqLineHeaderCtrl& header = headers.list[i];
		Track& track = song.tracks[i];
		header.track = &track;
	}
	
	RealizeSubCtrls(lines, lines.list, song.tracks);
	for(int i = 0; i < lines.list.GetCount(); i++) {
		SeqLineCtrl& line = lines.list[i];
		Track& track = song.tracks[i];
		line.track = &track;
	}
	
	DataContent();
}

void Sequencer::DataContent() {
	for (SeqLineHeaderCtrl& header : headers.list)
		header.Data();
	for (SeqLineCtrl& line : lines.list)
		line.Data();
}

void Sequencer::Layout() {
	SequencerParams& p = SequencerParams::Single();
	vsb.SetLine(p.line_height);
	vsb.SetPage(GetSize().cy);
	hsb.SetLine(p.line_height);
	hsb.SetPage(GetSize().cx);
}

void Sequencer::MouseWheel(Point, int zdelta, dword c) {
	if (c & K_SHIFT)
		hsb.Wheel(zdelta);
	else
		vsb.Wheel(zdelta);
}

bool Sequencer::Key(dword key, int) {
	return vsb.VertKey(key) || hsb.HorzKey(key);
}
