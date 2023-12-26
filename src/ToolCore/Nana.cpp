#include "ToolCore.h"



MockupPhraseParser::MockupPhraseParser() {
	
}

bool MockupPhraseParser::Parse(String txt) {
	fail = false;
	
	Vector<String> lines = Split(txt, "\n");
	
	int lc = lines.GetCount();
	if (!lc) {
		SetError("no lines");
		return false;
	}
	
	tokens.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		tokens << Split(l, " ");
	}
	
	DUMPCC(tokens);
	
	return true;
}

bool MockupPhraseParser::IsVowel(int chr) {
	chr = ToLower(chr);
	return	chr == 'a' ||
			chr == 'e' ||
			chr == 'i' ||
			chr == 'o' ||
			chr == 'u' ||
			chr == 'y';
}

bool MockupPhraseParser::IsConsonant(int chr) {
	chr = ToLower(chr);
	return	chr == 'b' ||
			chr == 'c' ||
			chr == 'd' ||
			chr == 'f' ||
			chr == 'g' ||
			chr == 'h' ||
			chr == 'j' ||
			chr == 'k' ||
			chr == 'l' ||
			chr == 'm' ||
			chr == 'n' ||
			chr == 'p' ||
			chr == 'q' ||
			chr == 'r' ||
			chr == 's' ||
			chr == 't' ||
			chr == 'v' ||
			chr == 'x' ||
			chr == 'z';
}


bool MockupPhraseParser::Process(RhymeContainer& rc) {
	int lc = tokens.GetCount();
	if (!lc) {
		SetError("no tokens");
		return false;
	}
	
	rc.Clear();
	
	
	for(int i = 0; i < tokens.GetCount(); i++) {
		auto& ltokens = tokens[i];
		bool prev_continued = false;
		RhymeContainer::Word* word_ptr = 0;
		RhymeContainer::Line& line = rc.lines.Add();
		for(int j = 0; j < ltokens.GetCount(); j++) {
			const String& s = ltokens[j];
			int sc = s.GetCount();
			ASSERT(sc);
			bool continues = false;
			if (s[sc-1] == '-') {
				continues = true;
				sc--;
			}
			if (!sc) {
				SetError("part without wovels nor consonants");
				return false;
			}
			if (sc < 2) {
				SetError("part is too short");
				return false;
			}
			int chr0 = s[0];
			if (!IsConsonant(chr0)) {
				SetError("part does not start with a consonant");
				return false;
			}
			bool at_beat = IsUpper(chr0);
			if (at_beat) chr0 = ToLower(chr0);
			
			int consonant_importance = -1;
			if      (chr0 == 'd')	consonant_importance = 0;
			else if (chr0 == 't')	consonant_importance = 1;
			else if (chr0 == 'n')	consonant_importance = 2;
			else if (chr0 == 's')	consonant_importance = 3;
			
			int chr1 = s[1];
			if (IsUpper(chr1)) chr1 = ToLower(chr1);
			if (!IsVowel(chr1)) {
				SetError("the second char is not a vowel");
				return false;
			}
			int vocal_importance = 0;
			if      (chr1 == 'u')	vocal_importance = 0;
			else if (chr1 == 'i')	vocal_importance = 1;
			else if (chr1 == 'a')	vocal_importance = 2;
			else if (chr1 == 'y')	vocal_importance = 3;
			
			
			if (prev_continued && word_ptr) {
				// pass
			}
			else {
				RhymeContainer::Word& w = line.words.Add();
				word_ptr = &w;
			}
			RhymeContainer::Syllable& syl = word_ptr->syllables.Add();
			syl.consonant_importance = consonant_importance;
			syl.vocal_importance = vocal_importance;
			syl.long_ = sc > 2;
			syl.at_beat = at_beat;
				
			prev_continued = continues;
		}
	}
	
	return true;
}

String RhymeContainer::ToString() const {
	String o;
	
	for (const auto& l : lines) {
		if (!o.IsEmpty())
			o << "\n";
		for (const auto& w : l.words) {
			o << "(";
			int i = 0;
			for (const auto& s : w.syllables) {
				if (i++) o << " ";
				o << s.AsString();
			}
			o << ")";
		}
	}
	
	return o;
}

