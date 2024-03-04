#include "SongCtrl.h"

#if 0



LineStructureEditor::LineStructureEditor() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << parts << vsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << lines << line_options << phrase_parts << phrase_structs;
	
	parts.AddColumn(t_("Part"));
	parts.AddColumn(t_("Type"));
	parts.WhenCursor << THISBACK(DataPart);
	
	lines.AddColumn(t_("Nana"));
	lines.AddColumn(t_("Primary option"));
	lines.AddColumn(t_("Option count"));
	lines.ColumnWidths("4 8 1");
	
	line_options.AddColumn(t_("Phrase parts"));
	line_options.AddColumn(t_("Phrase struct"));
	
	phrase_parts.AddColumn(t_("Type"));
	phrase_parts.AddColumn(t_("Example #1"));
	phrase_parts.AddColumn(t_("Example #2"));
	
	phrase_structs.AddColumn(t_("Struct name"));
	phrase_structs.AddColumn(t_("Types"));
	phrase_structs.AddColumn(t_("Example #1"));
	phrase_structs.AddColumn(t_("Example #2"));
	
}

void LineStructureEditor::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	int part_i = parts.GetCursor();
	String title = GetSongTitle();
	SongCandidateCache& cache = da.song_cache.GetAdd(title);
	cache.Realize(song);
	
	
	for(int i = 0; i < song.parts.GetCount(); i++) {
		StaticPart& sp = song.parts[i];
		SongCandidateCache::Part& part = cache.parts[i];
		Color clr = GetSongPartPaperColor(sp.type);
		SetColoredListValue(parts, i, 0, sp.name, clr, false);
		SetColoredListValue(parts, i, 1, sp.GetTypeString(), clr, false);
	}
	INHIBIT_CURSOR(parts);
	parts.SetCount(song.parts.GetCount());
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void LineStructureEditor::DataPart() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (!parts.IsCursor()) {
		lines.Clear();
		line_options.Clear();
		phrase_parts.Clear();
		phrase_structs.Clear();
		return;
	}
	
	Song& song = GetSong();
	int part_i = parts.GetCursor();
	StaticPart& sp = song.parts[part_i];
	String title = GetSongTitle();
	SongCandidateCache& cache = da.song_cache.GetAdd(title);
	cache.Realize(song);
	
	SongCandidateCache::Part& part = cache.parts[part_i];
	const auto& nana_lines = sp.nana.Get();
	for(int i = 0; i < nana_lines.GetCount(); i++) {
		lines.Set(i, 0, nana_lines[i].AsNana());
	}
	lines.SetCount(nana_lines.GetCount());
	
	DataSuggestions();
}

void LineStructureEditor::DataSuggestions() {
	UpdateSuggestions();
	
	// unique parts
	for(int i = 0; i < part_sugs.GetCount(); i++) {
		const String& key = part_sugs.GetKey(i);
		const PartSug& sug = part_sugs[i];
		
		phrase_parts.Set(i, 0, key);
		phrase_parts.Set(i, 1, sug.examples[0]);
		phrase_parts.Set(i, 2, sug.examples[1]);
	}
	phrase_parts.SetCount(part_sugs.GetCount());
	
	// unique structs
	for(int i = 0; i < struct_sugs.GetCount(); i++) {
		const String& key = struct_sugs.GetKey(i);
		const StructSug& sug = struct_sugs[i];
		
		phrase_structs.Set(i, 0, sug.category);
		phrase_structs.Set(i, 1, sug.parts);
		phrase_structs.Set(i, 2, sug.examples[0]);
		phrase_structs.Set(i, 3, sug.examples[1]);
	}
	phrase_structs.SetCount(struct_sugs.GetCount());
	
}

void LineStructureEditor::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
	
}

void LineStructureEditor::UpdateSuggestions() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	DatasetAnalysis& da = sd.a.datasets[ds_i];
	
	if (part_sugs.IsEmpty()) {
		part_sugs.Reserve(da.phrase_parts.GetCount());
		
		for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
			const PhrasePart& pp = da.phrase_parts[i];
			
			if (pp.virtual_phrase_part < 0)
				continue;
			const VirtualPhrasePart& vpp = da.virtual_phrase_parts[pp.virtual_phrase_part];
			
			if (vpp.struct_part_type < 0)
				continue;
			const String& struct_part_type = da.struct_part_types[vpp.struct_part_type];
			
			String phrase = da.GetWordString(pp.words);
			String type_str = da.GetTypeString(vpp.word_classes);
			
			PartSug& sug = part_sugs.GetAdd(struct_part_type);
			
			for(int j = 0; j < PartSug::EX_COUNT; j++) {
				if (sug.examples[j].IsEmpty()) {
					sug.examples[j] = phrase;
					break;
				}
			}
		}
		
		SortByKey(part_sugs, StdLess<String>());
	}
	
	if (struct_sugs.IsEmpty()) {
		DatasetAnalysis& da = sd.a.datasets[ds_i];
		for(int i = 0; i < da.token_texts.GetCount(); i++) {
			const TokenText& txt = da.token_texts[i];
			if (txt.virtual_phrase < 0)
				continue;
			
			VirtualPhrase& vp = da.virtual_phrases[txt.virtual_phrase];
			
			String txt_str = da.GetTokenTextString(txt);
			//String type_str = GetTypePhraseString(vp.word_classes, da);
			String parts_str;
			String struct_str;
			if (vp.virtual_phrase_struct >= 0) {
				VirtualPhraseStruct& vps = da.virtual_phrase_structs[vp.virtual_phrase_struct];
				int struct_type = vps.struct_type;
				if (struct_type >= 0)
					struct_str = da.struct_types[struct_type];
				for (int vpp_i : vps.virtual_phrase_parts) {
					if (!parts_str.IsEmpty()) parts_str += " + ";
					const VirtualPhrasePart& vpp = da.virtual_phrase_parts[vpp_i];
					parts_str += vpp.struct_part_type >= 0 ? da.struct_part_types[vpp.struct_part_type] : String("error");
				}
			}
			/*texts.Set(row, 0, txt_str);
			texts.Set(row, 1, type_str);
			texts.Set(row, 2, parts_str);
			texts.Set(row, 3, struct_str);
			row++;*/
			
			String key = struct_str + ": " + parts_str;
			
			StructSug& sug = struct_sugs.GetAdd(key);
			if (sug.category.IsEmpty()) {
				sug.category = struct_str;
				sug.parts = parts_str;
			}
			for(int j = 0; j < StructSug::EX_COUNT; j++) {
				if (sug.examples[j].IsEmpty()) {
					sug.examples[j] = txt_str;
					break;
				}
			}
		}
		
		SortByKey(struct_sugs, StdLess<String>());
	}
}

#endif
