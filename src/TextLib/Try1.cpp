#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TryNo1tStructureSolver::MakeSections() {
	transfers.Clear();
	
	Index<int> visited;
	for(int i = 0; i < transfers.GetCount(); i++) {
		Transfer& t = transfers[i];
		hash_t from = t.to;
		visited.Clear();
		visited.Add(t.from);
		visited.Add(t.to);
		Section* s = 0;
		while (1) {
			Transfer* t0 = FindTransfer(from);
			if (!t0 || t0->count <= 1)
				break;
			// The next line is accepted
			// At this point, the section has at least 2 lines, so we can actually add it
			if (!s) {
				s = &sections.Add();
				s->orig_count = t.count;
				s->hashes.Add(t.from);
				s->hashes.Add(t.to);
			}

			// Add line to section
			s->hashes.Add(t0->to);
			from = t0->to;

			// Prevent forever loops by checking visited lines
			int i = visited.Find(from);
			if (i >= 0)
				break;
			visited.Add(from);
		}
	}
}

void TryNo1tStructureSolver::MakeRepeatingSectionLines() {
	// At this point, all sections have at least 2 lines and they occur at least 2 times
	// If at least 2 consecutive lines belongs to any section, they can be attached to that section.
	// Lines, which haven't been attached can be joined to once occuring sections.
	Vector<int> matching_sections;
	Vector<int> rm_list;
	int dbg_assigned_section_count = 0;
	int prev_sect = -1;
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];

		// Get sections, which has matching first hash
		matching_sections.SetCount(0);
		hash_t h0 = line.descriptor.GetHash(section_cmp_header_len);
		for(int j = 0; j < sections.GetCount(); j++) {
			auto& sect = sections[j];
			ASSERT(!sect.hashes.IsEmpty());
			if (sect.hashes[0] == h0)
				matching_sections.Add(j);
		}

		if (matching_sections.GetCount() >= 2) {
			// LOG("A"); DUMPC(matching_sections);

			for(int j = i+1, pos = 1; j < lines.GetCount(); j++, pos++) {
				hash_t h = lines[j].descriptor.GetHash(section_cmp_header_len);

				// Check if section is too short
				rm_list.SetCount(0);
				for(int k = 0; k < matching_sections.GetCount(); k++) {
					Section& sect = sections[matching_sections[k]];
					if (pos >= sect.hashes.GetCount())
						rm_list.Add(k);
				}

				// Save first section for default selection in case of all are removed
				int def_selection = matching_sections[0];

				// If some sections remains, remove too short sections
				if (rm_list.GetCount() > 0) {
					matching_sections.Remove(rm_list);
					// LOG("B"); DUMPC(matching_sections);
					rm_list.SetCount(0);
				}

				// Remove sections without matching hash
				for(int k = 0; k < matching_sections.GetCount(); k++) {
					Section& sect = sections[matching_sections[k]];
					if (sect.hashes[pos] != h)
						rm_list.Add(k);
				}
				if (rm_list.GetCount() > 0) {
					matching_sections.Remove(rm_list);
					// LOG("C"); DUMPC(matching_sections);
				}
				if (matching_sections.IsEmpty())
					matching_sections << def_selection;
				if (matching_sections.GetCount() == 1)
					break;
			}
			ASSERT(matching_sections.GetCount() >= 1);
			line.section = matching_sections[0];
			dbg_assigned_section_count++;

			// Count section occurances
			if (prev_sect != line.section)
				sections[line.section].count++;
		}
		prev_sect = line.section;
	}

	double assigned_perc = (double)dbg_assigned_section_count / (double)lines.GetCount();

	
}









TryWithTransferStructureSolver::Transfer* TryWithTransferStructureSolver::FindTransfer(hash_t h) {
	int max_count = 0, pos = -1;
	for(int i = 0; i < transfers.GetCount(); i++) {
		Transfer& t = transfers[i];
		if (t.from == h && t.count > max_count) {
			pos = i;
			max_count = t.count;
		}
	}
	if (pos >= 0)
		return &transfers[pos];
	return 0;
}

TryWithTransferStructureSolver::Transfer* TryWithTransferStructureSolver::FindTransfer(hash_t from, hash_t to) {
	for(Transfer& t : transfers)
		if (t.from == from && t.to == to)
			return &t;
	return 0;
}

TryWithTransferStructureSolver::Transfer& TryWithTransferStructureSolver::GetAddTransfer(hash_t h0, hash_t h1) {
	for (Transfer& t : transfers) {
		if (t.from == h0 && t.to == h1)
			return t;
	}
	Transfer& t = transfers.Add();
	t.from = h0;
	t.to = h1;
	t.count = 0;
	return t;
}


END_TEXTLIB_NAMESPACE

