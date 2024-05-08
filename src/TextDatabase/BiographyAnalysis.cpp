#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String GetSocietyRoleEnum(int i) {
	switch (i) {
		#define SOCIETYROLE(x) case SOCIETYROLE_##x: return #x;
		SOCIETYROLE_LIST
		#undef SOCIETYROLE
		default: return String();
	}
}

String GetSocietyRoleKey(int i) {return KeyToName(GetSocietyRoleEnum(i));}


const Array<RoleProfile>& GetRoleProfile(int role_i) {
	static Array<RoleProfile> v[SOCIETYROLE_COUNT];
	ASSERT(role_i >= 0 && role_i < SOCIETYROLE_COUNT);
	Array<RoleProfile>& vv = v[role_i];
	if (vv.IsEmpty()) {
		switch (role_i) {
			
			default: TODO
		}
	}
	return vv;
}


END_TEXTLIB_NAMESPACE

