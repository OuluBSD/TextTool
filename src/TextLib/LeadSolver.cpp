#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



ArrayMap<hash_t, LeadSolver>& __LeadSolvers() {
	static ArrayMap<hash_t, LeadSolver> map;
	return map;
}


LeadSolver::LeadSolver() {
	
}

LeadSolver& LeadSolver::Get(Owner& e) {
	String t = e.name;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, LeadSolver>& map = __LeadSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	LeadSolver& ls = map.Add(h);
	ls.owner = &e;
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
	sa = &sda.GetLeadEntityAnalysis(owner->name);
	int lng_i = db.GetLanguageIndex();
	
	// Don't process all data with AI when using generic updater profile,
	// because more costly AI profile is used:
	// skip after booleans
	bool reduce_load = owner == &Owner::DatabaseUpdate();
	
	if (0) {
		Vector<int> rm_list;
		for(int i = 0; i < sd.opportunities.GetCount(); i++) {
			if (sd.opportunities[i].leadsite == LEADSITE_TAXI)
				rm_list << i;
		}
		sd.opportunities.Remove(rm_list);
	}
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == LS_BEGIN) {
			time_started = GetSysTime();
			//skip_ready = false;
			if (lng_i == LNG_ENGLISH)
				NextPhase();
			else
				MovePhase(LS_COUNT);
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
			if (reduce_load) {
				phase = LS_COUNT;
				continue;
			}
			ProcessAnalyzeStrings();
		}
		else if (phase == LS_ANALYZE_LISTS) {
			ProcessAnalyzeLists();
		}
		else if (phase == LS_COARSE_RANKING) {
			ProcessCoarseRanking();
		}
		else if (phase == LS_AVERAGE_PAYOUT_ESTIMATION) {
			ProcessAveragePayoutEstimation();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_TYPECAST) {
			ProcessAnalyzeSongTypecast();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_SONG_IDEAS) {
			ProcessAnalyzeLyricsIdeas();
		}
		else if (phase == LS_ANALYZE_POTENTIAL_MUSIC_STYLE_TEXT) {
			ProcessAnalyzeMusicStyle();
		}
		else if (phase == LS_TEMPLATE_TITLE_AND_TEXT) {
			ProcessTemplateTitleAndText();
		}
		else if (phase == LS_TEMPLATE_ANALYZE) {
			ProcessTemplateAnalyze();
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
		#if 1
		// MusicXray is dead 29.5.2024 :(
		NextBatch();
		#else
		int page = sub_batch+1;
		String url = "https://www.musicxray.com/interactions/browse?page=" + IntStr(page) + "&per_page=50";
		String content = ProcessDownloadWebsiteUrl(url);
		if (parse)
			ParseWebsite(batch, content);
		if (content.Find("?page=" + IntStr(page+1)) >= 0)
			NextSubBatch();
		else
			NextBatch();
		#endif
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
				
				// Add very coarse royalty estimates. Without any of these, calculations will
				// break.
				o.min_compensation += 500;
				o.max_compensation += 6000;
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
		//LOG(content);
		
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
		//DUMP(json);
		
		Value v = ParseJSON(json);
		//LOG(AsJSON(v, true));
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

static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((String*)userp)->Cat((char*)contents, size * nmemb);
    return size * nmemb;
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
		String proxy = GetGlobalProxy();
		String content;
		
		if (1) {
			int proxy_port = 0;
			
			int a = proxy.Find("://");
			if (a >= 0) a = proxy.Find(":", a+1);
			if (a >= 0) {
				proxy_port = ScanInt(proxy.Mid(a+1));
				proxy = proxy.Left(a);
			}
			a = proxy.Find("http://");
			if (a >= 0) proxy = proxy.Mid(a+7);
			a = proxy.Find("https://");
			if (a >= 0) proxy = proxy.Mid(a+8);
			
			HttpRequest http;
			//http.Trace();
			http.Url(url);
			http.UserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.3");
			
			if (proxy_port > 0) {
				http.Proxy(proxy, proxy_port);
				http.SSLProxy(proxy, proxy_port);
			}
			content = http.Execute();
		}
		else {
			CURL *curl;
			CURLcode res;
			
			curl = curl_easy_init();
			if(curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url.Begin());
				
				// url is redirected, so we tell libcurl to follow redirection
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				
				if (proxy.GetCount())
					curl_easy_setopt(curl, CURLOPT_PROXY, proxy.Begin());
				
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
				
				// Perform the request, res gets the return code
				res = curl_easy_perform(curl);
				
				// Check for errors
				if(res != CURLE_OK)
					fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
				
				// always cleanup
				curl_easy_cleanup(curl);
			}
		}
		
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
	m.GetLeadSolver(DB_SONG, args, cb);
}

