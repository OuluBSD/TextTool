#include "SocialCtrl.h"



CoverImage::CoverImage() {
	user_natural_english_key = "NATURAL_ENGLISH_OF_AUTHOR";
	
}

void CoverImage::Init() {
	ImagePlayerBase::Init();
	
}

void CoverImage::ToolMenu(Bar& bar) {
	bar.Add(t_("Create suggestions for prompts"), AppImg::Part(), THISBACK(CreateSuggestionsForPrompts)).Key(K_F5);
	bar.Add(t_("Make single image"), AppImg::Part(), THISBACK(MakeSingleImage)).Key(K_F6);
	bar.Add(t_("Make all images"), AppImg::Part(), THISBACK(MakeAllImages)).Key(K_F7);
	
}

void CoverImage::CreateSuggestionsForPrompts() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if(!p.program || !p.company ||!p.campaign)
		return;
	
	Campaign& rel = *p.campaign;
	
	
	
	if (items.GetCount()) {
		if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the list?"))))
			return;
	}
	
	int row = 0;
	for(int i = 0; i < rel.programs.GetCount(); i++) {
		const Program& program = rel.programs[i];
		PromptOK("TODO");
		String story;
		/*String story = program.data.Get(user_natural_english_key, "");
		if (story.IsEmpty())
			continue;*/
		
		String title;// = !program.english_title.IsEmpty() ? program.english_title : program.native_title;
		
		String raw_prompt;
		raw_prompt << "Company: " << p.company->english_name << "\n";
		raw_prompt << "Album: " << p.campaign->english_title << "\n";
		raw_prompt << "Year: " << (int)p.campaign->date.year << "\n";
		raw_prompt << "Genre: " << p.company->musical_style << "\n";
		raw_prompt << "Description of singer: " << p.company->vocalist_visual << "\n";
		raw_prompt << "Count of programs: " << p.campaign->programs.GetCount() << "\n";
		raw_prompt << "\n\n";
		
		raw_prompt << "Program: " << title << "\n\n";
		raw_prompt << story << "\n\n";
		
		raw_prompt << "Short description of the album's cover image for the generative image AI:\n";
		
		String result_part = title + " based cover";
		
		TaskMgr& m = TaskMgr::Single();
		m.RawCompletion(raw_prompt, THISBACK1(PostOnResult, result_part));
	}
}

void CoverImage::OnResult(String res, String part) {
	Item& item = items.Add();
	item.part = part;
	item.prompt = res;
	
	DataList();
}

void CoverImage::MakeSingleImage() {
	ImagePlayerBase::MakeSingleImage();
}

void CoverImage::MakeAllImages() {
	ImagePlayerBase::MakeAllImages();
}
