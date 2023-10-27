#include "Task.h"
#include <Database/Database.h>


void TaskMgrConfig::CreateDefaultTaskRules() {
	
	AddRule(TASK_TRANSLATE, "translate")
		.Input(&Task::CreateInput_Translate)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 3, 3)
		.Process(&Task::Process_Translate)
		;
	AddRule(TASK_TRANSLATE_SONG_DATA, "translate song data")
		.Input(&Task::CreateInput_TranslateSongData)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 4, 4)
		.Process(&Task::Process_TranslateSongData)
		;
	
	AddRule(TASK_UNPACK_STRUCTURE_SONG_DATA, "translate song data")
		.Input(&Task::CreateInput_UnpackStructureSongData)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_UnpackStructureSongData)
		;
	
	AddRule(TASK_CHECK_ERRORS_IN_SONG_STRUCT_DATA, "check errors in structural song data")
		.Input(&Task::CreateInput_CheckSongStructureErrors)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_CheckSongStructureErrors)
		;
	
	AddRule(TASK_CHECK_ERRORS_IN_SONG_NL_DATA, "check errors in natural language song data")
		.Input(&Task::CreateInput_CheckSongNaturalErrors)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_CheckSongNaturalErrors)
		;
	
	AddRule(TASK_CHECK_ERRORS_IN_SCREENPLAY_STRUCT_DATA, "check errors in structural screenplay data")
		.Input(&Task::CreateInput_CheckScreenplayStructureErrors)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_CheckScreenplayStructureErrors)
		;
	
	AddRule(TASK_RAW_COMPLETION, "raw prompt completion")
			.Arg(V_PTR_PIPE)
		.Process(&Task::Process_RawCompletion)
		;
	
	AddRule(TASK_EVALUATE_SUGGESTION_SCORES, "evaluate suggestion scores")
			.Arg(V_PTR_PIPE)
		.Input(&Task::CreateInput_EvaluateSuggestionScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 100)
		.Process(&Task::Process_EvaluateSuggestionScores)
		;
	
	AddRule(TASK_EVALUATE_SUGGESTION_ORDER, "evaluate suggestion order")
			.Arg(V_PTR_PIPE)
		.Input(&Task::CreateInput_EvaluateSuggestionOrder)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 100)
		.Process(&Task::Process_EvaluateSuggestionOrder)
		;
	
	AddRule(TASK_IMPROVE_SOURCE_TEXT, "improve source text")
			.Arg(V_PTR_PIPE)
		.Input(&Task::CreateInput_ImproveSourceText)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 100)
		.Process(&Task::Process_ImproveSourceText)
		;
	
	AddRule(TASK_LIMIT_SYLLABLE_COUNT, "limit syllable count")
			.Arg(V_PTR_PIPE)
		.Input(&Task::CreateInput_LimitSyllableCount)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 100)
		.Process(&Task::Process_LimitSyllableCount)
		;
	
	AddRule(TASK_CONVERT_SONG_STRUCTURE_TO_ENGLISH, "convert song structure to english")
		.Input(&Task::CreateInput_ConvertSongStructureToEnglish)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_ConvertSongStructureToEnglish)
		;
	
	AddRule(TASK_EVALUATE_SONG_AUDIENCE, "evaluate song audience")
		.Input(&Task::CreateInput_EvaluateSongAudience)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 4, 20)
		.Process(&Task::Process_EvaluateSongAudience)
		;
	
	AddRule(TASK_MAKE_SONG_POETIC, "make more poetic lyrics")
		.Input(&Task::CreateInput_MakePoetic)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 3, 3)
		.Process(&Task::Process_MakePoetic)
		;
	
	AddRule(TASK_EVALUATE_POETIC_STYLES, "evaluate poetic styles")
		.Input(&Task::CreateInput_EvaluatePoeticStyles)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_EvaluatePoeticStyles)
		;
	
	AddRule(TASK_MORPH_TO_ATTRIBUTES, "morph to attributes")
		.Input(&Task::CreateInput_MorphToAttributes)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_MorphToAttributes)
		;
	
	AddRule(TASK_GET_AI_ATTRIBUTES, "get ai attributes")
		.Input(&Task::CreateInput_GetAIAttributes)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_GetAIAttributes)
		;
	
	AddRule(TASK_GET_STRUCTURE_SUGGESTIONS, "get structure suggestions")
		.Input(&Task::CreateInput_GetStructureSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 4, 4)
		.Process(&Task::Process_GetStructureSuggestions)
		;
	
	AddRule(TASK_GET_SUGGESTION_ATTRIBUTES, "get suggestion attributes")
		.Input(&Task::CreateInput_GetSuggestionAttributes)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 100)
		.Process(&Task::Process_GetSuggestionAttributes)
		;
	
	AddRule(TASK_GET_NOVEL_THEMES, "get novel themes")
		.Input(&Task::CreateInput_GetNovelThemes)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 100)
		.Process(&Task::Process_GetNovelThemes)
		;
	
	AddRule(TASK_GET_NOVEL_IDEAS, "get novel ideas")
		.Input(&Task::CreateInput_GetNovelIdeas)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 100)
		.Process(&Task::Process_GetNovelIdeas)
		;
	
	AddRule(TASK_GET_TONE_SUGGESTIONS, "get tone suggestions")
		.Input(&Task::CreateInput_GetToneSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 3, 100)
		.Process(&Task::Process_GetToneSuggestions)
		;
	
	AddRule(TASK_GET_CONTENT_SUGGESTIONS, "get content suggestions")
		.Input(&Task::CreateInput_GetContentSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 4, 100)
		.Process(&Task::Process_GetContentSuggestions)
		;
	
	AddRule(TASK_GET_ALLEGORICAL_DEVICES, "get allegorical devices")
		.Input(&Task::CreateInput_GetAllegorySuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 4, 100)
		.Process(&Task::Process_GetAllegorySuggestions)
		;
	
	AddRule(TASK_GET_IMAGERY_SUGGESTIONS, "get imagery suggestions")
		.Input(&Task::CreateInput_GetImagerySuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 5, 100)
		.Process(&Task::Process_GetImagerySuggestions)
		;
	
	AddRule(TASK_GET_SYMBOLISM_SUGGESTIONS, "get symbolism suggestions")
		.Input(&Task::CreateInput_GetSymbolismSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 6, 100)
		.Process(&Task::Process_GetSymbolismSuggestions)
		;
	
	AddRule(TASK_GET_IDEA_SUGGESTIONS, "get idea suggestions")
		.Input(&Task::CreateInput_GetIdeaSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetIdeaSuggestions)
		;
	
	AddRule(TASK_GET_PART_CONTENT_SUGGESTIONS, "get part content suggestions")
		.Input(&Task::CreateInput_GetPartContentSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 6, 100)
		.Process(&Task::Process_GetPartContentSuggestions)
		;
	
	AddRule(TASK_GET_PART_IMAGERY_SUGGESTIONS, "get part imagery suggestions")
		.Input(&Task::CreateInput_GetPartImagerySuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 7, 100)
		.Process(&Task::Process_GetPartImagerySuggestions)
		;
	
	AddRule(TASK_GET_PART_SYMBOLISM_SUGGESTIONS, "get part symbolism suggestions")
		.Input(&Task::CreateInput_GetPartSymbolismSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartSymbolismSuggestions)
		;
	
	AddRule(TASK_GET_PART_IDEA, "get part idea suggestions")
		.Input(&Task::CreateInput_GetPartIdea)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartIdea)
		;
	
	AddRule(TASK_CONVERT_STRUCTURE_TO_SCREENPLAY, "convert structure to screenplay")
		.Input(&Task::CreateInput_ConvertStructureToScreenplay)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_ConvertStructureToScreenplay)
		;
	
	AddRule(TASK_CONVERT_SCREENPLAY_TO_PLAN, "convert screenplay to production plan")
		.Input(&Task::CreateInput_ConvertScreenplayToPlan)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_ConvertScreenplayToPlan)
		;
	
	AddRule(TASK_CREATE_IMAGE, "create image")
		.ImageTask()
		.Input(&Task::CreateInput_CreateImage)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 3, 3)
		.Process(&Task::Process_CreateImage)
		;
	
	AddRule(TASK_EDIT_IMAGE, "edit image")
		.ImageTask()
		.ImageEditTask()
		.Input(&Task::CreateInput_EditImage)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_EditImage)
		;
	
	AddRule(TASK_VARIATE_IMAGE, "variate image")
		.ImageTask()
		.ImageVariateTask()
		.Input(&Task::CreateInput_VariateImage)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_VariateImage)
		;
	
	AddRule(TASK_GET_INTERNAL_RHYMING_FIRST_LINE, "get internal rhyming: first line")
		.Input(&Task::CreateInput_GetInternalRhymingFirstLine)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetInternalRhymingFirstLine)
		;
	
	AddRule(TASK_GET_INTERNAL_RHYMING_CONTINUE_LINE, "get internal rhyming: continue previous line")
		.Input(&Task::CreateInput_GetInternalRhymingContinueLine)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetInternalRhymingContinueLine)
		;
	
	AddRule(TASK_GET_IDEA_FROM_LYRICS, "get idea from lyrics")
		.Input(&Task::CreateInput_GetIdeaFromLyrics)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetIdeaFromLyrics)
		;
	
	AddRule(TASK_GET_ATTRIBUTES_FROM_LYRICS, "get attributes from lyrics")
		.Input(&Task::CreateInput_GetAttributesFromLyrics)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetAttributesFromLyrics)
		;
	
	AddRule(TASK_GET_PRODUCTION_IDEA, "get production idea")
		.Input(&Task::CreateInput_GetProductionIdea)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetProductionIdea)
		;
	
	AddRule(TASK_GET_STORY_CONTEXT, "get story context")
		.Input(&Task::CreateInput_GetStoryContext)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_GetStoryContext)
		;
	
	AddRule(TASK_GET_PART_CONTEXT, "get part context")
		.Input(&Task::CreateInput_GetPartContext)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartContext)
		;
	
	AddRule(TASK_GET_PART_VISUAL_IDEA_CONTEXT, "get part visual idea context")
		.Input(&Task::CreateInput_GetPartVisualIdeaContext)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartVisualIdeaContext)
		;
	
	AddRule(TASK_GET_PART_VISUAL_IDEA_CHARACTERS, "get part visual idea characters")
		.Input(&Task::CreateInput_GetPartVisualIdeaCharacters)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartVisualIdeaCharacters)
		;
	
	AddRule(TASK_GET_PART_DIALOGUE_IDEA, "get part dialogue idea")
		.Input(&Task::CreateInput_GetPartDialogueIdeaContext)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartDialogueIdeaContext)
		;
	
	AddRule(TASK_GET_PART_DIALOGUE_IDEA_STYLE, "get part dialogue idea style suggestions")
		.Input(&Task::CreateInput_GetPartDialogueIdeaStyleSuggestions)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetPartDialogueIdeaStyleSuggestions)
		;
	
	AddRule(TASK_GET_COLOR_IDEA, "get color idea")
		.Input(&Task::CreateInput_GetColorIdea)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetColorIdea)
		;
	
	AddRule(TASK_GET_VOCABULARY, "get vocabulary")
		.Input(&Task::CreateInput_GetVocabulary)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetVocabulary)
		;
	
	AddRule(TASK_GET_VOCABULARY_IDEA, "get vocabulary idea")
		.Input(&Task::CreateInput_GetVocabularyIdea)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetVocabularyIdea)
		;
	
	AddRule(TASK_GET_WORD_SALAD_IDEA, "get word salad idea")
		.Input(&Task::CreateInput_GetWordSaladIdea)
			.Arg(V_PTR_PIPE)
			.Arg(V_ARGS, 1, 1)
		.Process(&Task::Process_GetWordSaladIdea)
		;
	
	
	
	
	
	
	AddRule(TASK_IMPORT_AND_REVERSE, "import song and make reversed song")
		.Process(&Task::Process_MakeImportTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_LYRICS)
			.Result(O_TASKS)
			.Result(O_ORDER_IMPORT)
			.Result(O_ORDER_IMPORT_DETAILED)
			.CrossMode()
		;
	
	AddRule(TASK_CONTEXT_IMPORT_AND_REVERSE, "import context")
		.Require(O_ORDER_IMPORT)
		.Require(O_NEXT_CTX_JUMP)
		.Process(&Task::Process_MakeContextImportTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_LYRICS)
			.Result(O_ORDER_REVERSE)
			//.Result(O_SONG_UNIQLINES)
			.Result(O_TASKS)
			.CrossMode()
		;
	
	AddRule(TASK_PATTERNMASK, "pattern masks of parts of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_PatternMask)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_PatternMask)
			.Result(O_SONG_MASK)
			.Result(O_PART_MASK)
			.Result(O_DB_ATTRS)
		.Hash(V_ATTRS)
		.Hash(V_TXT)
		.SeparateItems()
		;
	
	AddRule(TASK_PATTERNMASK_WEIGHTED, "weighted pattern masks")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_SONG_MASK, MODE_BEGIN, FEMALE)
		.RequireMode(O_PART_MASK, MODE_BEGIN, FEMALE)
		.Require(O_BREAK_LYRICS) //_WEIGHTED)
		.Input(&Task::CreateInput_PatternMaskWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_PatternMaskWeighted)
			.Result(O_SONG_MASK) //_WEIGHTED)
			.Result(O_PART_MASK) //_WEIGHTED)
			.Result(O_DB_ATTRS)
		.Hash(V_MALE_FEMALE_MASK)
		.SeparateItems()
		;
	
	AddRule(TASK_ANALYSIS, "analysis of parts of song")
		.Require(O_ORDER_IMPORT_DETAILED)
		.Input(&Task::CreateInput_Analysis)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_Analysis)
			.Arg(V_ARGS, 1, INT_MAX)
			.Result(O_SONG_ANALYSIS)
		.Hash(V_TXT)
		;
	
	AddRule(TASK_STORYARC, "story arc of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_StoryArc)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_HUMAN_INPUT_LINE_TXT)
		.Process(&Task::Process_StoryArc)
			.Result(O_PART_DATA_STORYLINE)
			.Result(O_SONG_DATA_STORYLINE)
		.Hash(V_TXT)
		;
	
	AddRule(TASK_STORYARC_WEIGHTED, "story arc WEIGHTED mode")
		.RequireMode(O_PART_DATA_STORYLINE, MODE_BEGIN, FEMALE)
		.RequireMode(O_SONG_DATA_STORYLINE, MODE_BEGIN, FEMALE)
		.Input(&Task::CreateInput_StoryArcWeighted)
		.Process(&Task::Process_StoryArcWeighted)
			.Result(O_PART_DATA_STORYLINE) //_WEIGHTED)
			.Result(O_SONG_DATA_STORYLINE) //_WEIGHTED)
		;
	
	AddRule(TASK_IMPACT, "impacts")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_Impact)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_Impact)
			.Result(O_BREAK_IMPACT)
		;
	
	AddRule(TASK_IMPACT_WEIGHTED, "weighted impacts")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_BREAK_IMPACT, MODE_BEGIN, FEMALE)
		.Input(&Task::CreateInput_ImpactWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_ImpactWeighted)
			.Result(O_BREAK_IMPACT) //_WEIGHTED)
		;
	
	AddRule(TASK_FORWARD_LYRICS_WEIGHTED, "lyrics from weighted impacts")
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT) //_WEIGHTED)
		.Input(&Task::CreateInput_ForwardLyricsWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_ForwardLyricsWeighted)
			.Result(O_BREAK_LYRICS) //_WEIGHTED)
		;
	
	AddRule(TASK_MAKE_IMPACT_SCORING_TASKS, "make impact scoring tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.RequireMode(O_BREAK_IMPACT, WEIGHTED, WEIGHTED)
		.RequireMode(O_BREAK_LYRICS, WEIGHTED, WEIGHTED)
		.Process(&Task::Process_MakeImpactScoringTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_IMPACT_SCORING, "impact scoring")
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.RequireMode(O_BREAK_IMPACT, WEIGHTED, WEIGHTED)
		.Input(&Task::CreateInput_ImpactScoring)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_ImpactScoring)
			.Arg(V_ATTR_SCORING)
			.Result(O_BREAK_IMPACT_SCORES)
		;
	
	AddRule(TASK_MAKE_PATTERN_TASKS, "make pattern tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Process(&Task::Process_MakePatternTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			//.Arg(V_PTR_PIPE_UNIQUELINES)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_PATTERN, "pattern")
		.Require(O_ORDER_IMPORT)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		//.Require(O_SONG_UNIQLINES)
		.Input(&Task::CreateInput_Pattern)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
		.Process(&Task::Process_Pattern)
			.Result(O_SONG_UNIQLINE_ATTRS)
			//.Result(O_SONG_SNAP)
			.Result(O_PART_SNAP)
			.Result(O_LINE_SNAP)
			.Result(O_BREAK_SNAP)
			.Result(O_DB_ATTRS)
		;
	
	AddRule(TASK_PATTERN_WEIGHTED, "pattern weighted")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_PART_SNAP, MALE, FEMALE)
		.RequireMode(O_LINE_SNAP, MALE, FEMALE)
		.RequireMode(O_BREAK_SNAP, MALE, FEMALE)
		.Input(&Task::CreateInput_PatternWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, WEIGHTED, MODE_COUNT)
		.Process(&Task::Process_PatternWeighted)
			.Result(O_SONG_UNIQLINE_ATTRS)
			//.Result(O_SONG_SNAP)
			.Result(O_PART_SNAP)
			.Result(O_LINE_SNAP)
			.Result(O_BREAK_SNAP)
		;
	
	AddRule(TASK_MAKE_ATTRSCORES_TASKS, "make attribute score tasks")
		.MultiSpawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Require(O_SONG_UNIQLINE_ATTRS)
		.Process(&Task::Process_MakeAttrScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_ATTRSCORES, "attribute scores")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Input(&Task::CreateInput_AttrScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
		.Process(&Task::Process_AttrScores)
		;
	
	AddRule(TASK_ATTRSCORES_READY, "attribute scores ready")
		.Result(O_DB_ATTR_SCORES)
		.DebugInput()
		;
	
	AddRule(TASK_SONGSCORE, "song score")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTR_SCORES)
		.Require(O_PART_SNAP)
		.Require(O_LINE_SNAP)
		.Require(O_BREAK_SNAP)
		.Require(O_PART_SNAP)
		.Require(O_LINE_SNAP)
		.Require(O_BREAK_SNAP)
		.Process(&Task::Process_SongScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
			.Result(O_PART_MASK_SCORE)
			.Result(O_PART_SNAP_SCORE)
			.Result(O_LINE_SNAP_SCORE)
			.Result(O_BREAK_SNAP_SCORE)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSE_IMPACT_TASK, "make reverse impact tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_MakeReverseImpactTask)
			.Arg(V_MODE, 0, 1)
		.DebugInput()
			.Result(O_TASKS)
		;
	
	AddRule(TASK_REVERSE_IMPACT, "reverse impact")
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_ReverseImpact)
			.Arg(V_MODE, 0, 1)
			.Result(O_BREAK_REVERSED_IMPACT)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSE_MASK_TASK, "make reverse mask tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_MakeReverseMaskTask)
			.Arg(V_MODE, 0, 1)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_REVERSE_COMMON_MASK, "reverse common mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_ReverseCommonMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK_COMMON)
		.DebugInput()
		;
	
	AddRule(TASK_REVERSE_SEPARATE_MASK, "reverse separate mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Require(O_SONG_REVERSED_MASK_COMMON)
		.Process(&Task::Process_ReverseSeparateMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSEPATTERN_TASK, "make reverse pattern tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_SNAP_SCORE)
		.Require(O_LINE_SNAP_SCORE)
		.Require(O_BREAK_SNAP_SCORE)
		.Require(O_SONG_REVERSED_MASK)
		.Process(&Task::Process_MakeReversePattern)
			.Arg(V_MODE, 0, 1)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_REVERSEPATTERN, "reverse pattern")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_SNAP_SCORE)
		.Require(O_LINE_SNAP_SCORE)
		.Require(O_BREAK_SNAP_SCORE)
		.Require(O_SONG_REVERSED_MASK)
		.Process(&Task::Process_ReversePattern)
			.Arg(V_MODE, 0, 1)
			.Result(O_PART_REVERSED_SNAP)
			.Result(O_LINE_REVERSED_SNAP)
			.Result(O_BREAK_REVERSED_SNAP)
			.Result(O_NEXT_CTX_JUMP)
		.DebugInput()
		;
	
	
	AddRule(TASK_MAKE_LYRICS_TASK, "make reversed lyrics task")
		.Spawnable()
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_DATA_STORYLINE)
		.Require(O_PART_DATA_STORYLINE)
		.Require(O_BREAK_REVERSED_IMPACT)
		.Require(O_PART_REVERSED_SNAP)
		.Require(O_LINE_REVERSED_SNAP)
		.Require(O_BREAK_REVERSED_SNAP)
		.Process(&Task::Process_MakeLyricsTask)
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_LYRICS, "reversed lyrics")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_DATA_STORYLINE)
		.Require(O_PART_DATA_STORYLINE)
		.Require(O_BREAK_REVERSED_IMPACT)
		.Require(O_PART_REVERSED_SNAP)
		.Require(O_LINE_REVERSED_SNAP)
		.Require(O_BREAK_REVERSED_SNAP)
		.Input(&Task::CreateInput_Lyrics)
		.Process(&Task::Process_Lyrics)
			.Result(O_SONG_REVERSED_LYRICS)
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
		.DebugInput()
		;
	
	AddRule(TASK_LYRICS_TRANSLATE, "reversed lyrics translate")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_REVERSED_LYRICS)
		.Input(&Task::CreateInput_LyricsTranslate)
		.Process(&Task::Process_LyricsTranslate)
			.Result(O_SONG_REVERSED_TRANSLATED_LYRICS)
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
		.DebugInput()
		;
	
	
}

