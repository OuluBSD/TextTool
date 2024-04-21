#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



ArrayMap<hash_t, LeadSolver>& __LeadSolvers() {
	static ArrayMap<hash_t, LeadSolver> map;
	return map;
}


LeadSolver::LeadSolver() {
	
}

LeadSolver& LeadSolver::Get(MetaEntity& e) {
	String t = e.file_title;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, LeadSolver>& map = __LeadSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	LeadSolver& ls = map.Add(h);
	ls.entity = &e;
	return ls;
}

void LeadSolver::ClearTasks() {
	for (LeadSolver& g : __LeadSolvers().GetValues())
		g.SetNotRunning();
}

void LeadSolver::RestartTasks() {
	for (LeadSolver& g : __LeadSolvers().GetValues())
		g.Start();
}

void LeadSolver::Process() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& sd = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	sa = &sda.GetLeadEntityAnalysis(entity->file_title);
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LS_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			NextPhase();
			ClearDB();
		}
		else if (phase == LS_DOWNLOAD_WEBSITES) {
			ProcessDownloadWebsites(false);
		}
		else if (phase == LS_PARSE_WEBSITES) {
			ProcessDownloadWebsites(true); // this won't actually download the website again
		}
		else if (phase == LS_ANALYZE_BOOLEANS) {
			ProcessAnalyzeBooleans();
		}
		else if (phase == LS_ANALYZE_STRINGS) {
			ProcessAnalyzeStrings();
		}
		else if (phase == LS_ANALYZE_LISTS) {
			ProcessAnalyzeLists();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_TYPECAST) {
			ProcessAnalyzeSongTypecast();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_CONTENT) {
			ProcessAnalyzeSongContent();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_ATTRS) {
			ProcessAnalyzeSongAttrs();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_COLORS) {
			ProcessAnalyzeSongColors();
		}
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = LS_BEGIN;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void LeadSolver::ClearDB() {
	/*for(int i = 0; i < script->parts.GetCount(); i++) {
		StaticPart& sp = script->parts[i];
		auto& lines = sp.nana.Get();
		for(int j = 0; j < lines.GetCount(); j++) {
			auto& line = lines[j];
			line.pp_i = -1;
			line.end_pp_i = -1;
		}
		sp.phrase_parts.Clear();
	}
	script->picked_phrase_parts.Clear();*/
}

String LeadSolver::GetLeadCacheDir() {
	String dir = ConfigFile("lead-cache");
	RealizeDirectory(dir);
	return dir;
}

void LeadSolver::ProcessDownloadWebsites(bool parse) {
	
	if (batch == LEADSITE_TAXI) {
		String url = "https://www.taxi.com/industry";
		String content = ProcessDownloadWebsiteUrl(url);
		if (parse)
			ParseWebsite(batch, content);
		NextBatch();
	}
	else if (batch == LEADSITE_MUSICXRAY) {
		int page = sub_batch+1;
		String url = "https://www.musicxray.com/interactions/browse?page=" + IntStr(page) + "&per_page=50";
		String content = ProcessDownloadWebsiteUrl(url);
		if (parse)
			ParseWebsite(batch, content);
		if (content.Find("?page=" + IntStr(page+1)) >= 0)
			NextSubBatch();
		else
			NextBatch();
	}
	else if (batch == LEADSITE_SONICBIDS) {
		int page = sub_batch+1;
		String url = "https://www.sonicbids.com/find-gigs?type=LICENSING&type=COMPETITION&page=" + IntStr(page);
		String content = ProcessDownloadWebsiteUrl(url);
		if (parse)
			ParseWebsite(batch, content);
		if (content.Find(";page=" + IntStr(page+1) + "\"") >= 0)
			NextSubBatch();
		else
			NextBatch();
	}
	else {
		SetWaiting(0);
		NextPhase();
	}
}