void LeadSolver::ProcessAnalyzeBooleans() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if ((skip_ready && !opp.analyzed_booleans.IsEmpty()) || opp.leadsite == LEADSITE_MUSICXRAY) {
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

double LeadSolver::GetAverageOpportunityScore() {
	MetaDatabase& mdb = MetaDatabase::Single();
	double score_sum = 0;
	for (const LeadOpportunity& opp : mdb.lead_data.opportunities)
		score_sum += owner->GetOpportunityScore(opp);
	double score_av = score_sum / mdb.lead_data.opportunities.GetCount();
	return score_av;
}

bool LeadSolver::SkipLowScoreOpportunity() {
	MetaDatabase& mdb = MetaDatabase::Single();
	double score_limit = GetAverageOpportunityScore() * score_limit_factor;
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	int score = owner->GetOpportunityScore(opp);
	return score < score_limit && opp.min_compensation <= 0;
}

void LeadSolver::ProcessAnalyzeStrings() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if ((skip_ready && (!opp.analyzed_string.IsEmpty()) ||
		opp.leadsite == LEADSITE_MUSICXRAY) ||
		SkipLowScoreOpportunity()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(1, THISBACK(OnProcessAnalyzeStrings));
}

void LeadSolver::OnProcessAnalyzeStrings(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	/*
	 deal structure (e.g. exclusive): not specified
	3. deal type (e.g. song placement, radio play): not specified
	4. artist's royalty percentage: not specified
	5. who is the decision maker: Jared Hassan Foles - Producer/Chief Engineer of World Eater Recordings
	6. what kind of sound the song should have: able to achieve desired results in sound
	7. type of the target movie / advertisement (e.g. romantic, sport product): not specified
	8. based on the language and tone, what type of company/person wrote this listing: professional and experienced in the music industry
	9. based on the language and tone, what type of artist does the company/person want to work with: open to collaborating with all types of artists
	10. based on general assumptions, what information is lacking about the context: specific goals or end result desired by the company/person
	11. based on general assumptions, what information is lacking about the song: current genre or style of the song
	12. based on general assumptions, what guidelines could be used while deciding what kind of song to make: any type of song that fits within the artist's goals and desired results
	13. based on general assumptions,  what kind of monetary income can be expected by getting accepted in this listing: not specified
	14. based on general assumptions, what kind of level of competition is expected for this listing: not specified, but it can be assumed that there will be competition among songwriters and musicians.
	*/
	RemoveEmptyLines2(res);
	
	Vector<String> lines = Split(res, "\n");
	
	if (lines.GetCount() == LISTING_SONG_STRING_COUNT+1)
		lines.Remove(0);
	
	bool fast_analyse = lines.GetCount() == LISTING_SONG_STRING_COUNT;
	
	opp.analyzed_string.SetCount(LISTING_SONG_STRING_COUNT);
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		line = TrimBoth(line);
		
		int a = line.Find(":");
		if (a < 0) continue;
		String key = TrimBoth(line.Left(a));
		String value = TrimBoth(line.Mid(a+1));
		
		int idx = i;
		if (!fast_analyse) {
			idx = -1;
			for(int j = 0; j < LISTING_SONG_STRING_COUNT; j++) {
				if (key == GetSongListingStringKey(j)) {
					idx = j;
					break;
				}
			}
			if (idx < 0)
				continue;
		}
		
		opp.analyzed_string[idx] = value;
	}
	
	
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
	if ((skip_ready && (!opp.analyzed_lists.IsEmpty())) ||
		opp.leadsite == LEADSITE_MUSICXRAY ||
		SkipLowScoreOpportunity()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(2, THISBACK(OnProcessAnalyzeLists));
}

void LeadSolver::OnProcessAnalyzeLists(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	/*
	list of similar sounding artists: [Calvin Harris, Martin Garrix, Avicii]
	3. list of Data, what can be interpreted from this: [completed single, EDM, writer, producer, management, artist development, production, publishing, co-publishing, record deal]
	4. what kind of tones and moods could be suggested for the song for this opportunity: [energetic, danceable, electronic]
	5. List of "does this listing have increased chances of" for "Based on assumptions about pop music and music producers/industry": [getting your single noticed, securing a record deal or co-publishing agreement, gaining exposure and potential success in the EDM genre]
	6. List of "does this kind of song get selected" for "Based on assumptions about pop music and music producers/industry": [songs that are well-produced and have a strong EDM influence, songs with high energy and a catchy beat, songs that are unique and stand out from the crowd]
	*/
	
	RemoveEmptyLines2(res);
	
	Vector<String> lines = Split(res, "\n");
	
	if (lines.GetCount() == LISTING_SONG_LIST_COUNT+1)
		lines.Remove(0);
	
	bool fast_analyse = lines.GetCount() == LISTING_SONG_LIST_COUNT;
	
	opp.analyzed_lists.SetCount(LISTING_SONG_LIST_COUNT);
	
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
			for(int j = 0; j < LISTING_SONG_LIST_COUNT; j++) {
				if (key == GetSongListingListKey(j)) {
					idx = j;
					break;
				}
			}
			if (idx < 0)
				continue;
		}
		
		Vector<String>& list = opp.analyzed_lists[idx];
		
		a = value.Find("[");
		if (a >= 0) {
			a++;
			int b = value.ReverseFind("]");
			if (b >= 0) {
				value = value.Mid(a,b-a);
			}
		}
		
		list = Split(value, ", ");
	}
	
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessCoarseRanking() {
	MetaDatabase& mdb = MetaDatabase::Single();
	
	
	VectorMap<int,double> money_scores, opp_scores;
	
	for(int i = 0; i < mdb.lead_data.opportunities.GetCount(); i++) {
		LeadOpportunity& o = mdb.lead_data.opportunities[i];
		
		double price = 0.01 * o.min_entry_price_cents;
		
		double av_compensation = 0.5 * (o.min_compensation + max(o.min_compensation, o.max_compensation));
		
		double money_score = 0;
		if (price > 0 && av_compensation) {
			money_score = av_compensation / price * 1000;
		}
		else if (av_compensation > 0) {
			money_score = av_compensation;
		}
		// Punish expensive listings
		if (price > 50)
			money_score -= 100;
		// Reward easy and significant income
		if (LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME < o.analyzed_booleans.GetCount() &&
			o.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME] > 0) {
			money_score += 100;
			if (o.analyzed_booleans[LISTING_SONG_BOOLEAN_MONETARY_DIFFICULT_TO_DETERMINE] == 0)
				money_score += 200;
		}
		
		money_scores.Add(i, money_score);
		
		int opp_score =
			owner ?
				owner->GetOpportunityScore(o) :
				-1;
		opp_scores.Add(i, opp_score);
	}
	
	SortByValue(money_scores, StdGreater<double>());
	SortByValue(opp_scores, StdGreater<double>());
	
	for(int i = 0; i < money_scores.GetCount(); i++) {
		int opp_i = money_scores.GetKey(i);
		LeadOpportunity& o = mdb.lead_data.opportunities[opp_i];
		int rank = min(max_rank, 1+i);
		o.money_score = money_scores[i];
		o.money_score_rank = rank;
	}
	for(int i = 0; i < opp_scores.GetCount(); i++) {
		int opp_i = opp_scores.GetKey(i);
		LeadOpportunity& o = mdb.lead_data.opportunities[opp_i];
		int rank = min(max_rank, 1+i);
		o.opp_score = opp_scores[i];
		o.opp_score_rank = rank;
	}
	for (LeadOpportunity& o : mdb.lead_data.opportunities) {
		o.weighted_rank =
			o.money_score_rank * 0.3 +
			o.opp_score_rank * 0.7;
	}
	
	NextPhase();
}

