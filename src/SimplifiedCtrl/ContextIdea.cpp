#include "SimplifiedCtrl.h"


ContextIdeaCtrl::ContextIdeaCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << notes << suggestion;
	hsplit.SetPos(2500, 0);
	hsplit.SetPos(5000, 1);
	
	vsplit.Vert() << distribution << percentage;
	
	distribution.AddColumn(t_("Key"));
	distribution.AddColumn(t_("Weight (\%)"));
	distribution.ColumnWidths("5 1");
	
	percentage.AddColumn(t_("Part"));
	String cw = "3";
	for(int i = 0; i < WORDGROUP_COUNT; i++) {
		percentage.AddColumn(WordGroupString[i]);
		cw << " 1";
	}
	percentage.ColumnWidths(cw);
	
	notes.AddColumn(t_("Rhyme #"));
	notes.AddColumn(t_("Note #"));
	notes.AddColumn(t_(""));
	notes.AddColumn(t_(""));
	
	suggestion.AddColumn(t_("Rhyme #"));
	suggestion.AddColumn(t_("Note #"));
	suggestion.AddColumn(t_(""));
	suggestion.AddColumn(t_(""));
	
	
}

void ContextIdeaCtrl::DisableAll() {
	
}

void ContextIdeaCtrl::EnableAll() {
	
}

void ContextIdeaCtrl::Data() {
	Song& s = GetSong();
	
	
	UpdateAll();
	
	
	distribution.SetCount(args.roles.GetCount());
	for(int i = 0; i < args.roles.GetCount(); i++) {
		distribution.Set(i, 0, args.roles.GetKey(i));
		distribution.Set(i, 1, args.roles[i] * 100);
	}
	
	percentage.SetCount(s.parts.GetCount());
	for(int i = 0; i < s.parts.GetCount(); i++) {
		const StaticPart& part = s.parts[i];
		percentage.Set(i, 0, part.name);
		for(int j = 0; j < WORDGROUP_COUNT; j++) {
			double val = j < part.wordgroup_factors.GetCount() ? part.wordgroup_factors[j] : 0;
			percentage.Set(i, 1+j, (int)(val*100+0.5));
		}
	}
	
}

void ContextIdeaCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Get word group percentages"), AppImg::BlueRing(), THISBACK(GetWordGroupPercentages)).Key(K_CTRL_Q);
	bar.Add(t_("Get suggestions"), AppImg::BlueRing(), THISBACK(GetSuggestions)).Key(K_CTRL_E);
	bar.Add(t_("Use suggestions"), AppImg::BlueRing(), THISBACK(UseSuggestions)).Key(K_CTRL_R);
	
}

void ContextIdeaCtrl::UpdateAll() {
	UpdateParts();
	UpdateRoles();
}

void ContextIdeaCtrl::GetWordGroupPercentages() {
	Song& s = GetSong();
	
	UpdateAll();
	
	s.RealizePipe();
	
	args.fn = 0;
	TaskMgr& m = *s.pipe;
	m.GetContextIdea(args, THISBACK(OnWordGroupPercentages));
}

void ContextIdeaCtrl::OnWordGroupPercentages(String res) {
	Song& s = GetSong();
	for(int i = 0; i < s.parts.GetCount(); i++)
		s.parts[i].wordgroup_factors.Clear();
	
	Vector<String> lines = Split(res, "\n");
	
	if (lines.GetCount() != s.parts.GetCount()) {
		PromptOK("unexpected line count");
		return;
	}
	
	VectorMap<String, Vector<double>> parsed;
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		int a = line.Find("\":");
		String key;
		if (a >= 0) {
			key = ToLower(TrimBoth(line.Left(a+1)));
			if (key.Left(1) == "-") key = TrimBoth(key.Mid(1));
			RemoveQuotes(key);
			line = TrimBoth(line.Mid(a+2));
		}
		else key = ToLower(s.parts[0].name);
		auto& out = parsed.Add(key);
		
		Vector<String> parts = Split(line, ",");
		if (parts.IsEmpty()) {
			PromptOK("unexpected line: no comma");
			return;
		}
		if (parts.GetCount() != WORDGROUP_COUNT) {
			PromptOK("unexpected line: count mismatch");
			return;
		}
		
		for(int j = 0; j < parts.GetCount(); j++) {
			Vector<String> kv = Split(parts[j], ":");
			if (kv.GetCount() != 2 && !(i == 0 && j == 0)) {
				PromptOK("unexpected line: no key/value pair");
				return;
			}
			String value = TrimBoth(kv.Top());
			double perc = ScanDouble(value) / 100.0;
			out << perc;
		}
	}
	
	//DUMPCC(parsed);
	
	for(int i = 0; i < parsed.GetCount(); i++) {
		String key = parsed.GetKey(i);
		bool found = false;
		for(int k = 0; k < s.parts.GetCount(); k++) {
			if (ToLower(s.parts[k].name) == key) {
				s.parts[k].wordgroup_factors <<= parsed[i];
				found = true;
				break;
			}
		}
		if (!found) {
			PromptOK("not found: " + key);
		}
	}
	
	PostCallback(THISBACK(Data));
}

void ContextIdeaCtrl::GetSuggestions() {
	
}

void ContextIdeaCtrl::UseSuggestions() {
	
}

void ContextIdeaCtrl::UpdateParts() {
	Song& s = GetSong();
	
	args.bpm = ScanInt(s.data.Get("BPM", "120"));
	double beats_per_sec = args.bpm / 60.0;
	double beat_sec = 1.0 / beats_per_sec;
	
	args.parts.SetCount(s.parts.GetCount());
	
	for(int i = 0; i < args.parts.GetCount(); i++) {
		const StaticPart& part = s.parts[i];
		ContextIdeaArgs::Part& cpart = args.parts[i];
		
		double duration = 0;
		int total_notes = 0;
		for(int j = 0; j < part.rhymes.GetCount(); j++) {
			const StaticRhyme& rhyme = part.rhymes[j];
			duration += rhyme.beat_length * beat_sec;
			total_notes += rhyme.notes.GetCount();
		}
		
		cpart.title       = part.name;
		cpart.duration    = (int)(duration + 0.5);
		cpart.total_notes = total_notes;
		cpart.est_words   = (int)(total_notes / 3.6);
	}
}

void ContextIdeaCtrl::UpdateRoles() {
	Song& s = GetSong();
	
	args.roles.Clear();
	
	for(int i = 0; i < s.prod_ideas.GetCount(); i++) {
		const ProductionIdea& id = s.prod_ideas[i];
		if (id.topic == "distribution of roles in lyrics") {
			Vector<String> parts = Split(id.value, ",");
			for (String& s : parts) {
				s = TrimBoth(s);
				Vector<String> kv = Split(s, ":");
				if (kv.GetCount() == 2) {
					String key = TrimBoth(kv[0]);
					String value = TrimBoth(kv[1]);
					double perc = ScanDouble(value) / 100.0;
					args.roles.GetAdd(key, 0) += perc;
				}
			}
		}
	}
	
	double sum = 0;
	for(int i = 0; i < args.roles.GetCount(); i++)
		sum += args.roles[i];
	if (sum == 0) return;
	double mul = 1.0 / sum;
	for(int i = 0; i < args.roles.GetCount(); i++)
		args.roles[i] *= mul;
	
	SortByValue(args.roles, StdGreater<double>());
	//DUMPM(args.roles);
}
