#include "SocialCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


CampaignIdeas::CampaignIdeas() {
	CtrlLayout(idea);
	
	Add(hsplit.SizePos());
	hsplit.Horz();
	hsplit << list << idea;
	hsplit.SetPos(2000);
	
	list.AddColumn("Title");
	//list.NoHeader();
	list.WhenBar << THISBACK(OnListMenu);
	list.WhenCursor << THISBACK(IdeaData);
	
	idea.title.WhenAction << THISBACK(OnValueChange);
	idea.target.WhenAction << THISBACK(OnValueChange);
	idea.ref.WhenAction << THISBACK(OnValueChange);
	idea.desc.WhenAction << THISBACK(OnValueChange);
}

void CampaignIdeas::Clear() {
	idea.title.Clear();
	idea.target.Clear();
	idea.ref.Clear();
	idea.desc.Clear();
}

void CampaignIdeas::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) {
		Clear();
		return;
	}
	Campaign& campaign = *p.campaign;
	
	for(int i = 0; i < campaign.ideas.GetCount(); i++) {
		const ProgramIdea& idea = campaign.ideas[i];
		list.Set(i, 0, idea.title);
	}
	list.SetCount(campaign.ideas.GetCount());
	
	int idea_idx = 0;
	if (idea_idx >= 0 && idea_idx < list.GetCount())
		list.SetCursor(idea_idx);
	
	IdeaData();
}

void CampaignIdeas::IdeaData() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign || !list.IsCursor()) {
		Clear();
		return;
	}
	Campaign& campaign = *p.campaign;
	int idea_idx = list.GetCursor();
	const ProgramIdea& obj = campaign.ideas[idea_idx];
	
	this->idea.title.SetData(obj.title);
	this->idea.target.SetData(obj.target_program);
	this->idea.ref.SetData(obj.reference_program);
	this->idea.desc.SetData(obj.description);
}

void CampaignIdeas::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	
	if (!list.IsCursor()) return;
	int idea_idx = list.GetCursor();
	ProgramIdea& obj = campaign.ideas[idea_idx];
	
	obj.title = idea.title.GetData();
	obj.target_program = idea.target.GetData();
	obj.reference_program = idea.ref.GetData();
	obj.description = idea.desc.GetData();
	
	list.Set(0, obj.title);
}

void CampaignIdeas::OnListMenu(Bar& bar) {
	bar.Add(t_("Add Idea"), THISBACK(AddIdea));
	bar.Add(t_("Remove Idea"), THISBACK(RemoveIdea));
}

void CampaignIdeas::AddIdea() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	
	
	String title;
	bool b = EditTextNotNull(
		title,
		t_("Add Idea"),
		t_("Idea's title"),
		0
	);
	if (!b) return;
	
	int idea_i = -1;
	for(int i = 0; i < campaign.ideas.GetCount(); i++) {
		ProgramIdea& idea = campaign.ideas[i];
		if (idea.title == title) {
			idea_i = i;
			break;
		}
	}
	if (idea_i >= 0) {
		PromptOK(DeQtf(t_("Idea with that title exist already")));
		return;
	}
	
	ProgramIdea& idea = campaign.ideas.Add();
	idea.title = title;
	
	list.Add(title);
	list.SetCursor(list.GetCount()-1);
	this->idea.target.SetFocus();
}

void CampaignIdeas::RemoveIdea() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	
	if (!list.IsCursor()) return;
	int idx = list.GetCursor();
	campaign.ideas.Remove(idx);
	Data();
}


END_SOCIALLIB_NAMESPACE