void LeadSolver::ProcessAveragePayoutEstimation() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (opp.weighted_rank >= (double)max_rank) {
		NextBatch();
		return;
	}
	
	if ((skip_ready && opp.chance_list.GetCount()) ||
		opp.leadsite == LEADSITE_MUSICXRAY) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(3, THISBACK(OnProcessAveragePayoutEstimation));
}

void LeadSolver::OnProcessAveragePayoutEstimation(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	/*
	Initial listen: 70%
	- Production quality: 50%
	- Comparison to other submissions: 40%
	- Collaboration potential: 20%
	- Refinement and final review: 15%
	- Top contender selection: 10%
	- Total chance of acceptance: 2.1% (0.7 x 0.5 x 0.4 x 0.2 x 0.15 x 0.1 = 0.0021 = 0.21%). Again, keep in mind that these numbers are theoretical and may vary.
	- Average payout estimation for accepted song: $1,250 x 2.1% = $26.25 or approximately $26.
	*/
	
	if (res.Find("\n2.") >= 0)
		RemoveEmptyLines2(res);
	else
		RemoveEmptyLines3(res);
	
	Vector<String> lines = Split(res, "\n");
	
	double chance = 1.0;
	
	opp.chance_list.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		line = TrimBoth(line);
		int a0 = line.Find("otal chance of acceptance");
		int a1 = line.Find("verage payout estimation");
		if (a0 >= 0) {
			/*int a = line.Find(":");
			if (a >= 0) {
				a++;
				String value = TrimBoth(line.Mid(a));
				
				a = value.Find("%");
				if (a >= 0) {
					String perc_str = value.Left(a);
					int begin = 0;
					for (int j = perc_str.GetCount()-1; j >= 0; j--) {
						int chr = perc_str[j];
						if (IsDigit(chr) || chr == '.')
							continue;
						else {
							begin = j+1;
							break;
						}
					}
					perc_str = TrimBoth(value.Mid(begin));
					opp.chance_of_acceptance = ScanDouble(perc_str) * 0.01;
				}
			}*/
		}
		/*else if (a1 >= 0) {
			int a = line.Find(":");
			if (a >= 0) {
				a++;
				String value = TrimBoth(line.Mid(a));
				
				a = value.Find("=");
				if (a >= 0) {
					String money_str;
					for(int j = a+1; j < value.GetCount(); j++) {
						int chr = value[j];
						if (IsSpace(chr) || chr == ',' || chr == '$')
							continue;
						if (IsDigit(chr) || chr == '.')
							money_str.Cat(chr);
						else
							break;
					}
					opp.average_payout_estimation = ScanDouble(money_str);
				}
			}
		}*/
		else {
			int a = line.Find(":");
			if (a >= 0) {
				a++;
				String value = TrimBoth(line.Mid(a));
				double factor = ScanDouble(value) * 0.01;
				chance *= factor;
			}
			opp.chance_list << line;
		}
	}
	
	if (chance == 1.0)
		opp.chance_of_acceptance = 0.0001;
	else
		opp.chance_of_acceptance = chance;
	
	// Hotfix taxi.com: have at least 4% chance. Their average is 6%
	if (opp.leadsite == LEADSITE_TAXI && opp.chance_of_acceptance <= 0.04)
		opp.chance_of_acceptance += 0.04;
	
	opp.average_payout_estimation =
		opp.chance_of_acceptance * (opp.min_compensation + opp.max_compensation) * 0.5;
	
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
	if ((skip_ready && !(opp.contents.IsEmpty() || opp.typeclasses.IsEmpty())) ||
		SkipLowScoreOpportunity() ||
		opp.weighted_rank >= (double)max_rank ||
		opp.average_payout_estimation <= 0.1 ||
		opp.leadsite == LEADSITE_MUSICXRAY) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(4, THISBACK(OnProcessAnalyzeSongTypecast));
}

