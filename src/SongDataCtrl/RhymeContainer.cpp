#include "SongDataCtrl.h"


RhymeContainerPage::RhymeContainerPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos());
	
	hsplit.Horz() << vsplit << data;
	hsplit.SetPos(1500);
	
	vsplit.Vert() << syl_counts << colors << attrs << actions;
	
	
	syl_counts.AddColumn(t_("Syllable count"));
	
	colors.AddColumn(t_("Color"));
	
	attrs.AddColumn(t_("Attributes"));
	
	actions.AddColumn(t_("Actions"));
	
	data.AddColumn(t_("Data"));
	
	prog.Set(0,1);
	
}

RhymeContainerPage::~RhymeContainerPage() {
	Stop();
}

void RhymeContainerPage::EnableAll() {
	
}

void RhymeContainerPage::DisableAll() {
	
}

void RhymeContainerPage::Data() {
	
}

void RhymeContainerPage::ToolMenu(Bar& bar) {
	//bar.Add(t_("Process data"), AppImg::BlueRing(), THISBACK(ProcessData)).Key(K_F5);
	bar.Add(t_("Start processing data"), AppImg::BlueRing(), THISBACK(Start)).Key(K_F5);
	bar.Add(t_("Stop processing data"), AppImg::BlueRing(), THISBACK(Stop)).Key(K_F6);
	
}

void RhymeContainerPage::ProcessData() {
	Database& db = Database::Single();
	
	
	
}

void RhymeContainerPage::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void RhymeContainerPage::Stop() {
	running = false;
	while (!stopped)
		Sleep(10);
}

void RhymeContainerPage::Process() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int ds_i = 0;
	int tmpl_i = 0;
	
	Vector<int> comb;
	int phrase_count = 0;
	
	while (running) {
		
		if (ds_i >= sda.datasets.GetCount())
			break;
		DatasetAnalysis& da = sda.datasets[ds_i];
		
		if (tmpl_i >= da.tmpl_phrases.GetCount()) {
			tmpl_i = 0;
			ds_i++;
			continue;
		}
		
		
		// Make sentences from templates
		TemplatePhrase& tp = da.tmpl_phrases[tmpl_i];
		
		if (tp.words.GetCount()) {
			comb.SetCount(tp.words.GetCount());
			for (auto& v : comb) v = 0;
			
			while (true) {
				bool roll_over = false;
				for (int i = comb.GetCount()-1; i >= 0; i--) {
					int& j = comb[i];
					j++;
					if (j < tp.words[i].GetCount())
						break;
					else {
						j = 0;
						roll_over = i == 0;
					}
				}
				if (roll_over)
					break;
				
				String phrase;
				int word_i = 0;
				bool fail = false;
				for (const auto& part : tp.parts) {
					if (part[0] == '{') {
						if (word_i >= comb.GetCount()) {
							fail = true;
							break;
						}
						int comb_i = comb[word_i];
						const auto& v = tp.words[word_i];
						if (comb_i >= v.GetCount()) {
							fail = true;
							break;
						}
						const String& wrd = v[comb_i];
						if (!phrase.IsEmpty())
							phrase.Cat(' ');
						phrase << wrd;
						word_i++;
					}
					else {
						if (part.GetCount() == 1 && (part[0] == ',' || part[1] == '.' || part[1] == '!' || part[1] == '?'))
							;
						else if (!phrase.IsEmpty())
							phrase.Cat(' ');
						phrase << part;
					}
				}
				
				if (!fail) {
					LOG(phrase_count << ": " << phrase);
					phrase_count++;
				}
			}
			
			// Make extra sentences using wordnets
		
		}
		
		//Sleep(1);
		tmpl_i++;
	}
	
	running = false;
	stopped = true;
}
