#include "SimplifiedCtrl.h"

SongDataPhrases::SongDataPhrases() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	prog.Set(0,1);
	
	hsplit.Horz() << vsplit << songs;
	hsplit.SetPos(4000);
	
	vsplit.Vert() << datasets << vphrases << phrases;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	vphrases.AddColumn(t_("#"));
	vphrases.AddColumn(t_("Virtual phrase"));
	vphrases.AddColumn(t_("Count"));
	vphrases.AddIndex("IDX");
	vphrases.ColumnWidths("1 5 2");
	vphrases.WhenCursor << THISBACK(DataVirtualPhrase);
	
	phrases.AddColumn(t_("Phrase"));
	
	songs.AddColumn(t_("#"));
	songs.AddColumn(t_("Artist"));
	songs.AddColumn(t_("Song"));
	
}

void SongDataPhrases::EnableAll() {
	disabled = false;
	datasets.Enable();
	vphrases.Enable();
	phrases.Enable();
	songs.Enable();
}

void SongDataPhrases::DisableAll() {
	disabled = true;
	datasets.Disable();
	vphrases.Disable();
	phrases.Disable();
	songs.Disable();
}

void SongDataPhrases::Data() {
	DataDataset();
}

void SongDataPhrases::DataDataset() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	datasets.SetCount(sda.datasets.GetCount());
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	if (!datasets.IsCursor() && datasets.GetCount())
		datasets.SetCursor(0);
	
	DataMain();
}

void SongDataPhrases::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor()) {
		vphrases.Clear();
		phrases.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	vphrases.SetCount(da.virtual_phrases.GetCount());
	for(int i = 0; i < da.virtual_phrases.GetCount(); i++) {
		const VirtualPhraseAnalysis& vpa = da.virtual_phrases[i];
		
		String s;
		for(int j = 0; j < vpa.group_ids.GetCount(); j++) {
			//WordGroupAnalysis& wga = da.groups[vpa.group_ids[j]];
			String g = da.groups.GetKey(vpa.group_ids[j]);
			if (s.GetCount()) s << ", ";
			s << g;
		}
		vphrases.Set(i, 0, i);
		vphrases.Set(i, 1, s);
		vphrases.Set(i, 2, vpa.phrases.GetCount());
		vphrases.Set(i, "IDX", i);
	}
	if (!vphrases.IsCursor() && vphrases.GetCount())
		vphrases.SetCursor(0);
	vphrases.SetSortColumn(2, true);
	
	DataVirtualPhrase();
}

void SongDataPhrases::DataVirtualPhrase() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !vphrases.IsCursor()) {
		phrases.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	int vpa_i = vphrases.Get("IDX");
	DatasetAnalysis& da = sda.datasets[ds_i];
	const VirtualPhraseAnalysis& vpa = da.virtual_phrases[vpa_i];
	
	phrases.SetCount(vpa.phrases.GetCount());
	for(int i = 0; i < vpa.phrases.GetCount(); i++) {
		int phrase_i = vpa.phrases[i];
		const PhraseAnalysis& pa = da.unique_phrases[phrase_i];
		
		String s;
		for(int j = 0; j < pa.word_ids.GetCount(); j++) {
			String g = da.words.GetKey(pa.word_ids[j]);
			if (s.GetCount()) s << " ";
			s << g;
		}
		phrases.Set(i, 0, s);
	}
	if (!phrases.IsCursor() && phrases.GetCount())
		phrases.SetCursor(0);
	
}

void SongDataPhrases::ToolMenu(Bar& bar) {
	//bar.Add(t_("Update data"), AppImg::RedRing(), THISBACK(DataDataset)).Key(K_CTRL_Q);
	//bar.Separator();
	bar.Add(t_("Make unique phrases"), AppImg::RedRing(), THISBACK(StartMakeUniquePhrases)).Key(K_F5);
	
}

void SongDataPhrases::StartMakeUniquePhrases() {
	if (disabled) return;
	Thread::Start(THISBACK(MakeUniquePhrases));
}
	
