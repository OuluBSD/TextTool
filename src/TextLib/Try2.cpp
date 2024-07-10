#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TryNo2tStructureSolver::MakeSections() {
	// Count transforms from unique line to other unique line
	transfers.Clear();
	for(int i = 0; i < lines.GetCount()-1; i++) {
		TextDescriptor& td0 = lines[i].descriptor;
		TextDescriptor& td1 = lines[i+1].descriptor;
		hash_t h0 = td0.GetHash(section_cmp_header_len);
		hash_t h1 = td1.GetHash(section_cmp_header_len);
		auto& t = GetAddTransfer(h0,h1);
		t.count++;
		auto& vec = t.vectors.Add();
		vec.line = i;
	}
	Sort(transfers, Transfer());
	
	// Calculate lengths of continuous transform vectors
	// Having only 1 instance of the transform will cut the Transfer::Vec length
	// But you are not searching for the longest Transfer::Vec lenght, but also considering 'weight'
	// The weight considers both length and how "popular" that transfer is.
	for (Transfer& t : transfers) {
		for(auto& vec : t.vectors) {
			ASSERT(t.from == lines[vec.line].descriptor.GetHash(section_cmp_header_len));
			vec.weight = t.count;
			vec.len = 2;
			hash_t from = t.to;
			for(int i = vec.line+1; i < lines.GetCount(); i++) {
				Line& line = lines[i];
				hash_t to = line.descriptor.GetHash(section_cmp_header_len);
				Transfer* t1 = FindTransfer(from, to);
				if (!t1 || t1->count <= 1) break;
				vec.weight += t1->count;
				vec.len++;
				from = to;
			}
		}
		Sort(t.vectors, Transfer::Vec());
	}
}

void TryNo2tStructureSolver::MakeRepeatingSectionLines() {
	// Make repeating sections
	sections.Clear();
	
	for (Transfer& t : transfers) {
		for(auto& vec : t.vectors) {
			if (vec.len < 3)
				continue;
			Section& sect = sections.Add();
			sect.orig_weight = vec.weight;
			sect.orig_count = t.count;
			for(int i = 0; i < vec.len-1; i++) {
				int pos = vec.line + i;
				Line& line = lines[pos];
				hash_t h = line.descriptor.GetHash(section_cmp_header_len);
				sect.hashes << h;
			}
		}
	}
	
	//#error TODO Fill all possible sections and pick best
	
	Sort(sections, Section());
	
	for (int section_i = 0; section_i < sections.GetCount(); section_i++) {
		Section& sect = sections[section_i];
		
		for(int i = 0; i < lines.GetCount(); i++) {
			Line& line = lines[i];
			if (line.section >= 0)
				continue;
			bool matches = sect.hashes.GetCount() > 0;
			for(int j = 0; j < sect.hashes.GetCount(); j++) {
				int pos = i+j;
				if (pos >= lines.GetCount()) {
					matches = false;
					break;
				}
				Line& l0 = lines[pos];
				if (l0.section >= 0 ||
					l0.descriptor.GetHash(section_cmp_header_len) != sect.hashes[j]) {
					matches = false;
					break;
				}
			}
			if (matches) {
				sect.count++;
				for(int j = 0; j < sect.hashes.GetCount(); j++)
					lines[i+j].section = section_i;
			}
		}
	}
}



END_TEXTLIB_NAMESPACE

