#include "SongTool.h"


void SnapAttrStr::RealizeId() const {
	if (has_id)
		return;
	SnapAttrStr& sa = const_cast<SnapAttrStr&>(*this);
	Database& db = Database::Single();
	sa.group_i = -1;
	sa.item_i = -1;
	for(int i = 0; i < db.attrs.groups.GetCount(); i++) {
		const Attributes::Group& gg = db.attrs.groups[i];
		if (gg.description == group) {
			sa.group_i = i;
			for(int j = 0; j < gg.values.GetCount(); j++) {
				if (gg.values[j] == item) {
					sa.item_i = j;
					return;
				}
			}
			DUMPC(gg.values);
			DUMP(sa.group);
			DUMP(sa.item);
			ASSERT_(0, "item not found");
		}
	}
	DUMP(sa.group);
	DUMP(sa.item);
	ASSERT_(0, "group and item not found");
}

void SnapAttrStr::Load(const SnapAttr& sa) {
	Panic("TODO");
}
