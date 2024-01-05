#include "Database.h"

#if 0

void SongData::Store() {
	StoreToFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Load() {
	LoadFromFile(*this, ConfigFile("SongData.bin"));
}

void SongData::Serialize(Stream& s) {
	s % artists_en % artists_fi;
}




void SongDataAnalysis::Store() {
	StoreToFile(*this, ConfigFile("SongData_Analysis.bin"));
	StoreJson();
}

void SongDataAnalysis::Load() {
	LoadFromFile(*this, ConfigFile("SongData_Analysis.bin"));
	if (datasets.IsEmpty())
		LoadJson();
}

void SongDataAnalysis::StoreJson() {
	String dir = Database::Single().dir;
	StoreAsJsonFileStandard(*this, ConfigFile("SongData.json"), false);
}

void SongDataAnalysis::LoadJson() {
	#if 0
	DatasetAnalysis da;
	LoadFromJsonFileStandard(da, ConfigFile("SongData.json"));
	
	SongData& sd = Database::Single().song_data;
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
	String dir = Database::Single().dir;
	LoadFromJsonFileStandard(*this, ConfigFile("SongData.json"));
	#endif
}



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




void SongData::Load() {
	for(int i = 0; i < GetCount(); i++) {
		String key = GetKey(i);
		auto& ds = a.datasets.GetAdd(key);
		ds.Load(i, key);
	}
}



DatasetAnalysis::DatasetAnalysis() {
	
}

void DatasetAnalysis::Load(int ds_i, const String& ds_key) {
	Database& db = Database::Single();
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
	
	
	tokens.Load(ds_dir, "tokens");
	token_texts.Load(ds_dir, "tokenized texts");
	word_classes.Load(ds_dir, "word classes");
	words.Load(ds_dir, "words");
	ambiguous_word_pairs.Load(ds_dir, "ambiguous word pairs");
	virtual_phrases.Load(ds_dir, "virtual phrases");

	/*MapFile<int,WordPairType> old_ambiguous_word_pairs;
	old_ambiguous_word_pairs.Load(ds_dir, "old ambiguous word pairs");
	for(int i = 0; i < old_ambiguous_word_pairs.GetCount(); i++) {
		auto& old = old_ambiguous_word_pairs[i];
		old.from = old_ambiguous_word_pairs.GetKey(i);
		hash_t h = old.GetHashValue();
		auto& n = ambiguous_word_pairs.GetAdd(h);
		n = old;
	}*/
	
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
