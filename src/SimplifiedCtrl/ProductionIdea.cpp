#include "SimplifiedCtrl.h"


ProductionIdeaPage::ProductionIdeaPage() {
	Add(mainsplit.SizePos());
	
	mainsplit.Vert() << hsplit << active_list;
	mainsplit.SetPos(7500);
	
	hsplit.Horz() << list << vsplit;
	hsplit.SetPos(3000);
	vsplit.Vert() << positive << negative;
	
	list.AddColumn(t_("Topic"));
	list.AddIndex("IDX");
	list.WhenCursor << THISBACK(DataTopic);
	
	positive.AddColumn(t_("Positive"));
	negative.AddColumn(t_("Negative"));
	
	active_list.AddColumn(t_("Topic"));
	active_list.AddColumn(t_("Type"));
	active_list.AddColumn(t_("Value"));
	active_list.AddIndex("IDX");
	active_list.ColumnWidths("4 1 12");
	
}

void ProductionIdeaPage::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.release || !p.artist) return;
	
	
	const Vector<ProductionIdeaTopic>& v = ProductionIdeas();
	
	for(int i = 0; i < v.GetCount(); i++) {
		const ProductionIdeaTopic& id = v[i];
		list.Set(i, 0, Capitalize(id.title));
		list.Set(i, "IDX", i);
	}
	list.SetSortColumn(0);
	if (list.GetCount() && !list.IsCursor())
		list.SetCursor(0);
	
	DataTopic();
	DataActive();
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

void ProductionIdeaPage::DataTopic() {
	
	if (!list.IsCursor()) {
		positive.Clear();
		negative.Clear();
		return;
	}
	
	int cur = list.Get("IDX");
	const Vector<ProductionIdeaTopic>& v = ProductionIdeas();
	const ProductionIdeaTopic& id = v[cur];
	
	positive.SetCount(id.positive.GetCount());
	for(int i = 0; i < id.positive.GetCount(); i++) {
		positive.Set(i, 0, id.positive[i]);
	}
	
	negative.SetCount(id.negative.GetCount());
	for(int i = 0; i < id.negative.GetCount(); i++) {
		negative.Set(i, 0, id.negative[i]);
	}
}

void ProductionIdeaPage::DataActive() {
	Song& s = GetSong();
	
	
	active_list.SetCount(s.prod_ideas.GetCount());
	for(int i = 0; i < s.prod_ideas.GetCount(); i++) {
		const ProductionIdea& id = s.prod_ideas[i];
		active_list.Set(i, 0, id.topic);
		active_list.Set(i, 1, id.is_positive ? t_("Positive") : t_("Negative"));
		active_list.Set(i, 2, id.value);
		active_list.Set(i, "IDX", i);
	}
	active_list.SetSortColumn(0);
	
	if (active_list.GetCount() && !active_list.IsCursor())
		active_list.SetCursor(0);
}

void ProductionIdeaPage::AddActive(bool is_positive) {
	if (!list.IsCursor()) return;
	String topic = this->list.Get(0);
	String value;
	if (is_positive) {
		if (!positive.IsCursor()) return;
		value = this->positive.Get(0);
	}
	else {
		if (!negative.IsCursor()) return;
		value = this->negative.Get(0);
	}
	
	Song& s = GetSong();
	
	for(int i = 0; i < s.prod_ideas.GetCount(); i++) {
		const ProductionIdea& id = s.prod_ideas[i];
		if (id.topic == topic && id.value == value)
			return;
	}
	
	ProductionIdea& id = s.prod_ideas.Add();
	id.topic = topic;
	id.value = value;
	id.is_positive = is_positive;
	
	PostCallback(THISBACK(DataActive));
}

void ProductionIdeaPage::RemoveActive() {
	if (!active_list.IsCursor())
		return;
	
	int cur = active_list.Get("IDX");
	
	Song& s = GetSong();
	if (cur >= 0 && cur < s.prod_ideas.GetCount())
		s.prod_ideas.Remove(cur);
	
	PostCallback(THISBACK(DataActive));
}

