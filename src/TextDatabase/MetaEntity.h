#ifndef _TextDatabase_MetaEntity_h_
#define _TextDatabase_MetaEntity_h_


BEGIN_TEXTLIB_NAMESPACE


struct MetaEntity :
	DataFile
{
	
	
	
	void Serialize(Stream& s) {
		
	}
	
	void Jsonize(JsonIO& json) {
		
	}
	
	static MetaEntity& DatabaseUpdate();
};




END_TEXTLIB_NAMESPACE


#endif