String RhymeContainer::AsNana() const {
	String s;
	for (const auto& l : lines) {
		if (s.GetCount()) s << "\n";
		s << l.AsNana();
	}
	return s;
}

String RhymeContainer::Line::AsNana() const {
	String s;
	int w_i = 0;
	for (const Word& w : words) {
		if (w_i++)
			s.Cat(' ');
		int sy_i = 0;
		for (const Syllable& sy : w.syllables) {
			if (sy_i++) s << "- ";
			s << sy.AsString();
		}
	}
	return s;
}

/*
consonant
n = important
t = meaningful
d = meaningless
s = specific

vocal
a = important
i = meaningful
u = meaningless


*/
String RhymeContainer::Syllable::AsString() const {
	int vocal = 0, conso = 0;
	switch (consonant_importance) {
		case 0: conso = 'd'; break;
		case 1: conso = 't'; break;
		case 2: conso = 'n'; break;
		case 3: conso = 's'; break;
		default: break;
	}
	
	switch (vocal_importance) {
		case 0: vocal = 'u'; break;
		case 1: vocal = 'i'; break;
		case 2: vocal = 'a'; break;
		case 3: vocal = 'y'; break;
		default: break;
	}
	
	String s;
	if (conso && at_beat) conso = ToUpper(conso);
	if (conso) s.Cat(conso);
	if (vocal) s.Cat(vocal);
	if (vocal && long_) s.Cat(vocal);
	return s;
}



