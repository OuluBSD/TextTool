#include "SocialDatabase.h"


BEGIN_SOCIALLIB_NAMESPACE


int Role::GetStoryCount() const {
	int total = 0;
	for (const auto& a : generics)
		total += a.stories.GetCount();
	return total;
}


END_SOCIALLIB_NAMESPACE