void LeadSolver::OnProcessAnalyzeSongTypecast(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	/*
	1,2
	- 3,4
	- 5,6
	- 7,8
	- 9,10
	*/
	Vector<String> lines = Split(res, "\n");
	
	opp.typeclasses.Clear();
	opp.contents.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		{
			int a;
			while ((a = line.Find("(")) >= 0) {
				int b = line.Find(")", a);
				if (b < 0) break;
				line = line.Left(a) + line.Mid(b+1);
			}
		}
		int a = line.Find(".");
		if (a >= 0)
			line = line.Mid(a+1);
		line = TrimBoth(line);
		Vector<String> parts = Split(line, ",");
		if (parts.GetCount() < 2)
			continue;
		int tc = ScanInt(TrimLeft(parts[0])) - 1;
		int co = ScanInt(TrimLeft(parts[1])) - 1;
		opp.typeclasses << tc;
		opp.contents << co;
	}
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeLyricsIdeas() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if ((skip_ready && !opp.lyrics_ideas.IsEmpty()) ||
		SkipLowScoreOpportunity() ||
		opp.weighted_rank >= (double)max_rank ||
		opp.average_payout_estimation <= 0.1 ||
		opp.leadsite == LEADSITE_MUSICXRAY) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(5, THISBACK(OnProcessAnalyzeLyricsIdeas));
}

