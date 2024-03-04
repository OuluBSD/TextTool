#include "SocialDatabase.h"


int Role::GetStoryCount() const {
	int total = 0;
	for (const auto& a : generics)
		total += a.stories.GetCount();
	return total;
}
