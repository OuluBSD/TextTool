#include "SongTool.h"

void ReverseTask::Store(bool force) {
	hash_t h = GetHashValue();
	String file_name = HexString((void*)&h, sizeof(h)) + ".bin";
	String dir = ConfigFile("local");
	RealizeDirectory(dir);
	String file = AppendFileName(dir, file_name);
	//ASSERT(force || !FileExists(file)); // checking hashes
	StoreToFile(*this, file);
}

void ReverseTask::LoadHash(hash_t h) {
	String file_name = HexString((void*)&h, sizeof(h)) + ".bin";
	String dir = ConfigFile("local");
	String file = AppendFileName(dir, file_name);
	if (FileExists(file))
		LoadFromFile(*this, file);
}
