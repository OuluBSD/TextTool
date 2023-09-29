#ifndef _SongTool_ChecklistProduction_h_
#define _SongTool_ChecklistProduction_h_


#define CHK_COMPOSITION_LIST \
	CHK_COMP(15,	CHORDS, t_("chords")) \
	CHK_COMP(5,		CHORD_PAD, t_("chord pad")) \
	CHK_COMP(15,	VOCAL_MELODY, t_("vocal melody")) \
	CHK_COMP(15,	VOCAL_MELODY_2ND, t_("2nd vocal melody")) \
	CHK_COMP(5,		DRUMKIT, t_("drumkit")) \
	CHK_COMP(5,		HAND_DRUMS, t_("hand drums")) \
	CHK_COMP(10,	LOW_RHYTHM, t_("low rhythm (bass)")) \
	CHK_COMP(10,	MID_RHYTHM, t_("mid rhythm (low guitar, e.g. strumming)")) \
	CHK_COMP(10,	HIGH_RHYTHM, t_("high rhythm (e.g. high muted guitar)")) \
	CHK_COMP(10,	LOW_SOLO, t_("low solo (bass solo)")) \
	CHK_COMP(10,	MID_SOLO, t_("mid solo (instrumental theme)")) \
	CHK_COMP(20,	HIGH_SOLO, t_("high solo (e.g. bridge climax solo)")) \
	CHK_COMP(10,	MID_SOLO_FILLS, t_("mid fills (e.g. between singing)")) \
	CHK_COMP(5,		IMPACTS, t_("impacts")) \
	CHK_COMP(5,		TEXTURE_PAD, t_("texture pad (arp)"))

class ChecklistComposition : public ChecklistBase {
	
	enum {
		#define CHK_COMP(time, e, s) e,
		CHK_COMPOSITION_LIST
		#undef CHK_COMP
		ITEM_LIST
	};
	
public:
	typedef ChecklistComposition CLASSNAME;
	ChecklistComposition();
	
	void InitKeys() override;
	
};



