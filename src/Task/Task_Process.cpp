#include "Task.h"



BEGIN_TEXTLIB_NAMESPACE



void AiTask::Process_Translate() {
	WhenResult(output);
}

void AiTask::Process_CreateImage() {
	WhenResultImages(recv_images);
}

void AiTask::Process_EditImage() {
	WhenResultImages(recv_images);
}

void AiTask::Process_VariateImage() {
	WhenResultImages(recv_images);
}

void AiTask::Process_RawCompletion() {
	WhenResult(output);
}

void AiTask::Process_MorphToAttributes() {
	WhenResult(output);
}

void AiTask::Process_GetStructureSuggestions() {
	WhenResult("-\"" + output);
}

void AiTask::Process_GetSuggestionAttributes() {
	WhenResult("-" + output);
}

void AiTask::Process_GetColorIdea() {
	WhenResult("1.RGB(" + output);
}

void AiTask::Process_GetVocabulary() {
	WhenResult(output);
}

void AiTask::Process_GetVocabularyIdea() {
	//WhenResult("A collection of 30 the most relevant words for the part 1 of\n" + output);
	WhenResult("1/30: a" + output);
}

void AiTask::Process_GetWordSaladIdea() {
	WhenResult("1/10: a" + output);
}

void AiTask::Process_GetContextIdea() {
	WhenResult(output);
}

void AiTask::Process_GetTextDataAnalysis() {
	WhenResult(output);
}

void AiTask::Process_GetActionAnalysis() {
	WhenResult(output);
}

void AiTask::Process_GetScriptPhrase() {
	WhenResult(output);
}

void AiTask::Process_GetTokenData() {
	WhenResult(output);
}

void AiTask::Process_GetPhraseData() {
	WhenResult(output);
}

void AiTask::Process_GetAttributes() {
	AttrArgs args;
	args.Put(this->args[0]);
	if (args.fn == 0)
		WhenResult("1. " + output);
	/*else if (args.fn == 1)
		WhenResult(tmp_str + output);*/
	else
		WhenResult(output);
}

void AiTask::Process_GetNanaData() {
	WhenResult(output);
}

void AiTask::Process_ScriptSolver() {
	WhenResult(output);
}

void AiTask::Process_GetProgramDataAnalysis() {
	WhenResult(output);
}

void AiTask::Process_StorySolver() {
	WhenResult(output);
}


END_TEXTLIB_NAMESPACE

