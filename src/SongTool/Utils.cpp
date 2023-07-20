#include "SongTool.h"


String Capitalize(String s) {
	return ToUpper(s.Left(1)) + s.Mid(1);
}

String GetSnapGroupString(PatternSnap& snap, int group_i, Index<String>& skip_list) {
	Attributes& g = Database::Single().attrs;
	Attributes::Group& gg = g.groups[group_i];
	
	String s;
	for(const SnapAttrStr& a : snap.attributes.GetKeys()) {
		a.RealizeId();
		if (a.group_i != group_i)
			continue;
		
		String item = g.Translate(gg.values[a.item_i]);
		
		if (skip_list.Find(item) < 0) {
			if (!s.IsEmpty())
				s << ", ";
			skip_list.Add(item);
			s << item;
		}
	}
	return s;
}

bool HasSnapGroupString(PatternSnap& snap, int group_i) {
	Attributes& g = Database::Single().attrs;
	Attributes::Group& gg = g.groups[group_i];
	
	for(const SnapAttrStr& a : snap.attributes.GetKeys()) {
		a.RealizeId();
		if (a.group_i != group_i)
			continue;
		
		return true;
	}
	return false;
}