bool PhoneticNanaAnalyser::Parse(wchar* phon, int len) {
	
	// Update length
	for(int i = 0; i < len; i++) {
		if (!phon[i]) {
			len = i;
			break;
		}
	}
	if (!len) {
		error = "empty string";
		return false;
	}
	tmp.SetCount(len);
	for(int i = 0; i < len; i++)
		tmp[i] = phon[i];
	
	int* begin = tmp.Begin();
	int* end = begin + len;
	
	
	TODO // fail starts here
	
	// Get unique vowels and consonants
	vowels.Clear();
	consonants.Clear();
	enums.SetCount(0);
	vowel_vec.SetCount(0);
	consonant_vec.SetCount(0);
	int prev = 0;
	for (int* c = begin; c < end;) {
		int len = 0;
		int p = GetPhonemeEnum(c[0], c[1], &len);
		if (p < 0) {
			int w = *c;
			if (w == '\'')
				enums << -2; // long vowel
			else if (IsSpace(w) && prev >= 0)
				enums << -1;
			else
				p = INT_MAX; // invalid phoneme
			c++;
		}
		else {
			if (IsPhonemeVowel(p)) {
				vowels.FindAdd(p);
				enums << p;
				vowel_vec << p;
			}
			else if (IsPhonemeConsonant(p)) {
				consonants.FindAdd(p);
				enums << p;
				consonant_vec << p;
			}
			c += len;
		}
		prev = p;
	}
	
	
	// Get average distance values per vowel and consonant
	
	for (auto& t : vowels.GetValues()) {t.a = 0; t.b = 0;}
	int* vbegin = vowel_vec.Begin();
	int* vend = vowel_vec.End();
	for(int i = 0; i < vowels.GetCount(); i++) {
		int c0 = vowels.GetKey(i);
		auto& vals = vowels[i];
		for (int* c = vbegin; c != vend; c++) {
			int c1 = *c;
			double distance = vowel_distance[c0][c1];
			int time = GetPhonemeRepeats(c1, STRESS_SECONDARY);
			double weight = distance * time;
			vals.a += weight;
			vals.b += time;
		}
	}
	
	for (auto& t : consonants.GetValues()) {t.a = 0; t.b = 0;}
	int* cbegin = consonant_vec.Begin();
	int* cend = consonant_vec.End();
	for(int i = 0; i < consonants.GetCount(); i++) {
		int c0 = consonants.GetKey(i) - PHONOME_VOWEL_COUNT;
		auto& vals = consonants[i];
		for (int* c = cbegin; c != cend; c++) {
			int c1 = *c - PHONOME_VOWEL_COUNT;
			double distance = consonant_distance[c0][c1];
			int time = GetPhonemeRepeats(c1, STRESS_SECONDARY);
			double weight = distance * time;
			vals.a += weight;
			vals.b += time;
		}
	}
	
	
	// Sort vowels and consonants based on lowest weighted distance to other phonemes
	struct PhonemeWeightedSorter {
		using T = Tuple2<double,int>;
		bool operator()(const T& a, const T& b) const {
			return a.a < b.a;
		}
	};
	Sort(vowels, PhonemeWeightedSorter());
	Sort(consonants, PhonemeWeightedSorter());
	
	
	// Start creating clusters from the lowest distance, and by adding all <0.5 distance phonemes
	// Compare to the first phoneme in the cluster.
	{
		auto& src_phonemes = vowels;
		auto& src_clusters = vowel_clusters;
		src_clusters.SetCount(0);
		Vector<int>* cur_cluster = 0;
		int cur_cluster_c0 = -1;
		for(int i = 0; i < src_phonemes.GetCount(); i++) {
			int c1 = src_phonemes.GetKey(i);
			if (!cur_cluster) {
				cur_cluster = &src_clusters.Add();
				cur_cluster_c0 = c1;
				*cur_cluster << c1;
			}
			else {
				double distance = vowel_distance[cur_cluster_c0][c1];
				if (distance <= cluster_phoneme_distance_limit)
					*cur_cluster << c1;
				else {
					cur_cluster = &src_clusters.Add();
					cur_cluster_c0 = c1;
					*cur_cluster << c1;
				}
			}
		}
	}
	{
		auto& src_phonemes = consonants;
		auto& src_clusters = consonant_clusters;
		src_clusters.SetCount(0);
		Vector<int>* cur_cluster = 0;
		int cur_cluster_c0 = -1;
		for(int i = 0; i < src_phonemes.GetCount(); i++) {
			int c1 = src_phonemes.GetKey(i);
			if (!cur_cluster) {
				cur_cluster = &src_clusters.Add();
				cur_cluster_c0 = c1;
				*cur_cluster << c1;
			}
			else {
				double distance = consonant_distance[cur_cluster_c0][c1];
				if (distance <= cluster_phoneme_distance_limit)
					*cur_cluster << c1;
				else {
					cur_cluster = &src_clusters.Add();
					cur_cluster_c0 = c1;
					*cur_cluster << c1;
				}
			}
		}
	}
	//DUMPCC(vowel_clusters);
	//DUMPCC(consonant_clusters);
	
	
	// Make nana-code
	int* ebegin = enums.Begin();
	int* eend = enums.End();
	bool continue_prev = false;
	bool prev_long = false;
	nana_out.SetCount(0);
	for (int* c = ebegin; c < eend; c++) {
		int p0 = *c;
		if (p0 < 0) {
			if (p0 == -1)
				continue_prev = false;
			if (p0 == -2)
				prev_long = true;
		}
		else {
			bool is_vowel = p0 < PHONOME_VOWEL_COUNT;
			int v_cl = -1;
			int c_cl = -1;
			
			TODO
			/*if (is_vowel) {
				int i = 0;
				for (const auto& vv : vowel_clusters) {
					bool found = false;
					for (int p1 : vv) {
						if (p0 == p1) {
							found = true;
							break;
						}
					}
					if (found) {
						v_cl = i;
						break;
					}
					i++;
				}
			}
			else {
				int i = 0;
				for (const auto& vv : consonant_clusters) {
					bool found = false;
					for (int p1 : vv) {
						if (p0 == p1) {
							found = true;
							break;
						}
					}
					if (found) {
						c_cl = i;
						break;
					}
					i++;
				}
			}
			ASSERT(v_cl >= 0);
			
			// Nana code item
			byte nana = 0;
			if (is_vowel) {
				cl0 = min(7, cl0);
				nana |= (byte)cl0;
			}
			else {
				cl0 = min(7, cl0);
				nana |= (byte)cl0 << 3; // consonant offset
			}
			
			if (prev_long) {
				prev_long = false;
				nana |= (byte)NANA_LONG;
			}
			
			if (continue_prev) {
				nana |= (byte)NANA_CONTINUE;
			}
			
			nana_out << nana;
			*/
			continue_prev = true;
		}
	}
	
	if (nana_out.IsEmpty())
		return false;
	
	return true;
}