void LeadSolver::ParseWebsite(int batch, String content) {
	LeadData& ld = MetaDatabase::Single().lead_data;
	content.Replace("\r", "");
	
	if (batch == LEADSITE_TAXI) {
		Vector<String> categories = Split(content, "<div class='genre-title'>");
		if (categories.IsEmpty()) return;
		categories.Remove(0);
		for (String& cat_str : categories) {
			Vector<String> listings = Split(cat_str, "<div class='listing' id='");
			if (listings.IsEmpty()) continue;
			listings.Remove(0);
			
			
			for (String& listing_str : listings) {
				// Pick unique listing id from the beginning of the string
				int a = listing_str.Find("'");
				if (a < 0) continue;
				String id = listing_str.Left(a);
				
				// Trim begin
				a = listing_str.Find("\n");
				if (a < 0) continue;
				listing_str = listing_str.Mid(a+1);
				
				// Trim end
				a = listing_str.Find("<p class='buttons-title'>");
				if (a < 0) continue;
				listing_str = listing_str.Left(a);
				
				Vector<String> links;
				String text = DeHtml(listing_str, links);
				
				
				double payout = 0;
				a = text.Find("$");
				if (a >= 0) {
					a++;
					String payout_str;
					for(int i = a; i < text.GetCount(); i++) {
						int chr = text[i];
						if (chr == ',')
							continue;
						if (IsDigit(chr) || chr == '.')
							payout_str.Cat(chr);
						else break;
					}
					payout = ScanDouble(payout_str);
				}
				
				/*LOG(id);
				LOG(text);
				DUMPC(links);*/
				
				
				String title;
				{
					Vector<String> words = Split(text, " ");
					for(int i = 0; i < words.GetCount(); i++) {
						String& w = words[i];
						if (w.Left(1) == "'" || IsAllUpper(w) ||
							w == "A" || w == "Bunch" || w == "Wide" || w == "Range" ||
							w == "Lots" || w == "Tons" || w == "of") {
							if (!title.IsEmpty()) title.Cat(' ');
							title << w;
						}
						else break;
					}
					if (title.IsEmpty()) {
						for(int i = 0; i < 4 && i < words.GetCount(); i++) {
							if (!title.IsEmpty()) title.Cat(' ');
							title << words[i];
						}
					}
				}
				
				LeadOpportunity& o = ld.GetAddOpportunity(batch, id);
				o.name = title;
				o.request_description = text;
				o.links <<= links;
				o.min_compensation = o.max_compensation = payout;
			}
		}
	}
	else if (batch == LEADSITE_MUSICXRAY) {
		Vector<String> listings = Split(content, "<bootstrap-card>");
		if (listings.IsEmpty()) return;
		listings.Remove(0);
		for (String& listing_str : listings) {
			int a = listing_str.Find("<span class");
			if (a < 0) continue;
			a = listing_str.Find(">", a);
			if (a < 0) continue;
			a++;
			int b = listing_str.Find("</span>");
			if (b < 0) continue;
			String title = TrimBoth(listing_str.Mid(a, b-a));
			
			double price = 0;
			a = listing_str.Find("<b>submission price:");
			if (a >= 0) {
				a = listing_str.Find("</b>", a);
				if (a < 0) continue;
				a += 4;
				b = listing_str.Find("<br/>", a);
				if (b < 0) continue;
				String price_str = TrimBoth(listing_str.Mid(a, b-a));
				price_str.Replace("$", "");
				price_str.Replace("€", "");
				price_str.Replace(",", "");
				price_str.Replace(" ", "");
				price_str = TrimBoth(price_str);
				price = ScanDouble(price_str);
			}
			
			double payout_from = 0;
			double payout_to = 0;
			a = listing_str.Find("<b>Payout:");
			if (a >= 0) {
				a = listing_str.Find("</b>", a);
				if (a < 0) continue;
				a += 4;
				b = listing_str.Find("<br/>", a);
				if (b < 0) continue;
				String str = TrimBoth(listing_str.Mid(a, b-a));
				str.Replace("$", "");
				str.Replace("€", "");
				str.Replace(",", "");
				str.Replace(" ", "");
				str = TrimBoth(str);
				Vector<String> parts = Split(str, "-");
				if (parts.GetCount() == 1) {
					payout_from = payout_to = ScanDouble(str);
				}
				else if (parts.GetCount() == 2) {
					payout_from = ScanDouble(parts[0]);
					payout_to = ScanDouble(parts[1]);
				}
			}
			
			a = listing_str.Find("<div class=\"col-xs-12\">");
			if (a < 0)
				continue;
			a = listing_str.Find("\n", a);
			if (a < 0) continue;
			b = listing_str.Find("</div>", a);
			if (b < 0) continue;
			String html = listing_str.Mid(a,b-a);
			Vector<String> links;
			String text = DeHtml(html, links);
			
			title = DeHtml(title, links);
			if (title.Find("new") == 0)
				title = TrimBoth(title.Mid(3));
			
			/*DUMP(title);
			DUMP(price);
			DUMP(payout_from);
			DUMP(payout_to);
			LOG(text);
			DUMPC(links);*/
			
			CombineHash ch;
			ch.Do(title).Do(text);
			String id_str = IntStr64(ch);
			
			LeadOpportunity& o = ld.GetAddOpportunity(batch, id_str);
			o.name = title;
			o.compensated = payout_from > 0;
			o.min_compensation = payout_from;
			o.max_compensation = payout_to;
			o.request_description = text;
			o.min_entry_price_cents = price * 100;
			o.links <<= links;
		}
	}
	else if (batch == LEADSITE_SONICBIDS) {
		LOG(content);
		
		int a = content.Find("require.config['opportunity-search']");
		if (a < 0) return;
		a = content.Find("{", a);
		if (a < 0) return;
		a = content.Find("data", a);
		if (a < 0) return;
		a = content.Find(":", a);
		if (a < 0) return;
		a++;
		int b = content.Find("</script>", a);
		if (b < 0) return;
		b = content.ReverseFind("};", b);
		String json = content.Mid(a,b-a);
		DUMP(json);
		
		Value v = ParseJSON(json);
		LOG(AsJSON(v, true));
		ValueMap root = v;
		ValueArray opportunities = root.GetAdd("opportunities");
		for(int i = 0; i < opportunities.GetCount(); i++) {
			ValueMap opp = opportunities[i];
			String id_str = IntStr(opp.GetAdd("id"));
			LeadOpportunity& o = ld.GetAddOpportunity(batch, id_str);
			
			o.genres.Clear();
			ValueArray genres = opp.GetAdd("entry_genres");
			for(int j = 0; j < genres.GetCount(); j++) {
				ValueMap genre = genres[j];
				LeadOpportunity::Genre& g = o.genres.Add();
				g.id = genre.GetAdd("id");
				g.name = genre.GetAdd("name");
				g.primary = genre.GetAdd("primary");
			}
			
			o.promoter_group_genres.Clear();
			ValueArray promoter_genres = opp.GetAdd("promoter_group_genres");
			for(int j = 0; j < promoter_genres.GetCount(); j++) {
				ValueMap genre = promoter_genres[j];
				LeadOpportunity::Genre& g = o.promoter_group_genres.Add();
				g.id = genre.GetAdd("id");
				g.name = genre.GetAdd("name");
				g.primary = genre.GetAdd("primary");
			}
			
			o.name = opp.GetAdd("name");
			o.band_opportunity_type = opp.GetAdd("band_opportunity_type");
			o.obj_class = opp.GetAdd("obj_class");
			o.request_entry_fee = opp.GetAdd("request_entry_fee");
			o.request_featured = opp.GetAdd("request_featured");
			o.request_exclusive = opp.GetAdd("request_exclusive");
			o.request_curated = opp.GetAdd("request_curated");
			o.request_contest = opp.GetAdd("request_contest");
			o.request_comments = opp.GetAdd("request_comments");
			o.request_first_name = opp.GetAdd("request_first_name");
			o.request_last_name = opp.GetAdd("request_last_name");
			o.request_email = opp.GetAdd("request_email");
			o.request_phone = opp.GetAdd("request_phone");
			o.request_description = opp.GetAdd("request_description");
			o.request_opportunity_description = opp.GetAdd("request_opportunity_description");
			o.request_band_description = opp.GetAdd("request_band_description");
			o.request_selection_description = opp.GetAdd("request_selection_description");
			o.vanity_url_id = opp.GetAdd("vanity_url_id");
			o.vanity_url_name = opp.GetAdd("vanity_url_name");
			o.status_text = opp.GetAdd("status_text");
			o.id = id_str;
			o.description = opp.GetAdd("description");
			o.band_opportunity_type_text = opp.GetAdd("band_opportunity_type_text");
			o.local_event_end_datetime = opp.GetAdd("local_event_end_datetime");
			o.public_image_url = opp.GetAdd("public_image_url");
			o.logo_image_url = opp.GetAdd("logo_image_url");
			o.promoter_group_name = opp.GetAdd("promoter_group_name");
			o.promoter_group_main_image_url = opp.GetAdd("promoter_group_main_image_url");
			o.promoter_group_facebook_url = opp.GetAdd("promoter_group_facebook_url");
			o.promoter_group_twitter_url = opp.GetAdd("promoter_group_twitter_url");
			o.promoter_group_youtube_url = opp.GetAdd("promoter_group_youtube_url");
			o.promoter_group_instagram_url = opp.GetAdd("promoter_group_instagram_url");
			o.promoter_group_talent_description = opp.GetAdd("promoter_group_talent_description");
			o.promoter_group_short_description = opp.GetAdd("promoter_group_short_description");
			o.promoter_group_talent_roster = opp.GetAdd("promoter_group_talent_roster");
			o.promoter_group_opportunity_frequency_count = opp.GetAdd("promoter_group_opportunity_frequency_count");
			o.promoter_group_opportunity_frequency = opp.GetAdd("promoter_group_opportunity_frequency");
			o.min_compensation = opp.GetAdd("min_compensation");
			o.max_compensation = opp.GetAdd("max_compensation");
			o.pay_to_apply = opp.GetAdd("pay_to_apply");
			o.free_to_apply = opp.GetAdd("free_to_apply");
			o.entry_count = opp.GetAdd("entry_count");
			o.entry_end_datetime = opp.GetAdd("entry_end_datetime");
			o.date_created = opp.GetAdd("date_created");
			o.compensated = opp.GetAdd("compensated");
			o.min_entry_price_cents = opp.GetAdd("min_entry_price_cents");
			
			if (o.compensated && o.max_compensation == 0) {
				o.min_compensation = o.max_compensation = 1;
			}
			
			if (o.pay_to_apply && o.min_entry_price_cents == 0) {
				o.min_entry_price_cents = 1;
			}
			
			o.request_description.Replace("\r", "");
			
			Vector<String> links;
			o.request_opportunity_description = DeHtml(o.request_opportunity_description, links);
			o.request_band_description = DeHtml(o.request_band_description, links);
			o.request_selection_description = DeHtml(o.request_selection_description, links);
			o.links <<= links;
		}
	}
	
}

