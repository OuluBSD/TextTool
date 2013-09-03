#include "SimplifiedCtrl.h"


AttributeDistribution::AttributeDistribution() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << attrs << vsplit;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << active << positive << negative;
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.WhenCursor << THISBACK(DataSuggestions);
	
	active.AddColumn(t_("Group"));
	active.AddColumn(t_("Value"));
	active.AddColumn(t_("Percent"));
	active.AddIndex("IDX");
	active.ColumnWidths("2 2 1");
	
	positive.AddColumn(t_("Group #1"));
	positive.AddColumn(t_("Value #1"));
	positive.AddColumn(t_("Percent #1"));
	positive.AddColumn(t_("Group #2"));
	positive.AddColumn(t_("Value #2"));
	positive.AddColumn(t_("Percent #2"));
	positive.AddColumn(t_("Group #3"));
	positive.AddColumn(t_("Value #3"));
	positive.AddColumn(t_("Percent #3"));
	positive.AddColumn(t_("Artist"));
	positive.AddColumn(t_("Song"));
	positive.ColumnWidths("3 3 1 3 3 1 3 3 1 3 3");
	positive.AddIndex("DATASET");
	positive.AddIndex("ARTIST");
	positive.AddIndex("SONG");
	positive.AddIndex("IDX");
	
	negative.AddColumn(t_("Group #1"));
	negative.AddColumn(t_("Value #1"));
	negative.AddColumn(t_("Percent #1"));
	negative.AddColumn(t_("Group #2"));
	negative.AddColumn(t_("Value #2"));
	negative.AddColumn(t_("Percent #2"));
	negative.AddColumn(t_("Group #3"));
	negative.AddColumn(t_("Value #3"));
	negative.AddColumn(t_("Percent #3"));
	negative.AddColumn(t_("Artist"));
	negative.AddColumn(t_("Song"));
	negative.ColumnWidths("3 3 1 3 3 1 3 3 1 3 3");
	negative.AddIndex("DATASET");
	negative.AddIndex("ARTIST");
	negative.AddIndex("SONG");
	negative.AddIndex("IDX");
	
}

void AttributeDistribution::DisableAll() {
	disabled = true;
	
}

void AttributeDistribution::EnableAll() {
	disabled = false;
	
}

void AttributeDistribution::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!db.ctx.ed.artist || !db.ctx.ed.release || !db.ctx.ed.song)
		return;
	Artist& artist = *db.ctx.ed.artist;
	Release& release = *db.ctx.ed.release;
	Song& song = *db.ctx.ed.song;
	
	active_attrs.Clear();
	free_attrs.Clear();
	
	int row = 0;
	for(int i = 0; i < Attr::ATTR_COUNT; i++) {
		const char* key = Attr::AttrKeys[i][0];
		const char* group = Attr::AttrKeys[i][1];
		const char* v0 = Attr::AttrKeys[i][2];
		const char* v1 = Attr::AttrKeys[i][3];
		
		int value = StrInt(song.data.Get(key, "0"));
		if (!value) value = StrInt(release.data.Get(key, "0"));
		if (!value) value = StrInt(artist.data.Get(key, "0"));
		
		bool accept_k0 = value == 1;
		bool accept_k1 = value == -1;
		bool has_k0 = false;
		bool has_k1 = false;
		for(const AttrProbability& p : song.active_roles) {
			if (p.group == group) {
				if (p.value == v0) {
					has_k0 = true;
					if (has_k1) break;
				}
				else if (p.value == v1) {
					has_k1 = true;
					if (has_k0) break;
				}
			}
		}
		
		if (!has_k0 && accept_k0) {
			attrs.Set(row, 0, group);
			attrs.Set(row, 1, v0);
			attrs.Set(row, "GROUP", i);
			attrs.Set(row, "VALUE", 0);
			row++;
			free_attrs.GetAdd(group).Add(v0);
		}
		
		if (!has_k1 && accept_k1) {
			attrs.Set(row, 0, group);
			attrs.Set(row, 1, v1);
			attrs.Set(row, "GROUP", i);
			attrs.Set(row, "VALUE", 1);
			row++;
			free_attrs.GetAdd(group).Add(v1);
		}
		if (has_k0 && accept_k0)
			active_attrs.GetAdd(group).Add(v0);
		if (has_k1 && accept_k1)
			active_attrs.GetAdd(group).Add(v1);
	}
	attrs.SetCount(row);
	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	row = 0;
	for(const AttrProbability& p : song.active_roles) {
		bool found = false;
		int i = active_attrs.Find(p.group);
		if (i >= 0)
			found = active_attrs[i].Find(p.value) >= 0;
		active.Set(row, 0, p.group);
		active.Set(row, 1, p.value);
		if (!found) {
			active.Set(row, 2, AttrText(IntStr(p.percent)).NormalPaper(LtRed()));
		}
		else {
			active.Set(row, 2, p.percent);
		}
		row++;
	}
	active.SetCount(song.active_roles.GetCount());
	
	DataSuggestions();
}

