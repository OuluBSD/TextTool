#include "SongTool.h"

AttrScore::AttrScore() {
	file_title = "attrscore";
}

void AttrScore::Store() {
	String dir = Database::Single().dir;
	RealizeDirectory(dir);
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	StoreAsJsonFile(*this, json_path, true);
}

void AttrScore::Load() {
	String dir = Database::Single().dir;
	String json_path = dir + DIR_SEPS "share" DIR_SEPS + file_title + ".json";
	//DUMP(json_path);
	LoadFromJsonFile(*this, json_path);
}

bool AttrScore::RealizeTemp() {
	// Connect attribute group&items to score groups
	// These classes are kept separate, because they center around different concepts.
	
	Attributes& g = Database::Single().attrs;
	
	// Realize conversion vector
	int gc = g.groups.GetCount();
	this->attr_to_score.SetCount(gc);
	
	// Loop Attributes groups and entries
	int total = 0, not_found = 0;
	for(int i = 0; i < gc; i++) {
		Attributes::Group& gg = g.groups[i];
		int vc = gg.values.GetCount();
		
		// Realize conversion vector
		Vector<int>& vv = attr_to_score[i];
		vv.SetCount(vc, -1);
		
		// Loop values
		for(int j = 0; j < vc; j++) {
			int& v = vv[j];
			total++;
			
			// Don't update resolved values for performance
			if (v >= 0)
				continue;
			
			// Here you have group and entry without connection
			
			// Loop groups of AttrScoreGroup, and match group&entry (==SnapAttr) value
			bool found = false;
			int asg_i = 0;
			for (const AttrScoreGroup& asg : groups) {
				for (const SnapAttr& sa : asg.attrs) {
					if (sa.group == i && sa.item == j) {
						// Match found
						v = asg_i;
						found = true;
						break;
					}
				}
				if (found) break;
				asg_i++;
			}
			
			if (!found) {
				//LOG("AttrScore::RealizeTemp: not found: " + gg.description + ", " + gg.values[j] + " (" << IntStr(i) + ":" + IntStr(j) + ")");
				not_found++;
				//return false;
			}
		}
	}
	
	//int ready = 100 * not_found / total;
	//LOG("AttrScore::RealizeTemp: total=" << total << ", not_found=" << not_found << " (" << ready << "\%)");
	return true;
}



void PartScore::Realize() {
	Attributes& g = Database::Single().attrs;
	values.SetCount(g.scorings.GetCount());
	for (auto& v : values)
		v.SetCount(len, 0);
}


String AttrScoreGroup::ToString() const {
	Attributes& g = Database::Single().attrs;
	
	String s;
	if (name.GetCount())
		s << t_("Name") << ": " << name << "\n";
	
	for(int i = 0; i < scores.GetCount(); i++) {
		int sc = scores[i];
		if (!sc) continue;
		
		if (i >= g.scorings.GetCount()) {
			s << "error\n";
			continue;
		}
		
		const Attributes::ScoringType& t = g.scorings[i];
		String name =
			Capitalize(g.Translate(t.klass)) + ": " + (sc > 0 ? "+" : "-") + " (" +
			Capitalize(g.Translate(sc > 0 ? t.axes0 : t.axes1)) + ")";
		s << name << "\n";
	}
	
	return s;
}
