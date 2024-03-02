#include "Database.h"


void Lyrics::Store(Artist& a) {
	String dir = a.GetLyricsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Lyrics::LoadTitle(Artist& a, String title) {
	String dir = a.GetLyricsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Lyrics::GetAnyTitle() const {
	if (native_title.GetCount())
		return native_title;
	
	if (english_title.GetCount())
		return english_title;
	
	return file_title;
}

int Lyrics::GetFirstPartPosition() const {
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(int j = 0; j < parts.GetCount(); j++) {
			if (parts[j].part_type != StaticPart::SKIP &&
				parts[j].type == type)
				return j;
		}
	}
	return -1;
}

Vector<int> Lyrics::GetPartPositions(const StaticPart& part) const {
	Vector<int> v;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		if (active_struct.parts[i] == part.type)
			v << i;
	}
	return v;
}

Vector<int> Lyrics::GetPreviousParts(const StaticPart& part) const {
	Vector<int> pos = GetPartPositions(part);
	for(int i = 0; i < pos.GetCount(); i++) {
		int& p = pos[i];
		if (!p) pos.Remove(i--);
		p--;
	}
	return pos;
}

Vector<StaticPart*> Lyrics::GetNonSkippedStructureParts() {
	Vector<StaticPart*> parts;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(StaticPart& sp : this->parts) {
			if (sp.type == type) {
				parts << &sp;
				break;
			}
		}
	}
	return parts;
}

int Lyrics::FindPart(const StaticPart& part) const {
	for(int i = 0; i < parts.GetCount(); i++) {
		if (&parts[i] == &part)
			return i;
	}
	return -1;
}

Vector<int> Lyrics::GetPreviousPartsNonSkipped(const StaticPart& part) const {
	Vector<int> pos = GetPartPositions(part);
	for(int i = 0; i < pos.GetCount(); i++) {
		int& p = pos[i];
		while (true) {
			if (!p) {
				pos.Remove(i--);
				break;
			}
			p--;
			const String& type = active_struct.parts[p];
			const StaticPart* sp = 0;
			for(int j = 0; j < parts.GetCount(); j++) {
				const StaticPart& part = parts[p];
				if (part.type != type) continue;
				sp = &part;
				break;
			}
			if (!sp)
				continue;
			if (sp == &part) {
				pos.Remove(i--);
				break;
			}
			if (sp->part_type != StaticPart::SKIP)
				break;
		}
	}
	return pos;
}

StaticPart* Lyrics::FindPartByType(const String& type) {
	for (StaticPart& sp : parts)
		if (sp.type == type)
			return &sp;
	return 0;
}

StaticPart* Lyrics::FindPartByName(const String& name) {
	String lname = ToLower(name);
	for (StaticPart& sp : parts)
		if (ToLower(sp.name) == lname)
			return &sp;
	return 0;
}

double StructSuggestion::GetEstimatedDuration(int bpm) const {
	double bars_per_min = (double)bpm / 4.0;
	double bars_per_sec = bars_per_min / 60.0;
	int bars = 8 * parts.GetCount();
	double sec = bars / bars_per_sec;
	return sec;
}
