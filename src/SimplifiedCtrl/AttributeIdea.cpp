#include "SimplifiedCtrl.h"


AttributeIdea::AttributeIdea() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(2500);
	
	vsplit0.Vert() << colors << attrs;
	vsplit1.Vert() << active << suggestions;
	
	colors.AddColumn(t_("Color"));
	colors.AddColumn(t_("Count"));
	colors.ColumnWidths("4 1");
	colors.WhenCursor << THISBACK(DataColor);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddColumn(t_("\%"));
	attrs.AddColumn(t_("Target diff \%"));
	attrs.ColumnWidths("3 3 1 1");
	attrs.AddIndex("IDX");
	attrs.WhenCursor << THISBACK(DataAttr);
	
	active.AddColumn(t_("#"));
	active.AddColumn(t_("Phrase"));
	active.AddColumn(t_("Group"));
	active.AddColumn(t_("Value"));
	active.AddColumn(t_("Artist"));
	active.AddColumn(t_("Song"));
	active.ColumnWidths("1 5 2 2 1 1");
	active.AddIndex("IDX");
	
	suggestions.AddColumn(t_("Phrase"));
	suggestions.AddColumn(t_("Artist"));
	suggestions.AddColumn(t_("Song"));
	suggestions.AddIndex("DATASET");
	suggestions.AddIndex("ARTIST");
	suggestions.AddIndex("SONG");
	suggestions.AddIndex("PHRASE");
	
}

void AttributeIdea::DisableAll() {
	disabled = true;
	
}

void AttributeIdea::EnableAll() {
	disabled = false;
	
}

void AttributeIdea::Data() {
	
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	
	
	DataActive();
	DataColor();
}

void AttributeIdea::DataActive() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (p.part) {
		StaticPart& part = *p.part;
		for(int i = 0; i < part.phrases.GetCount(); i++) {
			StaticPhrase& sp = part.phrases[i];
			active.Set(i, 0, i);
			active.Set(i, 1,
				AttrText(sp.txt)
					.NormalPaper(Blend(sp.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), sp.clr)).Ink(White()));
			active.Set(i, 2, sp.group);
			active.Set(i, 3, sp.group_value);
			active.Set(i, 4, sp.src_artist);
			active.Set(i, 5, sp.src_song);
		}
		active.SetCount(part.phrases.GetCount());
	}
	else {
		active.Clear();
	}
}

void AttributeIdea::DataColor() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Song& song = GetSong();
	
	int clr_i = !colors.IsCursor() ? 0 : colors.GetCursor();
	
	
	int row = 0;
	for(int i = 0; i < song.active_roles.GetCount(); i++) {
		const AttrProbability& ap = song.active_roles[i];
		attrs.Set(row, 0, ap.group);
		attrs.Set(row, 1, ap.value);
		attrs.Set(row, 2, ap.percent);
		attrs.Set(row, "IDX", i);
		row++;
	}
	attrs.SetCount(row);
	attrs.SetSortColumn(2, true);
	if (attrs.GetCount() && !attrs.IsCursor())
		attrs.SetCursor(0);
	
	DataAttr();
}

void AttributeIdea::DataAttr() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int clr_i = !colors.IsCursor() ? 0 : colors.GetCursor();
	bool is_clr_filter = clr_i > 0;
	clr_i--;
	
	if (!attrs.IsCursor()) {
		suggestions.Clear();
		return;
	}
	
	int attr_cur = attrs.GetCursor();
	String group = attrs.Get(attr_cur, 0);
	String value = attrs.Get(attr_cur, 1);
	
	int row = 0;
	for(int da_i = 0; da_i < sda.datasets.GetCount(); da_i++) {
		DatasetAnalysis& da = sda.datasets[da_i];
		for(int i = 0; i < da.artists.GetCount(); i++) {
			ArtistAnalysis& artist = da.artists[i];
			const String& artist_name = da.artists.GetKey(i);
			for(int j = 0; j < artist.songs.GetCount(); j++) {
				LyricsAnalysis& song = artist.songs[j];
				for(int k = 0; k < song.phrases.GetCount(); k++) {
					LyricsAnalysis::Phrase& phrase = song.phrases[k];
					if (phrase.group == group && phrase.value == value) {
						if (is_clr_filter) {
							int clr_group = GetColorGroup(phrase.clr);
							if (clr_group != clr_i)
								continue;
						}
						suggestions.Set(row, 0,
							AttrText(phrase.phrase)
								.NormalPaper(Blend(phrase.clr, White(), 128+64)).NormalInk(Black())
								.Paper(Blend(phrase.clr, GrayColor())).Ink(White()));
						suggestions.Set(row, 1, artist_name);
						suggestions.Set(row, 2, song.name);
						suggestions.Set(row, "DATASET", da_i);
						suggestions.Set(row, "ARTIST", i);
						suggestions.Set(row, "SONG", j);
						suggestions.Set(row, "PHRASE", k);
						row++;
					}
				}
			}
		}
	}
	suggestions.SetCount(row);
}

void AttributeIdea::ToolMenu(Bar& bar) {
	bar.Add(t_("Add phrase"), AppImg::BlueRing(), THISBACK(AddPhrase)).Key(K_CTRL_Q);
	bar.Add(t_("Remove phrase"), AppImg::BlueRing(), THISBACK(RemovePhrase)).Key(K_CTRL_W);
	
}

void AttributeIdea::AddPhrase() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!suggestions.IsCursor() || !attrs.IsCursor())
		return;
	int cur = suggestions.GetCursor();
	int a_cur = attrs.GetCursor();
	
	int da_i = suggestions.Get(cur, "DATASET");
	int a_i = suggestions.Get(cur, "ARTIST");
	int s_i = suggestions.Get(cur, "SONG");
	int p_i = suggestions.Get(cur, "PHRASE");
	DatasetAnalysis& da = sda.datasets[da_i];
	ArtistAnalysis& artist = da.artists[a_i];
	const String& artist_name = da.artists.GetKey(a_i);
	LyricsAnalysis& song = artist.songs[s_i];
	LyricsAnalysis::Phrase& phrase = song.phrases[p_i];
	String group = attrs.Get(a_cur, 0);
	String value = attrs.Get(a_cur, 1);
	
	EditorPtrs& p = db.ctx.ed;
	if (p.part) {
		StaticPart& part = *p.part;
		StaticPhrase& p = part.phrases.Add();
		p.clr = phrase.clr;
		p.txt = phrase.phrase;
		p.src_artist = artist_name;
		p.src_song = song.name;
		p.group = group;
		p.group_value = value;
	}
	
	PostCallback(THISBACK(DataActive));
}

void AttributeIdea::RemovePhrase() {
	Database& db = Database::Single();
	if (!active.IsCursor())
		return;
	
	int cur = active.GetCursor();
	EditorPtrs& p = db.ctx.ed;
	if (p.part) {
		StaticPart& part = *p.part;
		if (cur >= 0 && cur < part.phrases.GetCount()) {
			part.phrases.Remove(cur);
			PostCallback(THISBACK(DataActive));
		}
	}
}
