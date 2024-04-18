#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE




MetaEntity& MetaEntity::DatabaseUpdate() {
	static MetaEntity e;
	if (e.file_title.IsEmpty()) {
		e.file_title = "__generic_db_updater__";
	}
	return e;
}

END_TEXTLIB_NAMESPACE

