#ifndef _TextDatabase_Typeclass_h_
#define _TextDatabase_Typeclass_h_


BEGIN_TEXTLIB_NAMESPACE


struct Typeclass : DataFile {
	Array<Content>	contents;
	
	
	int GetScriptCount() const;
};


END_TEXTLIB_NAMESPACE


#endif
