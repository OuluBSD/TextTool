#include "SimplifiedCtrl.h"


TxtStructTranslate::TxtStructTranslate() {
	key = "REFERENCE_SONG__NATIVE_TEXT";
	trans_key = "REFERENCE_SONG__ENGLISH_TEXT";
	song = true;
}


TxtStructFromEnglish::TxtStructFromEnglish() {
	key = "REFERENCE_SONG__ENGLISH_TEXT";
	trans_key = "REFERENCE_SONG__ENGLISH_UNPACKED_STRUCTURE";
	song = true;
	fn = FN_UNPACK_STRUCTURE;
}