void TaskMgrConfig::Load() {
	LoadFromFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Store() {
	StoreToFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Process() {
	Database& db = Database::Single();
	
	while (running) {
		db.lock.EnterRead();
		for (Artist& art : db.artists) {
			for (Release& rel : art.releases) {
				for (Song& song : rel.songs) {
					if (song.pipe) {
						Pipe& pipe = *song.pipe;
						pipe.TaskMgr::Process();
					}
				}
			}
		}
		db.lock.LeaveRead();
		
		Sleep(10);
	}
	
	stopped = true;
}













Task& TaskMgr::AddTask() {
	task_lock.Enter();
	Task& t = tasks .Add();
	task_lock.Leave();
	return t;
}

void TaskMgr::LoadTaskOrder() {
	String dir = ConfigFile("taskmgr");
	RealizeDirectory(dir);
	String fname = IntStr64(GetSongHash()) + ".bin";
	String path = AppendFileName(dir, fname);
	if (FileExists(path))
		LoadFromFile(*this, path);
}

void TaskMgr::StoreTaskOrder() {
	String dir = ConfigFile("taskmgr");
	RealizeDirectory(dir);
	String fname = IntStr64(GetSongHash()) + ".bin";
	String path = AppendFileName(dir, fname);
	StoreToFile(*this, path);
}

void TaskMgr::Process() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	
	if (!iters) {
		LoadTaskOrder();
	}
	iters++;
	
	int ready = 0, got_ready = 0;
	
	this->total = tasks.GetCount();
	
	bool shorten_task_order = false;
	
	if (!task_order_cache_missed) {
		while (task_i < task_order.GetCount()) {
			hash_t task_hash0 = task_order[task_i];
			bool task_ready = false;
			for(int i = 0; i < tasks.GetCount() && mgr.running && !Thread::IsShutdownThreads(); i++) {
				Task& t = tasks[i];
				hash_t task_hash1 = t.GetOrderHash();
				if (task_hash0 == task_hash1) {
					if (!t.ready) {
						ProcessSingle(i);
						if (t.ready) {
							task_order_dbg << t.GetInfoInline();
							actual++;
							ready++;
							got_ready++;
							task_ready = true;
							// StoreTaskOrder(); // No need to store, because it follows old list
						}
					}
					else
						task_ready = true; // TODO almost an error, is it?
					break;
				}
			}
			if (task_ready) {
				task_i++;
			}
			else {
				// Task failed, but it's not necessarily fail of ordered task processing.
				
				// Possible reasons:
				// - task is being spawned in the end of this function
				// - the AI task input was changed and it caused some new problems.
				shorten_task_order = true;
				break;
			}
		}
	}
	
	for(int i = 0; i < tasks.GetCount() && mgr.running && !Thread::IsShutdownThreads(); i++) {
		Task& t = tasks[i];
		if (!t.ready) {
			ProcessSingle(i);
			if (t.ready) {
				actual++;
				ready++;
				got_ready++;
				task_i++;
				task_order_dbg << t.GetInfoInline();
				
				hash_t hash = t.GetOrderHash();
				if (shorten_task_order) {
					// If somehow we ended up being in the right hash (then keep going)
					// TODO: strengthen this! it's very weak mechanism to get back
					if (task_i < task_order.GetCount() && task_order[task_i] == hash) {
						task_i++;
						break; // loop indirectly back to ordered hash processing
					}
					// CACHE MISS!
					else {
						//DUMPC(task_order_dbg);
						task_order.SetCount(task_i); // the failure is certain here.
						task_order << hash;
						task_order_cache_missed = true;
					}
					shorten_task_order = false;
				}
				else {
					task_order << hash;
					//DUMPC(task_order_dbg);
				}
				
				StoreTaskOrder();
			}
		}
		else
			ready++;
	}
	this->actual = ready;
	
	if (!got_ready) {
		int tried_retry = 0;
		for (Task& t : tasks) {
			if (t.fatal_error)
				continue;
			if (t.failed && !t.ready && t.tries < mgr.max_tries) {
				t.tries++;
				t.Retry(false, false);
				tried_retry++;
			}
		}
		if (!tried_retry) {
			SpawnTasks();
		}
	}
}

void TaskMgr::ProcessSingle(int task_i) {
	task_lock.Enter();
	
	Task& t = tasks[task_i];
	
	Index<Task*> seen;
	t.is_waiting_deps = !IsDepsReady(t, seen);
	
	// Skip ready, failed and those with non-ready dependencies
	if (t.ready || t.failed || t.is_waiting_deps)
		;
	else {
		status = "Processing #" + IntStr(task_i);
		t.Process();
		status = "";
	}
	
	task_lock.Leave();
}

bool TaskMgr::IsDepsReady(Task& t, Index<Task*>& seen) const {
	bool all_ready = true;
	ASSERT(t.rule);
	const TaskRule& r = *t.rule;
	for (const TaskOutputType& o : r.reqs) {
		bool found = false;
		int user_mode_range_i = r.req_mode_ranges.Find(o);
		bool user_mode_range = user_mode_range_i >= 0;
		
		if (user_mode_range) {
			const auto& range = r.req_mode_ranges[user_mode_range_i];
			found = true;
			for (int m = range.a; m <= range.b; m++) {
				bool mode_found = false;
				for (const Task& t0 : tasks) {
					if (t0.p.a.mode != (SnapMode)m)
						continue;
					const TaskRule& r0 = *t0.rule;
					if (t0.ready) {
						for (const TaskOutputType& o0 : r0.results) {
							if (o == o0) {
								mode_found = true;
								break;
							}
						}
						if (mode_found)
							break;
					}
				}
				if (!mode_found) {
					found = false;
					break;
				}
			}
		}
		else {
			for (const Task& t0 : tasks) {
				if (!t0.rule->allow_cross_mode && t.p.a.mode != t0.p.a.mode)
					continue;
				const TaskRule& r0 = *t0.rule;
				if (t0.ready) {
					for (const TaskOutputType& o0 : r0.results) {
						if (o == o0) {
							// Special case (must be previous context)
							if (o == O_NEXT_CTX_JUMP) {
								if (t0.p.a.ctx != t.p.a.ctx-1)
									continue;
							}
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
			}
			// Special case: first context doesn't require this
			if (o == O_NEXT_CTX_JUMP) {
				if (t.p.a.ctx == CTX_BEGIN)
					found = true;
			}
		}
		
		if (!found)
			return false;
	}
	return true;
}

void TaskMgr::ImportSongAndMakeReversedSong() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_IMPORT_AND_REVERSE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	if (1) {
		Vector<int> rm_list;
		for(int i = 0; i < tasks.GetCount(); i++) {
			Task& t = tasks[i];
			if (t.p.pipe == &p)
				rm_list << i;
		}
		tasks.Remove(rm_list);
	}
	else {
		for (Task& t : tasks) {
			if (t.rule == &r) {
				LOG("TaskMgr::ImportSongAndMakeReversedSong: error: task exists already");
				return;
			}
		}
	}
	
	Task& t = AddTask();
	t.rule = &r;
	//t.p.CopyPtrs(p.p);
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	
	
}

void TaskMgr::TranslateSongData(String orig_lang, String orig_key, String trans_lang, String trans_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_TRANSLATE_SONG_DATA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_lang << orig_key << trans_lang << trans_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::ConvertScreenplayToPlan(String orig_key, String plan_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CONVERT_SCREENPLAY_TO_PLAN);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_key << plan_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_TRANSLATE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_lang << orig_txt << trans_lang;
	t.WhenResult << WhenResult;
}

void TaskMgr::UnpackStructureSongData(String orig_key, String struct_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_UNPACK_STRUCTURE_SONG_DATA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_key << struct_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::CheckSongStructureErrors(String main_key, String results_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CHECK_ERRORS_IN_SONG_STRUCT_DATA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << main_key << results_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::CheckScreenplayStructureErrors(String txt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CHECK_ERRORS_IN_SCREENPLAY_STRUCT_DATA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << txt;
	t.WhenResult << WhenResult;
}

void TaskMgr::RawCompletion(String prompt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_RAW_COMPLETION);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.raw_input = prompt;
	t.WhenResult << WhenResult;
}

void TaskMgr::EvaluateSuggestionScores(const Vector<String>& strs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_EVALUATE_SUGGESTION_SCORES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args <<= strs;
	t.WhenResult << WhenResult;
}

void TaskMgr::EvaluateSuggestionOrder(const Vector<String>& strs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_EVALUATE_SUGGESTION_ORDER);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args <<= strs;
	t.WhenResult << WhenResult;
}

void TaskMgr::ImproveSourceText(const Vector<String>& strs, int style, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_IMPROVE_SOURCE_TEXT);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << IntStr(style);
	t.args.Append(strs);
	t.WhenResult << WhenResult;
}

void TaskMgr::LimitSyllableCount(const Vector<String>& strs, int syllables, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_LIMIT_SYLLABLE_COUNT);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << IntStr(syllables);
	t.args.Append(strs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetAIAttributes(String orig_txt, int attr_count, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_AI_ATTRIBUTES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_txt << IntStr(attr_count);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetStructureSuggestions(String req, String avoid, String desc, int total, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_STRUCTURE_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << req << avoid << desc << IntStr(total);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetSuggestionAttributes(Vector<String>& structs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_SUGGESTION_ATTRIBUTES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args <<= structs;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetNovelThemes(Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_NOVEL_THEMES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args <<= attrs;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetNovelIdeas(String theme, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_NOVEL_IDEAS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetToneSuggestions(String theme, String idea, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_TONE_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetContentSuggestions(String theme, String idea, String tone, String alleg, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_CONTENT_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone << alleg;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetPartContentSuggestions(String theme, String idea, String tone, String alleg, String part, String known_part_ideas, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_CONTENT_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone << alleg << part << known_part_ideas;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetPartImagerySuggestions(String theme, String idea, String tone, String alleg, String content, String part, String known_part_ideas, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_IMAGERY_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone << alleg << content << part << known_part_ideas;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetPartSymbolismSuggestions(const IdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_SYMBOLISM_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetPartIdea(const IdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetAllegorySuggestions(String theme, String idea, String tone, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_ALLEGORICAL_DEVICES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetImagerySuggestions(String theme, String idea, String tone, String alleg, String content, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_IMAGERY_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone << alleg << content;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetSymbolismSuggestions(String theme, String idea, String tone, String alleg, String content, String imagery, Vector<String>& attrs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_SYMBOLISM_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << theme << idea << tone << alleg << content << imagery;
	t.args.Append(attrs);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetIdeaSuggestions(const IdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_IDEA_SUGGESTIONS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
}

void TaskMgr::CheckSongNaturalErrors(String main_key, String results_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CHECK_ERRORS_IN_SONG_NL_DATA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << main_key << results_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::ConvertSongStructureToEnglish(String struct_txt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CONVERT_SONG_STRUCTURE_TO_ENGLISH);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << struct_txt;
	t.WhenResult << WhenResult;
}

void TaskMgr::EvaluateSongAudience(String src_key, String dst_key, int mode, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_EVALUATE_SONG_AUDIENCE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << src_key << dst_key << IntStr(mode);
	t.WhenDone << WhenDone;
	
	t.args
			<< "Steve Packard (born 2013, likes Pop-Rock)"
			<< "Mila Smith (born 2009, likes KoreaPop)"
			<< "Paul Lee (born 2003, likes Rap)"
			<< "Tina Smith (born 1996, likes Pop)"
			<< "Bobby Reynolds (born 1995, likes Rock)"
			<< "Rachel Johnson (born 1992, likes Pop)"
			<< "Shawn Johnson (born 1991, likes Rock)"
			<< "Jake White (born 1989, likes Pop)"
			<< "Cyrus McCall (born 1988, likes Rap)"
			<< "Kelly Taylor (born 1987, likes Rock)"
			<< "Mike Richards (born 1977, likes Metal)"
			;
	
}

void TaskMgr::MakePoetic(String style, String src_key, String dst_key, Callback WhenDone) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_MAKE_SONG_POETIC);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << style << src_key << dst_key;
	t.WhenDone << WhenDone;
}

void TaskMgr::EvaluatePoeticStyles(
	const PoeticStylesArgs& args,
	Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_EVALUATE_POETIC_STYLES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args	<< s;
	t.WhenResult << WhenResult;
}

void TaskMgr::MorphToAttributes(const MorphArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_MORPH_TO_ATTRIBUTES);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
}

void TaskMgr::ConvertScreenplayToStructure(String orig_txt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CONVERT_SCREENPLAY_TO_STRUCTURE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_txt;
	t.WhenResult << WhenResult;
}

void TaskMgr::ConvertStructureToScreenplay(String orig_txt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CONVERT_STRUCTURE_TO_SCREENPLAY);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << orig_txt;
	t.WhenResult << WhenResult;
}

void TaskMgr::CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_CREATE_IMAGE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << prompt << IntStr(count) << IntStr(reduce_size_mode);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_EDIT_IMAGE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	if (orig.GetSize() != mask.GetSize()) {
		WhenError();
		return;
	}
	
	{
		Size sz = orig.GetSize();
		ImageBuffer ib(sz);
		RGBA* dst = ib.Begin();
		const RGBA* orig_it = orig.Begin();
		const RGBA* mask_it = mask.Begin();
		const RGBA* mask_end = mask.End();
		RGBA dark;
		dark.r = 0;
		dark.g = 0;
		dark.b = 0;
		dark.a = 0;
		while (mask_it != mask_end) {
			if (mask_it->a == 0)
				*dst = *orig_it;
			else
				*dst = dark;
			mask_it++;
			orig_it++;
			dst++;
		}
		orig = ib;
	}
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.send_images << orig;
	t.args << prompt << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_VARIATE_IMAGE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	Task& t = AddTask();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.send_images << orig;
	t.args << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetInternalRhymingFirstLine(const RhymingArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_INTERNAL_RHYMING_FIRST_LINE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetInternalRhymingContinueLine(const RhymingArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_INTERNAL_RHYMING_CONTINUE_LINE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetIdeaFromLyrics(const String& lyrics, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_IDEA_FROM_LYRICS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << lyrics;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetAttributesFromLyrics(const String& lyrics, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_ATTRIBUTES_FROM_LYRICS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << lyrics;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetProductionIdea(const ProductionArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PRODUCTION_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetStoryContext(const IdeaArgs& iargs, const StoryContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_STORY_CONTEXT);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String is = iargs.Get();
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << is << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPartContext(const StoryContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_CONTEXT);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPartVisualIdeaContext(const StoryContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_VISUAL_IDEA_CONTEXT);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPartVisualIdeaCharacters(const VisualContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_VISUAL_IDEA_CHARACTERS);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPartDialogueIdeaContext(const VisualContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_DIALOGUE_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPartDialogueIdeaStyleSuggestions(const VisualContextArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_PART_DIALOGUE_IDEA_STYLE);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetColorIdea(const ColorIdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_COLOR_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetVocabulary(const VocabularyArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_VOCABULARY);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetVocabularyIdea(const VocabularyIdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_VOCABULARY_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetWordSaladIdea(const WordSaladIdeaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_GET_WORD_SALAD_IDEA);
	Pipe& p = dynamic_cast<Pipe&>(*this);
	
	String s = args.Get();
	
	task_lock.Enter();
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}





TaskRule& TaskMgrConfig::GetRule(int code) {
	for (TaskRule& r : rules)
		if (r.code == code)
			return r;
	static TaskRule r;
	return r;
}

const TaskRule& TaskMgrConfig::GetRule(int code) const {
	for (const TaskRule& r : rules)
		if (r.code == code)
			return r;
	static TaskRule r;
	return r;
}

TaskRule& TaskMgrConfig::AddRule(int code, String name) {
	for (auto& r : rules) {
		ASSERT(r.code != code);
	}
	TaskRule& r = rules.Add();
	r.code = code;
	r.name = name;
	return r;
}

GroupContext TaskMgr::GetGroupContextLimit() const {
	int tt = 0;
	for (const Task& t : tasks) {
		if (t.rule->code == O_NEXT_CTX_JUMP && t.ready)
			tt = max(tt, (int)t.p.a.ctx + 1);
	}
	return (GroupContext)(tt+1);
}

bool TaskMgr::SpawnTasks() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	
	int spawned = 0;
	Index<Pipe*> task_songs;
	
	task_lock.Enter();
	for (Task& t : tasks)
		task_songs.FindAdd(t.p.pipe);
	task_lock.Leave();
	
	GroupContext ctx_limit = GetGroupContextLimit();
	for (const TaskRule& r : mgr.rules) {
		for (GroupContext ctx = CTX_BEGIN; ctx != ctx_limit; ((int&)ctx)++) {
			bool ctx_spawned = false;
			if (r.spawnable) {
				bool verbose = false; //r.code == TASK_MAKE_IMPACT_SCORING_TASKS && task_songs.GetCount();
				if (verbose) {
					DUMP((int)ctx);
				}
				ASSERT(r.reqs.GetCount());
				for (Pipe* s : task_songs.GetKeys()) {
					Task* exists_already = 0;
					for (Task& t : tasks) {
						if (t.rule == &r && t.p.pipe == s && t.p.a.ctx == ctx) {
							exists_already = &t;
							break;
						}
					}
					if (exists_already) {
						// Some rules can be allowed to spawn multiple times
						// if task has made successful tasks and have not been used to spawn another already
						if (r.multi_spawnable && exists_already->allow_multi_spawn) {
							if (!exists_already->ready ||
								!exists_already->HasCreatedTasks(*this, ctx) ||
								!exists_already->IsCreatedTasksReady(*this, ctx))
								continue;
							
							// Use this flag only once
							exists_already->allow_multi_spawn = false;
						}
						else
							continue;
					}
					
					bool found_all = true;
					int dbg_i = -1;
					for (TaskOutputType tt : r.reqs) {
						dbg_i++;
						bool skips_ctx = IsTaskSkippingContext(tt);
						bool found = false;
						for (Task& t : tasks) {
							if (t.p.a.ctx != ctx && !skips_ctx)
								continue;
							for (TaskOutputType t0 : t.rule->results) {
								if (t0 == tt) {
									found = true;
									break;
								}
							}
							if (found) break;
						}
						// Special case
						if (tt == O_NEXT_CTX_JUMP) {
							if (ctx == CTX_BEGIN)
								found = true;
						}
						if (!found) {
							if (verbose) {
								DUMP(dbg_i);
								DUMP((int)tt);
							}
							found_all = false;
							break;
						}
					}
					if (!found_all)
						continue;
					
					int mode_begin = -1, mode_end = 0;
					for (const TaskRule::ArgTuple& arg : r.args) {
						if (arg.a == V_MODE) {
							mode_begin = arg.b;
							mode_end = arg.c;
						}
					}
					
					for (int mode = mode_begin; mode < mode_end; mode++) {
						Task& t = AddTask();
						t.rule = &r;
						t.p.CopyPtrs(s->Get0());
						t.p.a.mode = (SnapMode)mode;
						t.p.a.ctx = ctx;
						t.id = ++spawn_id;
						spawned++;
						ctx_spawned = true;
					}
				}
			}
			
			// Only for the last context that allowed spawning
			if (ctx_spawned)
				break;
		}
	}
	return spawned > 0;
}

hash_t TaskMgr::GetSongHash() const {
	if (hash)
		return hash;
	
	const Pipe& pipe = static_cast<const Pipe&>(*this);
	ASSERT(pipe.song);
	Song& song = *pipe.song;
	
	CombineHash ch;
	ch << song.EditorPtrs::artist->native_name;
	ch << song.EditorPtrs::release->native_title;
	ch << song.native_title;
	hash = ch;
	ASSERT(hash != 0);
	return hash;
}













TaskRule& TaskRule::Input(void (Task::*fn)()) {
	this->input = fn;
	return *this;
}

TaskRule& TaskRule::Arg(TaskArgType arg, int i0, int i1) {
	ArgTuple& a = args.Add();
	a.a = arg;
	a.b = i0;
	a.c = i1;
	return *this;
}

TaskRule& TaskRule::Require(TaskOutputType arg) {
	reqs.Add(arg);
	return *this;
}

TaskRule& TaskRule::RequireMode(TaskOutputType arg, SnapMode begin, SnapMode end) {
	reqs.Add(arg);
	ASSERT(req_mode_ranges.Find(arg) < 0);
	auto& t = req_mode_ranges.Add(arg);
	t.a = begin;
	t.b = end;
	return *this;
}

TaskRule& TaskRule::Process(void (Task::*fn)()) {
	process = fn;
	return *this;
}

TaskRule& TaskRule::Result(TaskOutputType arg) {
	results.Add(arg);
	return *this;
}

TaskRule& TaskRule::Spawnable(bool b) {
	spawnable = b;
	return *this;
}

TaskRule& TaskRule::MultiSpawnable(bool b) {
	spawnable = b;
	multi_spawnable = b;
	return *this;
}

TaskRule& TaskRule::CrossMode(bool b) {
	allow_cross_mode = b;
	return *this;
}

TaskRule& TaskRule::Hash(TaskArgType t) {
	hashes.Add(t);
	return *this;
}

TaskRule& TaskRule::SeparateItems(bool b) {
	separate_items = b;
	return *this;
}

TaskRule& TaskRule::DebugInput(bool b) {
	debug_input = b;
	return *this;
}

TaskRule& TaskRule::ImageTask(bool b) {
	image_task = b;
	return *this;
}

TaskRule& TaskRule::ImageEditTask(bool b) {
	imageedit_task = b;
	return *this;
}

TaskRule& TaskRule::ImageVariateTask(bool b) {
	imagevariate_task = b;
	return *this;
}

