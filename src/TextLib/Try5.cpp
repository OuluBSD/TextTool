#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


TryStrDistSectionSolverBase::UniqueLine& TryStrDistSectionSolverBase::GetAddUniqueLine(const String& s) {
	ASSERT(s.GetCount());
	TextComparison cmp;
	int closest_i = -1;
	double closest_dist = 0;
	for(int i = 0; i < uniq_lines.GetCount(); i++) {
		UniqueLine& ul = uniq_lines[i];
		double dist = cmp.Process(s, ul.txt);
		if (dist >= unique_dist_limit && dist > closest_dist) {
			closest_i = i;
			closest_dist = dist;
		}
	}
	if (closest_i >= 0) {
		UniqueLine& ul = uniq_lines[closest_i];
		return ul;
	}
	UniqueLine& ul = uniq_lines.Add();
	ul.txt = TrimBoth(s);
	return ul;
}

Vector<int>& TryStrDistSectionSolverBase::GetAddHashRange(const SoftMatchString& s) {
	for(int i = 0; i < hash_ranges.GetCount(); i++) {
		if (hash_ranges[i].k == s)
			return hash_ranges[i].v;
	}
	auto& v = hash_ranges.Add();
	v.k = s;
	return v.v;
}

void TryNo5tStructureSolver::MakeSections() {
	
	int max_lines = uniq_lines[0].count;
	
	for(int i = 0; i < lines.GetCount(); i++) {
		Line& line = lines[i];
		UniqueLine& ul = GetAddUniqueLine(line.txt);
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
		SoftMatchString txt;
		int begin, end, len;
		Vector<int> sections;
		bool operator()(const RangeHash& a, const RangeHash& b) const {return (a.sections.GetCount()*a.len) > (b.sections.GetCount()*b.len);}
		String ToString() const {
			return Format("%d-%d (%d) %X: count %d", begin, end, len, (int)txt.lines.GetHashValue(), sections.GetCount());
		}
	};
	
	struct TmpSection : Moveable<TmpSection> {
		//Vector<hash_t> hashes;
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
			Vector<String> range_lines;
			for(int j = i+1; j < ts.end; j++) {
				Line& line = lines[j-1];
				range_lines << line.txt;
				
				int key = i * 1000 + j;
				int range_i = ranges.Find(key);
				if (range_i < 0) {
					range_i = ranges.GetCount();
					RangeHash& rh = ranges.Add(key);
					rh.begin = i;
					rh.end = j;
					rh.len = rh.end - rh.begin;
					rh.txt = range_lines;
				}
				
				RangeHash& rh = ranges[range_i];
				ASSERT(rh.begin == i && rh.end == j);
				ASSERT(rh.txt.lines.GetCount() == range_lines.GetCount());
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
	hash_ranges.Clear();
	for(int i = 0; i < ranges.GetCount(); i++) {
		RangeHash& rh = ranges[i];
		GetAddHashRange(rh.txt).Add(i);
	}
	struct VecSort {
		VectorMap<int,RangeHash>& ranges;
		bool operator()(const HashRangeLink& a, const HashRangeLink& b) const {
			RangeHash& ra = ranges[a.v[0]];
			RangeHash& rb = ranges[b.v[0]];
			int weight_a = ra.len * a.v.GetCount();
			int weight_b = rb.len * b.v.GetCount();
			return weight_a > weight_b;
		}
	};
	Sort(hash_ranges, VecSort{ranges});
	debug_out << "hash_ranges:\n";
	for(int i = 0; i < hash_ranges.GetCount(); i++) {
		debug_out << Format("%d: (%X) [", i, (int64)hash_ranges[i].k.lines.GetHashValue());
		const auto& v = hash_ranges[i].v;
		for(int j = 0; j < v.GetCount(); j++) {
			if (j) debug_out << ", ";
			debug_out << v[j];
		}
		debug_out << "]\n";
	}
	
	// Make sections based on non-overlapping important ranges
	for(int i = 0; i < hash_ranges.GetCount(); i++) {
		auto& rv = hash_ranges[i].v;
		
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
		sect.repeat = 1.0;
		sect.flag_repeating = true;
		sect.count = 0;
		int max_range = 0;
		for(int j = 0; j < rv.GetCount(); j++) {
			RangeHash& rh = ranges[rv[j]];
			for (int ts_i : rh.sections) {
				TmpSection& ts = tmp[ts_i];
				bool add_lines = sect.first_line < 0;
				if (add_lines)
					sect.first_line = ts.begin;
				for(int k = ts.begin; k < ts.end; k++) {
					Line& line = lines[k];
					line.section = sect_i;
					if (add_lines)
						sect.lines << line.txt;
				}
				max_range = max(max_range, ts.end - ts.begin);
				if (sect.first_line < 0)
					sect.first_line = ts.begin;
			}
		}
		
		// Count parts
		int prev_sect = -1;
		for (const Line& l : lines) {
			if (l.section == sect_i && prev_sect != sect_i)
				sect.count++;
			prev_sect = l.section;
		}
		
		sect.orig_weight = max_range;
	}
	
	
}

void TryNo5tStructureSolver::MakeRepeatingSectionLines() {
	
}


END_TEXTLIB_NAMESPACE