#define CHK_PRODUCTION_LIST \
	CHK_PRODUCTION(10,		FINISH_ENGLISH_VOCAL_STRUCTURE, t_("Search for a good reference song and convert it to a structure")) \
	CHK_PRODUCTION(5,		FINISH_WHAT_TO_SAY, t_("Decide what you want to say in lyrics")) \
	CHK_PRODUCTION(10,		FINISH_FINISH_ENGLISH_STRUCTURE, t_("Finish lyrics structure in english")) \
	CHK_PRODUCTION(30,		FINISH_FINISH_NATIVE_DRAFT, t_("Finish first draft in native language")) \
	CHK_PRODUCTION(30,		FINISH_FINISH_NATIVE, t_("Fix remaing problems in native language and in direct translation")) \
	CHK_PRODUCTION(30,		FINISH_FINISN_ENGLISH_DRAFT, t_("Finish first draft in english")) \
	CHK_PRODUCTION(30,		FINISH_FINISH_ENGLISH, t_("Fix remaing problems in english")) \
	CHK_PRODUCTION(1,		FINISH_RHYME_TEXT, t_("Make rhyme notation text of lyrics if it's rap")) \
	CHK_PRODUCTION(1,		EXPORT_MIDI, t_("Export composition to MIDI with bpm in name")) \
	CHK_PRODUCTION(3,		MAKE_PROJECT, t_("Make project from DAW project template and MIDI file")) \
	CHK_PRODUCTION(2,		MAKE_CHORD_PAD, t_("Make chord pad track with good sound presets")) \
	CHK_PRODUCTION(20,		CREATE_DRUMKIT_TRACK, t_("Create final drumkit-track using draft MIDI as reference")) \
	CHK_PRODUCTION(10,		CREATE_HAND_DRUMS_TRACK, t_("Create hand drums track")) \
	CHK_PRODUCTION(5,		RECORD_HIGH_RHYTHM, t_("Record high rhythm track (e.g. high muted guitar)")) \
	CHK_PRODUCTION(10,		RECORD_LOW_RHYTHM, t_("Record low rhythm track (bass)")) \
	CHK_PRODUCTION(20,		RECORD_MID_RHYTHM, t_("Record mid rhythm (low guitar, e.g. strumming)")) \
	CHK_PRODUCTION(5,		RECORD_MID_SOLO, t_("Record mid solo (instrumental theme)")) \
	CHK_PRODUCTION(5,		RECORD_LOW_SOLO, t_("Record low solo (bass solo)")) \
	CHK_PRODUCTION(10,		RECORD_HIGH_SOLO, t_("Record high solo (e.g. bridge climax solo)")) \
	CHK_PRODUCTION(5,		RECORD_MID_FILLS, t_("Record mid fills (e.g. between singing)")) \
	CHK_PRODUCTION(60,		FIX_TIMING_OF_RECORDINGS, t_("Fix the timing of recorded instruments with the Free Warp tool")) \
	CHK_PRODUCTION(5,		MAKE_TEXTURE_PAD, t_("Make texture pad track (arp)")) \
	CHK_PRODUCTION(5,		MAKE_IMPACT_TRACK, t_("Make impact track")) \
	CHK_PRODUCTION(5,		MAKE_TRANSITION_TRACK, t_("Make transition track (e.g. noise filter sweep, crash swells)")) \
	CHK_PRODUCTION(5,		MAKE_SOUND_FX_TRACK, t_("Make sound effect track (e.g. storyline, environment background, vehicles)")) \
	CHK_PRODUCTION(10,		MAKE_PRONOUNCIATION_TRACK_TTS, t_("Make pronunciation track using text to speech synthesis")) \
	CHK_PRODUCTION(10,		RECORD_PRONOUNCIATION_TRACK, t_("Record pronunciation track using own voice in correct tempo, but without pitch")) \
	CHK_PRODUCTION(5,		EXPORT_BACKGROUND, t_("Export background music track for vocal recording")) \
	CHK_PRODUCTION(3,		EXPORT_PRONOUNCIATION_TRACK, t_("Export pronounciation track for vocal recording")) \
	CHK_PRODUCTION(60,		RECORD_VOCALS, t_("Record the vocal track one short phrase at a time")) \
	CHK_PRODUCTION(30,		COMPILE_VOCALS, t_("Put together a vocal track from the best phrases")) \
	CHK_PRODUCTION(30,		FIX_VOCAL_TIMING, t_("Correct the timing of vocals with the free-warp tool")) \
	CHK_PRODUCTION(30,		FIX_VOCAL_PITCH, t_("Fix the pitch of the vocal track with the VariAudio tool")) \
	CHK_PRODUCTION(5,		VOCAL_FX, t_("Vocal track effects with iZotope Nectar automatic analysis")) \
	CHK_PRODUCTION(5,		BASS_FX, t_("Bass and kick harmonics creation with e.g. SyncerSoft Bass Landscapes")) \
	CHK_PRODUCTION(5,		INSTRUMENT_FX, t_("Instrument track effects with iZotope Neutron automatic analysis")) \
	CHK_PRODUCTION(5,		EQ_SPECTRAL_BALANCE, t_("EQ spectral balance for all tracks with SmartEQ grouping")) \
	CHK_PRODUCTION(3,		MASTER_FX, t_("Create master track effects with iZotope Ozone analysis")) \
	CHK_PRODUCTION(5,		EXPORT_AUDIO, t_("Export audio with the full name and a version number")) \
	//CHK_PRODUCTION(, "") \

class ChecklistProduction : public ChecklistBase {
	
	enum {
		#define CHK_PRODUCTION(t, e, s) e,
		CHK_PRODUCTION_LIST
		#undef CHK_PRODUCTION
		ITEM_LIST
	};
	
public:
	typedef ChecklistProduction CLASSNAME;
	ChecklistProduction();
	
	void InitKeys() override;
	
};


