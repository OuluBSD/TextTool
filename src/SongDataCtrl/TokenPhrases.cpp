#include "SongDataCtrl.h"


TokenPhrases::TokenPhrases() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << texts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("Text"));
	texts.AddColumn(t_("Virtual phrase"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1");
	texts.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 0);
			WriteClipboardText(text);
		});
		bar.Add("Copy virtual text", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 1);
			WriteClipboardText(text);
		});
	};
	
}

void TokenPhrases::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void TokenPhrases::DataDataset() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		
		String txt_str = da.GetTokenTextString(txt);
		String type_str = da.GetTokenTypeString(txt);
		
		texts.Set(i, 0, txt_str);
		texts.Set(i, 1, type_str);
	}
	texts.SetCount(da.tokens.GetCount());
	
	
}

void TokenPhrases::ToolMenu(Bar& bar) {
	bar.Add(t_("Get unknown pairs"), AppImg::RedRing(), THISBACK(GetUnknownPairs)).Key(K_F5);
	
}

void TokenPhrases::GetUnknownPairs() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
	
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		if (txt.tokens.GetCount() < 2)
			continue;
		int prev_w_i = -1;
		for(int j = 0; j < txt.tokens.GetCount(); j++) {
			int tk_i = txt.tokens[j];
			bool is_first = j == 0;
			bool is_last = j == txt.tokens.GetCount()-1;
			bool prev_unknown = false;
			
			const Token& tk = da.tokens[tk_i];
			int w_i = tk.word_;
			if (w_i < 0) {
				String key = ToLower(da.tokens.GetKey(tk_i));
				w_i = da.words.Find(key);
				tk.word_ = w_i;
			}
			if (w_i >= 0) {
				const ExportWord& ew = da.words[w_i];
				bool is_unknown = ew.class_count > 1;
				
				/*bool next_unknown = false;
				if (!is_last && !prev_unknown && is_unknown) {
					int next_tk_i = txt.tokens[j+1];
					const Token& next_tk = da.tokens[next_tk_i];
					if (next_tk.word_ >= 0) {
						const ExportWord& next_ew = da.words[next_tk.word_];
						next_unknown = next_ew.class_count > 1;
					}
				}
				
				if (!prev_unknown && is_unknown && next_unknown) {
					// do nothing: wait until next
				}
				else*/
				if (prev_unknown || (is_unknown && is_last)) {
					if (prev_w_i >= 0) {
						CombineHash c;
						c.Do(prev_w_i).Put(1).Do(w_i);
						hash_t h = c;
						WordPairType& wp = da.ambiguous_word_pairs.GetAdd(h);
						wp.from = prev_w_i;
						wp.to = w_i;
					}
				}
				
				prev_unknown = is_unknown;
			}
			prev_w_i = w_i;
		}
	}
	
}

