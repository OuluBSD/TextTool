#include "SimplifiedCtrl.h"


ProductionIdea::ProductionIdea() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << vsplit;
	hsplit.SetPos(3000);
	vsplit.Vert() << positive << negative;
	
	list.AddColumn(t_("Topic"));
	positive.AddColumn(t_("Positive"));
	negative.AddColumn(t_("Negative"));
	
}

void ProductionIdea::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist) return;
	
	
	/*for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		String key = (String)"PROD_" + Attr::AttrKeys[i][0];
		String value = p.song->data.Get(key, "0");
		int v = StrInt(value);
		int idx = 1 - v;
		DropList* dl = dynamic_cast<DropList*>(this->attrs.GetCtrl(i, 3));
		dl->SetIndex(idx);
	}
	
	result.SetData(p.song->data.Get("PRODUCTION_IDEA", ""));*/
}

void ProductionIdea::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	/*bar.Add(t_("Copy attributes from song, release and artist"), AppImg::RedRing(), THISBACK(CopyAttributes)).Key(K_CTRL_Q);
	bar.Add(t_("Update idea"), AppImg::BlueRing(), THISBACK(GetProductionIdea)).Key(K_CTRL_W);
	*/
}

String ProductionIdea::GetStatusText() {
	return "";
}


#if 0
void ProductionIdea::CopyAttributes() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist) return;
	
	VectorMap<String,String> attrs;
	GetAttrsValue(p.artist->data, attrs);
	GetAttrsValue(p.release->data, attrs);
	GetAttrsValue(p.song->data, attrs);
	
	for(int i = 0; i < attrs.GetCount(); i++)
		p.song->data.GetAdd(
			attrs.GetKey(i)) =
				attrs[i];
	
	PostCallback(THISBACK(Data));
}

void ProductionIdea::GetProductionIdea() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist) return;
	Song& song = *p.song;
	
	/*
	INPUT:
		attributes
		bpm
		structure
		part style (singing / rap)
		chords...
	
	OUTPUT:
		which instruments play
			and what
		what drums play
		which effective tricks
			e.g. drum fills
			e.g. all silent but vocal
		what in effect track
		chords...
	*/
	
	ProductionArgs args;
	
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		String key = (String)"PROD_" + Attr::AttrKeys[i][0];
		String value = p.song->data.Get(key, "0");
		DropList* dl = dynamic_cast<DropList*>(this->attrs.GetCtrl(i, 3));
		int idx = dl->GetIndex();
		int v = 1 - idx;
		if (v < 0)
			args.attrs << Attr::AttrKeys[i][3];
		else if (v > 0)
			args.attrs << Attr::AttrKeys[i][2];
	}
	
	String bpm = p.song->data.Get("BPM", "");
	if (!bpm.IsEmpty())
		args.bpm = StrInt(bpm);
	
	const Song::StructSuggestion& sug = p.song->active_struct;
	
	for(int i = 0; i < p.song->parts.GetCount(); i++) {
		StaticPart& part = p.song->parts[i];
		
		auto& part_arg = args.parts.Add();
		part_arg.name = part.name;
		
		{
			switch (part.part_type) {
				case StaticPart::SINGING: part_arg.type = "singing"; break;
				case StaticPart::RAPPING: part_arg.type = "rapping"; break;
				case StaticPart::POETRY: part_arg.type = "poetry"; break;
				case StaticPart::DIALOG: part_arg.type = "dialog"; break;
				default: break;
			}
		}
		
		/*part.type = sug.parts[i];
		
		if (i < sug.chords.GetCount())
			part_arg.chords = sug.chords[i];*/
	}
	
	song.RealizePipe();
	
	{
		TaskMgr& m = *song.pipe;
		m.GetProductionIdea(args, THISBACK1(OnProductionIdea, &song));
	}
}

void ProductionIdea::GetAttrsValue(const VectorMap<String,String>& data, VectorMap<String,String>& v) {
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		int value = StrInt(data.Get(key, "0"));
		
		if (value) {
			if (value > 0) {
				v.GetAdd((String)"PROD_" + key) = IntStr(value);
			}
			else {
				v.GetAdd((String)"PROD_" + key) = IntStr(value);
			}
		}
	}
}

void ProductionIdea::OnProductionIdea(String result, Song* song) {
	this->result.SetData(result);
	
	
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (p.song)
		p.song->data.GetAdd("PRODUCTION_IDEA") = result;
}

#endif
