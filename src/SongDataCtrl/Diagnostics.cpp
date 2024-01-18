#include "SongDataCtrl.h"


SongDataDiagnostics::SongDataDiagnostics() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	
	hsplit.Horz() << datasets << values;
	hsplit.SetPos(2500);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	values.AddColumn(t_("Key"));
	values.AddColumn(t_("Value"));
}

void SongDataDiagnostics::Data() {
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

void SongDataDiagnostics::DataDataset() {
	Database& db = Database::Single();
	
	if (!datasets.IsCursor()) return;
	int cur = datasets.GetCursor();
	const DatasetAnalysis& da = db.song_data.a.datasets[cur];
	
	for(int i = 0; i < da.diagnostics.GetCount(); i++) {
		const String& key = da.diagnostics.GetKey(i);
		const String& value = da.diagnostics[i];
		
		values.Set(i, 0, key);
		values.Set(i, 1, value);
	}
	values.SetCount(da.diagnostics.GetCount());
	
}

void SongDataDiagnostics::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataDataset)).Key(K_CTRL_Q);
}