void AttributeDistribution::DataSuggestions() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Song& song = GetSong();
	
	if (!attrs.IsCursor()) {
		positive.Clear();
		negative.Clear();
		return;
	}
	
	int attr_i = attrs.GetCursor();
	String group = attrs.Get(0);
	String value = attrs.Get(1);
	
	int prow = 0, nrow = 0;
	for(int da_i = 0; da_i < sda.datasets.GetCount(); da_i++) {
		DatasetAnalysis& da = sda.datasets[da_i];
		for(int i = 0; i < da.artists.GetCount(); i++) {
			ArtistAnalysis& artist = da.artists[i];
			const String& artist_name = da.artists.GetKey(i);
			for(int j = 0; j < artist.songs.GetCount(); j++) {
				LyricsAnalysis& song = artist.songs[j];
				for(int k = 0; k < song.positive_roles.GetCount(); k++) {
					Vector<LyricsAnalysis::Role>& v = song.positive_roles[k];
					if (v.GetCount() < 2) continue;
					/*bool is_partially_added = false;
					for (const LyricsAnalysis::Role& r : v) {
						int l = active_attrs.Find(r.group);
						if (l >= 0 && active_attrs[l].Find(r.value) >= 0) {
							is_partially_added = true;
							break;
						}
					}
					if (is_partially_added)
						continue;*/
					bool is_matching_cursor = false;
					for (const LyricsAnalysis::Role& r : v) {
						if (r.group == group && r.value == value) {
							is_matching_cursor = true;
							break;
						}
					}
					if (!is_matching_cursor)
						continue;
					bool is_free_and_matching = true;
					for (const LyricsAnalysis::Role& r : v) {
						int l = free_attrs.Find(r.group);
						if (l < 0 || free_attrs[l].Find(r.value) < 0) {
							is_free_and_matching = false;
							break;
						}
					}
					if (!is_free_and_matching)
						continue;
					for (int col = 0; col < v.GetCount() && col < 3; col++) {
						LyricsAnalysis::Role& r = v[col];
						positive.Set(prow, col*3+0, r.group);
						positive.Set(prow, col*3+1, r.value);
						positive.Set(prow, col*3+2, r.percent);
					}
					for (int col = v.GetCount(); col < 3; col++) {
						positive.Set(prow, col*3+0, Value());
						positive.Set(prow, col*3+1, Value());
						positive.Set(prow, col*3+2, Value());
					}
					positive.Set(prow, 9, artist_name);
					positive.Set(prow, 10, song.name);
					prow++;
				}
				for(int k = 0; k < song.negative_roles.GetCount(); k++) {
					Vector<LyricsAnalysis::Role>& v = song.negative_roles[k];
					if (v.GetCount() < 2) continue;
					/*bool is_partially_added = false;
					for (const LyricsAnalysis::Role& r : v) {
						int l = active_attrs.Find(r.group);
						if (l >= 0 && active_attrs[l].Find(r.value) >= 0) {
							is_partially_added = true;
							break;
						}
					}
					if (is_partially_added)
						continue;*/
					bool is_matching_cursor = false;
					for (const LyricsAnalysis::Role& r : v) {
						if (r.group == group && r.value == value) {
							is_matching_cursor = true;
							break;
						}
					}
					if (!is_matching_cursor)
						continue;
					bool is_free_and_matching = true;
					for (const LyricsAnalysis::Role& r : v) {
						int l = free_attrs.Find(r.group);
						if (l < 0 || free_attrs[l].Find(r.value) < 0) {
							is_free_and_matching = false;
							break;
						}
					}
					if (!is_free_and_matching)
						continue;
					for (int col = 0; col < v.GetCount() && col < 3; col++) {
						LyricsAnalysis::Role& r = v[col];
						negative.Set(nrow, col*3+0, r.group);
						negative.Set(nrow, col*3+1, r.value);
						negative.Set(nrow, col*3+2, r.percent);
					}
					for (int col = v.GetCount(); col < 3; col++) {
						negative.Set(nrow, col*3+0, Value());
						negative.Set(nrow, col*3+1, Value());
						negative.Set(nrow, col*3+2, Value());
					}
					negative.Set(nrow, 9, artist_name);
					negative.Set(nrow, 10, song.name);
					nrow++;
				}
			}
		}
	}
	positive.SetCount(prow);
	negative.SetCount(nrow);
	if (!positive.IsCursor() && positive.GetCount())
		positive.SetCursor(0);
	if (!negative.IsCursor() && negative.GetCount())
		negative.SetCursor(0);
}

