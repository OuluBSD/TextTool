#include "SongLib.h"

namespace SongLib {

void TaskManager::OnTokenData(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. suppote: verb | noun
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	int offset = 3+1;
	
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		/*int line_i = ScanInt(line);
		line_i -= offset;
		if (line_i < 0 || line_i >= args.words.GetCount())
			continue;
		
		const String& orig_word = args.words[line_i];*/
		
		int a = line.Find(".");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		
		a = line.Find(":");
		if (a == 0) {
			// Rare case of ":" being asked
			line = ":" + line;
			a = 1;
		}
		else if (a < 0)
			continue;
		
		//int orig_word_i = ;
		
		String result_word = TrimBoth(line.Left(a));
		/*ExportWord& wrd =
			orig_word_i >= 0 ?
				da.words[orig_word_i] :
				da.words.GetAdd(result_word, orig_word_i);*/
		int orig_word_i = -1;
		ExportWord& wrd = da.words.GetAdd(result_word, orig_word_i);
		
		//TODO // token to word
		
		line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, "|");
		for (String& p : parts) {
			p = TrimBoth(p);
			int wc_i = da.word_classes.FindAdd(p);
			if (wrd.class_count < wrd.MAX_CLASS_COUNT)
				FixedIndexFindAdd(wrd.classes, wrd.MAX_CLASS_COUNT, wrd.class_count, wc_i);
		}
		
	}
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnAmbiguousWordPairs(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 9. is something : verb, noun
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	
	int offset = 1+1;
	
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		
		a = line.ReverseFind(":");
		if (a < 0)
			continue;
		
		Vector<String> result_words = Split(TrimBoth(line.Left(a)), " ");
		if (result_words.GetCount() != 2)
			continue;
		
		int w_i0 = da.words.Find(result_words[0]);
		int w_i1 = da.words.Find(result_words[1]);
		CombineHash ch;
		ch.Do(w_i0).Put(1).Do(w_i1);
		hash_t h = ch;
		
		//ExportWord& wrd0 = da.words[w_i0];
		//ExportWord& wrd1 = da.words[w_i1];
		WordPairType& wp = da.ambiguous_word_pairs.GetAdd(h);
		
		line = TrimBoth(line.Mid(a+1));
		
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() != 2)
			continue;
		int wc_i_list[2];
		for(int i = 0; i < parts.GetCount(); i++) {
			String& p = parts[i];
			p = TrimBoth(p);
			wc_i_list[i] = da.word_classes.FindAdd(p);
		}
		
		wp.from_type = wc_i_list[0];
		wp.to_type = wc_i_list[1];
	}
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhrases(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	Vector<int> word_classes;
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		line = TrimBoth(line.Mid(a+1));
		
		a = line.ReverseFind(":");
		if (a < 0)
			continue;
		
		Vector<String> classes = Split(TrimBoth(line.Left(a)), ",");
		word_classes.SetCount(0);
		bool fail = false;
		CombineHash ch;
		for (String& c : classes) {
			c = TrimBoth(c);
			int wc_i = da.word_classes.FindAdd(c);
			if (wc_i < 0) {
				fail = true;
				break;
			}
			word_classes << wc_i;
			ch.Do(wc_i).Put(1);
		}
		if (fail) continue;
		hash_t h = ch;
		
		int vpp_i = -1;
		VirtualPhrasePart& vpp = da.virtual_phrase_parts.GetAdd(h, vpp_i);
		if (vpp.word_classes.IsEmpty())
			vpp.word_classes <<= word_classes;
		
		line = TrimBoth(line.Mid(a+1));
		
		vpp.struct_part_type = da.struct_part_types.FindAdd(line);
	}
	
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnVirtualPhraseTypes(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 61. compound-complex sentence + complex sentence: compound-complex sentence
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int vps_i = t->tmp[line_i];
		VirtualPhraseStruct& vps = da.virtual_phrase_structs[vps_i];
		
		line = TrimBoth(line.Mid(a+1));
		
		a = line.ReverseFind(":");
		if (a < 0)
			continue;
		
		Vector<String> classes = Split(TrimBoth(line.Left(a)), "+", false);
		//sp_is.SetCount(0);
		bool fail = false;
		CombineHash ch;
		for (String& c : classes) {
			c = TrimBoth(c);
			if (c.IsEmpty()) {
				fail = true;
				break;
			}
			int sp_i = da.struct_part_types.Find(c);
			if (sp_i < 0) {
				fail = true;
				break;
			}
			//sp_is << sp_i;
			ch.Do(sp_i).Put(1);
		}
		if (fail)
			continue;
		hash_t h = ch;
		
		/*int vps_i = da.virtual_phrase_structs.Find(h);
		if (vps_i < 0)
			continue;
		VirtualPhraseStruct& vps = da.virtual_phrase_structs[vps_i];*/
		/*int vps_i = -1;
		VirtualPhraseStruct& vps = da.virtual_phrase_structs.GetAdd(h, vps_i);
		if (vps.parts.IsEmpty())
			vps.parts <<= sp_is;*/
		vps.count++;
		
		String struct_type = TrimBoth(line.Mid(a+1));
		
		vps.struct_type = da.struct_types.FindAdd(struct_type);
	}
	
	t->running = false;
	t->batch_i++;
}

