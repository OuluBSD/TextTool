#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


SnapCoverImage::SnapCoverImage() {
	user_natural_english_key = "NATURAL_ENGLISH_OF_AUTHOR";
	
}

void SnapCoverImage::Init() {
	ImagePlayerBase::Init();
	
}

void SnapCoverImage::ToolMenu(Bar& bar) {
	bar.Add(t_("Create suggestions for prompts"), AppImg::Part(), THISBACK(CreateSuggestionsForPrompts)).Key(K_F5);
	bar.Add(t_("Make single image"), AppImg::Part(), THISBACK(MakeSingleImage)).Key(K_F6);
	bar.Add(t_("Make all images"), AppImg::Part(), THISBACK(MakeAllImages)).Key(K_F7);
	
}

void SnapCoverImage::CreateSuggestionsForPrompts() {
	TextDatabase& db = GetDatabase();
	EditorPtrs& p = GetPointers();
	if(!p.component || !p.entity ||!p.release)
		return;
	
	Snapshot& rel = *p.release;
	
	
	
	if (items.GetCount()) {
		if (!PromptYesNo(DeQtf(t_("Are you sure that you want to replace all in the list?"))))
			return;
	}
	
	int row = 0;
	for(int i = 0; i < rel.components.GetCount(); i++) {
		const Component& song = rel.components[i];
		PromptOK("TODO");
		String scripts;
		/*String scripts = song.data.Get(user_natural_english_key, "");
		if (scripts.IsEmpty())
			continue;*/
		
		String title;// = !song.english_title.IsEmpty() ? song.english_title : song.native_title;
		
		String raw_prompt;
		raw_prompt << "Entity: " << p.entity->english_name << "\n";
		raw_prompt << "Album: " << p.release->english_title << "\n";
		raw_prompt << "Year: " << (int)p.release->date.year << "\n";
		raw_prompt << "Genre: " << p.entity->musical_style << "\n";
		raw_prompt << "Description of singer: " << p.entity->vocalist_visual << "\n";
		raw_prompt << "Count of components: " << p.release->components.GetCount() << "\n";
		raw_prompt << "\n\n";
		
		raw_prompt << "Song: " << title << "\n\n";
		raw_prompt << scripts << "\n\n";
		
		raw_prompt << "Short description of the album's cover image for the generative image AI:\n";
		
		String result_part = title + " based cover";
		
		TaskMgr& m = TaskMgr::Single();
		m.RawCompletion(raw_prompt, THISBACK1(PostOnResult, result_part));
	}
}

void SnapCoverImage::OnResult(String res, String part) {
	Item& item = items.Add();
	item.part = part;
	item.prompt = res;
	
	DataList();
}

void SnapCoverImage::MakeSingleImage() {
	ImagePlayerBase::MakeSingleImage();
}

void SnapCoverImage::MakeAllImages() {
	ImagePlayerBase::MakeAllImages();
}


END_TEXTLIB_NAMESPACE
