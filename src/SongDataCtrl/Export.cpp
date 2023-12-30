#include "SongDataCtrl.h"



ExportPage::ExportPage() {
	Add(log.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	SetProgress(0,1);
}

void ExportPage::Data() {
	
}

void ExportPage::ToolMenu(Bar& bar) {
	if (!running)
		bar.Add(t_("Export"), AppImg::BlueRing(), THISBACK(Start)).Key(K_F5);
	else
		bar.Add(t_("Stop"), AppImg::BlueRing(), THISBACK(Stop)).Key(K_F5);
	
}

void ExportPage::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Export));
}

void ExportPage::Stop() {
	running = false;
	while (!stopped) Sleep(10);
	PostProgress(0,1);
}

struct ExportTextPart {
	
};

void ExportPage::Export() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "songdata");
	RealizeDirectory(dir);
	
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String key = sd.GetKey(ds_i);
		String ds_dir = AppendFileName(dir, key);
		RealizeDirectory(ds_dir);
		
		IndexFile struct_part_types(ds_dir, "structure part types");
		FileT<ExportTextPart> text_parts(ds_dir, "text parts");
		
		DatasetAnalysis& da = sda.datasets[ds_i];
		
		for (const StructurePhrase& sp : da.structure_phrases) {
			const StructureType& st = da.structure_types[sp.type];
			int struct_type_i = struct_part_types.FindAdd(st.struct_type);
			
			if (sp.sent_parts.GetCount() != st.part_types.GetCount())
				continue;
			
			for(int i = 0; i < st.part_types.GetCount(); i++) {
				const String& txt = sp.sent_parts[i];
				const String& part_type = st.part_types[i];
				int type_i = struct_part_types.FindAdd(part_type);
				
				// Sentence parts
				ExportTextPart& part = text_parts.Add(txt);
				part.sentence_type = type_i;
				
				// Full sentences (links to parts)
				
				
			}
		}
		
		
		
		Vector<ArtistDataset>& artists = sd[ds_i];
		for(int i = 0; i < artists.GetCount(); i++) {
			ArtistDataset& artist = artists[i];
			for(int j = 0; j < artist.lyrics.GetCount(); j++) {
				LyricsDataset& lyrics = artist.lyrics[j];
				
			}
		}
		
	}
}