void TaskManager::OnPhraseColors(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 12. RGB(255, 102, 0)
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		a = line.Find("RGB(", a+1);
		if (a < 0) continue;
		a += 4;
		int b = line.Find(")");
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = StrInt(TrimBoth(clr_parts[0]));
		int G = StrInt(TrimBoth(clr_parts[1]));
		int B = StrInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		pp.clr = clr;
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseAttrs(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 1. Belief communities: acceptance
	
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		line = TrimBoth(line.Mid(a+1));
		a = line.Find(":");
		if (a < 0) continue;
		
		AttrHeader ah;
		ah.group = ToLower(TrimBoth(line.Left(a)));
		ah.value = ToLower(TrimBoth(line.Mid(a+1)));
		da.attrs.GetAdd(ah, pp.attr);
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnPhraseActions(String result, Task* t) {
	TokenArgs& args = token_args;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// 5. tone(admiring) + msg(expressing attraction) + bias(physical appearance) + attention-attribute(referencing arms) + attention-physical_state(strength)
	
	Vector<int> actions;
	int offset = 3+1;
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		
		// Get line number
		if (line.IsEmpty() ||!IsDigit(line[0]))
			continue;
		int a = line.Find(".");
		if (a < 0) continue;
		
		int line_i = ScanInt(line.Left(a));
		line_i -= offset;
		if (line_i < 0 || line_i >= t->tmp.GetCount())
			continue;
		int pp_i = t->tmp[line_i];
		PhrasePart& pp = da.phrase_parts[pp_i];
		line = TrimBoth(line.Mid(a+1));
		
		// Split rest of the line at '+' character and parse single actions
		Vector<String> parts = Split(line, "+");
		CombineHash ch;
		actions.SetCount(0);
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			s = TrimBoth(s);
			int a = s.Find("(");
			int b = s.Find(")");
			if (a < 0 || b < 0 || a > b) {
				parts.Remove(i--);
				continue;
			}
			ActionHeader aa;
			aa.action = TrimBoth(s.Left(a));
			a++;
			aa.arg = TrimBoth(s.Mid(a,b-a));
			
			da.actions.GetAdd(aa, actions.Add());
		}
		Sort(actions, StdLess<int>());
		pp.actions <<= actions;
	}
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistColors(String result, Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		a = line.Find("RGB(", b);
		if (a < 0) continue;
		a += 4;
		b = line.Find(")", a);
		if (b < 0) continue;
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = ScanInt(TrimLeft(clr_parts[0]));
		int G = ScanInt(TrimLeft(clr_parts[1]));
		int B = ScanInt(TrimLeft(clr_parts[2]));
		Color clr(R,G,B);
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.clr = clr;
	}
	
	t->batch_i++;
	t->running = false;
	#endif
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	TODO
	#if 0
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-procedures(planning)" problem solving strategy / shortcut taking
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		
		a = b+1;
		b = line.Find("/", b);
		if (a < 0) continue;
		String attr_group = TrimBoth(line.Mid(a,b-a));
		a = b+1;
		String attr_value = TrimBoth(line.Mid(a));
		
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.group = attr_group;
		aa.value = attr_value;
	}
	
	t->batch_i++;
	t->running = false;
	#endif
}

}
