#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


PhraseParts::PhraseParts() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << vsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << texts << parts;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	texts.AddColumn(t_("Phrase"));
	texts.AddColumn(t_("Types"));
	texts.AddColumn(t_("Structural name"));
	texts.AddIndex("IDX");
	texts.ColumnWidths("1 1 1");
	texts.WhenBar << [this](Bar& bar){
		bar.Add("Copy virtual type", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 0);
			WriteClipboardText(text);
		});
		bar.Add("Copy type", [this]() {
			int i = texts.GetCursor();
			String text = texts.Get(i, 1);
			WriteClipboardText(text);
		});
	};
	
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Next scores"));
	parts.AddColumn(t_("Score sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("6 3 1");
	
}

void PhraseParts::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void PhraseParts::DataDataset() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	
	int ds_i = datasets.GetCursor();
	if (ds_i < 0) return;
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	int row = 0;
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		const PhrasePart& pp = da.phrase_parts[i];
		
		String struct_part_type, type_str;
		if (pp.virtual_phrase_part >= 0) {
			const VirtualPhrasePart& vpp = da.virtual_phrase_parts[pp.virtual_phrase_part];
			type_str = da.GetTypeString(vpp.word_classes);
			
			if (vpp.struct_part_type >= 0)
				const String& struct_part_type = da.struct_part_types[vpp.struct_part_type];
		}
		
		String phrase = da.GetWordString(pp.words);
		
		texts.Set(row, 0, phrase);
		texts.Set(row, 1, type_str);
		texts.Set(row, 2, struct_part_type);
		texts.Set(row, "IDX", i);
		row++;
		
		if (row >= 10000) break;
	}
	texts.SetCount(row);
	
	
	row = 0;
	for(int i = 0; i < da.action_phrases.GetCount(); i++) {
		const String& phrase = da.action_phrases.GetKey(i);
		const ExportDepActionPhrase& ap = da.action_phrases[i];
		
		parts.Set(row, 0, phrase);
		
		String ns;
		int score_sum = 0;
		for(int j = 0; j < ap.next_scores.GetCount(); j++) {
			if (j) ns << ", ";
			int row_sum = 0;
			for (int s : ap.next_scores[j])
				row_sum += s;
			ns << row_sum;
			score_sum += row_sum;
		}
		parts.Set(row, 1, ns);
		parts.Set(row, 2, score_sum);
		parts.Set(row, "IDX", i);
		row++;
		
		if (row >= 10000) break;
	}
	parts.SetCount(row);
	parts.SetSortColumn(2, true);
}

void PhraseParts::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	//bar.Add(t_("Get line change scores using existing"), AppImg::VioletRing(), THISBACK(ProcessUsingExisting)).Key(K_F5);
	bar.Add(t_("Get line change scores"), AppImg::RedRing(), THISBACK(Process)).Key(K_F6);
	
}

void PhraseParts::ProcessUsingExisting() {
	int ds_i = datasets.GetCursor();
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoWordsUsingExisting(ds_i, 4);
}

void PhraseParts::Process() {
	int ds_i = datasets.GetCursor();
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoWords(ds_i, 4);
}


END_TEXTLIB_NAMESPACE

