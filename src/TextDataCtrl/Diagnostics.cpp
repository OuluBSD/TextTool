#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


TextDataDiagnostics::TextDataDiagnostics() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	
	hsplit.Horz() << values;
	
	values.AddColumn(t_("Key"));
	values.AddColumn(t_("Value"));
}

void TextDataDiagnostics::Data() {
	TextDatabase& db = GetDatabase();
	const DatasetAnalysis& da = db.src_data.a.dataset;
	
	for(int i = 0; i < da.diagnostics.GetCount(); i++) {
		const String& key = da.diagnostics.GetKey(i);
		const String& value = da.diagnostics[i];
		
		values.Set(i, 0, key);
		
		if (key.Find("percentage") >= 0 && key.Find("fail percentage") < 0) {
			double perc = StrDbl(value);
			if (perc < 50)
				SetColoredListValue(values, i, 1, Format("%.3m", perc), Red());
			else
				values.Set(i, 1, value);
		}
		else values.Set(i, 1, value);
	}
	values.SetCount(da.diagnostics.GetCount());
	
}

void TextDataDiagnostics::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
}


END_TEXTLIB_NAMESPACE
