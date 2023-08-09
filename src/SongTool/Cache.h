#ifndef _SongTool_Cache_h_
#define _SongTool_Cache_h_


typedef enum : int {
	CA_LYRICS,
	CA_ATTRIBUTE_LIBRARY,
	CA_MASK,
	CA_SNAP,
	
} CacheAnchorType;


struct CacheItem : Moveable<CacheItem> {
	TaskType task;
	
	
};


struct CacheManager {
	Vector<CacheItem> items;
	
	
	
	
	CacheManager& Single() {static CacheManager mgr; return mgr;}
	
};

#endif