void LeadSolver::OnProcessAnalyzeLyricsIdeas(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	Vector<String> lines = Split(res, "\n");
	
	opp.lyrics_ideas.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		if (line.IsEmpty()) continue;
		
		int a = line.Find(".");
		bool no_beginning = (i == 0 && (a < 0 || a >= 4));
		if (a >= 0 && a < 4)
			line = line.Mid(a+1);
		line = TrimBoth(line);
		
		if (line.IsEmpty()) continue;
		int chr = line[0];
		if (no_beginning && chr >= 'a' && chr <= 'z' && line.Find("he lyrics is about") < 0)
			line = "The lyrics is about " + line;
		
		a = line.Find("** - ");
		if (a >= 0)
			line = line.Mid(a+5);
		
		opp.lyrics_ideas << line;
	}
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessAnalyzeMusicStyle() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if ((skip_ready && !opp.music_styles.IsEmpty()) ||
		SkipLowScoreOpportunity() ||
		opp.weighted_rank >= (double)max_rank||
		opp.average_payout_estimation <= 0.1 ||
		opp.leadsite == LEADSITE_MUSICXRAY) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(6, THISBACK(OnProcessAnalyzeMusicStyle));
}

void LeadSolver::OnProcessAnalyzeMusicStyle(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	Vector<String> lines = Split(res, "\n");
	
	opp.music_styles.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String& line = lines[i];
		if (line.IsEmpty()) continue;
		
		int a = line.Find(".");
		bool no_beginning = (i == 0 && (a < 0 || a >= 4));
		if (a >= 0 && a < 4)
			line = line.Mid(a+1);
		line = TrimBoth(line);
		
		if (line.IsEmpty()) continue;
		int chr = line[0];
		if (no_beginning && chr >= 'a' && chr <= 'z' && line.Find("he lyrics is about") < 0)
			line = "The lyrics is about " + line;
		
		a = line.Find("** - ");
		if (a >= 0)
			line = line.Mid(a+5);
		
		opp.music_styles << line;
	}
	
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessTemplateTitleAndText() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadData& ld = mdb.lead_data;
	if (batch >= mdb.lead_data.opportunities.GetCount()) {
		NextPhase();
		return;
	}
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	if (SkipLowScoreOpportunity() ||
		opp.weighted_rank >= (double)max_rank||
		opp.average_payout_estimation <= 0.1) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(7, THISBACK(OnProcessTemplateTitleAndText));
}

void LeadSolver::OnProcessTemplateTitleAndText(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadDataTemplate& ldt = LeadDataTemplate::Single();
	int lng = mdb.GetLanguageIndex();
	LeadData& ld = mdb.lead_data;
	LeadOpportunity& opp = mdb.lead_data.opportunities[batch];
	
	/*response example:
		New Music Opportunity Within Specific Genres"
		- Description (multiline):
		
		Are you a talented musician looking for new opportunities? ...
	*/
	
	String title, txt;
	int a = res.Find("\"");
	if (a >= 0) {
		title = TrimBoth(res.Left(a));
	}
	a = res.Find("Description");
	if (a >= 0) {
		a = res.Find("\n", a);
		if (a >= 0) {
			txt = TrimBoth(res.Mid(a+1));
		}
	}
	
	if (title.GetCount() && txt.GetCount()) {
		CombineHash ch;
		ch.Do(title).Do(txt);
		hash_t h = ch;
		bool found = false;
		for (const LeadTemplate& lt : ldt.templates) {
			if (lt.hash == h) {
				found = true;
				break;
			}
		}
		
		if (!found) {
			LeadTemplate& t = ldt.templates.Add();
			t.hash = h;
			t.title = title;
			t.text = txt;
			t.orig_lead_idx = batch;
			t.orig_lead_lng = lng;
			t.submission_price = opp.min_entry_price_cents * 0.01;
		}
	}
	
	NextBatch();
	SetWaiting(0);
}