String LeadSolver::ProcessDownloadWebsiteUrl(String url) {
	LeadCache cache;
	Time last_update = cache.last_update.Get(url, Time(1970,1,1));
	Time limit = GetSysTime() - 60*60*24;
	String dir = GetLeadCacheDir();
	String fname = Base64Encode(url) + ".html";
	fname.Replace("/", "");
	fname.Replace("\\", "");
	String path = AppendFileName(dir, fname);
	String prev = LoadFile(path);
	if (last_update <= limit || prev.IsEmpty()) {
		HttpRequest http;
		http.Url(url);
		http.UserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.3");
		String content = http.Execute();
		FileOut fout(path);
		fout << content;
		fout.Close();
		cache.last_update.GetAdd(url) = GetSysTime();
		return content;
	}
	return prev;
}

void LeadSolver::ProcessAnalyzeFn(int fn, Event<String> cb) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	LeadSolverArgs args;
	args.opp_i = batch;
	args.fn = fn;
	
	if (batch >= ld.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetLeadSolver(args, cb);
}

void LeadSolver::ProcessAnalyzeBooleans() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_booleans.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(0, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeBooleans(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	RemoveEmptyLines2(res);
	
	Vector<String> lines = Split(res, "\n");
	
	if (lines.GetCount() == LISTING_SONG_BOOLEAN_COUNT+1)
		lines.Remove(0);
	
	bool fast_analyse = lines.GetCount() == LISTING_SONG_BOOLEAN_COUNT;
	
	opp.analyzed_booleans.SetCount(LISTING_SONG_BOOLEAN_COUNT, 0);
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		line = TrimBoth(line);
		
		int a = line.Find(":");
		if (a < 0) continue;
		String key = TrimBoth(line.Left(a));
		String value = ToLower(TrimBoth(line.Mid(a+1)));
		
		int idx = i;
		if (!fast_analyse) {
			idx = -1;
			for(int j = 0; j < LISTING_SONG_BOOLEAN_COUNT; j++) {
				if (key == GetSongListingBooleanKey(j)) {
					idx = j;
					break;
				}
			}
			if (idx < 0)
				continue;
		}
		
		bool b = value.Find("true") == 0;
		opp.analyzed_booleans[idx] = b;
	}
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeStrings() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_string.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(1, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeStrings(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeLists() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_lists.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(2, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeLists(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeSongTypecast() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_song_typecast.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(3, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeSongTypecast(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeSongContent() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_song_content.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(4, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeSongContent(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeSongAttrs() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_song_attrs.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(5, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeSongAttrs(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeSongColors() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (!opp.analyzed_song_colors.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(6, THISBACK(OnProcessAnalyzeBooleans));
}

void LeadSolver::OnProcessAnalyzeSongColors(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}




END_TEXTLIB_NAMESPACE

