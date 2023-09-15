#ifndef _SongTool_ChecklistProduction_h_
#define _SongTool_ChecklistProduction_h_


#define CHK_COMPOSITION_LIST \
	CHK_COMP(CHORD_PAD, t_("chord pad")) \
	CHK_COMP(VOCAL_MELODY, t_("vocal melody")) \
	CHK_COMP(DRUMKIT, t_("drumkit")) \
	CHK_COMP(HAND_DRUMS, t_("hand drums")) \
	CHK_COMP(LOW_RHYTHM, t_("low rhythm (bass)")) \
	CHK_COMP(MID_RHYTHM, t_("mid rhythm (low guitar, e.g. strumming)")) \
	CHK_COMP(HIGH_RHYTHM, t_("high rhythm (e.g. high muted guitar)")) \
	CHK_COMP(LOW_SOLO, t_("low solo (bass solo)")) \
	CHK_COMP(MID_SOLO, t_("mid solo (instrumental theme)")) \
	CHK_COMP(HIGH_SOLO, t_("high solo (e.g. bridge climax solo)")) \
	CHK_COMP(MID_SOLO_FILLS, t_("mid fills (e.g. between singing)")) \
	CHK_COMP(IMPACTS, t_("impacts")) \
	CHK_COMP(TEXTURE_PAD, t_("texture pad (arp)"))

class ChecklistComposition : public ChecklistBase {
	
	enum {
		#define CHK_COMP(e, s) e,
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
	CHK_PRODUCTION(FINISH_VOCAL, t_("Finish lyrics with at least easy singing and guitar strumming")) \
	CHK_PRODUCTION(FINISH_COMPOSITION, t_("Finish composition with at least chord track, simple drums, structure")) \
	CHK_PRODUCTION(FINISH_2ND_VOCAL, t_("Finish vocal notes in composition, if song has 2+ simultaneous vocal melody")) \
	CHK_PRODUCTION(EXPORT_MIDI, t_("Export composition to MIDI with bpm in name")) \
	CHK_PRODUCTION(MAKE_PROJECT, t_("Make project from DAW project template and MIDI file")) \
	CHK_PRODUCTION(MAKE_CHORD_PAD, t_("Make chord pad track with good sound presets")) \
	CHK_PRODUCTION(CREATE_DRUMKIT_TRACK, t_("Create final drumkit-track using draft MIDI as reference")) \
	CHK_PRODUCTION(CREATE_HAND_DRUMS_TRACK, t_("Create hand drums track")) \
	CHK_PRODUCTION(RECORD_HIGH_RHYTHM, t_("Record high rhythm track (e.g. high muted guitar)")) \
	CHK_PRODUCTION(RECORD_LOW_RHYTHM, t_("Record low rhythm track (bass)")) \
	CHK_PRODUCTION(RECORD_MID_RHYTHM, t_("Record mid rhythm (low guitar, e.g. strumming)")) \
	CHK_PRODUCTION(RECORD_MID_SOLO, t_("Record mid solo (instrumental theme)")) \
	CHK_PRODUCTION(RECORD_LOW_SOLO, t_("Record low solo (bass solo)")) \
	CHK_PRODUCTION(RECORD_HIGH_SOLO, t_("Record high solo (e.g. bridge climax solo)")) \
	CHK_PRODUCTION(RECORD_MID_FILLS, t_("Record mid fills (e.g. between singing)")) \
	CHK_PRODUCTION(FIX_TIMING_OF_RECORDINGS, t_("Fix the timing of recorded instruments with the Free Warp tool")) \
	CHK_PRODUCTION(MAKE_TEXTURE_PAD, t_("Make texture pad track (arp)")) \
	CHK_PRODUCTION(MAKE_IMPACT_TRACK, t_("Make impact track")) \
	CHK_PRODUCTION(MAKE_TRANSITION_TRACK, t_("Make transition track (e.g. noise filter sweep, crash swells)")) \
	CHK_PRODUCTION(MAKE_SOUND_FX_TRACK, t_("Make sound effect track (e.g. storyline, environment background, vehicles)")) \
	CHK_PRODUCTION(MAKE_PRONOUNCIATION_TRACK_TTS, t_("Make pronunciation track using text to speech synthesis")) \
	CHK_PRODUCTION(RECORD_PRONOUNCIATION_TRACK, t_("Record pronunciation track using own voice in correct tempo, but without pitch")) \
	CHK_PRODUCTION(EXPORT_BACKGROUND, t_("Export background music track for vocal recording")) \
	CHK_PRODUCTION(EXPORT_PRONOUNCIATION_TRACK, t_("Export pronounciation track for vocal recording")) \
	CHK_PRODUCTION(RECORD_VOCALS, t_("Record the vocal track one short phrase at a time")) \
	CHK_PRODUCTION(COMPILE_VOCALS, t_("Put together a vocal track from the best phrases")) \
	CHK_PRODUCTION(FIX_VOCAL_TIMING, t_("Correct the timing of vocals with the free-warp tool")) \
	CHK_PRODUCTION(FIX_VOCAL_PITCH, t_("Fix the pitch of the vocal track with the VariAudio tool")) \
	CHK_PRODUCTION(VOCAL_FX, t_("Vocal track effects with iZotope Nectar automatic analysis")) \
	CHK_PRODUCTION(INSTRUMENT_FX, t_("Instrument track effects with iZotope Neutron automatic analysis")) \
	CHK_PRODUCTION(EQ_SPECTRAL_BALANCE, t_("EQ spectral balance for all tracks with SmartEQ grouping")) \
	CHK_PRODUCTION(MASTER_FX, t_("Create master track effects with iZotope Ozone analysis")) \
	CHK_PRODUCTION(EXPORT_AUDIO, t_("Export audio with the full name and a version number")) \
	//CHK_PRODUCTION(, "") \

class ChecklistProduction : public ChecklistBase {
	
	enum {
		#define CHK_PRODUCTION(e, s) e,
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
		#undef CHK_COMP
		ITEM_LIST
	};
	
public:
	typedef ChecklistMusicVideoProduction CLASSNAME;
	ChecklistMusicVideoProduction();
	
	void InitKeys() override;
	
};




#define CHK_RELEASE_LIST \
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



#endif
