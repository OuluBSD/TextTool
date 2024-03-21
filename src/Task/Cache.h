#ifndef _Task_Cache_h_
#define _Task_Cache_h_


BEGIN_TEXTLIB_NAMESPACE



typedef enum : int {
	CA_LYRICS,
	CA_ATTRIBUTE_LIBRARY,
	CA_MASK,
	CA_SNAP,
	
} CacheAnchorType;


struct CacheItem : Moveable<CacheItem> {
	AiTaskType task;
	
	
};


struct CacheManager {
	Vector<CacheItem> items;
	
	
	
	
	CacheManager& Single() {static CacheManager mgr; return mgr;}
	
};



END_TEXTLIB_NAMESPACE


#endif