void ProductionIdeaPage::RandomizeActive() {
	const Vector<ProductionIdeaTopic>& ideas = ProductionIdeas();
	
	if (!active_list.IsCursor())
		return;
	
	int cur = active_list.Get("IDX");
	
	PostCallback(THISBACK(Data)); // before calling FocusActiveIdea
	
	Song& s = GetSong();
	if (cur >= 0 && cur < s.prod_ideas.GetCount()) {
		const auto& o = s.prod_ideas[cur];
		String topic = ToLower(o.topic);
		bool is_positive = o.is_positive;
		s.prod_ideas.Remove(cur);
		
		for (const ProductionIdeaTopic& id : ideas) {
			if (ToLower(id.title) == topic) {
				if (is_positive && id.positive.GetCount()) {
					ProductionIdea& new_id = s.prod_ideas.Add();
					new_id.topic = topic;
					new_id.is_positive = is_positive;
					int idx = Random(id.positive.GetCount());
					new_id.value = id.positive[idx];
					PostCallback(THISBACK2(FocusActiveIdea, new_id.topic, new_id.value));
				}
				else if (!is_positive && id.negative.GetCount()) {
					ProductionIdea& new_id = s.prod_ideas.Add();
					new_id.topic = topic;
					new_id.is_positive = is_positive;
					int idx = Random(id.negative.GetCount());
					new_id.value = id.negative[idx];
					PostCallback(THISBACK2(FocusActiveIdea, new_id.topic, new_id.value));
				}
				break;
			}
		}
	}
}

void ProductionIdeaPage::RandomizeAll(bool skip_existing_topic) {
	const Vector<ProductionIdeaTopic>& ideas = ProductionIdeas();
	Song& s = GetSong();
	if (!skip_existing_topic)
		s.prod_ideas.Clear();
	for(int i = 0; i < ideas.GetCount(); i++) {
		const ProductionIdeaTopic& id = ideas[i];
		String topic = ToLower(id.title);
		for(int j = 0; j < 2; j++) {
			bool is_positive = !j;
			
			if (skip_existing_topic) {
				bool skip = false;
				for(int k = 0; k < s.prod_ideas.GetCount(); k++) {
					const ProductionIdea& id = s.prod_ideas[k];
					if (id.topic == topic && id.is_positive == is_positive) {
						skip = true;
						break;
					}
				}
				if (skip) continue;
			}
			
			if (is_positive && id.positive.GetCount()) {
				ProductionIdea& new_id = s.prod_ideas.Add();
				int idx = Random(id.positive.GetCount());
				new_id.topic = topic;
				new_id.value = id.positive[idx];
				new_id.is_positive = true;
			}
			
			if (!is_positive && id.negative.GetCount()) {
				ProductionIdea& new_id = s.prod_ideas.Add();
				int idx = Random(id.negative.GetCount());
				new_id.topic = topic;
				new_id.value = id.negative[idx];
				new_id.is_positive = false;
			}
		}
	}
	
	PostCallback(THISBACK(Data));
}

void ProductionIdeaPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Add positive"), AppImg::BlueRing(), THISBACK1(AddActive, true)).Key(K_CTRL_Q);
	bar.Add(t_("Add negative"), AppImg::BlueRing(), THISBACK1(AddActive, false)).Key(K_CTRL_W);
	bar.Add(t_("Remove active"), AppImg::BlueRing(), THISBACK(RemoveActive)).Key(K_CTRL_E);
	bar.Add(t_("Randomize selected active"), AppImg::BlueRing(), THISBACK(RandomizeActive)).Key(K_CTRL_R);
	bar.Add(t_("Randomize all"), AppImg::RedRing(), THISBACK1(RandomizeAll, false)).Key(K_F5);
	bar.Add(t_("Randomize missing"), AppImg::RedRing(), THISBACK1(RandomizeAll, true)).Key(K_F6);
	
}

void ProductionIdeaPage::FocusActiveIdea(String topic, String value) {
	for(int i = 0; i < active_list.GetCount(); i++) {
		if (active_list.Get(i, 0) == topic && active_list.Get(i, 2) == value) {
			active_list.SetCursor(i);
			return;
		}
	}
}

String ProductionIdeaPage::GetStatusText() {
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
