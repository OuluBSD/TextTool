#include "SocialDataCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


SocialDataPage::SocialDataPage() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	
	prog.Set(0,1);
	
	hsplit.Horz() << vsplit << story << analysis;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << datasets << companies << programs;// << active_programs;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	companies.AddColumn(t_("Company"));
	companies.WhenCursor << THISBACK(DataCompany);
	
	programs.AddColumn(t_("Program"));
	programs.WhenCursor << THISBACK(DataProgram);
	/*programs.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Add program to active list"), THISBACK(AddProgramToActiveList));
	};*/
	
	/*active_programs.AddColumn(t_("Program"));
	active_programs.WhenCursor << THISBACK(DataActiveProgram);
	active_programs.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Remove program from active list"), THISBACK(RemoveProgramFromActiveList));
	};*/
	
	
}

void SocialDataPage::EnableAll() {
	disabled = false;
	datasets.Enable();
	companies.Enable();
	programs.Enable();
	//active_programs.Enable();
	analysis.Enable();
}

void SocialDataPage::DisableAll() {
	disabled = true;
	datasets.Disable();
	companies.Disable();
	programs.Disable();
	//active_programs.Disable();
	analysis.Disable();
}

void SocialDataPage::ToolMenu(Bar& bar) {
	//bar.Add(t_("Add 10 random programs to list"), AppImg::BlueRing(), THISBACK1(AddRandomProgramsToList, 10)).Key(K_CTRL_Q);
	//bar.Add(t_("Remove program from list"), AppImg::BlueRing(), THISBACK(RemoveProgramFromActiveList)).Key(K_CTRL_W);
	//bar.Separator();
	//bar.Add(t_("Hotfix text"), AppImg::RedRing(), THISBACK(StartHotfixText)).Key(K_F5);
	bar.Add(t_("Import story"), AppImg::RedRing(), THISBACK(ImportStory)).Key(K_F5);
}

/*void SocialDataPage::AddRandomProgramsToList(int count) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !companies.IsCursor() || !programs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	if (company.story.IsEmpty()) return;
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	auto& programs = company.story;// ds.companies.GetAdd(company.name).programs;
	
	for(int i = 0; i < count; i++) {
		for (int tries = 0; tries < 1000; tries++) {
			int scur = Random(company.stories.GetCount());
			const auto& program = company.stories[scur];
			int k = -1;
			for(int j = 0; j < programs.GetCount(); j++) {
				if (programs[j].name == program.name) {
					k = j;
					break;
				}
			}
			if (k >= 0)
				continue;
			programs.Add().name = program.name;
			break;
		}
	}
	
	PostCallback(THISBACK(DataCompanyActivePrograms));
	PostCallback(THISBACK(DataActiveProgram));
}*/

/*void SocialDataPage::AddProgramToActiveList() {
	TODO
	#if 0
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !companies.IsCursor() || !programs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	int scur = programs.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	const auto& program = company.stories[scur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	auto& v = ds.companies.GetAdd(company.name).programs;
	int j = -1;
	for(int i = 0; i < v.GetCount(); i++) if (v[i].name == program.name) {j = i; break;}
	if (j < 0)
		v.Add().name = program.name;
	
	PostCallback(THISBACK(DataCompanyActivePrograms));
	PostCallback(THISBACK(DataActiveProgram));
	#endif
}*/

/*void SocialDataPage::RemoveProgramFromActiveList() {
	TODO
	#if 0
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !companies.IsCursor() || !active_programs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	int scur = active_programs.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	ds.companies.GetAdd(company.name).programs.Remove(scur);
	
	PostCallback(THISBACK(DataCompanyActivePrograms));
	PostCallback(THISBACK(DataActiveProgram));
	#endif
}*/

void SocialDataPage::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	
	datasets.Set(0, 0, "English");
	datasets.Set(1, 0, "Finnish");
	
	if (!datasets.IsCursor())
		datasets.SetCursor(0);
	
	DataDataset();
}

void SocialDataPage::DataDataset() {
	SocialDatabase& db = SocialDatabase::Single();
	
	if (!datasets.IsCursor()) return;
	int cur = datasets.GetCursor();
	const auto& data = db.program_data[cur];
	
	companies.SetCount(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		String s = data[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		companies.Set(i, 0, s);
	}
	
	if (!companies.IsCursor() && companies.GetCount())
		companies.SetCursor(0);
	
	DataCompany();
}

void SocialDataPage::DataCompany() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	
	if (!datasets.IsCursor() || !companies.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	
	programs.SetCount(company.stories.GetCount());
	for(int i = 0; i < company.stories.GetCount(); i++) {
		String s = company.stories[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		programs.Set(i, 0, s);
	}
	
	if (!programs.IsCursor() && programs.GetCount())
		programs.SetCursor(0);
	
	//DataCompanyActivePrograms();
	DataProgram();
}

#if 0
void SocialDataPage::DataCompanyActivePrograms() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !companies.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	/*int i = ds.companies.Find(company.name);
	if (i < 0) {
		active_programs.Clear();
	}
	else {*/
	//const auto& programs = ds.companies[i].programs;
	const auto& programs = company.story;
	active_programs.SetCount(programs.GetCount());
	for(int i = 0; i < programs.GetCount(); i++) {
		active_programs.Set(i, 0, programs[i].name);
	}
	//}
}
#endif

void SocialDataPage::DataProgram() {
	SocialDatabase& db = SocialDatabase::Single();
	
	if (!datasets.IsCursor() || !companies.IsCursor() || !programs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	int scur = programs.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	const auto& program = company.stories[scur];
	
	String s = program.text;
	if (GetDefaultCharset() != CHARSET_UTF8)
		s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
	story.SetData(s);
	analysis.Clear();
}

#if 0
void SocialDataPage::DataActiveProgram() {
	TODO
	#if 0
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	if (!datasets.IsCursor() || !companies.IsCursor() || !active_programs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = companies.GetCursor();
	int scur = active_programs.GetCursor();
	const auto& data = db.program_data[cur];
	const auto& company = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	story.Clear();
	analysis.Clear();
	
	int i = ds.companies.Find(company.name);
	if (i < 0)
		return;
	
	const StoryAnalysis& la = ds.companies[i].programs[scur];
	String program_name = la.name;
	for(int i = 0; i < company.stories.GetCount(); i++) {
		const StoryDataset& program = company.stories[i];
		if (program.name == program_name) {
			String s = program.text;
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			story.SetData(s);
			
			s = la.AsString();
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			analysis.SetData(s);
		}
	}
	#endif
}

void SocialDataPage::HotfixText() {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	int total = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<CompanyDataset>& companies = sd[i];
		for(int j = 0; j < companies.GetCount(); j++) {
			CompanyDataset& company = companies[j];
			total += company.stories.GetCount();
		}
	}
	
	int actual = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<CompanyDataset>& companies = sd[i];
		for(int j = 0; j < companies.GetCount(); j++) {
			CompanyDataset& company = companies[j];
			for(int k = 0; k < company.stories.GetCount(); k++) {
				StoryDataset& program = company.stories[k];
				
				HotfixReplaceWord(program.text);
				
				if (actual++ % 100 == 0)
					PostProgress(actual, total);
			}
		}
	}
	
	PostCallback(THISBACK(EnableAll));
}

#endif

void SocialDataPage::ImportStory() {
	if (!datasets.IsCursor())
		return;
	int ds_i = datasets.GetCursor();
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	tm.DoPrograms(ds_i, 0);
}


END_SOCIALLIB_NAMESPACE
