#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TryNo4tStructureSolver::MakeSections() {
	
	int max_lines = uniq_lines[0].count;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		UniqueLine& ul = uniq_lines.Get(line.hash);
		line.repeatability = max_lines > 0 ? (double)(ul.count - 1) / (double)(max_lines - 1) : 0;
	}
	for(int i = 1; i < lines.GetCount()-1; i++) {
		Line& line0 = lines[i-1];
		Line& line1 = lines[i];
		Line& line2 = lines[i+1];
		if (line1.repeatability < cr_limit && line0.repeatability > iter_r_limit && line2.repeatability > iter_r_limit) {
			line1.circumstacial_repeatability = (line0.repeatability + line2.repeatability) * 0.5;
		}
	}
	
	sections.Clear();
	
	struct RangeHash : Moveable<RangeHash> {
		hash_t h = 0;
		int begin, end, len;
		Vector<int> sections;
		bool operator()(const RangeHash& a, const RangeHash& b) const {return (a.sections.GetCount()*a.len) > (b.sections.GetCount()*b.len);}
		String ToString() const {
			return Format("%d-%d (%d) %X: count %d", begin, end, len, (int)h, sections.GetCount());
		}
	};
	
	struct TmpSection : Moveable<TmpSection> {
		Vector<hash_t> hashes;
		int begin, end, len;
		double r = 0, cr = 0;
		Vector<int> ranges;
		String ToString() const {
			return Format("%d-%d (%d) r=%f,cr=%f: count %d", begin, end, len, r, cr, ranges.GetCount());
		}
	};
	Vector<TmpSection> tmp;
	
	TmpSection* ts = 0;
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		if (line.repeatability > iter_r_limit || line.circumstacial_repeatability > iter_r_limit) {
			if (!ts) {
				ts = &tmp.Add();
				ts->begin = i;
				ts->end = -1;
				ts->len = 1;
			}
			ts->r += line.repeatability;
			ts->cr += line.circumstacial_repeatability;
		}
		else if (ts) {
			ts->end = i;
			ts->len = ts->end - ts->begin;
			ts->r /= ts->len;
			ts->cr /= ts->len;
			ts = 0;
		}
	}
	if (ts) {
		ts->end = lines.GetCount();
		ts->len = ts->end - ts->begin;
		ts->r /= ts->len;
		ts->cr /= ts->len;
	}
	
	// Make potential ranges
	VectorMap<int,RangeHash> ranges;
	int ts_i = 0;
	for (TmpSection& ts : tmp) {
		for(int i = ts.begin; i < ts.end; i++) {
			CombineHash ch;
			for(int j = i+1; j < ts.end; j++) {
				Line& line = lines[j-1];
				ch.Do(line.hash);
				
				int key = i * 1000 + j;
				int range_i = ranges.Find(key);
				if (range_i < 0) {
					range_i = ranges.GetCount();
					RangeHash& rh = ranges.Add(key);
					rh.begin = i;
					rh.end = j;
					rh.len = rh.end - rh.begin;
					rh.h = ch;
				}
				
				RangeHash& rh = ranges[range_i];
				ASSERT(rh.begin == i && rh.end == j);
				ASSERT(rh.h == ch);
				rh.sections << ts_i;
				ts.ranges << range_i;
			}
		}
		ts_i++;
	}
	SortByValue(ranges, RangeHash());
	
	debug_out << "tmp:\n";
	for(int i = 0; i < tmp.GetCount(); i++) {
		auto& t = tmp[i];
		debug_out << "[" << i << "]: " << t.ToString() << "\n";
	}
	debug_out << "\n";
	debug_out << "ranges:\n";
	for(int i = 0; i < ranges.GetCount(); i++) {
		auto& r = ranges[i];
		debug_out << "[" << i << "]: (" << ranges.GetKey(i) << "): " << r.ToString() << "\n";
	}
	debug_out << "\n";
	
	
	// Find most important ranges
	VectorMap<hash_t, Vector<int>> hash_ranges;
	for(int i = 0; i < ranges.GetCount(); i++) {
		RangeHash& rh = ranges[i];
		hash_ranges.GetAdd(rh.h).Add(i);
	}
	struct VecSort {
		VectorMap<int,RangeHash>& ranges;
		//bool operator()(const Vector<int>& a, const Vector<int>& b) const {return a.GetCount() > b.GetCount();}
		bool operator()(const Vector<int>& a, const Vector<int>& b) const {
			RangeHash& ra = ranges[a[0]];
			RangeHash& rb = ranges[b[0]];
			int weight_a = ra.len * a.GetCount();
			int weight_b = rb.len * b.GetCount();
			return weight_a > weight_b;
		}
	};
	SortByValue(hash_ranges, VecSort{ranges});
	debug_out << "hash_ranges:\n";
	for(int i = 0; i < hash_ranges.GetCount(); i++) {
		debug_out << Format("%d: (%X) [", i, (int64)hash_ranges.GetKey(i));
		const auto& v = hash_ranges[i];
		for(int j = 0; j < v.GetCount(); j++) {
			if (j) debug_out << ", ";
			debug_out << v[j];
		}
		debug_out << "]\n";
			
	}
	
	// Make sections based on non-overlapping important ranges
	for(int i = 0; i < hash_ranges.GetCount(); i++) {
		auto& rv = hash_ranges[i];
		
		bool fail = false;
		for(int j = 0; j < rv.GetCount(); j++) {
			RangeHash& rh = ranges[rv[j]];
			for (int ts_i : rh.sections) {
				TmpSection& ts = tmp[ts_i];
				for(int k = ts.begin; k < ts.end; k++) {
					Line& line = lines[k];
					if (line.section >= 0) {
						fail = true;
						break;
					}
				}
				if (fail) break;
			}
			if (fail) break;
		}
		if (fail)
			continue;
		
		if (rv.GetCount() <= 1)
			continue;
		
		int sect_i = sections.GetCount();
		Section& sect = sections.Add();
		int max_range = 0;
		for(int j = 0; j < rv.GetCount(); j++) {
			sect.count++;
			RangeHash& rh = ranges[rv[j]];
			for (int ts_i : rh.sections) {
				TmpSection& ts = tmp[ts_i];
				for(int k = ts.begin; k < ts.end; k++) {
					Line& line = lines[k];
					line.section = sect_i;
				}
				max_range = max(max_range, ts.end - ts.begin);
			}
		}
		sect.orig_weight = max_range;
	}
	
	
	
}

void TryNo4tStructureSolver::MakeRepeatingSectionLines() {
	
}


END_TEXTLIB_NAMESPACE

