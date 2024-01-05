#include "SongDataCtrl.h"

VirtualPhrases::VirtualPhrases() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << vsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << texts << parts;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("From"));
	texts.AddColumn(t_("To"));
	texts.AddColumn(t_(""));
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
	
	parts.AddColumn(t_("From"));
	parts.AddIndex("IDX");
	parts.WhenBar << [this](Bar& bar){
		bar.Add("Copy virtual text", [this]() {
			int i = parts.GetCursor();
			String text = parts.Get(i, 0);
			WriteClipboardText(text);
		});
	};
	
}

void VirtualPhrases::Data() {
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

String GetTypePhraseString(const Vector<int>& types, const DatasetAnalysis& da) {
	String o;
	for (int type_i : types) {
		String wc = da.word_classes[type_i];
		
		int a = wc.Find(",");
		if (a >= 0)
			wc = TrimBoth(wc.Left(a));
		
		a = wc.Find("(");
		if (a >= 0)
			wc = TrimBoth(wc.Left(a));
		
		o << "{" << wc << "}";
	}
	return o;
}

void VirtualPhrases::DataDataset() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	int row = 0;
	for(int i = 0; i < da.token_texts.GetCount(); i++) {
		const TokenText& txt = da.token_texts[i];
		if (txt.virtual_phrase < 0)
			continue;
		
		VirtualPhrase& vp = da.virtual_phrases[txt.virtual_phrase];
		
		String txt_str = da.GetTokenTextString(txt);
		String type_str = GetTypePhraseString(vp.types, da);
		texts.Set(row, 0, txt_str);
		texts.Set(row, 1, type_str);
		row++;
	}
	texts.SetCount(row);
	
	row = 0;
	for(int i = 0; i < da.virtual_phrase_parts.GetCount(); i++) {
		const VirtualPhrasePart& vpp = da.virtual_phrase_parts[i];
		if (vpp.types.IsEmpty())
			continue;
		
		String type_str = GetTypePhraseString(vpp.types, da);
		parts.Set(row, 0, type_str);
		row++;
	}
	parts.SetCount(row);
	
	
}

void VirtualPhrases::ToolMenu(Bar& bar) {
	bar.Add(t_("Make virtual phrases"), AppImg::RedRing(), THISBACK(ProcessVirtualPhrases)).Key(K_F5);
	bar.Add(t_("Make virtual phrase parts"), AppImg::RedRing(), THISBACK(ProcessVirtualPhraseParts)).Key(K_F6);
	
}

void VirtualPhrases::ProcessVirtualPhrases() {
	int ds_i = datasets.GetCursor();
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoVirtualPhrases(ds_i, 0);
}

void VirtualPhrases::ProcessVirtualPhraseParts() {
	int ds_i = datasets.GetCursor();
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoVirtualPhrases(ds_i, 1);
}