#define CHK_MUSICVID_LIST \
	CHK_MUSICVID(CREATE_PROJECT, t_("Create project file")) \
	CHK_MUSICVID(RECORD_REFERENCE_WEBCAM, t_("Record webcam-quality reference video for the song")) \
	CHK_MUSICVID(ADD_AI_IMAGES, t_("Add AI generated images to the timeline")) \
	CHK_MUSICVID(RECORD_A1, t_("Record A-Roll 1 footage (e.g. singer watching to the camera)")) \
	CHK_MUSICVID(RECORD_A2, t_("Record A-Roll 2 footage (e.g. singer from side)")) \
	CHK_MUSICVID(RECORD_A3, t_("Record A-Roll 3 footage (e.g. the whole band, guitar player only)")) \
	CHK_MUSICVID(RECORD_B1, t_("Record B-Roll 1 footage (e.g. immediate environment)")) \
	CHK_MUSICVID(RECORD_B2, t_("Record B-Roll 2 footage (e.g. outside the house environment)")) \
	CHK_MUSICVID(RECORD_B3, t_("Record B-Roll 3 footage (e.g. local area)")) \
	CHK_MUSICVID(SPLIT_USEFUL_PARTS, t_("Split useful parts of the footage")) \
	CHK_MUSICVID(STACK_SAME_SECTIONS, t_("Stack takes of the same sections")) \
	CHK_MUSICVID(COMPILE_BEST_OF_ROLL_FOOTAGE, t_("Compile best-of-roll footage track of footage tracks")) \
	CHK_MUSICVID(COMPILE_SINGLE_BEST_OF_FOOTAGE, t_("Compile the single best-of-footage track")) \
	CHK_MUSICVID(ADD_TEXTS, t_("Add texts")) \
	CHK_MUSICVID(ADJUST_COLORS, t_("Adjust colors")) \
	CHK_MUSICVID(ADD_EFFECTS, t_("Add effects")) \
	//CHK_MUSICVID(, t_("")) \

class ChecklistMusicVideoProduction : public ChecklistBase {
	
	enum {
		#define CHK_MUSICVID(e, s) e,
		CHK_MUSICVID_LIST
		#undef CHK_MUSICVID
		ITEM_LIST
	};
	
public:
	typedef ChecklistMusicVideoProduction CLASSNAME;
	ChecklistMusicVideoProduction();
	
	void InitKeys() override;
	
};




#define CHK_RELEASE_LIST \
	CHK_RELEASE(ORDER_OF_SONGS, t_("Decide final order of songs")) \
	CHK_RELEASE(FINISH_COVER_ART, t_("Finish cover art")) \
	CHK_RELEASE(FINISH_PITCHING_TEXT, t_("Finish pitching text for playlists")) \
	CHK_RELEASE(SEND_TO_DISTROKID, t_("Send to DistroKid")) \
	CHK_RELEASE(SEND_TO_GRAMEX, t_("Send to Gramex")) \
	CHK_RELEASE(SEND_TO_TEOSTO, t_("Send to Teosto")) \
	CHK_RELEASE(PITCH_IN_SPOTIFY, t_("Pitch your song at Spotify for Artists page")) \
	//CHK_RELEASE(, t_("")) \

class ChecklistRelease : public ChecklistBase {
	
	enum {
		#define CHK_RELEASE(e, s) e,
		CHK_RELEASE_LIST
		#undef CHK_RELEASE
		ITEM_LIST
	};
	
public:
	typedef ChecklistRelease CLASSNAME;
	ChecklistRelease();
	
	void InitKeys() override;
	
};




#define CHK_SOME_LIST \
	CHK_SOME(NATIVE_BLOG_UPDATE, t_("Native blog update")) \
	CHK_SOME(ENGLISH_BLOG_UPDATE, t_("English blog update")) \
	CHK_SOME(WEBSITE_UPDATE, t_("Website update")) \
	CHK_SOME(HOST_LISTENING_SESSION, t_("Host listening session and document it")) \
	CHK_SOME(PREMARKETING_FACEBOOK, t_("Pre-marketing in Facebook")) \
	CHK_SOME(PREMARKETING_INSTAGRAM, t_("Pre-marketing in Instagram")) \
	CHK_SOME(PREMARKETING_TWITTER, t_("Pre-marketing in Twitter")) \
	CHK_SOME(PREMARKETING_TIKTOK, t_("Pre-marketing in Tiktok")) \
	CHK_SOME(POSTMARKETING_FACEBOOK, t_("Post-marketing in Facebook")) \
	CHK_SOME(POSTMARKETING_INSTAGRAM, t_("Post-marketing in Instagram")) \
	CHK_SOME(POSTMARKETING_TWITTER, t_("Post-marketing in Twitter")) \
	CHK_SOME(POSTMARKETING_TIKTOK, t_("Post-marketing in Tiktok")) \
	//CHK_RELEASE(, t_("")) \

class SocialMediaChecklist : public ChecklistBase {
	
	enum {
		#define CHK_SOME(e, s) e,
		CHK_SOME_LIST
		#undef CHK_SOME
		ITEM_LIST
	};
	
public:
	typedef SocialMediaChecklist CLASSNAME;
	SocialMediaChecklist();
	
	void InitKeys() override;
	
};



#endif
