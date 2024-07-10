#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TryNo3tStructureSolver::MakeSections() {
	VectorMap<hash_t,int> hash_branches;
	Vector<int> rm_list;
	
	v.Clear();
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		UniqueLine& u = uniq_lines[i];
		
		if (u.lines.GetCount() < 2)
			continue;
		
		
		Vector<int> proc_v;
		{
			proc_v << v.GetCount();
			TempSection& t = v.Add();
			t.lines <<= u.lines;
			t.length = 1;
			t.Chk();
		}
		
		while (proc_v.GetCount()) {
			for(int pv_i = 0; pv_i < proc_v.GetCount(); pv_i++) {
				TempSection& t = v[proc_v[pv_i]];
				
				hash_branches.Clear();
				rm_list.SetCount(0);
				for(int j = 0; j < t.lines.GetCount(); j++) {
					int line_i = t.lines[j];
					int pos = line_i + t.length;
					if (pos >= lines.GetCount()) {
						rm_list << j;
					}
					else {
						Line& line = lines[pos];
						hash_branches.GetAdd(line.hash,0)++; // count next hashes
					}
				}
				SortByValue(hash_branches, StdGreater<int>());
				
				// Loop all branches
				for(int j = 1; j < hash_branches.GetCount(); j++) {
					// With 2 or more lines, a new section can be made
					if (hash_branches[j] >= 2) {
						hash_t hash = hash_branches.GetKey(j);
						proc_v << v.GetCount(); // add new section to processing queue
						TempSection& t0 = v.Add();
						for(int k = 0; k < t.lines.GetCount(); k++) {
							int line_i = t.lines[k];
							int pos = line_i + t.length;
							if (pos < lines.GetCount() && lines[pos].hash == hash) {
								t0.lines << line_i;
								t0.Chk();
								rm_list << k;
							}
						}
						t0.Chk();
					}
					// With 1 line, don't start new branch and remove it from the current,
					// which triggers the forking of current branch too.
					else {
						ASSERT(hash_branches[j] == 1);
						hash_t hash = hash_branches.GetKey(j);
						for(int k = 0; k < t.lines.GetCount(); k++) {
							int line_i = t.lines[k];
							int pos = line_i + t.length;
							if (pos < lines.GetCount() && lines[pos].hash == hash) {
								rm_list << k;
							}
						}
					}
				}
				
				// Stop, if everything would be removed
				if (rm_list.GetCount() == t.lines.GetCount()) {
					proc_v.Remove(pv_i--);
				}
				// Fork and modify the current branch, if some line-range has ended
				else if (rm_list.GetCount()) {
					int remaining = t.lines.GetCount() - rm_list.GetCount();
					
					// Don't start new branch, if only one section remains
					if (remaining == 1) {
						// Just discard this section from processing queue
						proc_v.Remove(pv_i);
					}
					else {
						// discard this section in processing queue (and replace with the new one)
						// IMPORTANT: this is processed only in the next 'while' iteration,
						//            because the next will be pv_i+1
						proc_v[pv_i] = v.GetCount();
						
						TempSection& t0 = v.Add();
						t0.lines <<= t.lines;
						t0.length = t.length + 1;
						
						Sort(rm_list, StdLess<int>());
						t0.lines.Remove(rm_list);
						
						t0.Chk();
					}
				}
				// Otherwise, this branch was extended without modification
				else {
					t.length++;
				}
			}
		}
	}
	
	Sort(v, TempSection());
	DUMPC(v);
	LOG("");
}

void TryNo3tStructureSolver::MakeRepeatingSectionLines() {
	// Try to add actual sections
	sections.Clear();
	for (TempSection& s : v) {
		
		// Check if lines are still assignable
		bool all_ok = true;
		for(int line_i : s.lines) {
			for(int j = 0; j < s.length; j++) {
				int pos = line_i+j;
				if (lines[pos].section >= 0) {
					all_ok = false;
					break;
				}
			}
			if (!all_ok) break;
		}
		if (!all_ok) continue;
		
		// Add section
		int sect_i = sections.GetCount();
		Section& sect = sections.Add();
		sect.count = s.lines.GetCount();
		sect.orig_count = sect.count;
		sect.orig_weight = s.length * s.lines.GetCount();
		
		// Write section
		for(int line_i : s.lines) {
			for(int j = 0; j < s.length; j++) {
				int pos = line_i+j;
				lines[pos].section = sect_i;
			}
		}
	}
}


END_TEXTLIB_NAMESPACE

