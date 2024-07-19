#include "ToolCore.h"


BEGIN_TEXTLIB_NAMESPACE


TextComparison::TextComparison() {
	
}

void TextComparison::ReplaceWithWhite(WString& a) {
	for(int i = 0; i < a.GetCount(); i++) {
		int chr = a[i];
		if (chr >= 0x2000 && chr < 0x3000) {
			a = a.Left(i) + WString(" ") + a.Mid(i+1);
		}
	}
}

double TextComparison::Process(const String& a_, const String& b_) {
	double ret = 0;
	
	this->a = a_.ToWString();
	this->b = b_.ToWString();
	
	ReplaceWithWhite(a);
	ReplaceWithWhite(b);
	
	aw = Split(a, String(" ").ToWString());
	bw = Split(b, String(" ").ToWString());
	
	a_in_use.SetCount(0);
	b_in_use.SetCount(0);
	a_in_use.SetCount(aw.GetCount(), false);
	b_in_use.SetCount(bw.GetCount(), false);
	link_weights.Clear();
	
	for(int i = 0; i < aw.GetCount(); i++) {
		const WString& a = aw[i];
		for(int j = 0; j < bw.GetCount(); j++) {
			const WString& b = bw[j];
			double dist = JaroWinklerDistance(a, b);
			int key = i * 10000 + j;
			link_weights.Add(key, dist);
		}
	}
	SortByValue(link_weights, StdGreater<double>());
	
	int a_remaining = aw.GetCount();
	int b_remaining = bw.GetCount();
	map_ab.SetCount(0);
	map_ab.SetCount(aw.GetCount(), -1);
	map_ab_weights.SetCount(0);
	map_ab_weights.SetCount(aw.GetCount(), 0);
	int union_count = 0;
	while (a_remaining && b_remaining) {
		int best_link = -1;
		for(int i = 0; i < link_weights.GetCount(); i++) {
			double weight = link_weights[i];
			if (weight < 0.5)
				break;
			int key = link_weights.GetKey(i);
			int ai = key / 10000;
			int bi = key % 10000;
			if (!a_in_use[ai] && !b_in_use[bi]) {
				best_link = i;
				break;
			}
		}
		if (best_link < 0)
			break;
		double weight = link_weights[best_link];
		int key = link_weights.GetKey(best_link);
		int ai = key / 10000;
		int bi = key % 10000;
		a_in_use[ai] = true;
		b_in_use[bi] = true;
		map_ab[ai] = bi;
		map_ab_weights[ai] = weight;
		a_remaining--;
		b_remaining--;
		union_count++;
	}
	
	unlinked_a.Clear();
	unlinked_b.Clear();
	for(int i = 0; i < a_in_use.GetCount(); i++)
		if (!a_in_use[i])
			unlinked_a << i;
	for(int i = 0; i < b_in_use.GetCount(); i++)
		if (!b_in_use[i])
			unlinked_b << i;
	
	// Use modified Jaccard similarity
	// Use weights instead of just 0/1 values
	double union_sum = 0;
	for (double d: map_ab_weights)
		union_sum += d; // just add meaningless 0-values too
	double div = max(0.0, aw.GetCount() + bw.GetCount() - union_sum);
	ASSERT(div >= union_sum);
	if (div <= 0)
		ret = 0;
	else
		ret = union_sum / div;
	
	if (0) {
		if (0) {
			DUMPC(map_ab);
			DUMPC(unlinked_a);
			DUMPC(unlinked_b);
		}
		for(int i = 0; i < map_ab.GetCount(); i++) {
			int j = map_ab[i];
			if (j < 0) continue;
			const WString& a = aw[i];
			const WString& b = bw[j];
			LOG("Link " << i << ": " << a << " --> " << b << ": " << map_ab_weights[i]);
		}
		for(int i = 0; i < unlinked_a.GetCount(); i++) {
			LOG("unlinked A: " << aw[unlinked_a[i]]);
		}
		for(int i = 0; i < unlinked_b.GetCount(); i++) {
			LOG("unlinked B: " << bw[unlinked_b[i]]);
		}
		LOG("similarity: " << ret);
	}
	
	
	return ret;
}

double TextComparison::JaroWinklerDistance(const WString& s, const WString& a) {
    int i, j, l;
    int m = 0, t = 0;
    int sl = s.GetCount();
    int al = a.GetCount();
    sflags.SetCount(0);
    aflags.SetCount(0);
    sflags.SetCount(sl, 0);
    aflags.SetCount(al, 0);
    int range = max(0, max(sl, al) / 2 - 1);
    double dw;

    if (!sl || !al)
        return 0.0;

    for (i = 0; i < al; i++)
        aflags[i] = 0;

    for (i = 0; i < sl; i++)
        sflags[i] = 0;

    /* calculate matching characters */
    for (i = 0; i < al; i++) {
        for (j = max(i - range, 0), l = min(i + range + 1, sl); j < l; j++) {
            if (a[i] == s[j] && !sflags[j]) {
                sflags[j] = 1;
                aflags[i] = 1;
                m++;
                break;
            }
        }
    }

    if (!m)
        return 0.0;

    /* calculate character transpositions */
    l = 0;
    for (i = 0; i < al; i++) {
        if (aflags[i] == 1) {
            for (j = l; j < sl; j++) {
                if (sflags[j] == 1) {
                    l = j + 1;
                    break;
                }
            }
            if (a[i] != s[j])
                t++;
        }
    }
    t /= 2;

    /* Jaro distance */
    dw = (((double)m / sl) + ((double)m / al) + ((double)(m - t) / m)) / 3.0;

    /* calculate common string prefix up to 4 chars */
    l = 0;
    for (i = 0; i < min(min(sl, al), 4); i++)
        if (s[i] == a[i])
            l++;

    /* Jaro-Winkler distance */
    dw = dw + (l * SCALING_FACTOR * (1 - dw));

    return dw;
}


bool SoftMatchString::operator==(const SoftMatchString& s) const {
	thread_local static TextComparison cmp;
	if (lines.GetCount() != s.lines.GetCount())
		return false;
	double weight_sum = 0;
	for(int i = 0; i < s.lines.GetCount(); i++) {
		const String& a = lines[i];
		const String& b = s.lines[i];
		double weight = cmp.Process(a, b);
		weight_sum += weight;
	}
	double av = weight_sum / lines.GetCount();
	return av;
}

hash_t SoftMatchString::GetHashValue() const {
	return lines.GetHashValue();
}


END_TEXTLIB_NAMESPACE