void AttributeDistribution::ToolMenu(Bar& bar) {
	bar.Add(t_("Add selected positive attribute"), AppImg::BlueRing(), THISBACK1(AddAttribute, 0)).Key(K_CTRL_Q);
	bar.Add(t_("Add selected negative attribute"), AppImg::BlueRing(), THISBACK1(AddAttribute, 1)).Key(K_CTRL_W);
	bar.Add(t_("Add random"), AppImg::RedRing(), THISBACK(AddRandom)).Key(K_CTRL_E);
	bar.Add(t_("Remove active attribute"), AppImg::BlueRing(), THISBACK(RemoveActiveAttribute)).Key(K_CTRL_R);
	bar.Add(t_("Add random combination"), AppImg::RedRing(), THISBACK(AddRandomCombination)).Key(K_CTRL_T);
}

void AttributeDistribution::AddAttribute(bool negative) {
	Song& song = GetSong();
	ArrayCtrl& list = !negative ? this->positive : this->negative;
	if (!list.IsCursor()) return;
	int cur = list.GetCursor();
	for(int i = 0; i < 3; i++) {
		String group = list.Get(cur, i*3+0);
		String value = list.Get(cur, i*3+1);
		int percent = list.Get(cur, i*3+2);
		if (group.IsEmpty()) break;
		
		AttrProbability& p = song.active_roles.Add();
		p.group = group;
		p.value = value;
		p.percent = percent;
	}
	PostCallback(THISBACK(Data));
}

void AttributeDistribution::AddRandom() {
	Song& song = GetSong();
	
	int pcount = this->positive.GetCount();
	int ncount = this->negative.GetCount();
	int count = pcount + ncount;
	if (!count) return;
	int i = Random(count);
	ArrayCtrl& list = i < pcount ? this->positive : this->negative;
	int cur = i < pcount ? i : i - pcount;
	for(int i = 0; i < 3; i++) {
		String group = list.Get(cur, i*3+0);
		String value = list.Get(cur, i*3+1);
		int percent = list.Get(cur, i*3+2);
		if (group.IsEmpty()) break;
		
		AttrProbability& p = song.active_roles.Add();
		p.group = group;
		p.value = value;
		p.percent = percent;
	}
	PostCallback(THISBACK(Data));
}

void AttributeDistribution::AddRandomCombination() {
	Song& song = GetSong();
	int ac = attrs.GetCount();
	int count = min(ac, 3);
	if (!count) return;
	
	
	VectorMap<int, int> idx;
	int perc_sum = 0;
	while(idx.GetCount() < count) {
		int j = Random(ac);
		if (idx.Find(j) >= 0)
			continue;
		int perc = 10 + Random(80);
		idx.Add(j, perc);
		perc_sum += perc;
	}
	
	double mul = 100.0 / (double)perc_sum;
	for(int i = 0; i < idx.GetCount(); i++)
		idx[i] *= mul;
	
	for(int i = 0; i < idx.GetCount(); i++) {
		int cur = idx.GetKey(i);
		int percent = idx[i];
		String group = attrs.Get(cur, 0);
		String value = attrs.Get(cur, 1);
		if (group.IsEmpty()) break;
		
		AttrProbability& p = song.active_roles.Add();
		p.group = group;
		p.value = value;
		p.percent = percent;
	}
	PostCallback(THISBACK(Data));
}

void AttributeDistribution::RemoveActiveAttribute() {
	Song& song = GetSong();
	if (!active.IsCursor()) return;
	int cur = active.GetCursor();
	String group = active.Get(cur, 0);
	String value = active.Get(cur, 1);
	int percent = active.Get(cur, 2);
	for(int i = 0; i < song.active_roles.GetCount(); i++) {
		AttrProbability& p = song.active_roles[i];
		if (p.group == group && p.value == value && p.percent == percent) {
			song.active_roles.Remove(i);
			break;
		}
	}
	PostCallback(THISBACK(Data));
}

