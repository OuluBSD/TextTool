#ifndef _SongTool_Common_h_
#define _SongTool_Common_h_


struct SnapAttr : Moveable<SnapAttr> {
	int group = 0, item = 0;
	
	bool operator==(const SnapAttr& a) const {return group == a.group && item == a.item;}
	void Clear() {
		group = 0;
		item = 0;
	}
	void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}
	void Serialize(Stream& s) {s % group % item;}
	String ToString() const {return IntStr(group) + ":" + IntStr(item);}
	hash_t GetHashValue() const {CombineHash c; c.Put(group); c.Put(item); return c;}
};

struct SnapAttrStr : Moveable<SnapAttrStr> {
	String group, item;
	int group_i = -1, item_i = -1;
	bool has_id = false;
	
	bool operator==(const SnapAttrStr& a) const {return group == a.group && item == a.item;}
	bool operator==(const SnapAttr& a) const {RealizeId(); return group_i == a.group && item_i == a.item;}
	void Clear() {
		group = "";
		item = "";
	}
	void Jsonize(JsonIO& json) {
		json
			("g", group)
			("i", item)
			;
	}
	void RealizeId() const;
	void Load(const SnapAttr& sa);
	void Serialize(Stream& s) {s % group % item % group_i % item_i % has_id;}
	String ToString() const {return group + ":" + item;}
	hash_t GetHashValue() const {CombineHash c; c << group << item; return c;}
};


struct DataFile {
	String file_title;
	
};

struct Artist;
struct Release;
struct Song;
struct Part;
struct Line;
struct Break;
struct PatternSnap;

struct Ptrs {
	Artist* artist = 0;
	Release* release = 0;
	Song* song = 0;
	Part* part = 0;
	Line* line = 0;
	Break* brk = 0;
	PatternSnap* snap = 0;
	
	void Clear() {memset(this, 0, sizeof(Ptrs));}
	
};

#endif
