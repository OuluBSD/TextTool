#ifndef _Task_TaskCommon_h_
#define _Task_TaskCommon_h_

typedef enum : int {
	V_ATTR_SCORING,
	V_PTR_PIPE,
	V_PTR_LINE,
	//V_PTR_PIPE_UNIQUELINES,
	V_MODE,
	V_DIR,
	V_ARGS,
	V_SONG_LYRICS,
	V_SONG_PARTS,
	V_LINE_TXT,
	V_HUMAN_INPUT_LINE_TXT,
	V_ATTRS,
	V_TXT,
	V_MALE_FEMALE_MASK,
} TaskArgType;

typedef enum : int {
	O_NEVER,
	
	O_ORDER_IMPORT,
	O_ORDER_IMPORT_DETAILED,
	O_ORDER_REVERSE,
	
	O_DB_ATTRS,
	O_DB_ATTR_SCORES,
	
	O_TASKS,
	
	O_SONG_MASK,
	//O_SONG_MASK_WEIGHTED,
	O_SONG_ANALYSIS,
	O_SONG_DATA_STORYLINE,
	//O_SONG_DATA_STORYLINE_WEIGHTED,
	O_SONG_UNIQLINE_ATTRS,
	O_SONG_SNAP,
	//O_SONG_SNAP_WEIGHTED,
	O_SONG_REVERSED_MASK_COMMON,
	O_SONG_REVERSED_MASK,
	O_SONG_REVERSED_LYRICS,
	O_SONG_REVERSED_TRANSLATED_LYRICS,
	
	O_PART_MASK,
	//O_PART_MASK_WEIGHTED,
	O_PART_MASK_SCORE,
	O_PART_DATA_STORYLINE,
	//O_PART_DATA_STORYLINE_WEIGHTED,
	O_PART_SNAP,
	O_PART_SNAP_SCORE,
	//O_PART_SNAP_WEIGHTED,
	O_PART_REVERSED_SNAP,
	
	O_LINE_SNAP,
	O_LINE_SNAP_SCORE,
	//O_LINE_SNAP_WEIGHTED,
	O_LINE_REVERSED_SNAP,
	
	O_BREAK_SNAP,
	O_BREAK_SNAP_SCORE,
	//O_BREAK_SNAP_WEIGHTED,
	O_BREAK_IMPACT,
	O_BREAK_IMPACT_SCORES,
	O_BREAK_REVERSED_IMPACT,
	O_BREAK_REVERSED_SNAP,
	//O_BREAK_IMPACT_WEIGHTED,
	O_BREAK_LYRICS,
	//O_BREAK_LYRICS_WEIGHTED,
	
	O_NEXT_CTX_JUMP, // allows next context to start being processed
	
} TaskOutputType;

inline bool IsTaskSkippingContext(TaskOutputType tt) {
	switch (tt) {
		case O_ORDER_IMPORT:
		case O_ORDER_IMPORT_DETAILED:
		case O_ORDER_REVERSE:
		case O_NEXT_CTX_JUMP:
			return true;
		default:
			return false;
	}
}

typedef enum : int {
	TASK_TRANSLATE,
	TASK_TRANSLATE_SONG_DATA,
	TASK_UNPACK_STRUCTURE_SONG_DATA,
	TASK_CHECK_ERRORS_IN_SONG_STRUCT_DATA,
	TASK_CHECK_ERRORS_IN_SONG_NL_DATA,
	TASK_CHECK_ERRORS_IN_SCREENPLAY_STRUCT_DATA,
	TASK_CONVERT_SONG_STRUCTURE_TO_ENGLISH,
	TASK_EVALUATE_SONG_AUDIENCE,
	TASK_MAKE_SONG_POETIC,
	TASK_EVALUATE_POETIC_STYLES,
	TASK_MORPH_TO_ATTRIBUTES,
	TASK_CONVERT_SCREENPLAY_TO_STRUCTURE,
	TASK_CONVERT_STRUCTURE_TO_SCREENPLAY,
	TASK_CONVERT_SCREENPLAY_TO_PLAN,
	TASK_CREATE_IMAGE,
	TASK_EDIT_IMAGE,
	TASK_VARIATE_IMAGE,
	TASK_RAW_COMPLETION,
	TASK_EVALUATE_SUGGESTION_SCORES,
	TASK_EVALUATE_SUGGESTION_ORDER,
	TASK_IMPROVE_SOURCE_TEXT,
	TASK_LIMIT_SYLLABLE_COUNT,
	TASK_GET_AI_ATTRIBUTES,
	TASK_GET_STRUCTURE_SUGGESTIONS,
	TASK_GET_SUGGESTION_ATTRIBUTES,
	TASK_GET_NOVEL_THEMES,
	TASK_GET_NOVEL_IDEAS,
	TASK_GET_TONE_SUGGESTIONS,
	TASK_GET_CONTENT_SUGGESTIONS,
	TASK_GET_ALLEGORICAL_DEVICES,
	
	TASK_IMPORT_AND_REVERSE,
	TASK_CONTEXT_IMPORT_AND_REVERSE,
	
	TASK_PATTERNMASK,
	TASK_PATTERNMASK_WEIGHTED,
	TASK_ANALYSIS,
	TASK_STORYARC,
	TASK_STORYARC_WEIGHTED,
	TASK_IMPACT,
	TASK_IMPACT_WEIGHTED,
	TASK_FORWARD_LYRICS_WEIGHTED,
	
	TASK_MAKE_IMPACT_SCORING_TASKS,
	TASK_IMPACT_SCORING,
	
	TASK_MAKE_PATTERN_TASKS,
	TASK_PATTERN,
	TASK_PATTERN_WEIGHTED,
	
	TASK_MAKE_ATTRSCORES_TASKS,
	TASK_ATTRSCORES,
	TASK_ATTRSCORES_READY,
	TASK_SONGSCORE,
	
	TASK_MAKE_REVERSE_IMPACT_TASK,
	TASK_REVERSE_IMPACT,
	
	TASK_MAKE_REVERSE_MASK_TASK,
	TASK_REVERSE_COMMON_MASK,
	TASK_REVERSE_SEPARATE_MASK,
	
	TASK_MAKE_REVERSEPATTERN_TASK,
	TASK_REVERSEPATTERN,
	
	TASK_MAKE_LYRICS_TASK,
	TASK_LYRICS,
	TASK_LYRICS_TRANSLATE,
	
	TASK_COUNT
} TaskType;

NAMESPACE_UPP
NTL_MOVEABLE(TaskArgType)
NTL_MOVEABLE(TaskOutputType)
NTL_MOVEABLE(TaskType)
END_UPP_NAMESPACE

#endif
