#include "SongTool.h"


int Attributes::trans_i = -1;

Attributes::Attributes() {
	
}

bool Attributes::FindAttr(String group, String item, SnapAttr& sa) const {
	group = ToLower(group);
	item = ToLower(item);
	int group_i = 0;
	for (const Group& gg : groups.GetValues()) {
		if (ToLower(gg.description) == group) {
			for(int i = 0; i < gg.values.GetCount(); i++) {
				if (ToLower(gg.values[i]) == item) {
					sa.group = group_i;
					sa.item = i;
					return true;
				}
			}
		}
		group_i++;
	}
	return false;
}

void Attributes::AddScoring(String s, Vector<Attributes::ScoringType>& scorings) {
	int a = s.Find(":");
	
	Attributes::ScoringType& t = scorings.Add();
	t.klass = s.Left(a);
	s =  s.Mid(a+2);
	a = s.Find("/");
	t.axes0 = s.Left(a);
	t.axes1 = s.Mid(a+1);
	
	Translate(t.klass);
	Translate(t.axes0);
	Translate(t.axes1);
}

String Attributes::Translate(const String& s) {
	if (trans_i < 0)
		return s;
	Translation& t = this->translation[trans_i];
	int i = t.data.Find(s);
	String o;
	if (i >= 0)
		o = t.data[i];
	if (i < 0 || o.IsEmpty()) {
		i = t.data.Find(ToLower(s));
		if (i >= 0)
			o = t.data[i];
		else
			t.data.Add(ToLower(s));
	}
	return o.IsEmpty() ? s : o;
}

