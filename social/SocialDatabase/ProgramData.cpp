#include "SocialDatabase.h"


BEGIN_SOCIALLIB_NAMESPACE


ProgramData::ProgramData() {
	
}

void ProgramData::Store() {
	StoreToFile(*this, ConfigFile("ProgramData.bin"));
}

void ProgramData::Load() {
	LoadFromFile(*this, ConfigFile("ProgramData.bin"));
}

void ProgramData::Serialize(Stream& s) {
	s % companies_en % companies_fi;
}

/*void ProgramData::StoreJson() {
	String dir = SocialDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "ProgramData.json", true);
}

void ProgramData::LoadJson() {
	String dir = SocialDatabase::Single().dir;
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "ProgramData.json");
}

void ProgramData::Jsonize(JsonIO& json) {
	json
		("companies_en", companies_en)
		("companies_fi", companies_fi)
		;
}*/



void ProgramDataAnalysis::Store() {
	StoreToFile(*this, ConfigFile("ProgramData_Analysis.bin"));
	StoreJson();
}

void ProgramDataAnalysis::Load() {
	LoadFromFile(*this, ConfigFile("ProgramData_Analysis.bin"));
	if (datasets.IsEmpty())
		LoadJson();
	
	ProgramData& sd = SocialDatabase::Single().program_data;
	for(int i = 0; i < sd.GetCount(); i++) {
		String key = sd.GetKey(i);
		auto& ds = sd.a.datasets.GetAdd(key);
		ds.Load(i, key);
	}
}

void ProgramDataAnalysis::StoreJson() {
	String dir = SocialDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, ConfigFile("ProgramData.json"), false);
}

void ProgramDataAnalysis::LoadJson() {
	#if 0
	DatasetAnalysis da;
	LoadFromJsonFileStandard(da, ConfigFile("ProgramData.json"));
	
	ProgramData& sd = SocialDatabase::Single().program_data;
	for(int j = 0; j < sd.GetCount(); j++) {
		DatasetAnalysis& tgt = datasets.GetAdd(sd.GetKey(j));
		
		for(int k = 0; k < da.companies.GetCount(); k++) {
			String name0 = da.companies.GetKey(k);
			if (name0.IsEmpty()) continue;
			
			for(int i = 0; i < sd.companies_en.GetCount(); i++) {
				String name1 = sd.companies_en[i].name;
				if (name0 == name1) {
					Swap(da.companies[k], tgt.companies.Add(name0));
					break;
				}
			}
		}
	}
	#else
	String dir = SocialDatabase::Single().dir;
	LoadFromJsonFileStandard(*this, ConfigFile("ProgramData.json"));
	#endif
}




DatasetAnalysis::DatasetAnalysis() {
	
}

void DatasetAnalysis::Load(int ds_i, const String& ds_key) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "programdata");
	RealizeDirectory(dir);
	
	String ds_dir = AppendFileName(dir, ds_key);
	RealizeDirectory(ds_dir);
	String trans_ds_key;
	if (ds_i == 0)
		trans_ds_key = "fi";
	else
		trans_ds_key = "en";
	
	this->ds_i = ds_i;
	
	tokens.Load(ds_dir, "tokens");
	token_texts.Load(ds_dir, "tokenized texts");
	word_classes.Load(ds_dir, "word classes");
	words.Load(ds_dir, "words");
	ambiguous_word_pairs.Load(ds_dir, "ambiguous word pairs");
	virtual_phrases.Load(ds_dir, "virtual phrases");
	virtual_phrase_parts.Load(ds_dir, "virtual phrase parts");
	virtual_phrase_structs.Load(ds_dir, "virtual phrase structure types");
	struct_part_types.Load(ds_dir, "structure part types");
	struct_types.Load(ds_dir, "structure types");
	phrase_parts.Load(ds_dir, "phrase parts");
	attrs.Load(ds_dir, "attributes");
	actions.Load(ds_dir, "action");
	parallel.Load(ds_dir, "action parallel");
	trans.Load(ds_dir, "action transition");
	action_phrases.Load(ds_dir, "action phrases");
	translations.Load(ds_dir, trans_ds_key);
	wordnets.Load(ds_dir, "wordnets");
	diagnostics.Load(ds_dir, "diagnostics");
	simple_attrs.Load(ds_dir, "simple_attrs");
	
	
	String program_dir = AppendFileName(ds_dir, "programs");
	RealizeDirectory(program_dir);
	
	programs.Clear();
	FindFile ff(AppendFileName(program_dir, "*"));
	do {
		if (!ff.IsDirectory()) continue;
		String title = ff.GetName();
		if (title.Find(".") == 0) continue;
		
		ProgramAnalysis& sa = programs.Add(title);
		sa.da = this;
		sa.Load(ff.GetPath());
	}
	while (ff.Next());
}

