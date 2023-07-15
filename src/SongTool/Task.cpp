#include "SongTool.h"

void ReverseTask::Store() {
	hash_t h = GetHashValue();
	String file_name = HexString((void*)&h, sizeof(h)) + ".bin";
	String dir = ConfigFile("local");
	RealizeDirectory(dir);
	StoreToFile(*this, AppendFileName(dir, file_name));
}

void ReverseTask::LoadHash(hash_t h) {
	String file_name = HexString((void*)&h, sizeof(h)) + ".bin";
	String dir = ConfigFile("local");
	LoadFromFile(*this, AppendFileName(dir, file_name));
}

	