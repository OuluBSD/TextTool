#include <Core/Core.h>
#include <ToolCore/ToolCore.h>
using namespace Upp;

CONSOLE_APP_MAIN
{
	VectorMap<String,WString> map;
	map.Add("cat", "kæt");
	map.Add("mat", "mæt");
	map.Add("dog", "dɒg");
	map.Add("dogs", "dɒgz");
	map.Add("catalog", "ˈkæt.ə.lɔɡ");
	map.Add("university", "ˌjuː.nə.vər.sɪ.ti");
	
	GetSpellingRawDistance("ˌjuː.nə.vər.sɪ.ti", "mæt");
	
	VectorMap<String,String> matches;
	for(int i = 0; i < map.GetCount(); i++) {
		String k0 = map.GetKey(i);
		WString& ws0 = map[i];
		
		double best_dist = DBL_MAX;
		int best_j = -1;
		for(int j = 0; j < map.GetCount(); j++) {
			String k1 = map.GetKey(j);
			WString& ws1 = map[j];
			
			double raw_dist = GetSpellingRawDistance(ws0, ws1);
			double rel_dist = GetSpellingRelativeDistance(ws0, ws1);
			
			LOG(k0 << " -> " << k1 << ": raw = " << raw_dist << ", rel = " << rel_dist);
			
			if (rel_dist < best_dist && i != j) {
				best_dist = rel_dist;
				best_j = j;
			}
		}
		
		matches.Add(k0, map.GetKey(best_j));
	}
	DUMPM(matches);
}