ProgramAnalysis& DatasetAnalysis::GetProgramAnalysis(const String& name) {
	SocialDatabase& db = SocialDatabase::Single();
	ProgramData& sd = db.program_data;
	ProgramDataAnalysis& sda = db.program_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "programdata");
	RealizeDirectory(dir);
	
	String ds_dir = AppendFileName(dir, sd.GetKey(ds_i));
	RealizeDirectory(ds_dir);
	
	String program_dir = AppendFileName(ds_dir, "programs" DIR_SEPS + name);
	RealizeDirectory(program_dir);
	
	ProgramAnalysis& sa = programs.GetAdd(name);
	
	if (sa.da == 0) {
		sa.da = this;
		sa.Load(program_dir);
	}
	
	return sa;
}

void ProgramAnalysis::Load(const String& dir) {
	
	for(int i = 0; i < ContrastType::PART_COUNT; i++) {
		phrase_parts[i].Load(dir, "phrase parts " + IntStr(i));
		source_pool[i].Load(dir, "source pool " + IntStr(i));
		phrase_combs[i].Load(dir, "phrase combinations " + IntStr(i));
	}
	
	story_suggs.Load(dir, "stories suggestions");
	
}

String DatasetAnalysis::GetTokenTextString(const TokenText& txt) const {
	String o;
	for(int tk_i : txt.tokens) {
		//const Token& tk = tokens[tk_i];
		const String& key = tokens.GetKey(tk_i);
		
		if (key.GetCount() == 1 && NaturalTokenizer::IsToken(key[0])) {
			o << key;
		}
		else {
			if (!o.IsEmpty())
				o << " ";
			o << key;
		}
	}
	return o;
}

String DatasetAnalysis::GetTokenTypeString(const TokenText& txt) const {
	String o;
	for(int tk_i : txt.tokens) {
		const Token& tk = tokens[tk_i];
		int w_i = tk.word_;
		if (w_i < 0) {
			String key = ToLower(tokens.GetKey(tk_i));
			w_i = words.Find(key);
			tk.word_ = w_i;
		}
		if (w_i < 0) {
			o << "{error}";
		}
		else {
			const ExportWord& ew = words[w_i];
			o << "{";
			for(int i = 0; i < ew.class_count; i++) {
				if (i) o << "|";
				int class_i = ew.classes[i];
				const String& wc = word_classes[class_i];
				o << wc;
			}
			o << "}";
			/*if (key.GetCount() == 1 && NaturalTokenizer::IsToken(key[0])) {
				o << key;
			}
			else {
				if (!o.IsEmpty())
					o << " ";
				o << key;
			}*/
		}
	}
	return o;
}

String DatasetAnalysis::GetWordString(const Vector<int>& words) const {
	String o;
	for(int w_i : words) {
		if (w_i < 0) continue;
		const String& key = this->words.GetKey(w_i);
		
		if (key.GetCount() == 1 && NaturalTokenizer::IsToken(key[0])) {
			o << key;
		}
		else {
			if (!o.IsEmpty())
				o << " ";
			o << key;
		}
	}
	return o;
}

WString DatasetAnalysis::GetWordPronounciation(const Vector<int>& words) const {
	WString o;
	for(int w_i : words) {
		if (w_i < 0) continue;
		const ExportWord& ew = this->words[w_i];
		const WString& key = ew.phonetic;
		
		if (key.GetCount() == 1 && NaturalTokenizer::IsToken(key[0])) {
			o << key;
		}
		else {
			if (!o.IsEmpty())
				o << " ";
			o << key;
		}
	}
	return o;
}

String DatasetAnalysis::GetTypeString(const Vector<int>& word_classes) const {
	String o;
	for(int wc_i : word_classes) {
		if (wc_i < 0)
			o << "{error}";
		else {
			const String& wc = this->word_classes[wc_i];
			o << "{" << wc << "}";
		}
	}
	return o;
}

String DatasetAnalysis::GetActionString(const Vector<int>& actions) const {
	String o;
	for(int act_i : actions) {
		if (!o.IsEmpty()) o << ", ";
		if (act_i < 0)
			o << "error";
		else {
			const ActionHeader& ah = this->actions.GetKey(act_i);
			o << ah.action;
			if (!ah.arg.IsEmpty())
				o << "(" << ah.arg << ")";
		}
	}
	return o;
}



void ProgramCandidateCache::Realize(Story& l) {
	if (l.parts.GetCount() != parts.GetCount()) {
		parts.SetCount(l.parts.GetCount());
		
	}
}

String StorySuggestions::GetText() const {
	String content;
	for(int j = 0; j < lines.GetCount(); j++) {
		String part = lines.GetKey(j);
		const auto& v = lines[j];
		if (part.IsEmpty() || v.IsEmpty()) continue;
		content << part << ":\n";
		
		for(int k = 0; k < v.GetCount(); k++) {
			content << v[k] << "\n";
		}
		content << "\n";
	}
	return content;
}


END_SOCIALLIB_NAMESPACE
