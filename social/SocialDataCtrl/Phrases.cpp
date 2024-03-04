#include "SocialDataCtrl.h"

#if 0

ProgramDataPhrases::ProgramDataPhrases() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	prog.Set(0,1);
	
	hsplit.Horz() << vsplit << programs;
	hsplit.SetPos(4000);
	
	vsplit.Vert() << datasets << attrs;
	vsplit.SetPos(1000);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	programs.AddColumn(t_("Phrase"));
	programs.AddColumn(t_("Company"));
	programs.AddColumn(t_("Program"));
	programs.ColumnWidths("3 1 1");
	
}

void ProgramDataPhrases::EnableAll() {
	disabled = false;
	datasets.Enable();
	attrs.Enable();
	programs.Enable();
}

void ProgramDataPhrases::DisableAll() {
	disabled = true;
	datasets.Disable();
	attrs.Disable();
	programs.Disable();
}

void ProgramDataPhrases::Data() {
	DataDataset();
}

void ProgramDataPhrases::DataDataset() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	datasets.SetCount(sda.datasets.GetCount());
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	if (!datasets.IsCursor() && datasets.GetCount())
		datasets.SetCursor(0);
	
	DataMain();
}

void ProgramDataPhrases::DataMain() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor()) {
		attrs.Clear();
		return;
	}
	
	
	int gi = 0;
	#define ATTR_ITEM(e, g, i0, i1) \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i0); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 0); \
		i++; \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i1); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 1); \
		i++, gi++;
	int i = 0;
	ATTR_LIST
	#undef ATTR_ITEM

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	DataAttribute();
}

void ProgramDataPhrases::DataAttribute() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !attrs.IsCursor()) {
		programs.Clear();
		return;
	}
	
	int ds_i = datasets.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String group_str = ToLower(Attr::AttrKeys[attr_group_i][1]);
	String value_str = ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]);
	
	int row = 0;
	for(int i = 0; i < da.companies.GetCount(); i++) {
		CompanyAnalysis& company = da.companies[i];
		for(int j = 0; j < company.programs.GetCount(); j++) {
			StoryAnalysis& program = company.programs[j];
			for(int k = 0; k < program.phrases.GetCount(); k++) {
				StoryAnalysis::Phrase& phrase = program.phrases[k];
				if (phrase.group == group_str && phrase.value == value_str) {
					programs.Set(row, 0,
						AttrText(phrase.phrase)
							.NormalPaper(Blend(phrase.clr, White(), 128+64)).NormalInk(Black())
							.Paper(Blend(phrase.clr, GrayColor())).Ink(White()));
					programs.Set(row, 1, da.companies.GetKey(i));
					programs.Set(row, 2, program.name);
					row++;
				}
			}
		}
	}
	programs.SetCount(row);
	if (!programs.IsCursor() && programs.GetCount())
		programs.SetCursor(0);
	
}

void ProgramDataPhrases::ToolMenu(Bar& bar) {
	ToolAppCtrl::ToolMenu(bar);
}

#endif