void PhoneticNanaAnalyser::WritePackedNana(byte* nana, int len) {
	int write = min(len, nana_out.GetCount());
	int zero = len - write;
	if (write > 0)
		memcpy(nana, nana_out.Begin(), write);
	if (zero > 0)
		memset(nana + write, 0, zero);
}




int NanaCompare::GetDistance(const byte* nana0, int nana0_len, const byte* nana1, int nana1_len) {
	if (!nana0 || nana0_len < 0 ||
		!nana1 || nana1_len < 0)
		return INT_MAX;
	
	int len = min(nana0_len, nana1_len);
	const byte* end0 = nana0 + len;
	const byte* end1 = nana1 + len;
	
	seen_v0.Clear();
	seen_c0.Clear();
	seen_v1.Clear();
	seen_c1.Clear();
	
	for (const byte *it0 = nana0, *it1 = nana1; it0 != end0; it0++, it1++) {
		int v0, v1, c0, c1;
		{
			int p0 = *it0 & NANA_CLUSTER_MASK;
			int p1 = *it1 & NANA_CLUSTER_MASK;
			
			v0 = p0 & 0x7;
			v1 = p1 & 0x7;
			c0 = (p0 >> 3) & 0x7;
			c1 = (p1 >> 3) & 0x7;
			
			// Skip non-meaningful items
			if (v0 == NANA_MEANINGLESS || c0 == NANA_MEANINGLESS ||
				v1 == NANA_MEANINGLESS || c1 == NANA_MEANINGLESS)
				continue;
		}
		
		// Keep track of seen clusters (where both are non-meaningless)
		
		if (v0 < NANA_MEANINGLESS) {
			seen_v0.FindAdd(v0);
		}
		else if (c0 < NANA_MEANINGLESS) {
			seen_c0.FindAdd(c0);
		}
		
		if (v1 < NANA_MEANINGLESS) {
			seen_v1.FindAdd(v1);
		}
		else if (c1 < NANA_MEANINGLESS) {
			seen_c1.FindAdd(c1);
		}
	}
	// If lowest seen cluster is not #0, then use lowest seen cluster as #0
	// -> re-map seen clusters, and remove holes in the cluster-id-vector
	SortIndex(seen_v0, StdLess<int>());
	SortIndex(seen_c0, StdLess<int>());
	SortIndex(seen_v1, StdLess<int>());
	SortIndex(seen_c1, StdLess<int>());
	
	int distance_sum = 0;
	for (const byte *it0 = nana0, *it1 = nana1; it0 != end0; it0++, it1++) {
		int v0, v1, c0, c1;
		{
			int p0 = *it0 & NANA_CLUSTER_MASK;
			int p1 = *it1 & NANA_CLUSTER_MASK;
			
			v0 = p0 & 0x7;
			v1 = p1 & 0x7;
			c0 = (p0 >> 3) & 0x7;
			c1 = (p1 >> 3) & 0x7;
			
			// Skip non-meaningful items
			if (v0 == NANA_MEANINGLESS || c0 == NANA_MEANINGLESS ||
				v1 == NANA_MEANINGLESS || c1 == NANA_MEANINGLESS)
				continue;
		}
		if (v0 < NANA_MEANINGLESS) {
			v0 = seen_v0.Find(v0);
		}
		else if (c0 < NANA_MEANINGLESS) {
			c0 = seen_v0.Find(c0);
		}
		
		if (v1 < NANA_MEANINGLESS) {
			v1 = seen_v0.Find(v1);
		}
		else if (c1 < NANA_MEANINGLESS) {
			c1 = seen_v0.Find(c1);
		}
		
		
		// Get nana-distance
		// TODO NANA_SPECIFIC is not considered here, even though it should be
		
		int vowel_dist = 0;
		if (v0 < NANA_MEANINGLESS && v1 < NANA_MEANINGLESS) {
			vowel_dist = abs(v0 - v1);
		}
		int const_dist = 0;
		if (c0 < NANA_MEANINGLESS && c1 < NANA_MEANINGLESS) {
			const_dist = abs(c1 - c1);
		}
		
		int distance = vowel_dist + const_dist;
		distance_sum += distance;
	}
	
	return distance_sum;
}