void SongDataPhrases::MakeUniquePhrases() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	TimeStop ts;
	
	Vector<int> word_ids;
	Vector<int> group_ids;
	Vector<String> main_classes;
	Vector<Color> clrs;
	
	PostProgress(0,1);
	
	int actual = 0, total = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		const Vector<ArtistDataset>& dataset = sd[ds_i];
		for(int j = 0; j < dataset.GetCount(); j++) {
			const ArtistDataset& artist = dataset[j];
			total += artist.lyrics.GetCount();
		}
	}
	
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		if (Thread::IsShutdownThreads()) break;
				
		ds.unique_phrases.Clear();
		
		const Vector<ArtistDataset>& dataset = sd[ds_i];
		for(int j = 0; j < dataset.GetCount(); j++) {
			const ArtistDataset& artist = dataset[j];
			//const ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
			
			if (Thread::IsShutdownThreads()) break;
				
			for(int k = 0; k < artist.lyrics.GetCount(); k++) {
				const LyricsDataset& song = artist.lyrics[k];
				
				if (Thread::IsShutdownThreads()) break;
				
				ASSERT(song.text.Find("\r") < 0);
				Vector<String> lines = Split(song.text, "\n");
				
				for (String& l : lines) {
					l = TrimBoth(l);
					if (l.IsEmpty()) continue;
					
					// Separate words in the line
					Vector<String> words;
					GetWords(l, words);
					if (words.IsEmpty()) continue;
					
					// Get ids of words
					bool all_found = true;
					word_ids.SetCount(0);
					main_classes.SetCount(0);
					clrs.SetCount(0);
					group_ids.SetCount(0);
					CombineHash ch;
					CombineHash virt_ch;
					for (String& w : words) {
						int i = ds.words.Find(w);
						if (i < 0) {
							all_found = false;
							break;
						}
						word_ids << i;
						ch.Do(i);
						
						const WordAnalysis& wa = ds.words[i];
						main_classes << wa.main_class;
						clrs << wa.clr;
						virt_ch.Do(wa.main_class);
						
						int j = ds.groups.Find(wa.main_class);
						if (j < 0) {
							j = ds.groups.GetCount();
							ds.groups.Add(wa.main_class);
						}
						group_ids << j;
					}
					if (!all_found)
						continue;
					
					// Get/Add unique phrase analysis object
					int phrase_i;
					{
						hash_t hash = ch;
						String hash_s = IntStr64(hash);
						phrase_i = ds.unique_phrases.Find(hash_s);
						if (phrase_i < 0) {
							phrase_i = ds.unique_phrases.GetCount();
							ds.unique_phrases.Add(hash_s);
						}
						PhraseAnalysis& pa = ds.unique_phrases[phrase_i];
						if (pa.word_ids.IsEmpty())
							Swap(word_ids, pa.word_ids);
						SongId& sid = pa.songs.Add();
						sid.dataset = ds_i;
						sid.artist = j;
						sid.song = k;
						pa.clr = GetSentenceColor(main_classes, clrs);
					}
					
					// Get/Add virtual phrase analysis object
					{
						hash_t vhash = virt_ch;
						String vhash_s = IntStr64(vhash);
						VirtualPhraseAnalysis& vpa = ds.virtual_phrases.GetAdd(vhash_s);
						if (vpa.group_ids.IsEmpty())
							Swap(group_ids, vpa.group_ids);
						vpa.phrases.FindAdd(phrase_i);
					}
				}
				
				if ((actual++ % 10) == 0)
					PostProgress(actual, total);
			}
			
			if (ds.unique_phrases.GetCount() > phrase_limit)
				break;
		}
		/*for(int i = 0; i < ds.words.GetCount(); i++) {
			WordAnalysis& wa = ds.words[i];
			
		}*/
		
		if (ds.unique_phrases.GetCount() > phrase_limit)
			break;
	}
	
	LOG("MakeUniquePhrases took " << ts.ToString());
	
	PostCallback(THISBACK(EnableAll));
}

/*
- normi phrases
     - uniikki fraasit
         - sana id integer vektori, EI string
             - tarkista, ettei sanoja poisteta
         - kerätään artistit
         - kerätään biisit missä esiintyy
     - optimointi?
         - sijainti integer vektorissa
         - integer vektorissa 3 aluetta eroteltuna 0 päätteellä
     - gui
         - ei kovin reaktiivinen data, mutta testaamiseen
             - ehkä testaamiseen, että mitkä lauseet sisältää sanan
*/
