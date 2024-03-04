#include "SocialDatabase.h"


void Story::Store(Company& a) {
	String dir = a.GetStoryDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Story::LoadTitle(Company& a, String title) {
	String dir = a.GetStoryDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

String Story::GetAnyTitle() const {
	if (native_title.GetCount())
		return native_title;
	
	if (english_title.GetCount())
		return english_title;
	
	return file_title;
}

int Story::GetFirstPartPosition() const {
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(int j = 0; j < parts.GetCount(); j++) {
			if (parts[j].part_type != StoryPart::SKIP &&
				parts[j].type == type)
				return j;
		}
	}
	return -1;
}

Vector<int> Story::GetPartPositions(const StoryPart& part) const {
	Vector<int> v;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		if (active_struct.parts[i] == part.type)
			v << i;
	}
	return v;
}

Vector<int> Story::GetPreviousParts(const StoryPart& part) const {
	Vector<int> pos = GetPartPositions(part);
	for(int i = 0; i < pos.GetCount(); i++) {
		int& p = pos[i];
		if (!p) pos.Remove(i--);
		p--;
	}
	return pos;
}

Vector<StoryPart*> Story::GetNonSkippedStructureParts() {
	Vector<StoryPart*> parts;
	for(int i = 0; i < active_struct.parts.GetCount(); i++) {
		String type = active_struct.parts[i];
		for(StoryPart& sp : this->parts) {
			if (sp.type == type) {
				parts << &sp;
				break;
			}
		}
	}
	return parts;
}

int Story::FindPart(const StoryPart& part) const {
	for(int i = 0; i < parts.GetCount(); i++) {
		if (&parts[i] == &part)
			return i;
	}
	return -1;
}

Vector<int> Story::GetPreviousPartsNonSkipped(const StoryPart& part) const {
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
			const StoryPart* sp = 0;
			for(int j = 0; j < parts.GetCount(); j++) {
				const StoryPart& part = parts[p];
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
			if (sp->part_type != StoryPart::SKIP)
				break;
		}
	}
	return pos;
}

StoryPart* Story::FindPartByType(const String& type) {
	for (StoryPart& sp : parts)
		if (sp.type == type)
			return &sp;
	return 0;
}

StoryPart* Story::FindPartByName(const String& name) {
	String lname = ToLower(name);
	for (StoryPart& sp : parts)
		if (ToLower(sp.name) == lname)
			return &sp;
	return 0;
}

double StoryStructSuggestion::GetEstimatedDuration(int bpm) const {
	double bars_per_min = (double)bpm / 4.0;
	double bars_per_sec = bars_per_min / 60.0;
	int bars = 8 * parts.GetCount();
	double sec = bars / bars_per_sec;
	return sec;
}
