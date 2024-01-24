#include "Task.h"
#include <Database/Database.h>





void Task::Process_Translate() {
	WhenResult(output);
}

void Task::Process_CreateImage() {
	WhenResultImages(recv_images);
}

void Task::Process_EditImage() {
	WhenResultImages(recv_images);
}

void Task::Process_VariateImage() {
	WhenResultImages(recv_images);
}

void Task::Process_RawCompletion() {
	WhenResult(output);
}

void Task::Process_MorphToAttributes() {
	WhenResult(output);
}

void Task::Process_GetStructureSuggestions() {
	WhenResult("-\"" + output);
}

void Task::Process_GetSuggestionAttributes() {
	WhenResult("-" + output);
}

void Task::Process_GetColorIdea() {
	WhenResult("1.RGB(" + output);
}

void Task::Process_GetVocabulary() {
	WhenResult(output);
}

void Task::Process_GetVocabularyIdea() {
	//WhenResult("A collection of 30 the most relevant words for the part 1 of\n" + output);
	WhenResult("1/30: a" + output);
}

void Task::Process_GetWordSaladIdea() {
	WhenResult("1/10: a" + output);
}

void Task::Process_GetContextIdea() {
	WhenResult(output);
}

void Task::Process_GetSongDataAnalysis() {
	WhenResult(output);
}

void Task::Process_GetActionAnalysis() {
	WhenResult(output);
}

void Task::Process_GetLyricsPhrase() {
	WhenResult(output);
}

void Task::Process_GetTokenData() {
	WhenResult(output);
}

void Task::Process_GetPhraseData() {
	WhenResult(output);
}

void Task::Process_GetAttributes() {
	AttrArgs args;
	args.Put(this->args[0]);
	if (args.fn == 0)
		WhenResult("1. " + output);
	/*else if (args.fn == 1)
		WhenResult(tmp_str + output);*/
	else
		WhenResult(output);
}