void LeadSolver::ProcessTemplateAnalyze() {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadDataTemplate& ldt = LeadDataTemplate::Single();
	if (batch >= ldt.templates.GetCount()) {
		NextPhase();
		return;
	}
	LeadTemplate& lt = ldt.templates[batch];
	if (skip_ready && !lt.author_classes.IsEmpty() &&
		!lt.author_specialities.IsEmpty() &&
		!lt.profit_reasons.IsEmpty() &&
		!lt.organizational_reasons.IsEmpty()) {
		NextBatch();
		return;
	}
	
	ProcessAnalyzeFn(8, THISBACK(OnProcessTemplateAnalyze));
}

void LeadSolver::OnProcessTemplateAnalyze(String res) {
	MetaDatabase& mdb = MetaDatabase::Single();
	LeadDataTemplate& ldt = LeadDataTemplate::Single();
	LeadTemplate& lt = ldt.templates[batch];
	
	/*
	- Speciality of the listing's author in short (music genre speciality, clients speciality): The listing author specializes in sourcing and selecting songs for film soundtracks and has a particular interest in Country, Folk, and Bluegrass genres. They also have a keen understanding of the themes and musical cues most suitable for this specific project.
	- Class of the listing's author (e.g. publisher / A&R / licensing agent etc. ) with 1-3 words: Music Licensing Agent
	- Profit reasons for the author of this listing:
	1. To earn a percentage of the profits from the eventual soundtrack sales.
	2. To establish relationships with talented artists and potentially work with them on future projects.
	3. To showcase their expertise in song selection and music curation within the film industry.
	- Positive organizational reasons for the author of this listing:
	1. To support and promote independent artists by featuring their music in a major film release.
	2. To create a diverse and well-curated soundtrack that enhances the overall viewing experience.
	3. To build a strong relationship with the film producers and establish a reputation for providing quality music choices.
	*/
	int a, b;
	String speciality, classes, profits, orgs;
	
	a = res.Find("- Speciality of the listing's author in short");
	if (a >= 0) {
		a = res.Find(":", a);
		if (a >= 0) {
			a++;
			b = res.Find("\n", a);
			if (b >= 0)
				speciality = TrimBoth(res.Mid(a, b-a));
		}
	}
	
	a = res.Find("- Class of the listing");
	if (a >= 0) {
		a = res.Find(":", a);
		if (a >= 0) {
			a++;
			b = res.Find("\n", a);
			if (b >= 0)
				classes = TrimBoth(res.Mid(a, b-a));
		}
	}
	
	a = res.Find("- Profit reasons");
	if (a >= 0) {
		a = res.Find(":", a);
		if (a >= 0) {
			a++;
			b = res.Find("- Positive organi", a);
			if (b >= 0)
				profits = TrimBoth(res.Mid(a, b-a));
		}
	}
	
	a = res.Find("- Positive organizational");
	if (a >= 0) {
		a = res.Find(":", a);
		if (a >= 0) {
			a++;
			b = res.GetCount();
			if (b >= 0)
				orgs = TrimBoth(res.Mid(a, b-a));
		}
	}
	
	if (speciality.GetCount() && classes.GetCount() && profits.GetCount() && orgs.GetCount()) {
		lt.author_specialities.Clear();
		lt.author_classes.Clear();
		lt.profit_reasons.Clear();
		lt.organizational_reasons.Clear();
		
		lt.author_specialities.FindAdd(ldt.author_specialities.FindAdd(speciality));
		lt.author_classes.FindAdd(ldt.author_classes.FindAdd(classes));
		
		RemoveEmptyLines2(profits);
		RemoveEmptyLines2(orgs);
		Vector<String> profit_reasons = Split(profits, "\n");
		Vector<String> organizational_reasons = Split(orgs, "\n");
		for (String& s : profit_reasons)
			lt.profit_reasons.FindAdd(ldt.profit_reasons.FindAdd(s));
		for (String& s : organizational_reasons)
			lt.organizational_reasons.FindAdd(ldt.organizational_reasons.FindAdd(s));
	}
	
	NextBatch();
	SetWaiting(0);
}




END_TEXTLIB_NAMESPACE

