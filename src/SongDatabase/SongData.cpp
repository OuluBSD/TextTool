#include "SongDatabase.h"


BEGIN_SONGLIB_NAMESPACE


SongData::SongData() {
	
}

void SongData::Store() {
	StoreToFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Load() {
	LoadFromFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Serialize(Stream& s) {
	s % artists_en % artists_fi;
}

/*void SongData::StoreJson() {
	String dir = SongDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json", true);
}

void SongData::LoadJson() {
	String dir = SongDatabase::Single().dir;
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "SongData.json");
}

void SongData::Jsonize(JsonIO& json) {
	json
		("artists_en", artists_en)
		("artists_fi", artists_fi)
		;
}*/



void SongDataAnalysis::Store() {
	StoreToFile(*this, ConfigFile("SongData_Analysis.bin"));
	StoreJson();
}

void SongDataAnalysis::Load() {
	LoadFromFile(*this, ConfigFile("SongData_Analysis.bin"));
	if (datasets.IsEmpty())
		LoadJson();
	
	SongData& sd = SongDatabase::Single().song_data;
	for(int i = 0; i < sd.GetCount(); i++) {
		String key = sd.GetKey(i);
		auto& ds = sd.a.datasets.GetAdd(key);
		ds.Load(i, key);
	}
}

void SongDataAnalysis::StoreJson() {
	String dir = SongDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, ConfigFile("SongData.json"), false);
}

void SongDataAnalysis::LoadJson() {
	#if 0
	DatasetAnalysis da;
	LoadFromJsonFileStandard(da, ConfigFile("SongData.json"));
	
	SongData& sd = SongDatabase::Single().song_data;
	for(int j = 0; j < sd.GetCount(); j++) {
		DatasetAnalysis& tgt = datasets.GetAdd(sd.GetKey(j));
		
		for(int k = 0; k < da.artists.GetCount(); k++) {
			String name0 = da.artists.GetKey(k);
			if (name0.IsEmpty()) continue;
			
			for(int i = 0; i < sd.artists_en.GetCount(); i++) {
				String name1 = sd.artists_en[i].name;
				if (name0 == name1) {
					Swap(da.artists[k], tgt.artists.Add(name0));
					break;
				}
			}
		}
	}
	#else
	String dir = SongDatabase::Single().dir;
	LoadFromJsonFileStandard(*this, ConfigFile("SongData.json"));
	#endif
}


#if 0

String LyricsAnalysis::AsString() const {
	String s;
	s << "Phrase:\n";
	for (const LyricsAnalysis::Phrase& p : phrases) {
		s << "\"" << p.phrase << "\": " << p.group << ": " << p.value << ": RGB(" << p.clr.GetR() << ", " << p.clr.GetG() << ", " << p.clr.GetB() << ")\n";
	}
	s << "\n\n";
	
	s << "Positive roles:\n";
	for (const Vector<LyricsAnalysis::Role>& v : positive_roles) {
		s << "- ";
		int j = 0;
		for (const LyricsAnalysis::Role& r : v) {
			if (j++) s << ", ";
			s << r.group << "/" << r.value << ": " << r.percent << "\%";
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Negative roles:\n";
	for (const Vector<LyricsAnalysis::Role>& v : negative_roles) {
		s << "- ";
		int j = 0;
		for (const LyricsAnalysis::Role& r : v) {
			if (j++) s << ", ";
			s << r.group << "/" << r.value << ": " << r.percent << "\%";
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Word groups:\n";
	for(int i = 0; i < word_groups.GetCount(); i++) {
		String key = word_groups.GetKey(i);
		s << "- " << key << ": ";
		const auto& v = word_groups[i];
		for(int j = 0; j < v.GetCount(); j++) {
			if (j) s << ", ";
			s << v[j];
		}
		s << "\n";
	}
	s << "\n\n";
	
	s << "Rhyme locations:\n";
	for(int i = 0; i < rhyme_locations.GetCount(); i++) {
		const RhymeLocationLine& l = rhyme_locations[i];
		for(int j = 0; j < l.words.GetCount(); j++) {
			if (j) s << " ";
			bool b = j < l.rhyming.GetCount() ? l.rhyming[j] : false;
			if (b) s << "(";
			s << l.words[j];
			if (b) s << ")";
		}
		s << "\n";
	}
	
	return s;
}

String TemplatePhrase::GetText() const {
	String s;
	for (const String& p : parts) {
		if (p == ",")
			s << ",";
		else {
			if (s.GetCount())
				s << " ";
			s << p;
		}
	}
	return s;
}

String ActionPhrase::GetActionText() const {
	String l;
	for (const auto& arg : actions) {
		if (!l.IsEmpty()) l << " + ";
		l << arg.action << "(" << arg.arg << ")";
	}
	return l;
}

double GetActionMatching(const Vector<ActionHeader>& a, const Vector<ActionHeader>& b) {
	if (!a.GetCount()) return 0;
	int partial_matches = 0, full_matches = 0;
	for (const ActionHeader& aa : a) {
		for (const ActionHeader& bb : b) {
			if (aa.action == bb.action) {
				partial_matches++;
				if (aa.arg == bb.arg) {
					full_matches++;
				}
			}
		}
	}
	//int max_matches = min(a.GetCount(), b.GetCount());
	int max_matches = a.GetCount();
	/*double perc0 = (double)partial_matches / (double)max_matches;
	double perc1 = (double)full_matches / (double)max_matches;
	double perc = (perc0 + perc1) * 0.5;
	*/
	double perc = (double)full_matches / (double)max_matches;
	return perc;
}




bool DatasetAnalysis::RealizeAttribute(const char* group, const char* value, int& attr_group, int& attr_value) {
	attr_group = Attr::FindAttrGroup(group);
	if (attr_group < 0) {
		attr_group = Attr::FindAttrGroupByValue(value);
		if (attr_group < 0) {
			attr_group = dynamic_attrs.Find(group);
			if (attr_group < 0) {
				attr_group = Attr::ATTR_COUNT + dynamic_attrs.GetCount();
				attr_value = 0;
				dynamic_attrs.Add(group).Add(value);
			}
			else {
				attr_value = dynamic_attrs[attr_group].FindAdd(value);
			}
		}
		else {
			attr_value = Attr::FindAttrValue(attr_group, value);
			return true; // this value can be backported (it was semi-error)
		}
	}
	else
		attr_value = Attr::FindAttrValue(attr_group, value);
	return false;
}

void DatasetAnalysis::RealizeAction(const char* action, const char* arg, int16& action_i, int16& arg_i) {
	action_i = dynamic_actions.Find(action);
	if (action_i < 0) {
		action_i = dynamic_actions.GetCount();
		arg_i = 0;
		auto& idx = dynamic_actions.Add(action);
		idx.Add(arg);
	}
	else {
		auto& idx = dynamic_actions[action_i];
		arg_i = idx.FindAdd(arg);
	}
}

#endif






DatasetAnalysis::DatasetAnalysis() {
	
}

void DatasetAnalysis::Load(int ds_i, const String& ds_key) {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "songdata");
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
	
	
	String song_dir = AppendFileName(ds_dir, "songs");
	RealizeDirectory(song_dir);
	
	songs.Clear();
	FindFile ff(AppendFileName(song_dir, "*"));
	do {
		if (!ff.IsDirectory()) continue;
		String title = ff.GetName();
		if (title.Find(".") == 0) continue;
		
		SongAnalysis& sa = songs.Add(title);
		sa.da = this;
		sa.Load(ff.GetPath());
	}
	while (ff.Next());
}

SongAnalysis& DatasetAnalysis::GetSongAnalysis(const String& name) {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	String dir = AppendFileName(db.dir, "share" DIR_SEPS "songdata");
	RealizeDirectory(dir);
	
	String ds_dir = AppendFileName(dir, sd.GetKey(ds_i));
	RealizeDirectory(ds_dir);
	
	String song_dir = AppendFileName(ds_dir, "songs" DIR_SEPS + name);
	RealizeDirectory(song_dir);
	
	SongAnalysis& sa = songs.GetAdd(name);
	
	if (sa.da == 0) {
		sa.da = this;
		sa.Load(song_dir);
	}
	
	return sa;
}

void SongAnalysis::Load(const String& dir) {
	
	for(int i = 0; i < ContrastType::PART_COUNT; i++) {
		phrase_parts[i].Load(dir, "phrase parts " + IntStr(i));
		source_pool[i].Load(dir, "source pool " + IntStr(i));
		phrase_combs[i].Load(dir, "phrase combinations " + IntStr(i));
	}
	
	lyrics_suggs.Load(dir, "lyrics suggestions");
	
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



void SongCandidateCache::Realize(Lyrics& l) {
	if (l.parts.GetCount() != parts.GetCount()) {
		parts.SetCount(l.parts.GetCount());
		
	}
}

String LyricsSuggestions::GetText() const {
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


END_SONGLIB_NAMESPACE
